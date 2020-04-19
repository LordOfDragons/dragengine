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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "meHeightTerrain.h"
#include "meHeightTerrainSector.h"
#include "meHeightTerrainPFLayer.h"
#include "meHeightTerrainPFType.h"
#include "../meWorld.h"
#include "../../utils/meByteArray.h"

#include <dragengine/deEngine.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageManager.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelManager.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinManager.h>
#include <dragengine/resources/propfield/dePropField.h>
#include <dragengine/resources/propfield/dePropFieldType.h>
#include <dragengine/resources/propfield/dePropFieldInstance.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/exceptions.h>



// Definitions
////////////////

#define LOGSOURCE "World Editor"



// Class meHeightTerrainPFLayer
/////////////////////////////////

// Constructor, destructor
////////////////////////////

meHeightTerrainPFLayer::meHeightTerrainPFLayer( deEngine *engine ) :
pHTSector( NULL ),
pEngine( engine ),
pMask( NULL ),
pMaskChanged( false ),
pMaskSaved( false ),
pTypes( NULL ),
pTypeCount( 0 ),
pTypeSize( 0 )
{
	if( ! engine ){
		DETHROW( deeInvalidParam );
	}
	
	try{
		pResizeMask();
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

meHeightTerrainPFLayer::~meHeightTerrainPFLayer(){
	pCleanUp();
}



// Management
///////////////

void meHeightTerrainPFLayer::SetHTSector( meHeightTerrainSector *htsector ){
	if( htsector == pHTSector ){
		return;
	}
	
	pHTSector = htsector;
	
	if( htsector ){
		pResizeMask();
		RebuildInstances();
	}
}



void meHeightTerrainPFLayer::SetPathMask( const char *path ){
	if( ! path ){
		DETHROW( deeInvalidParam );
	}
	
	if( pPathMask.Equals( path ) ){
		return;
	}
	
	pPathMask = path;
	
	if( pHTSector && pHTSector->GetHeightTerrain() ){
		pHTSector->GetHeightTerrain()->SetChanged( true );
		pHTSector->GetHeightTerrain()->SetDepChanged( true );
	}
}

void meHeightTerrainPFLayer::SetMaskChanged( bool changed ){
	if( changed == pMaskChanged ){
		return;
	}
	
	pMaskChanged = changed;
	
	if( pHTSector && pHTSector->GetHeightTerrain() ){
		pHTSector->GetHeightTerrain()->SetDepChanged( true );
	}
}

void meHeightTerrainPFLayer::SetMaskSaved( bool saved ){
	pMaskSaved = saved;
}

void meHeightTerrainPFLayer::NotifyMaskChanged(){
	if( ! pHTSector || ! pHTSector->GetHeightTerrain() ){
		return;
	}
	
	pHTSector->GetHeightTerrain()->SetDepChanged( true );
	//pHTSector->GetHeightTerrain()->GetWorld().NotifyHTSPropFieldMaskChanged( pHTSector, this );
}

void meHeightTerrainPFLayer::LoadMaskFromImage(){
	if( ! pHTSector || ! pHTSector->GetHeightTerrain() ){
		return;
	}
	
	const int resolution = pHTSector->GetHeightTerrain()->GetSectorResolution();
	deImage *image = NULL;
	decPath path;
	int x, y, i;
	
	if( ! pPathMask.IsEmpty() ){
		path.SetFromUnix( pPathMask.GetString() );
		
		if( pEngine->GetVirtualFileSystem()->ExistsFile( path ) ){
			try{
				image = pEngine->GetImageManager()->LoadImage( pPathMask.GetString(), "/" );
				
				if( image ){
					if( image->GetComponentCount() != 1 || image->GetWidth() != resolution || image->GetHeight() != resolution ){
						DETHROW( deeInvalidParam );
						// TODO tell the user this is not going to work
					}
					
					if( image->GetBitCount() == 8 ){
						sGrayscale8 *data = image->GetDataGrayscale8();
						i = 0;
						for( y=0; y<resolution; y++ ){
							for( x=0; x<resolution; x++ ){
								pMask->SetValueAt( x, y, data[ i++ ].value );
							}
						}
						
					}else if( image->GetBitCount() == 16 ){
						sGrayscale16 *data = image->GetDataGrayscale16();
						
						i = 0;
						for( y=0; y<resolution; y++ ){
							for( x=0; x<resolution; x++ ){
								pMask->SetValueAt( x, y, data[ i++ ].value >> 8 );
							}
						}
						
					}else if( image->GetBitCount() == 32 ){
						sGrayscale32 *data = image->GetDataGrayscale32();
						float value;
						
						i = 0;
						for( y=0; y<resolution; y++ ){
							for( x=0; x<resolution; x++ ){
								value = data[ i++ ].value;
								
								if( value < 0.0f ){
									pMask->SetValueAt( x, y, 0 );
									
								}else if( value > 1.0f ){
									pMask->SetValueAt( x, y, 255 );
									
								}else{
									pMask->SetValueAt( x, y, ( int )( value * 255.0f ) );
								}
							}
						}
					}
					
					image->FreeReference();
				}
				
			}catch( const deException &e ){
				if( image ){
					image->FreeReference();
					image = NULL;
				}
				if( pHTSector && pHTSector->GetHeightTerrain() ){
					pHTSector->GetHeightTerrain()->GetWorld().GetLogger()->LogException( LOGSOURCE, e );
				}
			}
		}
	}
	
	SetMaskSaved( image != NULL );
	SetMaskChanged( true );
	
	if( pHTSector ){
		pHTSector->InvalidatePropFields( decPoint(), decPoint( resolution, resolution ) );
	}
	
	NotifyMaskChanged();
}

void meHeightTerrainPFLayer::SectorSizeOrResChanged(){
	pResizeMask();
	RebuildInstances();
}



void meHeightTerrainPFLayer::RebuildInstances(){
}



// Types
//////////

meHeightTerrainPFType *meHeightTerrainPFLayer::GetTypeAt( int index ) const{
	if( index < 0 || index >= pTypeCount ){
		DETHROW( deeInvalidParam );
	}
	return pTypes[ index ];
}

int meHeightTerrainPFLayer::IndexOfType( meHeightTerrainPFType *type ) const{
	if( ! type ){
		DETHROW( deeInvalidParam );
	}
	
	int i;
	for( i=0; i<pTypeCount; i++ ){
		if( type == pTypes[ i ] ){
			return i;
		}
	}
	
	return -1;
}

bool meHeightTerrainPFLayer::HasType( meHeightTerrainPFType *type ) const{
	if( ! type ){
		DETHROW( deeInvalidParam );
	}
	
	int i;
	for( i=0; i<pTypeCount; i++ ){
		if( type == pTypes[ i ] ){
			return true;
		}
	}
	
	return false;
}

void meHeightTerrainPFLayer::AddType( meHeightTerrainPFType *type ){
	if( ! type ){
		DETHROW( deeInvalidParam );
	}
	
	if( pTypeCount == pTypeSize ){
		const int newSize = pTypeCount * 3 / 2 + 1;
		meHeightTerrainPFType ** const newArray = new meHeightTerrainPFType*[ newSize ];
		if( pTypes ){
			memcpy( newArray, pTypes, sizeof( meHeightTerrainPFType* ) * pTypeSize );
			delete [] pTypes;
		}
		pTypes = newArray;
		pTypeSize = newSize;
	}
	
	pTypes[ pTypeCount ] = type;
	pTypeCount++;
	
	type->AddReference();
	type->SetPFLayer( this );
	
	if( pHTSector && pHTSector->GetHeightTerrain() ){
		pHTSector->GetHeightTerrain()->SetChanged( true );
		pHTSector->GetHeightTerrain()->RebuildVegetationPropFieldTypes();
		//pHTSector->GetHeightTerrain()->GetWorld().NotifyHTSPFTypeCountChanged( pHTSector, this );
	}
}

void meHeightTerrainPFLayer::RemoveType( meHeightTerrainPFType *type ){
	const int index = IndexOfType( type );
	if( index == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	int i;
	for( i=index+1; i<pTypeCount; i++ ){
		pTypes[ i - 1 ] = pTypes[ i ];
	}
	pTypeCount--;
	
	type->SetPFLayer( NULL );
	type->FreeReference();
	
	if( pHTSector && pHTSector->GetHeightTerrain() ){
		pHTSector->GetHeightTerrain()->SetChanged( true );
		pHTSector->GetHeightTerrain()->RebuildVegetationPropFieldTypes();
		//pHTSector->GetHeightTerrain()->GetWorld().NotifyHTSPFTypeCountChanged( pHTSector, this );
	}
}

void meHeightTerrainPFLayer::RemoveAllTypes(){
	while( pTypeCount > 0 ){
		pTypeCount--;
		pTypes[ pTypeCount ]->SetPFLayer( NULL );
		pTypes[ pTypeCount ]->FreeReference();
	}
	
	if( pHTSector && pHTSector->GetHeightTerrain() ){
		pHTSector->GetHeightTerrain()->SetChanged( true );
		pHTSector->GetHeightTerrain()->RebuildVegetationPropFieldTypes();
		//pHTSector->GetHeightTerrain()->GetWorld().NotifyHTSPFTypeCountChanged( pHTSector, this );
	}
}




// Private Functions
//////////////////////

void meHeightTerrainPFLayer::pCleanUp(){
	RemoveAllTypes();
	if( pTypes ){
		delete [] pTypes;
	}
	
	if( pMask ){
		delete pMask;
	}
}

void meHeightTerrainPFLayer::pResizeMask(){
	if( ! pHTSector || ! pHTSector->GetHeightTerrain() ){
		return;
	}
	
	const int resolution = pHTSector->GetHeightTerrain()->GetSectorResolution();
	meByteArray *newMask = NULL;
	try{
		newMask = new meByteArray( resolution, resolution );
		newMask->SetAll( 255 );
		
		if( pMask ){
			delete pMask;
		}
		
		pMask = newMask;
		
	}catch( const deException & ){
		if( newMask ){
			delete newMask;
		}
		throw;
	}
	
	NotifyMaskChanged();
}
