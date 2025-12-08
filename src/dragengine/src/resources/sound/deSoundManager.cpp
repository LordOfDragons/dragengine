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
#include <string.h>

#include "deSound.h"
#include "deSoundDecoder.h"
#include "deSoundManager.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"
#include "../../common/file/decBaseFileReader.h"
#include "../../common/file/decBaseFileWriter.h"
#include "../../common/file/decPath.h"
#include "../../errortracing/deErrorTrace.h"
#include "../../errortracing/deErrorTracePoint.h"
#include "../../errortracing/deErrorTraceValue.h"
#include "../../filesystem/deVirtualFileSystem.h"
#include "../../systems/deAudioSystem.h"
#include "../../systems/deModuleSystem.h"
#include "../../systems/deSynthesizerSystem.h"
#include "../../systems/modules/deLoadableModule.h"
#include "../../systems/modules/sound/deBaseSoundModule.h"
#include "../../systems/modules/sound/deBaseSoundInfo.h"
#include "../../systems/modules/sound/deBaseSoundDecoder.h"
#include "../../systems/modules/audio/deBaseAudioSound.h"
#include "../../systems/modules/synthesizer/deBaseSynthesizerSound.h"



// Class deSoundManager
/////////////////////////

// Constructor, destructor
////////////////////////////

deSoundManager::deSoundManager(deEngine *engine) :
deFileResourceManager(engine, ertSound),

pDecoderRoot(NULL),
pDecoderTail(NULL),
pDecoderCount(0)
{
	SetLoggingName("sound");
}

deSoundManager::~deSoundManager(){
	ReleaseLeakingResources();
}



// Management
///////////////

int deSoundManager::GetSoundCount() const{
	return pSounds.GetCount();
}

deSound *deSoundManager::GetRootSound() const{
	return (deSound*)pSounds.GetRoot();
}

deSound *deSoundManager::GetSoundWith(const char *filename) const{
	return GetSoundWith(GetEngine()->GetVirtualFileSystem(), filename);
}

deSound *deSoundManager::GetSoundWith(deVirtualFileSystem *vfs, const char *filename) const{
	deSound * const sound = (deSound*)pSounds.GetWithFilename(vfs, filename);
	return sound && !sound->GetOutdated() ? sound : NULL;
}

deSound *deSoundManager::LoadSound(const char *filename, const char *basePath, bool asynchron){
	return LoadSound(GetEngine()->GetVirtualFileSystem(), filename, basePath, asynchron);
}

deSound *deSoundManager::LoadSound(deVirtualFileSystem *vfs, const char *filename,
const char *basePath, bool asynchron){
	if(!vfs || !filename){
		DETHROW(deeInvalidParam);
	}
	decBaseFileReader::Ref fileReader = NULL;
	deBaseSoundInfo soundInfo;
	deBaseSoundModule *module;
	deSound::Ref sound = NULL;
	decPath path;
	deSound::Ref findSound;
	
	try{
		// locate file
		if(!FindFileForReading(path, *vfs, filename, basePath)){
			DETHROW_INFO(deeFileNotFound, filename);
		}
		const TIME_SYSTEM modificationTime = vfs->GetFileModificationTime(path);
		
		// check if the sound with this filename already exists
		findSound = (deSound*)pSounds.GetWithFilename(vfs, path.GetPathUnix());
		
		if(findSound && findSound->GetModificationTime() != modificationTime){
			LogInfoFormat("Sound '%s' (base path '%s') changed on VFS: Outdating and Reloading",
				filename, basePath ? basePath : "");
			findSound->MarkOutdated();
			findSound = NULL;
		}
		
		if(findSound){
			sound = findSound;
			
		}else{
			// find the module able to handle this sound file
			module = (deBaseSoundModule*)GetModuleSystem()->GetModuleAbleToLoad(
				deModuleSystem::emtSound, path.GetPathUnix());
			
			// open file and read sound infos
			fileReader = OpenFileForReading(*vfs, path.GetPathUnix());
			module->InitLoadSound(*fileReader, soundInfo);
			
			// create sound file using the infos
			sound.TakeOver(new deSound(this, vfs, path.GetPathUnix(), modificationTime,
				soundInfo.GetBytesPerSample(), soundInfo.GetSampleRate(),
				soundInfo.GetSampleCount(), soundInfo.GetChannelCount()));
			sound->SetAsynchron(asynchron);
			
			// clean up
			fileReader = NULL;
			
			// load into systems. modules can request to load the data if small enough
			GetAudioSystem()->LoadSound(sound);
			GetSynthesizerSystem()->LoadSound(sound);
			
			// add sound
			pSounds.Add(sound);
		}
		
	}catch(const deException &){
		LogErrorFormat("Loading sound '%s' (base path '%s') failed",
			filename, basePath ? basePath : "");
		throw;
	}
	
	if(sound){
		sound->SetAsynchron(false);
	}
	return sound;
}

void deSoundManager::SaveSound(deSound *sound, const char *filename){
	return SaveSound(GetEngine()->GetVirtualFileSystem(), sound, filename);
}

void deSoundManager::SaveSound(deVirtualFileSystem *vfs, deSound *sound, const char *filename){
	if(!sound || !vfs || !filename){
		DETHROW(deeInvalidParam);
	}
	decBaseFileWriter::Ref fileWriter = NULL;
	deBaseSoundModule *module;
	
	try{
		// find the module able to handle this sound file
		module = (deBaseSoundModule*)GetModuleSystem()->GetModuleAbleToLoad(
			deModuleSystem::emtSound, filename);
		
		// save the file with it
		fileWriter = OpenFileForWriting(*vfs, filename);
		module->SaveSound(*fileWriter, *sound);
	}catch(const deException &){
		throw;
	}
}

void deSoundManager::AddLoadedSound(deSound *sound){
	if(!sound){
		DETHROW(deeInvalidParam);
	}
	
	pSounds.Add(sound);
}



deSoundDecoder *deSoundManager::CreateDecoder(deSound *sound){
	if(!sound || !sound->GetVirtualFileSystem()){
		DETHROW(deeInvalidParam);
	}
	
	deSoundDecoder::Ref soundDecoder = NULL;
	deBaseSoundDecoder *peer = NULL;
	decBaseFileReader::Ref reader = NULL;
	deBaseSoundModule *module = NULL;
	
	try{
		// sound decoder is what the user actually gets
		soundDecoder.TakeOver(new deSoundDecoder(*this, sound));
		
		// find module which handles the sound file. it is not so nice to check using
		// the file name again for the right module to choose but the result is correct
		module = (deBaseSoundModule*)GetModuleSystem()->GetModuleAbleToLoad(
			deModuleSystem::emtSound, sound->GetFilename());
		
		// open file and create decoder peer
		reader = OpenFileForReading(*sound->GetVirtualFileSystem(), sound->GetFilename());
		peer = module->CreateDecoder(reader);
		reader = NULL;
		
		// assign the peer
		soundDecoder->SetPeerSound(peer);
		
	}catch(const deException &e){
		if(peer){
			delete peer;
		}
		LogErrorFormat("Creating decoder for sound '%s' failed",
			sound->GetFilename().GetString());
		
		deErrorTrace &errorTrace = *GetEngine()->GetErrorTrace();
		errorTrace.AddAndSetIfEmpty(e.GetName(), NULL, e.GetFile(), e.GetLine());
		
		deErrorTracePoint &etp = *errorTrace.AddPoint(
			NULL, "deSoundManager::CreateDecoder", __LINE__);
		
		deErrorTraceValue &etv = *etp.AddValue("sound", "<deSound>");
		etv.AddSubValue("filename", sound->GetFilename());
		etv.AddSubValueInt("bytesPerSample", sound->GetBytesPerSample());
		etv.AddSubValueInt("sampleRate", sound->GetSampleRate());
		etv.AddSubValueInt("channelCount", sound->GetChannelCount());
		etv.AddSubValueInt("sampleCount", sound->GetSampleCount());
		etv.AddSubValueFloat("playTime", sound->GetPlayTime());
		
		if(module){
			etp.AddValue("module", module->GetLoadableModule().GetName());
		}
		
		throw;
	}
	
	// track sound decoder and return it
	pMutexDecoder.Lock();
	
	if(pDecoderTail){
		pDecoderTail->SetLLManagerNext(soundDecoder);
		soundDecoder->SetLLManagerPrev(pDecoderTail);
		pDecoderTail = soundDecoder;
		
	}else{ // it can never happen that root is non-NULL if tail is NULL
		pDecoderRoot = soundDecoder;
		pDecoderTail = soundDecoder;
	}
	
	pDecoderCount++;
	
	pMutexDecoder.Unlock();
	
	return soundDecoder;
}



void deSoundManager::ReleaseLeakingResources(){
	// decoders
	pMutexDecoder.Lock();
	
	if(pDecoderCount > 0){
		try{
			LogWarnFormat("%i leaking sound decoders", pDecoderCount);
			
			while(pDecoderRoot){
				LogWarnFormat("- %s", pDecoderRoot->GetSound()->GetFilename().IsEmpty()
					? "<temporary>" : pDecoderRoot->GetSound()->GetFilename().GetString());
				pDecoderRoot->MarkLeaking();
				pDecoderRoot= pDecoderRoot->GetLLManagerNext();
			}
			
			pDecoderTail = NULL;
			pDecoderCount = 0;
			
		}catch(const deException &){
			pMutexDecoder.Unlock();
			throw;
		}
	}
	
	pMutexDecoder.Unlock();
	
	// sounds
	const int count = GetSoundCount();
	
	if(count > 0){
		const deSound::Ref sound = (const deSound *)pSounds.GetRoot();
		
		LogWarnFormat("%i leaking sounds", count);
		
		while(sound){
			LogWarnFormat("- %s", sound->GetFilename().IsEmpty()
				? "<temporary>" : sound->GetFilename().GetString());
			sound = (const deSound *)sound->GetLLManagerNext();
		}
		
		pSounds.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// Systems Support
////////////////////

void deSoundManager::SystemAudioLoad(){
	deSound::Ref sound = (deSound*)pSounds.GetRoot();
	deAudioSystem &audSys = *GetAudioSystem();
	
	while(sound){
		if(!sound->GetPeerAudio()){
			audSys.LoadSound(sound);
		}
		
		sound = (deSound*)sound->GetLLManagerNext();
	}
}

void deSoundManager::SystemAudioUnload(){
	deSound::Ref sound = (deSound*)pSounds.GetRoot();
	
	while(sound){
		sound->SetPeerAudio(NULL);
		sound = (deSound*)sound->GetLLManagerNext();
	}
}



void deSoundManager::SystemSynthesizerLoad(){
	deSound::Ref sound = (deSound*)pSounds.GetRoot();
	deSynthesizerSystem &synthSys = *GetSynthesizerSystem();
	
	while(sound){
		if(!sound->GetPeerSynthesizer()){
			synthSys.LoadSound(sound);
		}
		
		sound = (deSound*)sound->GetLLManagerNext();
	}
}

void deSoundManager::SystemSynthesizerUnload(){
	deSound::Ref sound = (deSound*)pSounds.GetRoot();
	
	while(sound){
		sound->SetPeerSynthesizer(NULL);
		sound = (deSound*)sound->GetLLManagerNext();
	}
}



void deSoundManager::RemoveResource(deResource *resource){
	pSounds.RemoveIfPresent(resource);
}

void deSoundManager::RemoveDecoder(deSoundDecoder *decoder){
	if(!decoder){
		DETHROW(deeInvalidParam);
	}
	
	pMutexDecoder.Lock();
	
	try{
		if(decoder != pDecoderRoot && !decoder->GetLLManagerNext()
		&& !decoder->GetLLManagerPrev()){
			return;
		}
		
		if(pDecoderCount == 0){
			DETHROW(deeInvalidParam);
		}
		
		if(decoder == pDecoderTail){
			if(decoder->GetLLManagerNext()){
				DETHROW(deeInvalidParam);
			}
			
			pDecoderTail = pDecoderTail->GetLLManagerPrev();
			if(pDecoderTail){
				pDecoderTail->SetLLManagerNext(NULL);
			}
			
		}else{
			if(!decoder->GetLLManagerNext()){
				DETHROW(deeInvalidParam);
			}
			
			decoder->GetLLManagerNext()->SetLLManagerPrev(decoder->GetLLManagerPrev());
		}
		
		if(decoder == pDecoderRoot){
			if(decoder->GetLLManagerPrev()){
				DETHROW(deeInvalidParam);
			}
			
			pDecoderRoot = pDecoderRoot->GetLLManagerNext();
			if(pDecoderRoot){
				pDecoderRoot->SetLLManagerPrev(NULL);
			}
			
		}else{
			if(!decoder->GetLLManagerPrev()){
				DETHROW(deeInvalidParam);
			}
			
			decoder->GetLLManagerPrev()->SetLLManagerNext(decoder->GetLLManagerNext());
		}
		
		decoder->SetLLManagerNext(NULL);
		decoder->SetLLManagerPrev(NULL);
		pDecoderCount--;
		
		decoder->MarkLeaking();
		
	}catch(const deException &){
		pMutexDecoder.Unlock();
		throw;
	}
	
	pMutexDecoder.Unlock();
}
