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
#include "meSaveSupport.h"
#include "meLoadSaveSystem.h"
#include "../world/meWorld.h"
#include "../world/navspace/meNavigationSpace.h"
#include "../world/terrain/meHeightTerrain.h"
#include "../world/terrain/meHeightTerrainSector.h"
#include "../world/terrain/meHeightTerrainTexture.h"
#include "../world/terrain/meHeightTerrainNavSpace.h"
#include "../gui/meWindowMain.h"
#include "../gui/properties/meWindowProperties.h"
#include "../utils/meBitArray.h"
#include "../utils/meByteArray.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/filedialog/igdeFilePattern.h>
#include <deigde/gui/filedialog/igdeFilePatternList.h>
#include <deigde/loadsave/igdeLoadSaveHTNavSpace.h>

#include <dragengine/deEngine.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageManager.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrain.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/file/decBaseFileWriterReference.h>
#include <dragengine/common/exceptions.h>



// Definitions
////////////////

#define LOGSOURCE "World Editor"



// Class meSaveSupport
////////////////////////

// Constructor, destructor
////////////////////////////

meSaveSupport::meSaveSupport( meWindowMain *windowMain ){
	if( ! windowMain ) DETHROW( deeInvalidParam );
	
	pWindowMain = windowMain;
}

meSaveSupport::~meSaveSupport(){
}



// Management
///////////////

void meSaveSupport::SaveWorldWithDependencies( meWorld *world, bool forceAskForFilename ){
	if( ! world ){
		DETHROW( deeInvalidParam );
	}
	
	// we have to pull saving the work ahead of time to allow relative path to work properly
	const bool worldDepChanged = world->GetDepChanged();
	SaveWorld( world, forceAskForFilename );
	
	// now continue saving
	if( ! worldDepChanged ){
		return;
	}
	
	meHeightTerrain &heightTerrain = *world->GetHeightTerrain();
	const bool depChanged = heightTerrain.GetDepChanged();
	int i, j;
	
	const int sectorCount = heightTerrain.GetSectorCount();
	for( i=0; i<sectorCount; i++ ){
		meHeightTerrainSector &sector = *heightTerrain.GetSectorAt( i );
		
		if( depChanged ){
			SaveHTHeightImage( world, &sector, false );
			SaveHTVisibilityImage( world, &sector, false );
			SaveHTPFCache( world, &sector, false );
			
			const int textureCount = sector.GetTextureCount();
			for( j=0; j<textureCount; j++ ){
				SaveHTTextureMaskImage( world, &sector, sector.GetTextureAt( j ), false );
			}
			
			const int navSpaceCount = sector.GetNavSpaceCount();
			for( j=0; j<navSpaceCount; j++ ){
				SaveHTNavSpace( *world, *sector.GetNavSpaceAt( j ), false );
			}
		}
	}
	
	heightTerrain.SetDepChanged( false );
	
	SaveHeightTerrain( *world, forceAskForFilename );
	
	world->SetChanged( false );
	world->SetDepChanged( false );
	world->CheckDepChanged();
}



void meSaveSupport::SaveWorld( meWorld *world, bool forceAskForFilename ){
	if( ! world ){
		DETHROW( deeInvalidParam );
	}
	
	if( ! world->GetChanged() && ! forceAskForFilename ){
		return;
	}
	
	decString filename( world->GetFilePath() );
	
	if( ! world->GetSaved() || forceAskForFilename ){
		igdeFilePatternList filePatternList;
		
		GetWorldPatterns( filePatternList );
		if( ! igdeCommonDialogs::GetFileSave( pWindowMain, "Save World",
		*pWindowMain->GetEnvironment().GetFileSystemGame(), filePatternList, filename ) ){
			return;
		}
	}
	
	const decString basePath( world->GetDirectoryPath() );
	
	pWindowMain->GetLogger()->LogInfoFormat( LOGSOURCE, "Saving world to %s", filename.GetString() );
	try{
		pWindowMain->GetLoadSaveSystem().SaveWorld( world, filename );
		
	}catch( const deException &e ){
		pWindowMain->GetLogger()->LogException( LOGSOURCE, e );
		throw;
	}
	
	world->SetFilePath( filename );
	world->SetSaved( true );
	world->SetChanged( false );
	world->SetDepChanged( false );
	world->CheckDepChanged();
	
	if( world->GetDirectoryPath() != basePath ){
		pWindowMain->GetWindowProperties()->OnWorldPathChanged();
	}
	
	pWindowMain->GetRecentFiles().AddFile( filename );
}

void meSaveSupport::SaveHeightTerrain( meWorld &world, bool forceAskForFilename ){
	meHeightTerrain &heightTerrain = *world.GetHeightTerrain();
	if( ! heightTerrain.GetChanged() && ! forceAskForFilename ){
		return;
	}
	
	decString filename( decPath::AbsolutePathUnix( heightTerrain.GetPathHT(),
		world.GetDirectoryPath() ).GetPathUnix() );
	
	if( ! heightTerrain.GetSaved() || forceAskForFilename ){
		igdeFilePatternList filePatternList;
		
		GetHeightTerrainSectorPatterns( filePatternList );
		if( ! igdeCommonDialogs::GetFileSave( pWindowMain, "Save Height Terrain Sector",
		*pWindowMain->GetEnvironment().GetFileSystemGame(), filePatternList, filename ) ){
			return;
		}
	}
	
	pWindowMain->GetLogger()->LogInfoFormat( LOGSOURCE, "Saving height terrain to %s", filename.GetString() );
	try{
		pWindowMain->GetLoadSaveSystem().SaveHeightTerrain( heightTerrain, filename );
		
	}catch( const deException &e ){
		pWindowMain->GetLogger()->LogException( LOGSOURCE, e );
		throw;
	}
	
	heightTerrain.SetPathHT( decPath::RelativePathUnix( filename,
		world.GetDirectoryPath(), true ).GetPathUnix() );
	heightTerrain.SetSaved( true );
	heightTerrain.SetChanged( false );
	heightTerrain.SetDepChanged( false );
}

void meSaveSupport::SaveHTHeightImage( meWorld *world, meHeightTerrainSector *sector, bool forceAskForFilename ){
	if( ! world || ! sector ){
		DETHROW( deeInvalidParam );
	}
	
	if( ! sector->GetHeightImageChanged() && ! forceAskForFilename ){
		return;
	}
	
	decString filename( decPath::AbsolutePathUnix( sector->GetPathHeightImage(),
		world->GetDirectoryPath() ).GetPathUnix() );
	
	deImageManager *imgmgr = world->GetEngine()->GetImageManager();
	int imageDim = sector->GetHeightTerrain()->GetSectorResolution();
	sGrayscale32 *srcData = sector->GetHeightImage()->GetDataGrayscale32();
	int p, pixelCount = imageDim * imageDim;
	deImage *saveImage = NULL;
	int bitCount;
	
	if( sector->GetDataType() == meHeightTerrainSector::edtInt8 ){
		bitCount = 8;
		
	}else if( sector->GetDataType() == meHeightTerrainSector::edtInt16 ){
		bitCount = 16;
		
	}else{ // sector->GetDataType() == meHeightTerrainSector::edtInt32
		bitCount = 32;
	}
	
	if( ! sector->GetHeightImageSaved() || forceAskForFilename ){
		if( ! igdeCommonDialogs::GetFileSave( pWindowMain, "Save Height Terrain Height Image",
		*pWindowMain->GetEnvironment().GetFileSystemGame(),
		*pWindowMain->GetEnvironment().GetSaveFilePatternList( igdeEnvironment::efpltImage ),
		filename ) ){
			return;
		}
	}
	
	pWindowMain->GetLogger()->LogInfoFormat( LOGSOURCE, "Saving height terrain height image to %s", filename.GetString() );
	try{
		saveImage = imgmgr->CreateImage( imageDim, imageDim, 1, 1, bitCount );
		
		if( bitCount == 8 ){
			sGrayscale8 *destData = saveImage->GetDataGrayscale8();
			
			for( p=0; p<pixelCount; p++ ){
				destData[ p ].value = ( unsigned char )( ( int )( srcData[ p ].value * HT_8BIT_HTOP ) + HT_8BIT_BASE );
			}
			
		}else if( bitCount == 16 ){
			sGrayscale16 *destData = saveImage->GetDataGrayscale16();
			
			for( p=0; p<pixelCount; p++ ){
				destData[ p ].value = ( unsigned short )( ( int )( srcData[ p ].value * HT_16BIT_HTOP ) + HT_16BIT_BASE );
			}
			
		}else{ // image->GetBitCount() == 32
			memcpy( saveImage->GetDataGrayscale32(), srcData, sizeof( sGrayscale32 ) * pixelCount );
		}
		
		imgmgr->SaveImage( saveImage, filename );
		saveImage->FreeReference();
		
	}catch( const deException &e ){
		if( saveImage ) saveImage->FreeReference();
		pWindowMain->GetLogger()->LogException( LOGSOURCE, e );
		throw;
	}
	
	sector->SetPathHeightImage( decPath::RelativePathUnix( filename,
		world->GetDirectoryPath(), true ).GetPathUnix(), false );
	sector->SetHeightImageSaved( true );
	sector->SetHeightImageChanged( false );
}

void meSaveSupport::SaveHTVisibilityImage( meWorld *world, meHeightTerrainSector *sector, bool forceAskForFilename ){
	if( ! sector ){
		DETHROW( deeInvalidParam );
	}
	
	if( ! sector->GetVisibilityChanged() && ! forceAskForFilename ){
		return;
	}
	
	decString filename( decPath::AbsolutePathUnix( sector->GetPathVisibilityImage(),
		world->GetDirectoryPath() ).GetPathUnix() );
	
	deImageManager *imgmgr = world->GetEngine()->GetImageManager();
	meBitArray *visbits = sector->GetVisibilityFaces();
	int x, y, cols, rows, p;
	deImage *saveImage = NULL;
	sGrayscale8 *pixels;
	
	if( ! sector->GetVisibilitySaved() || forceAskForFilename ){
		if( ! igdeCommonDialogs::GetFileSave( pWindowMain, "Save Height Terrain Visibility Image",
		*pWindowMain->GetEnvironment().GetFileSystemGame(),
		*pWindowMain->GetEnvironment().GetSaveFilePatternList( igdeEnvironment::efpltImage ),
		filename ) ){
			return;
		}
	}
	
	cols = visbits->GetColons();
	rows = visbits->GetRows();
	pWindowMain->GetLogger()->LogInfoFormat( LOGSOURCE, "Saving height terrain visibility image to %s", filename.GetString() );
	try{
		saveImage = imgmgr->CreateImage( cols, rows, 1, 1, 8 );
		
		pixels = saveImage->GetDataGrayscale8();
		for( p=0, y=0; y<rows; y++ ){
			for( x=0; x<cols; x++ ){
				if( visbits->GetValueAt( x, y ) ){
					pixels[ p++ ].value = 255;
					
				}else{
					pixels[ p++ ].value = 0;
				}
			}
		}
		
		imgmgr->SaveImage( saveImage, filename );
		saveImage->FreeReference();
		
	}catch( const deException &e ){
		if( saveImage ) saveImage->FreeReference();
		pWindowMain->GetLogger()->LogException( LOGSOURCE, e );
		throw;
	}
	
	sector->SetPathVisibilityImage( decPath::RelativePathUnix( filename,
		world->GetDirectoryPath(), true ).GetPathUnix() );
	sector->SetVisibilitySaved( true );
	sector->SetVisibilityChanged( false );
}

void meSaveSupport::SaveHTTextureMaskImage( meWorld *world, meHeightTerrainSector *sector,
meHeightTerrainTexture *texture, bool forceAskForFilename ){
	if( ! world || ! sector || ! texture ){
		DETHROW( deeInvalidParam );
	}
	
	if( ! texture->GetMaskChanged() && ! forceAskForFilename ){
		return;
	}
	
	deImage * const mask = texture->GetMaskImage();
	if( ! mask ){
		return;
	}
	
	decString filename( decPath::AbsolutePathUnix( texture->GetPathMask(),
		world->GetDirectoryPath() ).GetPathUnix() );
	
	deImageManager *imgmgr = texture->GetEngine()->GetImageManager();
	
	if( ! texture->GetMaskSaved() || forceAskForFilename ){
		if( ! igdeCommonDialogs::GetFileSave( pWindowMain, "Save Height Terrain Texture Mask Image",
		*pWindowMain->GetEnvironment().GetFileSystemGame(),
		*pWindowMain->GetEnvironment().GetSaveFilePatternList( igdeEnvironment::efpltImage ),
		filename ) ){
			return;
		}
	}
	
	pWindowMain->GetLogger()->LogInfoFormat( LOGSOURCE, "Saving height terrain texture mask image to %s", filename.GetString() );
	try{
		imgmgr->SaveImage( mask, filename );
		
	}catch( const deException &e ){
		pWindowMain->GetLogger()->LogException( LOGSOURCE, e );
		throw;
	}
	
	texture->SetPathMask( decPath::RelativePathUnix( filename,
		world->GetDirectoryPath(), true ).GetPathUnix(), false );
	texture->SetMaskSaved( true );
	texture->SetMaskChanged( false );
}

void meSaveSupport::SaveHTNavSpace( meWorld &world, meHeightTerrainNavSpace &navspace, bool forceAskForFilename ){
	if( ! navspace.GetNavSpaceChanged() && ! forceAskForFilename ){
		return;
	}
	if( ! navspace.GetEngineNavSpace() ){
		DETHROW( deeInvalidParam );
	}
	
	decString filename( decPath::AbsolutePathUnix( navspace.GetPathNavSpace(),
		world.GetDirectoryPath() ).GetPathUnix() );
	
	igdeLoadSaveHTNavSpace saveNavSpace( pWindowMain->GetEnvironment(), LOGSOURCE );
	
	if( ! navspace.GetNavSpaceSaved() || forceAskForFilename ){
		igdeFilePatternList fpl;
		igdeFilePattern *fp = new igdeFilePattern( saveNavSpace.GetName(),
			saveNavSpace.GetPattern(), saveNavSpace.GetDefaultExtension() );
		fpl.AddFilePattern( fp );
		
		if( ! igdeCommonDialogs::GetFileSave( pWindowMain, "Save height terrain navigation space",
		*pWindowMain->GetEnvironment().GetFileSystemGame(), fpl, filename ) ){
			return;
		}
	}
	
	pWindowMain->GetLogger()->LogInfoFormat( LOGSOURCE, "Saving height terrain navigation space to %s", filename.GetString() );
	decBaseFileWriterReference writer;
	writer.TakeOver( navspace.GetEngine().GetVirtualFileSystem()->OpenFileForWriting(
		decPath::CreatePathUnix( filename ) ) );
	saveNavSpace.Save( *navspace.GetEngineNavSpace(), writer );
	
	navspace.SetPathNavSpace( decPath::RelativePathUnix( filename,
		world.GetDirectoryPath(), true ).GetPathUnix(), false );
	navspace.SetNavSpaceSaved( true );
	navspace.SetNavSpaceChanged( false );
}

void meSaveSupport::SaveHTPFCache( meWorld *world, meHeightTerrainSector *sector, bool forceAskForFilename ){
	if( ! world || ! sector ){
		DETHROW( deeInvalidParam );
	}
	
	if( ! sector->GetPFCacheChanged() && ! forceAskForFilename ){
		return;
	}
	
	decString filename( decPath::AbsolutePathUnix( sector->GetPathPFCacheImage(),
		world->GetDirectoryPath() ).GetPathUnix() );
	
	if( ! sector->GetPFCacheSaved() || forceAskForFilename ){
		if( ! igdeCommonDialogs::GetFileSave( pWindowMain, "Save Height Terrain Prop Field Cache",
		*pWindowMain->GetEnvironment().GetFileSystemGame(),
		pWindowMain->GetLoadSaveSystem().GetPropFieldCacheFilePatterns(), filename ) ){
			return;
		}
	}
	
	pWindowMain->GetLogger()->LogInfoFormat( LOGSOURCE, "Saving height terrain prop field cache to %s", filename.GetString() );
	try{
		sector->Update();
		sector->UpdateVInstances();
		pWindowMain->GetLoadSaveSystem().SavePFCache( *sector, filename );
		
	}catch( const deException &e ){
		pWindowMain->GetLogger()->LogException( LOGSOURCE, e );
		throw;
	}
	
	sector->SetPathPFCache( decPath::RelativePathUnix( filename,
		world->GetDirectoryPath(), true ).GetPathUnix() );
	sector->SetPFCacheSaved( true );
	sector->SetPFCacheChanged( false );
}

void meSaveSupport::SaveNavigationSpace( meWorld *world, meNavigationSpace *navspace, bool forceAskForFilename ){
	if( ! world || ! navspace ){
		DETHROW( deeInvalidParam );
	}
	
	if( /* ! navspace->GetChanged() && */ ! forceAskForFilename ){
		return;
	}
	
	decString filename( decPath::AbsolutePathUnix( navspace->GetFilename(),
		world->GetDirectoryPath() ).GetPathUnix() );
	
	if( /* ! navspace->GetSaved() || */ forceAskForFilename ){
		if( ! igdeCommonDialogs::GetFileSave( pWindowMain, "Save Navigation Space",
		*pWindowMain->GetEnvironment().GetFileSystemGame(),
		*pWindowMain->GetEnvironment().GetSaveFilePatternList( igdeEnvironment::efpltImage ),
		filename ) ){
			return;
		}
	}
	
	pWindowMain->GetLogger()->LogInfoFormat( LOGSOURCE, "Saving navigation space to %s", filename.GetString() );
	try{
		navspace->SetFilename( decPath::RelativePathUnix( filename,
			world->GetDirectoryPath(), true ).GetPathUnix() );
		navspace->SaveToFile();
		
	}catch( const deException &e ){
		pWindowMain->GetLogger()->LogException( LOGSOURCE, e );
		throw;
	}
}



void meSaveSupport::GetWorldPatterns( igdeFilePatternList &patternList ) const{
	igdeFilePattern *pattern = NULL;
	
	patternList.RemoveAllFilePatterns();
	
	try{
		pattern = new igdeFilePattern( "Drag[en]gine XML World", "*.deworld", ".deworld" );
		patternList.AddFilePattern( pattern );
		pattern = NULL;
		
	}catch( const deException &e ){
		pWindowMain->DisplayException( e );
		throw;
	}
}

void meSaveSupport::GetHeightTerrainSectorPatterns( igdeFilePatternList &patternList ) const{
	igdeFilePattern *pattern = NULL;
	
	patternList.RemoveAllFilePatterns();
	
	try{
		pattern = new igdeFilePattern( "Drag[en]gine XML Height Terrain Sector", "*.dehterrain", ".dehterrain" );
		patternList.AddFilePattern( pattern );
		pattern = NULL;
		
	}catch( const deException &e ){
		pWindowMain->DisplayException( e );
		throw;
	}
}
