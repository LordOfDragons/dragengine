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
deFileResourceManager(engine, ertVideo),

pDecoderRoot(NULL),
pDecoderTail(NULL),
pDecoderCount(0),

pAudioDecoderRoot(NULL),
pAudioDecoderTail(NULL),
pAudioDecoderCount(0)
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
	return video && !video->GetOutdated() ? video : NULL;
}

deVideo *deVideoManager::LoadVideo(const char *filename, const char *basePath, bool asynchron){
	return LoadVideo(GetEngine()->GetVirtualFileSystem(), filename, basePath, asynchron);
}

deVideo *deVideoManager::LoadVideo(deVirtualFileSystem *vfs, const char *filename,
const char *basePath, bool asynchron){
	if(!vfs || !filename){
		DETHROW(deeInvalidParam);
	}
	
	decBaseFileReader::Ref fileReader;
	deVideo::Ref video = NULL;
	decPath path;
	
	try{
		if(!FindFileForReading(path, *vfs, filename, basePath)){
			DETHROW_INFO(deeFileNotFound, filename);
		}
		const TIME_SYSTEM modificationTime = vfs->GetFileModificationTime(path);
		
		deVideo::Ref findVideo = (deVideo*)pVideos.GetWithFilename(vfs, path.GetPathUnix());
		
		if(findVideo && findVideo->GetModificationTime() != modificationTime){
			LogInfoFormat("Video '%s' (base path '%s') changed on VFS: Outdating and Reloading",
				filename, basePath ? basePath : "");
			findVideo->MarkOutdated();
			findVideo = NULL;
		}
		
		if(findVideo){
			video = findVideo;
			
		}else{
			deBaseVideoModule * const module = (deBaseVideoModule*)GetModuleSystem()
				->GetModuleAbleToLoad(deModuleSystem::emtVideo, path.GetPathUnix());
			
			fileReader.TakeOver(OpenFileForReading(*vfs, path.GetPathUnix()));
			deBaseVideoInfo videoInfo;
			module->InitLoadVideo(fileReader, videoInfo);
			
			video.TakeOver(new deVideo(this, vfs, path.GetPathUnix(), modificationTime,
				videoInfo.GetWidth(), videoInfo.GetHeight(), videoInfo.GetComponentCount(),
				videoInfo.GetBitCount(), videoInfo.GetFrameRate(), videoInfo.GetFrameCount(),
				videoInfo.GetColorConversionMatrix(), videoInfo.GetBytesPerSample(),
				videoInfo.GetSampleCount(), videoInfo.GetSampleRate(), videoInfo.GetChannelCount()));
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

void deVideoManager::SaveVideo(deVideo::Ref video, const char *filename){
	SaveVideo(GetEngine()->GetVirtualFileSystem(), video, filename);
}

void deVideoManager::SaveVideo(deVirtualFileSystem *vfs, deVideo::Ref video, const char *filename){
	if(!video || !vfs || !filename){
		DETHROW(deeInvalidParam);
	}
	
	decBaseFileWriter::Ref fileWriter = NULL;
	
	try{
		deBaseVideoModule * const module = (deBaseVideoModule*)GetModuleSystem()
			->GetModuleAbleToLoad(deModuleSystem::emtVideo, filename);
		
		fileWriter = OpenFileForWriting(*vfs, filename);
		module->SaveVideo(*fileWriter, *video);
	}catch(const deException &){
		LogErrorFormat("Saving video '%s' failed", filename);
		throw;
	}
}

void deVideoManager::AddLoadedVideo(deVideo::Ref video){
	if(!video){
		DETHROW(deeInvalidParam);
	}
	
	pVideos.Add(video);
}



deVideoDecoder *deVideoManager::CreateDecoder(deVideo::Ref video){
	if(!video || !video->GetVirtualFileSystem()){
		DETHROW(deeInvalidParam);
	}
	
	deVideoDecoder::Ref videoDecoder = NULL;
	deBaseVideoDecoder *peer = NULL;
	decBaseFileReader::Ref reader;
	deBaseVideoModule *module = NULL;
	
	try{
		videoDecoder.TakeOver(new deVideoDecoder(*this, video));
		
		module = (deBaseVideoModule*)GetModuleSystem()->GetModuleAbleToLoad(
			deModuleSystem::emtVideo, video->GetFilename());
		
		reader.TakeOver(OpenFileForReading(*video->GetVirtualFileSystem(), video->GetFilename()));
		peer = module->CreateDecoder(reader);
		
		videoDecoder->SetPeerVideo(peer);
		
	}catch(const deException &e){
		if(peer){
			delete peer;
		}
		LogErrorFormat("Creating decoder for video '%s' failed",
			video->GetFilename().GetString());
		
		deErrorTrace &errorTrace = *GetEngine()->GetErrorTrace();
		errorTrace.AddAndSetIfEmpty(e.GetName(), NULL, e.GetFile(), e.GetLine());
		
		deErrorTracePoint &etp = *errorTrace.AddPoint(
			NULL, "deVideoManager::CreateDecoder", __LINE__);
		
		deErrorTraceValue &etv = *etp.AddValue("video", "<deVideo>");
		etv.AddSubValue("filename", video->GetFilename());
		if(module){
			etp.AddValue("module", module->GetLoadableModule().GetName());
		}
		
		throw;
	}
	
	// track video decoder and return it
	deMutexGuard guard(pMutexDecoder);
	
	if(pDecoderTail){
		pDecoderTail->SetLLManagerNext(videoDecoder);
		videoDecoder->SetLLManagerPrev(pDecoderTail);
		pDecoderTail = videoDecoder;
		
	}else{ // it can never happen that root is non-NULL if tail is NULL
		pDecoderRoot = videoDecoder;
		pDecoderTail = videoDecoder;
	}
	
	pDecoderCount++;
	
	return videoDecoder;
}

deVideoAudioDecoder *deVideoManager::CreateAudioDecoder(deVideo::Ref video){
	if(!video || !video->GetVirtualFileSystem()){
		DETHROW(deeInvalidParam);
	}
	
	deVideoAudioDecoder::Ref audioDecoder = NULL;
	deBaseVideoAudioDecoder *peer = NULL;
	decBaseFileReader::Ref reader;
	deBaseVideoModule *module = NULL;
	
	try{
		module = (deBaseVideoModule*)GetModuleSystem()->GetModuleAbleToLoad(
			deModuleSystem::emtVideo, video->GetFilename());
		
		reader.TakeOver(OpenFileForReading(*video->GetVirtualFileSystem(), video->GetFilename()));
		peer = module->CreateAudioDecoder(reader);
		if(!peer){
			return NULL;  // no audio or not supported
		}
		
		audioDecoder.TakeOver(new deVideoAudioDecoder(*this, video));
		audioDecoder->SetPeerVideo(peer);
		
	}catch(const deException &e){
		if(peer){
			delete peer;
		}
		LogErrorFormat("Creating decoder for video '%s' failed",
			video->GetFilename().GetString());
		
		deErrorTrace &errorTrace = *GetEngine()->GetErrorTrace();
		errorTrace.AddAndSetIfEmpty(e.GetName(), NULL, e.GetFile(), e.GetLine());
		
		deErrorTracePoint &etp = *errorTrace.AddPoint(
			NULL, "deVideoManager::CreateSoundDecoder", __LINE__);
		
		deErrorTraceValue &etv = *etp.AddValue("video", "<deVideo>");
		etv.AddSubValue("filename", video->GetFilename());
		if(module){
			etp.AddValue("module", module->GetLoadableModule().GetName());
		}
		
		throw;
	}
	
	// track video decoder and return it
	deMutexGuard guard(pMutexDecoder);
	
	if(pAudioDecoderTail){
		pAudioDecoderTail->SetLLManagerNext(audioDecoder);
		audioDecoder->SetLLManagerPrev(pAudioDecoderTail);
		pAudioDecoderTail = audioDecoder;
		
	}else{ // it can never happen that root is non-NULL if tail is NULL
		pAudioDecoderRoot = audioDecoder;
		pAudioDecoderTail = audioDecoder;
	}
	
	pAudioDecoderCount++;
	
	return audioDecoder;
}



void deVideoManager::ReleaseLeakingResources(){
	// decoders
	deMutexGuard guard(pMutexDecoder);
	
	if(pDecoderCount > 0){
		LogWarnFormat("%i leaking video decoders", pDecoderCount);
		
		while(pDecoderRoot){
			LogWarnFormat("- %s", pDecoderRoot->GetVideo()->GetFilename().IsEmpty()
				? "<temporary>" : pDecoderRoot->GetVideo()->GetFilename().GetString());
			pDecoderRoot->MarkLeaking();
			pDecoderRoot= pDecoderRoot->GetLLManagerNext();
		}
		
		pDecoderTail = NULL;
		pDecoderCount = 0;
	}
	
	if(pAudioDecoderCount > 0){
		LogWarnFormat("%i leaking video audio decoders", pAudioDecoderCount);
		
		while(pAudioDecoderRoot){
			LogWarnFormat("- %s", pAudioDecoderRoot->GetVideo()->GetFilename().IsEmpty()
				? "<temporary>" : pAudioDecoderRoot->GetVideo()->GetFilename().GetString());
			pAudioDecoderRoot->MarkLeaking();
			pAudioDecoderRoot= pAudioDecoderRoot->GetLLManagerNext();
		}
		
		pAudioDecoderTail = NULL;
		pAudioDecoderCount = 0;
	}
	
	guard.Unlock();
	
	// videos
	const int count = GetVideoCount();
	
	if(count > 0){
		const deVideo::Ref video = (const deVideo *)pVideos.GetRoot();
		
		LogWarnFormat("%i leaking videos", count);
		
		while(video){
			LogWarnFormat("- %s", video->GetFilename().IsEmpty() ? "<temporary>"
				: video->GetFilename().GetString());
			video = (const deVideo *)video->GetLLManagerNext();
		}
		
		pVideos.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// Systems Support
////////////////////

void deVideoManager::SystemGraphicLoad(){
	deVideo::Ref video = (deVideo*)pVideos.GetRoot();
	deGraphicSystem &grasys = *GetGraphicSystem();
	
	while(video){
		if(!video->GetPeerGraphic()){
			grasys.LoadVideo(video);
		}
		
		video = (deVideo*)video->GetLLManagerNext();
	}
}

void deVideoManager::SystemGraphicUnload(){
	deVideo::Ref video = (deVideo*)pVideos.GetRoot();
	
	while(video){
		video->SetPeerGraphic(NULL);
		video = (deVideo*)video->GetLLManagerNext();
	}
}



void deVideoManager::RemoveResource(deResource *resource){
	pVideos.RemoveIfPresent(resource);
}

void deVideoManager::RemoveDecoder(deVideoDecoder *decoder){
	if(!decoder){
		DETHROW(deeInvalidParam);
	}
	
	deMutexGuard guard(pMutexDecoder);
	
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
}

void deVideoManager::RemoveAudioDecoder(deVideoAudioDecoder *decoder){
	if(!decoder){
		DETHROW(deeInvalidParam);
	}
	
	deMutexGuard guard(pMutexDecoder);
	
	if(decoder != pAudioDecoderRoot && !decoder->GetLLManagerNext()
	&& !decoder->GetLLManagerPrev()){
		return;
	}
	
	if(pAudioDecoderCount == 0){
		DETHROW(deeInvalidParam);
	}
	
	if(decoder == pAudioDecoderTail){
		if(decoder->GetLLManagerNext()){
			DETHROW(deeInvalidParam);
		}
		
		pAudioDecoderTail = pAudioDecoderTail->GetLLManagerPrev();
		if(pAudioDecoderTail){
			pAudioDecoderTail->SetLLManagerNext(NULL);
		}
		
	}else{
		if(!decoder->GetLLManagerNext()){
			DETHROW(deeInvalidParam);
		}
		
		decoder->GetLLManagerNext()->SetLLManagerPrev(decoder->GetLLManagerPrev());
	}
	
	if(decoder == pAudioDecoderRoot){
		if(decoder->GetLLManagerPrev()){
			DETHROW(deeInvalidParam);
		}
		
		pAudioDecoderRoot = pAudioDecoderRoot->GetLLManagerNext();
		if(pAudioDecoderRoot){
			pAudioDecoderRoot->SetLLManagerPrev(NULL);
		}
		
	}else{
		if(!decoder->GetLLManagerPrev()){
			DETHROW(deeInvalidParam);
		}
		
		decoder->GetLLManagerPrev()->SetLLManagerNext(decoder->GetLLManagerNext());
	}
	
	decoder->SetLLManagerNext(NULL);
	decoder->SetLLManagerPrev(NULL);
	pAudioDecoderCount--;
	
	decoder->MarkLeaking();
}
