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

#include "igdeGDPreviewListener.h"
#include "igdeGDPreviewManager.h"
#include "creators/igdeGDPCObjectClass.h"
#include "creators/igdeGDPCSkin.h"
#include "creators/igdeGDPCSky.h"
#include "../igdeGameDefinition.h"
#include "../class/igdeGDClass.h"
#include "../skin/igdeGDSkin.h"
#include "../sky/igdeGDSky.h"
#include "../../environment/igdeEnvironment.h"
#include "../../engine/igdeEngineController.h"
#include "../../gameproject/igdeGameProject.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageManager.h>



// Definitions
////////////////

#define LOGSOURCE "IGDE"



// Class igdeGDPreviewManager
///////////////////////////////

// Constructor, destructor
////////////////////////////

igdeGDPreviewManager::igdeGDPreviewManager( igdeEnvironment &environment ) :
pEnvironment( environment ),
pPathCache( "/igde/cache/preview" ),
pImageSize( 256 ), // large=128, medium=96, small=64
pHasCreators( false ){
}

igdeGDPreviewManager::~igdeGDPreviewManager(){
	OnBeforeEngineStop();
}



// Management
///////////////

void igdeGDPreviewManager::SetPathCache( const char *path ){
	pPathCache = path;
}

void igdeGDPreviewManager::SetDirectoryObjectClass( const char *directory ){
	pDirObjectClass = directory;
}

void igdeGDPreviewManager::SetDirectorySkin( const char *directory ){
	pDirSkin = directory;
}

void igdeGDPreviewManager::SetDirectorySky( const char *directory ){
	pDirSky = directory;
}



void igdeGDPreviewManager::OnAfterEngineStart(){
	if( pImageCreating || pImageFailed ){
		DETHROW( deeInvalidParam );
	}
	
	deImageManager &imgmgr = *pEnvironment.GetEngineController()->GetEngine()->GetImageManager();
	deVirtualFileSystem * const vfs = pEnvironment.GetFileSystemIGDE();
	
	try{
		pImageCreating.TakeOver( imgmgr.LoadImage( vfs, "/data/data/models/previewBuilder/rendering.png", "/" ) );
		pImageFailed.TakeOver( imgmgr.LoadImage( vfs, "/data/data/models/previewBuilder/failed.png", "/" ) );
		
	}catch( const deException & ){
		OnBeforeEngineStop();
		throw;
	}
}

void igdeGDPreviewManager::OnBeforeEngineStop(){
	AbortAllCreators();
}

void igdeGDPreviewManager::Update(){
	if( ! pHasCreators ){
		return;
	}
	
	// update object classes
	int count = pCreatorsObjectClass.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		igdeGDPCObjectClass &creator = *( ( igdeGDPCObjectClass* )pCreatorsObjectClass.GetAt( i ) );
		if( ! creator.FinishCreation() ){
			continue;
		}
		
		if( creator.GetImage() ){
			creator.GetGDClass()->SetPreviewImage( creator.GetImage() );
			pSaveToFile( creator.GetImage(), pDirObjectClass, creator.GetGDClass()->GetName() );
			
		}else{
			creator.GetGDClass()->SetPreviewImage( pImageFailed );
		}
		
		pCreatorsObjectClass.RemoveFrom( i );
		i--;
		count--;
	}
	
	// update skins
	count = pCreatorsSkin.GetCount();
	
	for( i=0; i<count; i++ ){
		igdeGDPCSkin &creator = *( ( igdeGDPCSkin* )pCreatorsSkin.GetAt( i ) );
		if( ! creator.FinishCreation() ){
			continue;
		}
		
		if( creator.GetImage() ){
			creator.GetGDSkin()->SetPreviewImage( creator.GetImage() );
			pSaveToFile( creator.GetImage(), pDirSkin, creator.GetGDSkin()->GetName() );
			
		}else{
			creator.GetGDSkin()->SetPreviewImage( pImageFailed );
		}
		
		pCreatorsSkin.RemoveFrom( i );
		i--;
		count--;
	}
	
	// update skies
	count = pCreatorsSky.GetCount();
	for( i=0; i<count; i++ ){
		igdeGDPCSky &creator = *( ( igdeGDPCSky* )pCreatorsSky.GetAt( i ) );
		if( ! creator.FinishCreation() ){
			continue;
		}
		
		if( creator.GetImage() ){
			creator.GetGDSky()->SetPreviewImage( creator.GetImage() );
			pSaveToFile( creator.GetImage(), pDirSky, creator.GetGDSky()->GetName() );
			
		}else{
			creator.GetGDSky()->SetPreviewImage( pImageFailed );
		}
		
		pCreatorsSky.RemoveFrom( i );
		i--;
		count--;
	}
	
	// check if still some creators are left
	pHasCreators = pCreatorsObjectClass.GetCount() > 0
		|| pCreatorsSkin.GetCount() > 0
		|| pCreatorsSky.GetCount() > 0;
}

void igdeGDPreviewManager::AbortAllCreators(){
	if( ! pHasCreators ){
		return;
	}
	
	// update object classes
	int count = pCreatorsObjectClass.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		( ( igdeGDPCObjectClass* )pCreatorsObjectClass.GetAt( i ) )->AbortCreation();
	}
	pCreatorsObjectClass.RemoveAll();
	
	// update skins
	count = pCreatorsSkin.GetCount();
	for( i=0; i<count; i++ ){
		( ( igdeGDPCSkin* )pCreatorsSkin.GetAt( i ) )->AbortCreation();
	}
	pCreatorsSkin.RemoveAll();
	
	// update skies
	count = pCreatorsSky.GetCount();
	for( i=0; i<count; i++ ){
		( ( igdeGDPCSky* )pCreatorsSky.GetAt( i ) )->AbortCreation();
	}
	pCreatorsSky.RemoveAll();
	
	pHasCreators = false;
}



deImage *igdeGDPreviewManager::GetPreviewObjectClass( igdeGDClass *gdclass ) const{
	if( ! gdclass ){
		DETHROW( deeInvalidParam );
	}
	
	if( gdclass->GetPreviewImage() ){
		return gdclass->GetPreviewImage();
	}
	
	deImage::Ref image;
	pLoadFromFile( image, pDirObjectClass, gdclass->GetName() );
	gdclass->SetPreviewImage( image );
	return image;
}

void igdeGDPreviewManager::CreatePreviewObjectClass( igdeGDClass *gdclass, igdeGDPreviewListener *listener ){
	if( ! gdclass || ! listener ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pCreatorsObjectClass.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		igdeGDPCObjectClass &creator = *( ( igdeGDPCObjectClass* )pCreatorsObjectClass.GetAt( i ) );
		if( creator.GetGDClass() == gdclass ){
			creator.AddListener( listener );
			return;
		}
	}
	
	deObjectReference creator;
	creator.TakeOver( new igdeGDPCObjectClass( pEnvironment, gdclass, decPoint( pImageSize, pImageSize ) ) );
	( ( igdeGDPCObjectClass& )( deObject& )creator ).AddListener( listener );
	( ( igdeGDPCObjectClass& )( deObject& )creator ).BeginCreation();
	pCreatorsObjectClass.Add( creator );
	
	pHasCreators = true;
}

void igdeGDPreviewManager::ClearPreviewObjectClass( igdeGDClass *gdclass ){
	if( ! gdclass ){
		DETHROW( deeInvalidParam );
	}
	
	gdclass->SetPreviewImage( NULL );
	
	pDeleteFile( pDirObjectClass, gdclass->GetName() );
}



deImage *igdeGDPreviewManager::GetPreviewSkin( igdeGDSkin *gdskin ) const{
	if( ! gdskin ){
		DETHROW( deeInvalidParam );
	}
	
	if( gdskin->GetPreviewImage() ){
		return gdskin->GetPreviewImage();
	}
	
	deImage::Ref image;
	pLoadFromFile( image, pDirSkin, gdskin->GetName() );
	gdskin->SetPreviewImage( image );
	return image;
}

void igdeGDPreviewManager::CreatePreviewSkin( igdeGDSkin *gdskin, igdeGDPreviewListener *listener ){
	if( ! gdskin || ! listener ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pCreatorsSkin.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		igdeGDPCSkin &creator = *( ( igdeGDPCSkin* )pCreatorsSkin.GetAt( i ) );
		if( creator.GetGDSkin() == gdskin ){
			creator.AddListener( listener );
			return;
		}
	}
	
	deObjectReference creator;
	creator.TakeOver( new igdeGDPCSkin( pEnvironment, gdskin, decPoint( pImageSize, pImageSize ) ) );
	( ( igdeGDPCSkin& )( deObject& )creator ).AddListener( listener );
	( ( igdeGDPCSkin& )( deObject& )creator ).BeginCreation();
	pCreatorsSkin.Add( creator );
	
	pHasCreators = true;
}

void igdeGDPreviewManager::ClearPreviewSkin( igdeGDSkin *gdskin ){
	if( ! gdskin ){
		DETHROW( deeInvalidParam );
	}
	
	gdskin->SetPreviewImage( NULL );
	
	pDeleteFile( pDirSkin, gdskin->GetName() );
}



deImage *igdeGDPreviewManager::GetPreviewSky( igdeGDSky *gdsky ) const{
	if( ! gdsky ){
		DETHROW( deeInvalidParam );
	}
	
	if( gdsky->GetPreviewImage() ){
		return gdsky->GetPreviewImage();
	}
	
	deImage::Ref image;
	pLoadFromFile( image, pDirSky, gdsky->GetName() );
	gdsky->SetPreviewImage( image );
	return image;
}

void igdeGDPreviewManager::CreatePreviewSky( igdeGDSky *gdsky, igdeGDPreviewListener *listener ){
	if( ! gdsky || ! listener ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pCreatorsSky.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		igdeGDPCSky &creator = *( ( igdeGDPCSky* )pCreatorsSky.GetAt( i ) );
		if( creator.GetGDSky() == gdsky ){
			creator.AddListener( listener );
			return;
		}
	}
	
	deObjectReference creator;
	creator.TakeOver( new igdeGDPCSky( pEnvironment, gdsky, decPoint( pImageSize, pImageSize ) ) );
	( ( igdeGDPCSky& )( deObject& )creator ).AddListener( listener );
	( ( igdeGDPCSky& )( deObject& )creator ).BeginCreation();
	pCreatorsSky.Add( creator );
	
	pHasCreators = true;
}

void igdeGDPreviewManager::ClearPreviewSky( igdeGDSky *gdsky ){
	if( ! gdsky ){
		DETHROW( deeInvalidParam );
	}
	
	gdsky->SetPreviewImage( NULL );
	
	pDeleteFile( pDirSky, gdsky->GetName() );
}



// Private Functions
//////////////////////

void igdeGDPreviewManager::pLoadFromFile( deImage::Ref &image, const decString &typedir, const decString &filename ) const{
	image = NULL;
	
	deVirtualFileSystem * const vfs = pEnvironment.GetFileSystemGame();
	decPath path;
	path.SetFromUnix( pPathCache );
	path.AddComponent( typedir );
	path.AddComponent( filename + ".png" );
	
	deEngine &engine = *pEnvironment.GetEngineController()->GetEngine();
	if( ! vfs->ExistsFile( path ) ){
		return;
	}
	
	try{
		image.TakeOver( engine.GetImageManager()->LoadImage( vfs, path.GetPathUnix(), "/" ) );
		if( image->GetBitCount() != 8 ){
			image = NULL;
		}
		
	}catch( const deException &e ){
		e.PrintError();
	}
}

void igdeGDPreviewManager::pSaveToFile( deImage *image, const decString &typedir, const decString &filename ) const{
	if( ! image ){
		DETHROW( deeInvalidParam );
	}
	
	deVirtualFileSystem * const vfs = pEnvironment.GetFileSystemGame();
	decPath path;
	path.SetFromUnix( pPathCache );
	path.AddComponent( typedir );
	path.AddComponent( filename + ".png" );
	
	deEngine &engine = *pEnvironment.GetEngineController()->GetEngine();
	try{
		engine.GetImageManager()->SaveImage( vfs, image, path.GetPathUnix() );
		
	}catch( const deException & ){
		pEnvironment.GetLogger()->LogWarnFormat( LOGSOURCE,
			"Can not save preview image to cache file '%s'", path.GetPathUnix().GetString() );
	}
}

void igdeGDPreviewManager::pDeleteFile( const decString &typedir, const decString &filename ) const{
	deVirtualFileSystem * const vfs = pEnvironment.GetFileSystemGame();
	decPath path;
	path.SetFromUnix( pPathCache );
	path.AddComponent( typedir );
	path.AddComponent( filename + ".png" );
	
	if( ! vfs->ExistsFile( path ) ){
		return;
	}
	
	try{
		vfs->DeleteFile( path );
		
	}catch( const deException &e ){
		e.PrintError();
	}
}
