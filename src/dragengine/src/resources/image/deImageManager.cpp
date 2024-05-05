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

#include "deImage.h"
#include "deImageManager.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"
#include "../../common/file/decBaseFileReader.h"
#include "../../common/file/decBaseFileWriter.h"
#include "../../common/file/decPath.h"
#include "../../common/utils/decXpmImage.h"
#include "../../common/xpm/no_tex.xpm"
#include "../../filesystem/deVirtualFileSystem.h"
#include "../../systems/deGraphicSystem.h"
#include "../../systems/deModuleSystem.h"
#include "../../systems/modules/deLoadableModule.h"
#include "../../systems/modules/image/deBaseImageModule.h"
#include "../../systems/modules/image/deBaseImageInfo.h"



// definitions
#define IMAGE_NO_TEXTURE	"<no_texture>"



// Class deImageManager
/////////////////////////

// Constructor, destructor
////////////////////////////

deImageManager::deImageManager( deEngine *engine ) : deFileResourceManager( engine, ertImage ){
	SetLoggingName( "image" );
}

deImageManager::~deImageManager(){
	ReleaseLeakingResources();
}



// Management
///////////////

int deImageManager::GetImageCount() const{
	return pImages.GetCount();
}

deImage *deImageManager::GetRootImage() const{
	return ( deImage* )pImages.GetRoot();
}

deImage * deImageManager::GetImageWith( const char *filename ) const{
	return GetImageWith( GetEngine()->GetVirtualFileSystem(), filename );
}

deImage *deImageManager::GetImageWith( deVirtualFileSystem *vfs, const char *filename ) const{
	deImage * const image = ( deImage* )pImages.GetWithFilename( vfs, filename );
	return image && ! image->GetOutdated() ? image : NULL;
}

deImage *deImageManager::CreateImage( int width, int height, int depth, int componentCount, int bitCount ){
	deImage *image = NULL;
	
	try{
		image = new deImage( this, GetEngine()->GetVirtualFileSystem(), "",
			decDateTime::GetSystemTime(), width, height, depth, componentCount, bitCount );
		GetGraphicSystem()->LoadImage(image);
		
		pImages.Add(image);
		
	}catch( const deException & ){
		if( image ){
			image->FreeReference();
		}
		throw;
	}
	return image;
}

deImage *deImageManager::LoadImage( const char *filename, const char *basePath ){
	return LoadImage( GetEngine()->GetVirtualFileSystem(), filename, basePath );
}

deImage *deImageManager::LoadImage( deVirtualFileSystem *vfs, const char *filename, const char *basePath ){
	decBaseFileReader *fileReader = NULL;
	deBaseImageModule *module;
	deBaseImageInfo *imageInfos = NULL;
	deImage *image = NULL, *findImage;
	decPath path;
	
//	LogInfoFormat( "Loading '%s'", filename );
	try{
		// locate file
		if( ! FindFileForReading( path, *vfs, filename, basePath ) ){
			DETHROW_INFO( deeFileNotFound, filename );
		}
		const TIME_SYSTEM modificationTime = vfs->GetFileModificationTime( path );
		
		// check if the image with this filename already exists
		findImage = ( deImage* )pImages.GetWithFilename( vfs, path.GetPathUnix() );
		
		if( findImage && findImage->GetModificationTime() != modificationTime ){
			LogInfoFormat( "Image '%s' (base path '%s') changed on VFS: Outdating and Reloading",
				filename, basePath ? basePath : "" );
			findImage->MarkOutdated();
			findImage = NULL;
		}
		
		if( findImage ){
			findImage->AddReference();
			image = findImage;
			
		}else{
			// find the module able to handle this image file
			module = ( deBaseImageModule* )GetModuleSystem()->GetModuleAbleToLoad(
				deModuleSystem::emtImage, path.GetPathUnix() );
			
			// load the file with it
			fileReader = OpenFileForReading( *vfs, path.GetPathUnix());
			
			imageInfos = module->InitLoadImage( *fileReader );
			if( ! imageInfos ) DETHROW( deeInvalidParam );
			
			image = new deImage( this, vfs, path.GetPathUnix(), modificationTime,
				imageInfos->GetWidth(), imageInfos->GetHeight(), imageInfos->GetDepth(),
				imageInfos->GetComponentCount(), imageInfos->GetBitCount() );
			image->SetAsynchron( false );
			
			module->LoadImage( *fileReader, *image, *imageInfos );
			fileReader->FreeReference(); fileReader = NULL;
			delete imageInfos; imageInfos = NULL;
			
			// load into graphic system
			GetGraphicSystem()->LoadImage( image );
			
			// add image
			pImages.Add( image );
//			LogInfoFormat( "Loading '%s' succeeded", path->GetPath() );
		}
		
	}catch( const deException & ){
		LogErrorFormat( "Loading image '%s' (base path '%s') failed", filename, basePath ? basePath : "" );
		//LogException( e );
		if( fileReader ){
			fileReader->FreeReference();
		}
		if( image ){
			image->FreeReference();
		}
		if( imageInfos ) delete imageInfos;
		throw;
	}
	
	if( image ){
		image->SetAsynchron( false );
		image->PeersRetainImageData();
	}
	
	return image;
}

deImage *deImageManager::LoadDefault(){
	deImage *image = NULL, *findImage;
	decXpmImage *xpmImage = NULL;
	
	try{
		findImage = ( deImage* )pImages.GetWithFilename(
			GetEngine()->GetVirtualFileSystem(), IMAGE_NO_TEXTURE );
		if( findImage ){
			if( ! findImage->GetPeerGraphic() ){
				GetGraphicSystem()->LoadImage( findImage );
			}
			findImage->AddReference();
			image = findImage;
			
		}else{
			// load image
			xpmImage = new decXpmImage( no_tex_xpm, true );
			image = new deImage( this, GetEngine()->GetVirtualFileSystem(),
				IMAGE_NO_TEXTURE, decDateTime::GetSystemTime(), xpmImage );
			delete xpmImage; xpmImage = NULL;
			
			// load into graphic system
			GetGraphicSystem()->LoadImage( image );
			
			// add image
			pImages.Add( image );
		}
		
	}catch( const deException & ){
		if( image ){
			image->FreeReference();
		}
		if( xpmImage ) delete xpmImage;
		throw;
	}
	
	return image;
}

void deImageManager::SaveImage( deImage *image, const char *filename ){
	SaveImage( GetEngine()->GetVirtualFileSystem(), image, filename );
}

void deImageManager::SaveImage( deVirtualFileSystem *vfs, deImage *image, const char *filename ){
	if( ! image || ! vfs || ! filename ){
		DETHROW( deeInvalidParam );
	}
	decBaseFileWriter *fileWriter = NULL;
	deBaseImageModule *module;
	
	try{
		// find the module able to handle this image file
		module = ( deBaseImageModule* )GetModuleSystem()->GetModuleAbleToLoad(
			deModuleSystem::emtImage, filename );
		
		// save the file with it
		fileWriter = OpenFileForWriting( *vfs, filename );
		module->SaveImage( *fileWriter, *image );
		fileWriter->FreeReference(); fileWriter = NULL;
		
	}catch( const deException & ){
		if( fileWriter ){
			fileWriter->FreeReference();
		}
		LogErrorFormat( "Saving image '%s' failed", filename );
		throw;
	}
}

void deImageManager::AddLoadedImage( deImage *image ){
	if( ! image ){
		DETHROW( deeInvalidParam );
	}
	
	pImages.Add( image );
}



void deImageManager::ReleaseLeakingResources(){
	const int count = GetImageCount();
	
	if( count > 0 ){
		deImage *image = ( deImage* )pImages.GetRoot();
		
		LogWarnFormat( "%i leaking images", count );
		
		while( image ){
			LogWarnFormat( "- %s", image->GetFilename().IsEmpty() ? "<temporary>" : image->GetFilename().GetString() );
			image = ( deImage* )image->GetLLManagerNext();
		}
		
		pImages.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// System Peer Management
///////////////////////////

void deImageManager::SystemGraphicLoad(){
	deImage *image = ( deImage* )pImages.GetRoot();
	deGraphicSystem &graSys = *GetGraphicSystem();
	
	while( image ){
		if( ! image->GetPeerGraphic() ){
			image->RetainImageData();
			try{
				graSys.LoadImage( image );
				
			}catch( const deException & ){
				image->ReleaseImageData();
				throw;
			}
			image->ReleaseImageData();
		}
		image = ( deImage* )image->GetLLManagerNext();
	}
}

void deImageManager::SystemGraphicUnload(){
	deImage *image = ( deImage* )pImages.GetRoot();
	
	while( image ){
		image->SetPeerGraphic( NULL );
		image = ( deImage* )image->GetLLManagerNext();
	}
}



void deImageManager::RemoveResource( deResource *resource ){
	pImages.RemoveIfPresent( resource );
}
