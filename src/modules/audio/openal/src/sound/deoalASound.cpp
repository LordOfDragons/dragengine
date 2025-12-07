/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>

#include "deoalASound.h"
#include "deoalDecodeBuffer.h"
#include "../deoalCaches.h"
#include "../deAudioOpenAL.h"
#include "../audiothread/deoalAudioThread.h"
#include "../audiothread/deoalATDelayed.h"
#include "../audiothread/deoalATLogger.h"
#include "../audiothread/deoalDelayedDeletion.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/filesystem/deCacheHelper.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/resources/sound/deSound.h>
#include <dragengine/resources/sound/deSoundDecoder.h>
#include <dragengine/resources/sound/deSoundManager.h>



// Definitions
////////////////

// Cache version in the range from 0 to 255. Increment each time the cache
// format changed. If reaching 256 wrap around to 0. Important is only the
// number changes to force discarding old caches
#define CACHE_VERSION 0

#define FLAG_IS_USED 0x1

#define ENABLE_CACHE_LOGGING false



// Cache file header
//////////////////////

struct sCacheHeader{
	uint64_t filetime;
	uint8_t version;
	uint8_t flags;
	uint8_t bytesPerSample;
	uint8_t channelCount;
	uint32_t sampleCount;
	uint32_t sampleRate;
	uint32_t format;
	uint32_t bufferSize;
};



// Class deoalASound
/////////////////////

// Constructor, destructor
////////////////////////////

deoalASound::deoalASound(deoalAudioThread &audioThread, deSound &sound) :
pAudioThread(audioThread),
pFilename(sound.GetFilename()),

pBytesPerSample(sound.GetBytesPerSample()),
pSampleCount(sound.GetSampleCount()),
pChannelCount(sound.GetChannelCount()),
pSampleRate(sound.GetSampleRate()),
pPlayTime(sound.GetPlayTime()),

pBuffer(0),
pFormat(0),
pValid(false),

pStreamData(NULL),
pStreamDataSize(0),
pStreaming(true),
pIsUsed(false),
pIsCached(false)
{
	pDetermineFormat();
	if(! pValid){
		LEAK_CHECK_CREATE(audioThread, Sound);
		return;
	}
	
	pDetermineStreaming();
	if(pStreaming){
		LEAK_CHECK_CREATE(audioThread, Sound);
		return;
	}
	
	pLoadFromCache();
	if(pIsCached){
		LEAK_CHECK_CREATE(audioThread, Sound);
		return;
	}
	
	pLoadEntireSound(sound);
	pWriteToCache();
	
	LEAK_CHECK_CREATE(audioThread, Sound);
}

deoalASound::~deoalASound(){
	LEAK_CHECK_FREE(pAudioThread, Sound);
	
	pCleanUp();
}



// Management
///////////////

void deoalASound::PreloadSound(deSound &sound){
	// WARNING Called during synchronization time from main thread.
	
	if(pStreaming || ! pValid || ! pFormat){
		return;
	}
	
	if(! pIsUsed){
		// first time the sound is used. samples data could be already loaded
		// asynchronously during construction time. if loaded from cache and marked
		// not used we have to first load the data before we can create the buffer
		if(ENABLE_CACHE_LOGGING){
			pAudioThread.GetLogger().LogInfoFormat("Sound '%s': Marked used for the first time",
				pFilename.GetString());
		}
		
		if(! pStreamData){
			pLoadEntireSound(sound);
		}
		
		pIsUsed = true;
		pWriteToCache();
	}
}

void deoalASound::PrepareBuffers(){
	if(pStreaming || ! pValid || ! pFormat){
		return;
	}
	
	if(! pBuffer){
		OAL_CHECK(pAudioThread, alGenBuffers(1, &pBuffer));
		OAL_CHECK(pAudioThread, alBufferData(pBuffer, pFormat,
			(const ALvoid *)pStreamData, pStreamDataSize, pSampleRate));
	}
}



// Private Functions
//////////////////////

class deoalASoundDeletion : public deoalDelayedDeletion{
public:
	ALuint buffer;
	
	deoalASoundDeletion() :
	buffer(0){
	}
	
	virtual ~deoalASoundDeletion(){
	}
	
	virtual void DeleteObjects(deoalAudioThread&){
		if(buffer){
			alDeleteBuffers(1, &buffer);
		}
	}
};

void deoalASound::pCleanUp(){
	if(pStreamData){
		delete [] pStreamData;
	}
	
	// delayed deletion
	if(pBuffer){
		deoalASoundDeletion *delayedDeletion = NULL;
		
		try{
			delayedDeletion = new deoalASoundDeletion;
			delayedDeletion->buffer = pBuffer;
			pAudioThread.GetDelayed().AddDeletion(delayedDeletion);
			
		}catch(const deException &e){
			if(delayedDeletion){
				delete delayedDeletion;
			}
			pAudioThread.GetLogger().LogException(e);
			throw;
		}
	}
}



void deoalASound::pLoadFromCache(){
	const bool enableCacheLogging = ENABLE_CACHE_LOGGING;
	
	deVirtualFileSystem &vfs = pAudioThread.GetOal().GetVFS();
	deoalCaches &caches = pAudioThread.GetCaches();
	deoalATLogger &logger = pAudioThread.GetLogger();
	deCacheHelper &cacheSound = caches.GetSound();
	decBaseFileReader::Ref reader;
	
	const decPath path(decPath::CreatePathUnix(pFilename));
	if(! vfs.CanReadFile(path)){
		// without a source file no cache since it is no more unique
		return;
	}
	
	caches.Lock();
	
	try{
		reader.TakeOver(cacheSound.Read(pFilename));
		if(! reader){
			// cache file absent
			caches.Unlock();
			return;
		}
		
		if(enableCacheLogging){
			logger.LogInfoFormat("Sound '%s': Load from cache", pFilename.GetString());
		}
		
		// read header and compare parameters
		sCacheHeader header;
		reader->Read(&header, sizeof(header));
		
		// check file modification times to reject the cached file if the source model changed
		if(header.filetime != (uint64_t)vfs.GetFileModificationTime(path)){
			// cache file outdated
			reader = NULL;
			cacheSound.Delete(pFilename);
			caches.Unlock();
			
			if(enableCacheLogging){
				logger.LogInfoFormat("Sound '%s': Modification time changed. Cache discarded",
					pFilename.GetString());
			}
			return;
		}
		
		// check cache version in case we upgraded
		if(header.version != CACHE_VERSION){
			// cache file outdated
			reader = NULL;
			cacheSound.Delete(pFilename);
			caches.Unlock();
			
			if(enableCacheLogging){
				logger.LogInfoFormat("Sound '%s': Cache version changed. Cache discarded",
					pFilename.GetString());
			}
			return;
		}
		
		// read header and check if the parameters are matching
		if((int)header.bytesPerSample != pBytesPerSample
		|| (int)header.channelCount != pChannelCount
		|| (int)header.sampleCount != pSampleCount
		|| (int)header.sampleRate != pSampleRate
		|| (ALenum)header.format != pFormat){
			// cache file outdated
			reader = NULL;
			cacheSound.Delete(pFilename);
			caches.Unlock();
			
			if(enableCacheLogging){
				logger.LogInfoFormat("Sound '%s': Sound parameters mismatch. Cache discarded",
					pFilename.GetString());
			}
			return;
		}
		
		// read buffer data. data is present only if FLAG_IS_USED is set
		if(header.bufferSize > 10000000){
			DETHROW(deeInvalidParam);
		}
		
		if(header.bufferSize > 0){
			pStreamData = new char[header.bufferSize];
			pStreamDataSize = header.bufferSize;
			reader->Read(pStreamData, pStreamDataSize);
		}
		
		// done
		reader = NULL;
		
		pIsUsed = (header.flags & FLAG_IS_USED) == FLAG_IS_USED;
		pIsCached = true;
		
		caches.Unlock();
		
	}catch(const deException &){
		// damaged cache file
		reader = NULL;
		cacheSound.Delete(pFilename);
		caches.Unlock();
		
		if(enableCacheLogging){
			logger.LogInfoFormat("Sound '%s': Cache file damaged. Cache discarded",
				pFilename.GetString());
		}
	}
}

void deoalASound::pWriteToCache(){
	if(! pValid || pStreaming){
		return;
	}
	
	const bool enableCacheLogging = ENABLE_CACHE_LOGGING;
	
	deVirtualFileSystem &vfs = pAudioThread.GetOal().GetVFS();
	deoalCaches &caches = pAudioThread.GetCaches();
	deoalATLogger &logger = pAudioThread.GetLogger();
	deCacheHelper &cacheSound = caches.GetSound();
	decBaseFileWriter::Ref writer;
	
	const decPath path(decPath::CreatePathUnix(pFilename));
	if(! vfs.CanReadFile(path)){
		return; // without a source file no cache since it is no more unique
	}
	
	// collect cache parameters
	sCacheHeader header;
	header.filetime = (uint64_t)vfs.GetFileModificationTime(path);
	header.version = (uint8_t)CACHE_VERSION;
	
	header.flags = 0;
	if(pIsUsed){
		header.flags |= FLAG_IS_USED;
	}
	
	header.bytesPerSample = (uint8_t)pBytesPerSample;
	header.channelCount = (uint8_t)pChannelCount;
	header.sampleCount = (uint32_t)pSampleCount;
	header.sampleRate = (uint32_t)pSampleRate;
	header.format = (uint32_t)pFormat;
	header.bufferSize = (uint32_t)pStreamDataSize;
	
	// write cache
	caches.Lock();
	
	try{
		writer.TakeOver(cacheSound.Write(pFilename));
		writer->Write(&header, sizeof(header));
		if(pStreamDataSize > 0){
			writer->Write(pStreamData, pStreamDataSize);
		}
		writer = NULL;
		
		caches.Unlock();
		if(enableCacheLogging){
			logger.LogInfoFormat("Sound '%s': Cache written", pFilename.GetString());
		}
		
	}catch(const deException &e){
		writer = NULL;
		cacheSound.Delete(pFilename);
		caches.Unlock();
		
		if(enableCacheLogging){
			logger.LogException(e);
			logger.LogErrorFormat("Sound '%s': Failed writing cache file", pFilename.GetString());
		}
	}
}



void deoalASound::pDetermineStreaming(){
	// it is tricky to decide if data should be kept in memory or streamed.
	// 
	// some values to get an idea what memory consumption we are looking at. for these
	// values we assume single channel 16 bit samples at 44100 sampling rate.
	// this gives a basic memory consumption per second sound data of:
	//   bytesPerSecond = 44100 * 1 * 2;  // rate * channelCount * bytesPerSample
	// 
	// this is 88200 bytes or roughly 88kb per second sound data.
	// 
	// short sound effects are below 2s length. this results in ~176kb sound data for
	// effect samples. using a limit of 250kb should keep short sound effect of <3s
	// playtime in memory. with a limit of 600kb this keeps sounds effects and ambient
	// or voice sounds with up to 7s in memory. for the time being this limit is used.
	// 
	// an important criteria is single or double channel sound. double channel is only
	// used for music files. these have to be streamed always. single channel data can
	// be effects, voice data or localized music files played in the 3D world. for this
	// data using the 1MB threashold should be okay.
	
	const int requiredBufferSize = pSampleCount * pBytesPerSample * pChannelCount;
	pStreaming = pChannelCount > 1
		|| requiredBufferSize > pAudioThread.GetConfiguration().GetStreamBufSizeThreshold();
	/*
	pAudioThread.GetLogger().LogInfoFormat("Sound(%s) samp=%d(%.1fs) bps=%d cc=%d rbs=%d rs=%d",
		pFilename.GetString(), pSampleCount, (float)pSampleCount / (float)pSampleRate,
		pBytesPerSample, pChannelCount, requiredBufferSize, pStreaming);
	*/
}

void deoalASound::pDetermineFormat(){
	deoalATLogger &logger = pAudioThread.GetLogger();
	
	if(pBytesPerSample != 1 && pBytesPerSample != 2){
		logger.LogWarnFormat("%s: %i bytes per sample not supported only 1 or 2.\n",
			pFilename.GetString(), pBytesPerSample);
	}
	
	if(pChannelCount != 1 && pChannelCount != 2){
		logger.LogWarnFormat("%s: %i channels not supported only 1 or 2.\n",
			pFilename.GetString(), pChannelCount);
	}
	
	if(pSampleCount == 0){
		return;
	}
	
	if(pBytesPerSample == 1){
		if(pChannelCount == 1){
			pFormat = AL_FORMAT_MONO8;
			pValid = true;
			
		}else if(pChannelCount == 2){
			pFormat = AL_FORMAT_STEREO8;
			pValid = true;
		}
		
	}else if(pBytesPerSample == 2){
		if(pChannelCount == 1){
			pFormat = AL_FORMAT_MONO16;
			pValid = true;
			
		}else if(pChannelCount == 2){
			pFormat = AL_FORMAT_STEREO16;
			pValid = true;
		}
	}
}



void deoalASound::pLoadEntireSound(deSound &sound){
	if(pStreamData){
		return;
	}
	
	const int bufferSize = pSampleCount * pBytesPerSample * pChannelCount;
	if(bufferSize == 0){
		pStreamData = new char[1]; // just so we know there is nothing
		return;
	}
	
	deSoundDecoder::Ref decoder(deSoundDecoder::Ref::New(
		 pAudioThread.GetOal().GetGameEngine()->GetSoundManager()->CreateDecoder(&sound)));
	
	pStreamData = new char[bufferSize];
	pStreamDataSize = bufferSize;
	
	decoder->ReadSamples(pStreamData, bufferSize);
}
