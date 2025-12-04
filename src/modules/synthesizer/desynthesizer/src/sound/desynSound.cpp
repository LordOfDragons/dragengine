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

#include "desynSound.h"
#include "desynDecodeBuffer.h"
#include "../deDESynthesizer.h"
#include "../desynCaches.h"
#include "../desynConfiguration.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileReader::Ref.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/file/decBaseFileWriter::Ref.h>
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
	uint32_t bufferSize;
};



// Class desynSound
/////////////////////

// Constructor, destructor
////////////////////////////

desynSound::desynSound( deDESynthesizer &module, deSound &sound ) :
pModule( module ),
pSound( sound ),

pValid( false ),
pIsUsed( false ),
pIsCached( false ),

pStreamData( NULL ),
pStreamDataSize( 0 ),
	pStreaming( true )
{
	pDetermineValid();
	if( ! pValid ){
		return;
	}
	
	pDetermineStreaming();
	if( pStreaming ){
		return;
	}
	
	pLoadFromCache();
	if( pIsCached ){
		return;
	}
	
	// we load sounds if possible for the first time we encounter them. most sounds are not
	// used by any synthesizers. for those we waste some time loading them. the cache file
	// writes them though as not used. if no synthesizer uses the sounds they stay marked
	// unsued. all unused sounds will not be loaded the next time speeding things up
	LoadEntireSound();
	pWriteToCache();
}

desynSound::~desynSound(){
	pCleanUp();
}



// Management
///////////////

void desynSound::LoadEntireSound(){
	if( pStreamData ){
		return;
	}
	
	// for the time being all sounds are fully loaded into memory since the synthesizer
	// sources do not yet support streaming properly
	/*if( ! pRequireStreaming ){
		return;
	}*/
	
	const int bufferSize = pSound.GetSampleCount()
		* pSound.GetBytesPerSample() * pSound.GetChannelCount();
	if( bufferSize == 0 ){
		pStreamData = new char[ 1 ]; // just so we know there is nothing
		return;
	}
	
	deSoundDecoder *decoder = NULL;
	
	try{
		decoder = pModule.GetGameEngine()->GetSoundManager()->CreateDecoder( &pSound );
		
		pStreamData = new char[ bufferSize ];
		pStreamDataSize = bufferSize;
		
		decoder->ReadSamples( pStreamData, bufferSize );
		
		decoder->FreeReference();
		
	}catch( const deException & ){
		if( decoder ){
			decoder->FreeReference();
		}
		throw;
	}
}

void desynSound::Prepare(){
	// for the time being streaming is not supported. this has to be fixed
	if( pStreaming ){
		if( ! pStreamData ){
			LoadEntireSound();
		}
		return;
	}
	// end
	
	if( pStreaming || ! pValid ){
		return;
	}
	
	if( ! pIsUsed ){
		// first time the sound is used. samples data could be already loaded
		// asynchronously during construction time. if loaded from cache and marked
		// not used we have to load data
		if( ENABLE_CACHE_LOGGING ){
			pModule.LogInfoFormat( "Sound '%s': Marked used for the first time",
				pSound.GetFilename().GetString() );
		}
		
		if( ! pStreamData ){
			LoadEntireSound();
		}
		
		pIsUsed = true;
		pWriteToCache();
	}
}



// Private Functions
//////////////////////

void desynSound::pCleanUp(){
	if( pStreamData ){
		delete [] pStreamData;
	}
}



void desynSound::pLoadFromCache(){
	const bool enableCacheLogging = ENABLE_CACHE_LOGGING;
	
	deVirtualFileSystem &vfs = *pModule.GetGameEngine()->GetVirtualFileSystem();
	const decString &filename = pSound.GetFilename();
	desynCaches &caches = pModule.GetCaches();
	deCacheHelper &cacheSound = caches.GetSound();
	decBaseFileReader::Ref reader;
	
	const decPath path( decPath::CreatePathUnix( filename ) );
	if( ! vfs.CanReadFile( path ) ){
		// without a source file no cache since it is no more unique
		return;
	}
	
	caches.Lock();
	
	try{
		reader.TakeOver( cacheSound.Read( filename ) );
		if( ! reader ){
			// cache file absent
			caches.Unlock();
			return;
		}
		
		if( enableCacheLogging ){
			pModule.LogInfoFormat( "Sound '%s': Load from cache", filename.GetString() );
		}
		
		// read header and compare parameters
		sCacheHeader header;
		reader->Read( &header, sizeof( header ) );
		
		// check file modification times to reject the cached file if the source model changed
		if( header.filetime != ( uint64_t )vfs.GetFileModificationTime( path ) ){
			// cache file outdated
			reader = NULL;
			cacheSound.Delete( filename );
			caches.Unlock();
			
			if( enableCacheLogging ){
				pModule.LogInfoFormat( "Sound '%s': Modification time changed. Cache discarded",
					filename.GetString() );
			}
			return;
		}
		
		// check cache version in case we upgraded
		if( header.version != CACHE_VERSION ){
			// cache file outdated
			reader = NULL;
			cacheSound.Delete( filename );
			caches.Unlock();
			
			if( enableCacheLogging ){
				pModule.LogInfoFormat( "Sound '%s': Cache version changed. Cache discarded",
					filename.GetString() );
			}
			return;
		}
		
		// read header and check if the parameters are matching
		if( ( int )header.bytesPerSample != pSound.GetBytesPerSample()
		|| ( int )header.channelCount != pSound.GetChannelCount()
		|| ( int )header.sampleCount != pSound.GetSampleCount()
		|| ( int )header.sampleRate != pSound.GetSampleRate() ){
			// cache file outdated
			reader = NULL;
			cacheSound.Delete( filename );
			caches.Unlock();
			
			if( enableCacheLogging ){
				pModule.LogInfoFormat( "Sound '%s': Sound parameters mismatch. Cache discarded",
					filename.GetString() );
			}
			return;
		}
		
		// read buffer data. data is present only if FLAG_IS_USED is set
		if( header.bufferSize > 10000000 ){
			DETHROW( deeInvalidParam );
		}
		
		if( header.bufferSize > 0 ){
			pStreamData = new char[ header.bufferSize ];
			pStreamDataSize = header.bufferSize;
			reader->Read( pStreamData, pStreamDataSize );
		}
		
		// done
		reader = NULL;
		
		pIsUsed = ( header.flags & FLAG_IS_USED ) == FLAG_IS_USED;
		pIsCached = true;
		
		caches.Unlock();
		
	}catch( const deException & ){
		// damaged cache file
		reader = NULL;
		cacheSound.Delete( filename );
		caches.Unlock();
		
		if( enableCacheLogging ){
			pModule.LogInfoFormat( "Sound '%s': Cache file damaged. Cache discarded",
				filename.GetString() );
		}
	}
}

void desynSound::pWriteToCache(){
	if( ! pValid || pStreaming ){
		return;
	}
	
	const bool enableCacheLogging = ENABLE_CACHE_LOGGING;
	
	deVirtualFileSystem &vfs = *pModule.GetGameEngine()->GetVirtualFileSystem();
	const decString &filename = pSound.GetFilename();
	desynCaches &caches = pModule.GetCaches();
	deCacheHelper &cacheSound = caches.GetSound();
	decBaseFileWriter::Ref writer;
	
	const decPath path( decPath::CreatePathUnix( filename ) );
	if( ! vfs.CanReadFile( path ) ){
		return; // without a source file no cache since it is no more unique
	}
	
	// collect cache parameters
	sCacheHeader header;
	header.filetime = ( uint64_t )vfs.GetFileModificationTime( path );
	header.version = ( uint8_t )CACHE_VERSION;
	
	header.flags = 0;
	if( pIsUsed ){
		header.flags |= FLAG_IS_USED;
	}
	
	header.bytesPerSample = ( uint8_t )pSound.GetBytesPerSample();
	header.channelCount = ( uint8_t )pSound.GetChannelCount();
	header.sampleCount = ( uint32_t )pSound.GetSampleCount();
	header.sampleRate = ( uint32_t )pSound.GetSampleRate();
	header.bufferSize = ( uint32_t )pStreamDataSize;
	
	// write cache
	caches.Lock();
	
	try{
		writer.TakeOver( cacheSound.Write( filename ) );
		writer->Write( &header, sizeof( header ) );
		if( pStreamDataSize > 0 ){
			writer->Write( pStreamData, pStreamDataSize );
		}
		writer = NULL;
		
		caches.Unlock();
		if( enableCacheLogging ){
			pModule.LogInfoFormat( "Sound '%s': Cache written", filename.GetString() );
		}
		
	}catch( const deException &e ){
		writer = NULL;
		cacheSound.Delete( filename );
		caches.Unlock();
		
		if( enableCacheLogging ){
			pModule.LogException( e );
			pModule.LogErrorFormat( "Sound '%s': Failed writing cache file", filename.GetString() );
		}
	}
}

void desynSound::pDetermineStreaming(){
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
	const int bytesPerSample = pSound.GetBytesPerSample();
	const int channelCount = pSound.GetChannelCount();
	const int sampleCount = pSound.GetSampleCount();
	
	const int requiredBufferSize = sampleCount * bytesPerSample * channelCount;
	pStreaming = channelCount > 1
		|| requiredBufferSize > pModule.GetConfiguration().GetStreamBufSizeThreshold();
	/*
	pModule.LogInfoFormat( "Sound(%s) samp=%d(%.1fs) bps=%d cc=%d rbs=%d rs=%d",
		pSound.GetFilename().GetString(), sampleCount,
		( float )sampleCount / ( float )pSound.GetSampleRate(), bytesPerSample,
		channelCount, requiredBufferSize, pStreaming );
	*/
}

void desynSound::pDetermineValid(){
	const int bytesPerSample = pSound.GetBytesPerSample();
	const int channelCount = pSound.GetChannelCount();
	const int sampleCount = pSound.GetSampleCount();
	
	if( bytesPerSample != 1 && bytesPerSample != 2 ){
		pModule.LogWarnFormat( "%s: %i bytes per sample not supported only 1 or 2.\n",
			pSound.GetFilename().GetString(), bytesPerSample );
	}
	
	if( channelCount != 1 && channelCount != 2 ){
		pModule.LogWarnFormat( "%s: %i channels not supported only 1 or 2.\n",
			pSound.GetFilename().GetString(), channelCount );
	}
	
	if( sampleCount == 0 ){
		return;
	}
	
	if( bytesPerSample == 1 ){
		if( channelCount == 1 ){
			pValid = true;
			
		}else if( channelCount == 2 ){
			pValid = true;
		}
		
	}else if( bytesPerSample == 2 ){
		if( channelCount == 1 ){
			pValid = true;
			
		}else if( channelCount == 2 ){
			pValid = true;
		}
	}
}
