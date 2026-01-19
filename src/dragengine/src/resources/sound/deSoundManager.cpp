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
#include "../../threading/deMutexGuard.h"
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
deFileResourceManager(engine, ertSound)
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
	return sound && !sound->GetOutdated() ? sound : nullptr;
}

deSound::Ref deSoundManager::LoadSound(const char *filename, const char *basePath, bool asynchron){
	return LoadSound(GetEngine()->GetVirtualFileSystem(), filename, basePath, asynchron);
}

deSound::Ref deSoundManager::LoadSound(deVirtualFileSystem *vfs, const char *filename,
const char *basePath, bool asynchron){
	if(!vfs || !filename){
		DETHROW(deeInvalidParam);
	}
	decBaseFileReader::Ref fileReader;
	deBaseSoundInfo soundInfo;
	deBaseSoundModule *module;
	deSound::Ref sound;
	decPath path;
	
	try{
		// locate file
		if(!FindFileForReading(path, *vfs, filename, basePath)){
			DETHROW_INFO(deeFileNotFound, filename);
		}
		const TIME_SYSTEM modificationTime = vfs->GetFileModificationTime(path);
		
		// check if the sound with this filename already exists
		deSound *findSound = (deSound*)pSounds.GetWithFilename(vfs, path.GetPathUnix());
		
		if(findSound && findSound->GetModificationTime() != modificationTime){
			LogInfoFormat("Sound '%s' (base path '%s') changed on VFS: Outdating and Reloading",
				filename, basePath ? basePath : "");
			findSound->MarkOutdated();
			findSound = nullptr;
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
			sound = deSound::Ref::New(this, vfs, path.GetPathUnix(), modificationTime,
				soundInfo.GetBytesPerSample(), soundInfo.GetSampleRate(),
				soundInfo.GetSampleCount(), soundInfo.GetChannelCount());
			sound->SetAsynchron(asynchron);
			
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
	
	deBaseSoundModule * const module = (deBaseSoundModule*)GetModuleSystem()->
		GetModuleAbleToLoad(deModuleSystem::emtSound, filename);
	DEASSERT_NOTNULL(module);
	
	module->SaveSound(OpenFileForWriting(*vfs, filename), *sound);
}

void deSoundManager::AddLoadedSound(deSound *sound){
	if(!sound){
		DETHROW(deeInvalidParam);
	}
	
	pSounds.Add(sound);
}



deSoundDecoder::Ref deSoundManager::CreateDecoder(deSound *sound){
	if(!sound || !sound->GetVirtualFileSystem()){
		DETHROW(deeInvalidParam);
	}
	
	deBaseSoundModule *module = nullptr;
	deBaseSoundDecoder *peer = nullptr;
	deSoundDecoder::Ref soundDecoder;
	
	try{
		// sound decoder is what the user actually gets
		soundDecoder = deSoundDecoder::Ref::New(*this, sound);
		
		// find module which handles the sound file. it is not so nice to check using
		// the file name again for the right module to choose but the result is correct
		module = (deBaseSoundModule*)GetModuleSystem()->GetModuleAbleToLoad(
			deModuleSystem::emtSound, sound->GetFilename());
		DEASSERT_NOTNULL(module);
		
		// open file and create decoder peer
		peer = module->CreateDecoder(OpenFileForReading(
			*sound->GetVirtualFileSystem(), sound->GetFilename()));
		
		// assign the peer
		soundDecoder->SetPeerSound(peer);
		
	}catch(const deException &e){
		if(peer){
			delete peer;
		}
		LogErrorFormat("Creating decoder for sound '%s' failed",
			sound->GetFilename().GetString());
		
		deErrorTrace &errorTrace = *GetEngine()->GetErrorTrace();
		errorTrace.AddAndSetIfEmpty(e.GetName(), nullptr, e.GetFile(), e.GetLine());
		
		deErrorTracePoint &etp = errorTrace.AddPoint(
			nullptr, "deSoundManager::CreateDecoder", __LINE__);
		
		deErrorTraceValue &etv = etp.AddValue("sound", "<deSound>");
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
	const deMutexGuard lock(pMutexDecoder);
	pDecoders.Add(&soundDecoder->GetLLManager());
	
	return soundDecoder;
}



void deSoundManager::ReleaseLeakingResources(){
	// decoders
	{
	const deMutexGuard lock(pMutexDecoder);
	if(pDecoders.GetCount() > 0){
		try{
			LogWarnFormat("%i leaking sound decoders", pDecoders.GetCount());
			
			pDecoders.Visit([&](deSoundDecoder *decoder){
				LogWarnFormat("- %s", decoder->GetSound()->GetFilename().IsEmpty()
					? "<temporary>" : decoder->GetSound()->GetFilename().GetString());
				decoder->MarkLeaking();
			});
			pDecoders.RemoveAll();
			
		}catch(const deException &){
			pMutexDecoder.Unlock();
			throw;
		}
	}
	}
	
	// sounds
	const int count = GetSoundCount();
	
	if(count > 0){
		LogWarnFormat("%i leaking sounds", count);
		
		// visit all resources and log them (use void returning lambda and C++ pointer casts)
		pSounds.GetResources().Visit([&](deResource *res){
			const deSound *sound = static_cast<const deSound*>(res);
			LogWarnFormat("- %s", sound->GetFilename().IsEmpty()
				? "<temporary>" : sound->GetFilename().GetString());
		});
		
		pSounds.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// Systems Support
////////////////////

void deSoundManager::SystemAudioLoad(){
	deAudioSystem &audSys = *GetAudioSystem();
	
	// visit all sounds and ensure audio peer is loaded
	pSounds.GetResources().Visit([&](deResource *res){
		deSound *sound = static_cast<deSound*>(res);
		if(!sound->GetPeerAudio()){
			audSys.LoadSound(sound);
		}
	});
}

void deSoundManager::SystemAudioUnload(){
	pSounds.GetResources().Visit([](deResource *res){
		static_cast<deSound*>(res)->SetPeerAudio(nullptr);
	});
}



void deSoundManager::SystemSynthesizerLoad(){
	deSynthesizerSystem &synthSys = *GetSynthesizerSystem();
	
	pSounds.GetResources().Visit([&](deResource *res){
		deSound *sound = static_cast<deSound*>(res);
		if(!sound->GetPeerSynthesizer()){
			synthSys.LoadSound(sound);
		}
	});
}

void deSoundManager::SystemSynthesizerUnload(){
	pSounds.GetResources().Visit([](deResource *res){
		static_cast<deSound*>(res)->SetPeerSynthesizer(nullptr);
	});
}



void deSoundManager::RemoveResource(deResource *resource){
	pSounds.RemoveIfPresent(resource);
}

void deSoundManager::RemoveDecoder(deSoundDecoder *decoder){
	if(!decoder){
		DETHROW(deeInvalidParam);
	}
	
	const deMutexGuard lock(pMutexDecoder);
	pDecoders.Remove(&decoder->GetLLManager());
	decoder->MarkLeaking();
}
