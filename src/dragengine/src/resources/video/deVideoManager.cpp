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

#include "deVideo.h"
#include "deVideoDecoder.h"
#include "deVideoAudioDecoder.h"
#include "deVideoManager.h"
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
#include "../../systems/deGraphicSystem.h"
#include "../../systems/deModuleSystem.h"
#include "../../systems/modules/deLoadableModule.h"
#include "../../systems/modules/video/deBaseVideoModule.h"
#include "../../systems/modules/video/deBaseVideoInfo.h"
#include "../../systems/modules/video/deBaseVideoDecoder.h"
#include "../../systems/modules/video/deBaseVideoAudioDecoder.h"



// Class deVideoManager
/////////////////////////

// Constructor, destructor
////////////////////////////

deVideoManager::deVideoManager(deEngine *engine) :
deFileResourceManager(engine, ertVideo)
{
	SetLoggingName("video");
}

deVideoManager::~deVideoManager(){
	ReleaseLeakingResources();
}



// Management
///////////////

int deVideoManager::GetVideoCount() const{
	return pVideos.GetCount();
}

deVideo *deVideoManager::GetRootVideo() const{
	return (deVideo*)pVideos.GetRoot();
}

deVideo *deVideoManager::GetVideoWith(const char *filename) const{
	return GetVideoWith(GetEngine()->GetVirtualFileSystem(), filename);
}

deVideo *deVideoManager::GetVideoWith(deVirtualFileSystem *vfs, const char *filename) const{
	deVideo * const video = (deVideo*)pVideos.GetWithFilename(vfs, filename);
	return video && !video->GetOutdated() ? video : nullptr;
}

deVideo::Ref deVideoManager::LoadVideo(const char *filename, const char *basePath, bool asynchron){
	return LoadVideo(GetEngine()->GetVirtualFileSystem(), filename, basePath, asynchron);
}

deVideo::Ref deVideoManager::LoadVideo(deVirtualFileSystem *vfs, const char *filename,
const char *basePath, bool asynchron){
	if(!vfs || !filename){
		DETHROW(deeInvalidParam);
	}
	
	deVideo::Ref video;
	decPath path;
	
	try{
		if(!FindFileForReading(path, *vfs, filename, basePath)){
			DETHROW_INFO(deeFileNotFound, filename);
		}
		const TIME_SYSTEM modificationTime = vfs->GetFileModificationTime(path);
		
		deVideo::Ref findVideo((deVideo*)pVideos.GetWithFilename(vfs, path.GetPathUnix()));
		
		if(findVideo && findVideo->GetModificationTime() != modificationTime){
			LogInfoFormat("Video '%s' (base path '%s') changed on VFS: Outdating and Reloading",
				filename, basePath ? basePath : "");
			findVideo->MarkOutdated();
			findVideo = nullptr;
		}
		
		if(findVideo){
			video = findVideo;
			
		}else{
			deBaseVideoModule * const module = (deBaseVideoModule*)GetModuleSystem()
				->GetModuleAbleToLoad(deModuleSystem::emtVideo, path.GetPathUnix());
			
				deBaseVideoInfo videoInfo;
			module->InitLoadVideo(OpenFileForReading(*vfs, path.GetPathUnix()), videoInfo);
			
			video = deVideo::Ref::New(this, vfs, path.GetPathUnix(), modificationTime,
				videoInfo.GetWidth(), videoInfo.GetHeight(), videoInfo.GetComponentCount(),
				videoInfo.GetBitCount(), videoInfo.GetFrameRate(), videoInfo.GetFrameCount(),
				videoInfo.GetColorConversionMatrix(), videoInfo.GetBytesPerSample(),
				videoInfo.GetSampleCount(), videoInfo.GetSampleRate(), videoInfo.GetChannelCount());
			video->SetAsynchron(asynchron);
			
			GetGraphicSystem()->LoadVideo(video);
			
			pVideos.Add(video);
		}
		
	}catch(const deException &){
		LogErrorFormat("Loading video '%s' (base path '%s') failed",
			filename, basePath ? basePath : "");
		throw;
	}
	
	video->SetAsynchron(false);
	return video;
}

void deVideoManager::SaveVideo(deVideo *video, const char *filename){
	SaveVideo(GetEngine()->GetVirtualFileSystem(), video, filename);
}

void deVideoManager::SaveVideo(deVirtualFileSystem *vfs, deVideo *video, const char *filename){
	if(!video || !vfs || !filename){
		DETHROW(deeInvalidParam);
	}
	
	try{
		deBaseVideoModule * const module = (deBaseVideoModule*)GetModuleSystem()
			->GetModuleAbleToLoad(deModuleSystem::emtVideo, filename);
		DEASSERT_NOTNULL(module);
		
		module->SaveVideo(OpenFileForWriting(*vfs, filename), *video);
	}catch(const deException &){
		LogErrorFormat("Saving video '%s' failed", filename);
		throw;
	}
}

void deVideoManager::AddLoadedVideo(deVideo *video){
	if(!video){
		DETHROW(deeInvalidParam);
	}
	
	pVideos.Add(video);
}



deVideoDecoder::Ref deVideoManager::CreateDecoder(deVideo *video){
	if(!video || !video->GetVirtualFileSystem()){
		DETHROW(deeInvalidParam);
	}
	
	deVideoDecoder::Ref videoDecoder;
	deBaseVideoModule *module = nullptr;
	deBaseVideoDecoder *peer = nullptr;
	
	try{
		videoDecoder = deVideoDecoder::Ref::New(*this, video);
		
		module = (deBaseVideoModule*)GetModuleSystem()->GetModuleAbleToLoad(
			deModuleSystem::emtVideo, video->GetFilename());
		DEASSERT_NOTNULL(module);
		
		peer = module->CreateDecoder(OpenFileForReading(
			*video->GetVirtualFileSystem(), video->GetFilename()));
		
		videoDecoder->SetPeerVideo(peer);
		
	}catch(const deException &e){
		if(peer){
			delete peer;
		}
		LogErrorFormat("Creating decoder for video '%s' failed",
			video->GetFilename().GetString());
		
		deErrorTrace &errorTrace = *GetEngine()->GetErrorTrace();
		errorTrace.AddAndSetIfEmpty(e.GetName(), nullptr, e.GetFile(), e.GetLine());
		
		deErrorTracePoint &etp = errorTrace.AddPoint(
			nullptr, "deVideoManager::CreateDecoder", __LINE__);
		
		deErrorTraceValue &etv = etp.AddValue("video", "<deVideo>");
		etv.AddSubValue("filename", video->GetFilename());
		if(module){
			etp.AddValue("module", module->GetLoadableModule().GetName());
		}
		
		throw;
	}
	
	// track video decoder and return it
	deMutexGuard guard(pMutexDecoder);
	pDecoders.Add(&videoDecoder->GetLLManager());
	return videoDecoder;
}

deVideoAudioDecoder::Ref deVideoManager::CreateAudioDecoder(deVideo *video){
	if(!video || !video->GetVirtualFileSystem()){
		DETHROW(deeInvalidParam);
	}
	
	deVideoAudioDecoder::Ref audioDecoder;
	deBaseVideoAudioDecoder *peer = nullptr;
	deBaseVideoModule *module = nullptr;
	
	try{
		module = (deBaseVideoModule*)GetModuleSystem()->GetModuleAbleToLoad(
			deModuleSystem::emtVideo, video->GetFilename());
		DEASSERT_NOTNULL(module);
		
		peer = module->CreateAudioDecoder(OpenFileForReading(
			*video->GetVirtualFileSystem(), video->GetFilename()));
		if(!peer){
			return deVideoAudioDecoder::Ref(); // no audio or not supported
		}
		
		audioDecoder = deVideoAudioDecoder::Ref::New(*this, video);
		audioDecoder->SetPeerVideo(peer);
		
	}catch(const deException &e){
		if(peer){
			delete peer;
		}
		LogErrorFormat("Creating decoder for video '%s' failed",
			video->GetFilename().GetString());
		
		deErrorTrace &errorTrace = *GetEngine()->GetErrorTrace();
		errorTrace.AddAndSetIfEmpty(e.GetName(), nullptr, e.GetFile(), e.GetLine());
		
		deErrorTracePoint &etp = errorTrace.AddPoint(
			nullptr, "deVideoManager::CreateSoundDecoder", __LINE__);
		
		deErrorTraceValue &etv = etp.AddValue("video", "<deVideo>");
		etv.AddSubValue("filename", video->GetFilename());
		if(module){
			etp.AddValue("module", module->GetLoadableModule().GetName());
		}
		
		throw;
	}
	
	// track video decoder and return it
	deMutexGuard guard(pMutexDecoder);
	pAudioDecoders.Add(&audioDecoder->GetLLManager());
	return audioDecoder;
}



void deVideoManager::ReleaseLeakingResources(){
	// decoders
	{
	const deMutexGuard guard(pMutexDecoder);
	
	if(pDecoders.GetCount() > 0){
		LogWarnFormat("%i leaking video decoders", pDecoders.GetCount());
		
		pDecoders.Visit([&](deVideoDecoder *decoder){
			LogWarnFormat("- %s", decoder->GetVideo()->GetFilename().IsEmpty()
				? "<temporary>" : decoder->GetVideo()->GetFilename().GetString());
			decoder->MarkLeaking();
		});
		pDecoders.RemoveAll();
	}
	
	if(pAudioDecoders.GetCount() > 0){
		LogWarnFormat("%i leaking video audio decoders", pAudioDecoders.GetCount());
		
		pAudioDecoders.Visit([&](deVideoAudioDecoder *decoder){
			LogWarnFormat("- %s", decoder->GetVideo()->GetFilename().IsEmpty()
				? "<temporary>" : decoder->GetVideo()->GetFilename().GetString());
			decoder->MarkLeaking();
		});
		pAudioDecoders.RemoveAll();
	}
	}
	
	// videos
	const int count = GetVideoCount();
	
	if(count > 0){
		LogWarnFormat("%i leaking videos", count);
		
		pVideos.GetResources().Visit([&](deResource *res){
			const deVideo *video = static_cast<const deVideo*>(res);
			LogWarnFormat("- %s", video->GetFilename().IsEmpty() ? "<temporary>"
				: video->GetFilename().GetString());
		});
		
		pVideos.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// Systems Support
////////////////////

void deVideoManager::SystemGraphicLoad(){
	deGraphicSystem &graSys = *GetGraphicSystem();
	
	pVideos.GetResources().Visit([&](deResource *res){
		deVideo *video = static_cast<deVideo*>(res);
		if(!video->GetPeerGraphic()){
			graSys.LoadVideo(video);
		}
	});
}

void deVideoManager::SystemGraphicUnload(){
	pVideos.GetResources().Visit([](deResource *res){
		static_cast<deVideo*>(res)->SetPeerGraphic(nullptr);
	});
}


void deVideoManager::RemoveResource(deResource *resource){
	pVideos.RemoveIfPresent(resource);
}

void deVideoManager::RemoveDecoder(deVideoDecoder *decoder){
	if(!decoder){
		DETHROW(deeInvalidParam);
	}
	
	deMutexGuard guard(pMutexDecoder);
	pDecoders.Remove(&decoder->GetLLManager());
	decoder->MarkLeaking();
}

void deVideoManager::RemoveAudioDecoder(deVideoAudioDecoder *decoder){
	if(!decoder){
		DETHROW(deeInvalidParam);
	}
	
	deMutexGuard guard(pMutexDecoder);
	pAudioDecoders.Remove(&decoder->GetLLManager());
	decoder->MarkLeaking();
}