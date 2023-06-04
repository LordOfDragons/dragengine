/* 
 * Drag[en]gine IGDE World Editor
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "meHeightTerrain.h"
#include "meHeightTerrainSector.h"
#include "meHeightTerrainTexture.h"
#include "../meWorld.h"

#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageReference.h>
#include <dragengine/resources/image/deImageManager.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinManager.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrain.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrainSector.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrainTexture.h>



// Definitions
////////////////

#define LOGSOURCE "World Editor"



// Class meHeightTerrainTexture
/////////////////////////////////

// Constructor, destructor
////////////////////////////

meHeightTerrainTexture::meHeightTerrainTexture( deEngine *engine, const char *name ){
	if( ! engine || ! name ) DETHROW( deeInvalidParam );
	
	pEngine = engine;
	pEngTexture = NULL;
	
	pTypeNumber = 0;
	
	pSector = NULL;
	
	pProjScaling.Set( 1.0f, 1.0f );
	pProjRotation = 0.0f;
	
	pMaskChanged = false;
	pMaskSaved = false;
	
	try{
		pName = name;
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

meHeightTerrainTexture::~meHeightTerrainTexture(){
	pCleanUp();
}



// Management
///////////////

void meHeightTerrainTexture::SetSector( meHeightTerrainSector *sector ){
	if( sector == pSector ){
		return;
	}
	
	SetEngineTexture( NULL );
	
	pSector = sector;
	
	pLoadSkin();
	LoadMaskImage();
}

deHeightTerrainTexture *meHeightTerrainTexture::CreateEngineTexture() const{
	deHeightTerrainTexture *engTexture = NULL;
	
	try{
		engTexture = new deHeightTerrainTexture;
		engTexture->SetProjectionOffset( pProjOffset );
		engTexture->SetProjectionScaling( pProjScaling );
		engTexture->SetProjectionRotation( pProjRotation * DEG2RAD );
		engTexture->SetSkin( pSkin );
		engTexture->SetMaskImage( pMaskImage );
		
	}catch( const deException & ){
		if( engTexture ) delete engTexture;
		throw;
	}
	
	return engTexture;
}

void meHeightTerrainTexture::SetEngineTexture( deHeightTerrainTexture *engTexture ){
	pEngTexture = engTexture;
}



void meHeightTerrainTexture::SetName( const char *name ){
	pName = name;
}

void meHeightTerrainTexture::SetTypeNumber( int typeNumber ){
	if( typeNumber != pTypeNumber ){
		pTypeNumber = typeNumber;
		
		NotifyTextureChanged();
	}
}

void meHeightTerrainTexture::SetPathSkin( const char *path ){
	if( pPathSkin == path ){
		return;
	}
	
	pPathSkin = path;
	
	pLoadSkin();
	
	NotifyTextureChanged();
}



void meHeightTerrainTexture::SetProjectionOffsetU( float offset ){
	if( fabsf( offset - pProjOffset.x ) > 1e-5f ){
		pProjOffset.x = offset;
		
		if( pEngTexture ){
			pEngTexture->SetProjectionOffset( pProjOffset );
		}
		
		NotifyTextureChanged();
	}
}

void meHeightTerrainTexture::SetProjectionOffsetV( float offset ){
	if( fabsf( offset - pProjOffset.y ) > 1e-5f ){
		pProjOffset.y = offset;
		
		if( pEngTexture ){
			pEngTexture->SetProjectionOffset( pProjOffset );
		}
		
		NotifyTextureChanged();
	}
}

void meHeightTerrainTexture::SetProjectionScalingU( float scaling ){
	if( fabsf( scaling - pProjScaling.x ) > 1e-5f ){
		pProjScaling.x = scaling;
		
		if( pEngTexture ){
			pEngTexture->SetProjectionScaling( pProjScaling );
		}
		
		NotifyTextureChanged();
	}
}

void meHeightTerrainTexture::SetProjectionScalingV( float scaling ){
	if( fabsf( scaling - pProjScaling.y ) > 1e-5f ){
		pProjScaling.y = scaling;
		
		if( pEngTexture ){
			pEngTexture->SetProjectionScaling( pProjScaling );
		}
		
		NotifyTextureChanged();
	}
}

void meHeightTerrainTexture::SetProjectionRotation( float rotation ){
	if( fabsf( rotation - pProjRotation ) > 1e-5f ){
		pProjRotation = rotation;
		
		if( pEngTexture ){
			pEngTexture->SetProjectionRotation( rotation * DEG2RAD );
		}
		
		NotifyTextureChanged();
	}
}



void meHeightTerrainTexture::SetPathMask( const char *path, bool load ){
	if( ! path ) DETHROW( deeInvalidParam );
	
	if( ! pPathMask.Equals( path ) ){
		pPathMask = path;
		
		if( load ){
			LoadMaskImage();
			
		}else{
			SetMaskSaved( true );
		}
		SetMaskChanged( true );
		
		NotifyTextureMaskChanged();
	}
}

deImage *meHeightTerrainTexture::GetOrAddMaskImage(){
	if( pMaskImage ){
		return pMaskImage;
	}
	
	if( ! pSector || ! pSector->GetHeightTerrain() ){
		DETHROW( deeNullPointer );
	}
	
	const int imageDim =  pSector->GetHeightTerrain()->GetSectorResolution();
	deImageReference image;
	image.TakeOver( pEngine->GetImageManager()->CreateImage( imageDim, imageDim, 1, 1, 8 ) );
	sGrayscale8 * const pixels = image->GetDataGrayscale8();
	const int pixelCount = imageDim * imageDim;
	int i;
	for( i=0; i<pixelCount; i++ ){
		pixels[ i ].value = 255;
	}
	image->NotifyImageDataChanged();
	SetMaskImage( image );
	
	return image;
}

void meHeightTerrainTexture::SetMaskImage( deImage *image ){
	// TODO: sanity tests
	
	if( pMaskImage == image ){
		return;
	}
	
	if( pMaskImage ){
		pMaskImage->ReleaseImageData();
	}
	
	pMaskImage = image;
	
	if( pEngTexture ){
		pEngTexture->SetMaskImage( image );
	}
}

void meHeightTerrainTexture::LoadMaskImage(){
	if( pMaskImage ){
		pMaskImage->ReleaseImageData();
	}
	pMaskImage = NULL;
	
	if( pSector && pSector->GetHeightTerrain() && ! pPathMask.IsEmpty() ){
		const decString baseDir( pSector->GetHeightTerrain()->GetBaseDirectory() );
		if( pEngine->GetVirtualFileSystem()->ExistsFile( decPath::AbsolutePathUnix( pPathMask, baseDir ) ) ){
			try{
				pMaskImage.TakeOver( pEngine->GetImageManager()->LoadImage( pPathMask, baseDir ) );
				
			}catch( const deException &e ){
				pSector->GetHeightTerrain()->GetWorld().GetLogger()->LogException( LOGSOURCE, e );
			}
			
			if( pMaskImage->GetComponentCount() != 1 || pMaskImage->GetDepth() != 1 ){
				pMaskImage = NULL;
			}
		}
	}
	
	if( pMaskImage ){
		// we have to create a copy of the mask to allow editing it
		const deImageReference loadedImage( pMaskImage );
		loadedImage->RetainImageData();
		
		pMaskImage.TakeOver( pEngine->GetImageManager()->CreateImage( loadedImage->GetWidth(),
			loadedImage->GetHeight(), loadedImage->GetDepth(), loadedImage->GetComponentCount(),
			loadedImage->GetBitCount() ) );
		memcpy( pMaskImage->GetData(), loadedImage->GetData(), loadedImage->GetWidth()
			* loadedImage->GetHeight() * ( loadedImage->GetBitCount() / 8 ) );
		
		loadedImage->ReleaseImageData();
		
		pMaskImage->NotifyImageDataChanged();
	}
	
	if( pEngTexture ){
		pEngTexture->SetMaskImage( pMaskImage );
	}
	
	if( pMaskImage ){
		SetMaskSaved( true );
		
	}else{
		SetMaskSaved( false );
	}
	
	SetMaskChanged( false );
	
	NotifyTextureMaskChanged();
}

void meHeightTerrainTexture::SetMaskChanged( bool changed ){
	if( changed != pMaskChanged ){
		pMaskChanged = changed;
		
		if( changed && pSector && pSector->GetHeightTerrain() ){
			pSector->GetHeightTerrain()->SetDepChanged( true );
		}
	}
}

void meHeightTerrainTexture::SetMaskSaved( bool saved ){
	pMaskSaved = saved;
}

void meHeightTerrainTexture::NotifyTextureChanged(){
	if( pSector ){
		pSector->NotifySectorChanged();
		if( pSector->GetHeightTerrain() ){
			pSector->GetHeightTerrain()->GetWorld().NotifyHTSTextureChanged( pSector, this );
		}
	}
}

void meHeightTerrainTexture::NotifyTextureMaskChanged(){
	if( pSector ){
		pSector->NotifySectorChanged();
		if( pSector->GetHeightTerrain() ){
			pSector->GetHeightTerrain()->SetDepChanged( true );
			pSector->GetHeightTerrain()->GetWorld().NotifyHTSTextureMaskChanged( pSector, this );
		}
	}
}



// Private Functions
//////////////////////

void meHeightTerrainTexture::pCleanUp(){
}

void meHeightTerrainTexture::pLoadSkin(){
	pSkin = NULL;
	
	if( pSector && pSector->GetHeightTerrain() && ! pPathSkin.IsEmpty() ){
		const decString baseDir( pSector->GetHeightTerrain()->GetBaseDirectory() );
		if( pEngine->GetVirtualFileSystem()->ExistsFile( decPath::AbsolutePathUnix( pPathSkin, baseDir ) ) ){
			try{
				pSkin.TakeOver( pEngine->GetSkinManager()->LoadSkin( pPathSkin, baseDir ) );
				
			}catch( const deException &e ){
				pSector->GetHeightTerrain()->GetWorld().GetLogger()->LogException( LOGSOURCE, e );
				pSkin = pSector->GetHeightTerrain()->GetWorld().GetEnvironment()->GetStockSkin( igdeEnvironment::essError );
			}
		}
	}
	
	if( pEngTexture ){
		pEngTexture->SetSkin( pSkin );
	}
	
	NotifyTextureChanged();
}
