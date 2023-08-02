/* 
 * Drag[en]gine IGDE Skin Editor
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

#include "seProperty.h"
#include "node/sePropertyNodeGroup.h"
#include "../seSkin.h"
#include "../texture/seTexture.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageManager.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeGroup.h>
#include <dragengine/resources/video/deVideo.h>
#include <dragengine/resources/video/deVideoManager.h>

#include <deigde/engine/textureProperties/igdeTextureProperty.h>
#include <deigde/engine/textureProperties/igdeTexturePropertyList.h>



// Class seProperty
/////////////////////

// Constructor, destructor
////////////////////////////

seProperty::seProperty( deEngine *engine, const char *name ) :
pEngine( engine ),

pTexture( NULL ),

pName( name ),
pValueType( evtColor ),

pValue( 0.0f ),

pColor( 1.0f, 1.0f, 1.0f ),

pVideoSharedTime( true ),

pNodeGroup( NULL ),
pEngNodeGroup( NULL ),
pActiveNodeGroup( NULL ),
pNodeSelection( *this ),
pActiveNodeLayer( 0 ),
pNodeTileX( false ),
pNodeTileY( false ),
pNodeBitCount( 8 ),

pSelected( false ),
pActive( false )
{
	if( ! engine ){
		DETHROW( deeInvalidParam );
	}
	
	pNodeGroup = new sePropertyNodeGroup( *engine );
	pNodeGroup->SetProperty( this );
}

seProperty::seProperty( const seProperty &property ) :
pEngine( property.pEngine ),

pTexture( NULL ),

pName( property.pName ),
pValueType( property.pValueType ),
pRenderableName( property.pRenderableName ),
pBoneName( property.pBoneName ),

pValue( property.pValue ),

pColor( property.pColor ),

pPathImage( property.pPathImage ),
pEngImage( property.pEngImage ),

pPathVideo( property.pPathVideo ),
pEngVideo( property.pEngVideo ),
pVideoSharedTime( property.pVideoSharedTime ),

pNodeGroup( NULL ),
pEngNodeGroup( NULL ),
pActiveNodeGroup( NULL ),
pNodeSelection( *this ),
pActiveNodeLayer( 0 ),
pNodeColor( property.pNodeColor ),
pNodeTileX( property.pNodeTileX ),
pNodeTileY( property.pNodeTileY ),
pNodeBitCount( property.pNodeBitCount ),

pSelected( false ),
pActive( false )
{
	pMappedComponents[ 0 ] = property.pMappedComponents[ 0 ];
	pMappedComponents[ 1 ] = property.pMappedComponents[ 1 ];
	pMappedComponents[ 2 ] = property.pMappedComponents[ 2 ];
	pMappedComponents[ 3 ] = property.pMappedComponents[ 3 ];
	
	pNodeGroup = new sePropertyNodeGroup( *property.pNodeGroup );
	pNodeGroup->SetProperty( this );
}

seProperty::~seProperty(){
	if( pActiveNodeGroup ){
		pActiveNodeGroup->FreeReference();
	}
	pNodeSelection.RemoveAll();
	if( pNodeGroup ){
		pNodeGroup->SetProperty( NULL );
		pNodeGroup->FreeReference();
	}
}



// Management
///////////////

void seProperty::SetTexture( seTexture *texture ){
	if( texture == pTexture ){
		return;
	}
	
	pTexture = texture;
	UpdateResources();
}

void seProperty::SetName( const char *name ){
	pName = name;
}

void seProperty::SetRenderableName( const char *name ){
	if( pRenderableName.Equals( name ) ){
		return;
	}
	
	pRenderableName = name;
	NotifyChanged();
}

void seProperty::SetBoneName( const char *name ){
	if( pBoneName.Equals( name ) ){
		return;
	}
	
	pBoneName = name;
	NotifyChanged();
}

void seProperty::SetValueType( eValueTypes type ){
	if( type == pValueType ){
		return;
	}
	
	pValueType = type;
	NotifyChanged();
}

void seProperty::SetValue( float value ){
	if( fabsf( value - pValue ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pValue = value;
	NotifyChanged();
}

void seProperty::SetColor( const decColor &color ){
	if( color.IsEqualTo( pColor ) ){
		return;
	}
	
	pColor = color;
	NotifyChanged();
}

void seProperty::SetImagePath( const char *imagePath ){
	if( pPathImage.Equals( imagePath ) ){
		return;
	}
	
	pPathImage = imagePath;
	UpdateImage();
	NotifyChanged();
}

void seProperty::UpdateImage(){
	deImageReference image;
	
	if( ! pPathImage.IsEmpty() && pTexture && pTexture->GetSkin() ){
		try{
			image.TakeOver( pEngine->GetImageManager()->LoadImage(
				pPathImage, pTexture->GetSkin()->GetDirectoryPath() ) );
			
		}catch( const deException &e ){
			pTexture->GetSkin()->GetLogger()->LogException( "Skin Editor", e );
		}
	}
	
	pEngImage = image;
}

void seProperty::SetVideoPath( const char *videoPath ){
	if( pPathVideo.Equals( videoPath ) ){
		return;
	}
	
	pPathVideo = videoPath;
	UpdateVideo();
	NotifyChanged();
}

void seProperty::UpdateVideo(){
	deVideoReference video;
	
	if( ! pPathVideo.IsEmpty() && pTexture && pTexture->GetSkin() ){
		try{
			video.TakeOver( pEngine->GetVideoManager()->LoadVideo(
				pPathVideo, pTexture->GetSkin()->GetDirectoryPath(), false ) );
			
		}catch( const deException &e ){
			pTexture->GetSkin()->GetLogger()->LogException( "Skin Editor", e );
		}
	}
	
	pEngVideo = video;
}

void seProperty::SetVideoSharedTime( bool shareTime ){
	if( shareTime == pVideoSharedTime ){
		return;
	}
	
	pVideoSharedTime = shareTime;
	NotifyChanged();
}



const seMapped::Ref &seProperty::GetMappedComponent( int index ) const{
	DEASSERT_TRUE( index >= 0 )
	DEASSERT_TRUE( index <= 3 )
	
	return pMappedComponents[ index ];
}

void seProperty::SetMappedComponent( int index, seMapped *mapped ){
	DEASSERT_TRUE( index >= 0 )
	DEASSERT_TRUE( index <= 3 )
	
	pMappedComponents[ index ] = mapped;
	NotifyChanged();
}



void seProperty::SetNodeGroup( sePropertyNodeGroup *nodeGroup ){
	if( ! nodeGroup || nodeGroup->GetProperty() ){
		DETHROW( deeInvalidParam );
	}
	
	pNodeSelection.RemoveAll();
	
	pNodeGroup->SetProperty( NULL );
	pNodeGroup->FreeReference();
	
	pNodeGroup = nodeGroup;
	
	nodeGroup->AddReference();
	nodeGroup->SetProperty( this );
	
	nodeGroup->NotifyChanged();
	nodeGroup->NotifyStructreChanged();
	
	UpdateEngineNodeGroup();
	NotifyChanged();
}

void seProperty::UpdateEngineNodeGroup(){
	// TODO
}

void seProperty::SetActiveNodeGroup( sePropertyNodeGroup *node ){
	if( node == pActiveNodeGroup ){
		return;
	}
	
	const bool selectionChanged = pNodeSelection.GetSelected().GetCount() > 0;
	const bool activeNodeChanged = pNodeSelection.HasActive();
	
	pNodeSelection.RemoveAll();
	
	if( pActiveNodeGroup ){
		pActiveNodeGroup->SetActiveGroup( false );
		pActiveNodeGroup->FreeReference();
	}
	
	pActiveNodeGroup = node;
	
	if( node ){
		node->AddReference();
		node->SetActiveGroup( true );
	}
	
	if( pTexture && pTexture->GetSkin() ){
		seSkin &skin = *pTexture->GetSkin();
		if( selectionChanged ){
			skin.NotifyPropertyNodeSelectionChanged( pTexture, this );
		}
		if( activeNodeChanged ){
			skin.NotifyPropertyActiveNodeChanged( pTexture, this );
		}
		skin.NotifyPropertyActiveNodeGroupChanged( pTexture, this );
	}
}

void seProperty::SetActiveNodeLayer( int layer ){
	if( layer == pActiveNodeLayer ){
		return;
	}
	
	pActiveNodeLayer = layer;
	
	if( pTexture && pTexture->GetSkin() ){
		pTexture->GetSkin()->NotifyPropertyActiveNodeLayerChanged( pTexture, this );
	}
}

void seProperty::SetNodeColor( const decColor &color ){
	if( color.IsEqualTo( pNodeColor ) ){
		return;
	}
	
	pNodeColor = color;
	NotifyChanged();
}

void seProperty::SetNodeTileX( bool tileX ){
	if( tileX == pNodeTileX ){
		return;
	}
	
	pNodeTileX = tileX;
	NotifyChanged();
}

void seProperty::SetNodeTileY( bool tileY ){
	if( tileY == pNodeTileY ){
		return;
	}
	
	pNodeTileY = tileY;
	NotifyChanged();
}

void seProperty::SetNodeBitCount( int bitCount ){
	if( bitCount == pNodeBitCount ){
		return;
	}
	
	switch( bitCount ){
	case 8:
	case 16:
	case 32:
		pNodeBitCount = bitCount;
		NotifyChanged();
		break;
		
	default:
		DETHROW( deeInvalidParam );
	}
}



void seProperty::SetSelected( bool selected ){
	pSelected = selected;
}

void seProperty::SetActive( bool active ){
	pActive = active;
}

void seProperty::NotifyChanged(){
	if( ! pTexture ){
		return;
	}
	
	pTexture->InvalidateEngineSkin();
	
	if( pTexture->GetSkin() ){
		pTexture->GetSkin()->NotifyPropertyChanged( pTexture, this );
	}
}

void seProperty::UpdateResources(){
	UpdateImage();
	UpdateVideo();
	pNodeGroup->UpdateResources();
}

void seProperty::InitDefaults( const igdeTexturePropertyList &knownPropertyList ){
	const igdeTextureProperty * const knownProperty = knownPropertyList.GetNamed( pName );
	if( ! knownProperty ){
		return;
	}
	
	SetValue( knownProperty->GetDefaultValue() );
	SetColor( knownProperty->GetDefaultColor() );
	
	if( knownProperty->GetComponentCount() == 1 ){
		SetValueType( seProperty::evtValue );
		SetNodeColor( decColor( knownProperty->GetDefaultValue(),
			knownProperty->GetDefaultValue(), knownProperty->GetDefaultValue() ) );
		
	}else{
		SetValueType( seProperty::evtColor );
		SetNodeColor( knownProperty->GetDefaultColor() );
	}
}
