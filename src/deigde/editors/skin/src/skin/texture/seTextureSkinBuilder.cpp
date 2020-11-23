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

#include "seTexture.h"
#include "seTextureSkinBuilder.h"
#include "../property/seProperty.h"
#include "../property/node/sePropertyNodeGroup.h"
#include "../property/node/sePropertyNodeImage.h"
#include "../property/node/sePropertyNodeShape.h"
#include "../property/node/sePropertyNodeText.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinTexture.h>
#include <dragengine/resources/skin/property/deSkinProperty.h>
#include <dragengine/resources/skin/property/deSkinPropertyValue.h>
#include <dragengine/resources/skin/property/deSkinPropertyColor.h>
#include <dragengine/resources/skin/property/deSkinPropertyConstructed.h>
#include <dragengine/resources/skin/property/deSkinPropertyImage.h>
#include <dragengine/resources/skin/property/deSkinPropertyVideo.h>
#include <dragengine/resources/skin/property/deSkinPropertyMapped.h>
#include <dragengine/resources/skin/property/deSkinPropertyVisitorIdentify.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeGroup.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeImage.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeShape.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeText.h>



// Class seTextureSkinBuilder
///////////////////////////////

// Constructor, destructor
////////////////////////////

seTextureSkinBuilder::seTextureSkinBuilder( const seTexture *texture ){
	if( ! texture ){
		DETHROW( deeInvalidParam );
	}
	
	pTexture = texture;
}

seTextureSkinBuilder::~seTextureSkinBuilder(){
}



// Management
///////////////

void seTextureSkinBuilder::BuildSkin( deSkin *engSkin ){
	if( ! engSkin ){
		DETHROW( deeInvalidParam );
	}
	
	deSkinPropertyConstructed *engPropertyConstructed = NULL;
	deSkinPropertyValue *engPropertyValue = NULL;
	deSkinPropertyColor *engPropertyColor = NULL;
	deSkinPropertyImage *engPropertyImage = NULL;
	deSkinPropertyVideo *engPropertyVideo = NULL;
	deSkinPropertyMapped *engPropertyMapped = NULL;
	deSkinTexture *engTexture = NULL;
	int i;
	
	try{
		const sePropertyList &propertyList = pTexture->GetPropertyList();
		const int propertyCount = propertyList.GetCount();
		
		engTexture = new deSkinTexture( "texture" );
		
		for( i=0; i<propertyCount; i++ ){
			const seProperty &property = *propertyList.GetAt( i );
			
			switch( property.GetValueType() ){
			case seProperty::evtValue:
				engPropertyValue = new deSkinPropertyValue( property.GetName() );
				engPropertyValue->SetValue( property.GetValue() );
				engPropertyValue->SetRenderable( property.GetRenderableName() );
				
				engTexture->AddProperty( engPropertyValue );
				engPropertyValue = NULL;
				break;
				
			case seProperty::evtColor:
				engPropertyColor = new deSkinPropertyColor( property.GetName() );
				engPropertyColor->SetColor( property.GetColor() );
				engPropertyColor->SetRenderable( property.GetRenderableName() );
				
				engTexture->AddProperty( engPropertyColor );
				engPropertyColor = NULL;
				break;
				
			case seProperty::evtImage:
				engPropertyImage = new deSkinPropertyImage( property.GetName() );
				engPropertyImage->SetPath( property.GetImagePath() );
				engPropertyImage->SetImage( property.GetEngineImage() );
				engPropertyImage->SetRenderable( property.GetRenderableName() );
				
				engTexture->AddProperty( engPropertyImage );
				engPropertyImage = NULL;
				break;
				
			case seProperty::evtVideo:
				engPropertyVideo = new deSkinPropertyVideo( property.GetName() );
				engPropertyVideo->SetPath( property.GetVideoPath() );
				engPropertyVideo->SetVideo( property.GetEngineVideo() );
				engPropertyVideo->SetRenderable( property.GetRenderableName() );
				
				engTexture->AddProperty( engPropertyVideo );
				engPropertyVideo = NULL;
				break;
				
			case seProperty::evtMapped:
				engPropertyMapped = new deSkinPropertyMapped( property.GetName() );
				
				engPropertyMapped->GetRed() = property.GetMappedComponent( 0 );
				engPropertyMapped->GetGreen() = property.GetMappedComponent( 1 );
				engPropertyMapped->GetBlue() = property.GetMappedComponent( 2 );
				engPropertyMapped->GetAlpha() = property.GetMappedComponent( 3 );
				
				engPropertyMapped->SetRenderable( property.GetRenderableName() );
				
				engTexture->AddProperty( engPropertyMapped );
				engPropertyMapped = NULL;
				break;
				
			case seProperty::evtConstructed:
				engPropertyConstructed = new deSkinPropertyConstructed( property.GetName() );
				
				engPropertyConstructed->SetColor( property.GetNodeColor() );
				engPropertyConstructed->GetContent().SetSize( property.GetNodeGroup()->GetSize() );
				engPropertyConstructed->SetTileX( property.GetNodeTileX() );
				engPropertyConstructed->SetTileY( property.GetNodeTileY() );
				engPropertyConstructed->SetBitCount( property.GetNodeBitCount() );
				
				CreateNodeGroup( engPropertyConstructed->GetContent(), *property.GetNodeGroup() );
				
				engPropertyConstructed->SetRenderable( property.GetRenderableName() );
				
				engTexture->AddProperty( engPropertyConstructed );
				engPropertyConstructed = NULL;
				break;
			}
		}
		
		engSkin->AddTexture( engTexture );
		engTexture = NULL;
		
	}catch( const deException &e ){
		if( engPropertyConstructed ){
			delete engPropertyConstructed;
		}
		if( engPropertyMapped ){
			delete engPropertyMapped;
		}
		if( engPropertyVideo ){
			delete engPropertyVideo;
		}
		if( engPropertyImage ){
			delete engPropertyImage;
		}
		if( engPropertyColor ){
			delete engPropertyColor;
		}
		if( engPropertyValue ){
			delete engPropertyValue;
		}
		if( engTexture ){
			delete engTexture;
		}
	}
}

void seTextureSkinBuilder::CreateNodeGroup( deSkinPropertyNodeGroup &engNodeGroup,
const sePropertyNodeGroup &nodeGroup ){
	const int count = nodeGroup.GetNodeCount();
	deSkinPropertyNode *engNode = NULL;
	int i;
	
	try{
		for( i=0; i<count; i++ ){
			engNode = CreateNode( *nodeGroup.GetNodeAt( i ) );
			engNodeGroup.AddNode( engNode );
			engNode = NULL;
		}
		
	}catch( const deException &e ){
		if( engNode ){
			delete engNode;
		}
		throw;
	}
}

deSkinPropertyNode *seTextureSkinBuilder::CreateNode( const sePropertyNode &node ){
	deSkinPropertyNode *engNode = NULL;
	
	try{
		switch( node.GetNodeType() ){
		case sePropertyNode::entImage:{
			const sePropertyNodeImage &nodeImage = ( const sePropertyNodeImage& )node;
			deSkinPropertyNodeImage * const engNodeImage = new deSkinPropertyNodeImage;
			engNode = engNodeImage;
			engNodeImage->SetPath( nodeImage.GetPath() );
			engNodeImage->SetImage( nodeImage.GetImage() );
			engNodeImage->SetRepeat( nodeImage.GetRepeat() );
			}break;
			
		case sePropertyNode::entShape:{
			const sePropertyNodeShape &nodeShape = ( const sePropertyNodeShape& )node;
			deSkinPropertyNodeShape * const engNodeShape = new deSkinPropertyNodeShape;
			engNode = engNodeShape;
			engNodeShape->SetShapeType( nodeShape.GetShapeType() );
			engNodeShape->SetFillColor( nodeShape.GetFillColor() );
			engNodeShape->SetLineColor( nodeShape.GetLineColor() );
			engNodeShape->SetThickness( nodeShape.GetThickness() );
			}break;
			
		case sePropertyNode::entText:{
			const sePropertyNodeText &nodeText = ( const sePropertyNodeText& )node;
			deSkinPropertyNodeText * const engNodeText = new deSkinPropertyNodeText;
			engNode = engNodeText;
			engNodeText->SetPath( nodeText.GetPath() );
			engNodeText->SetFont( nodeText.GetFont() );
			engNodeText->SetFontSize( nodeText.GetFontSize() );
			engNodeText->SetText( nodeText.GetText() );
			engNodeText->SetColor( nodeText.GetColor() );
			}break;
			
		case sePropertyNode::entGroup:{
			deSkinPropertyNodeGroup * const engNodeGroup = new deSkinPropertyNodeGroup;
			engNode = engNodeGroup;
			CreateNodeGroup( *engNodeGroup, ( const sePropertyNodeGroup& )node );
			}break;
			
		default:
			DETHROW( deeInvalidParam );
		}
		
		engNode->SetPosition( node.GetPosition() );
		engNode->SetSize( node.GetSize() );
		engNode->SetRotation( node.GetRotation() * DEG2RAD );
		engNode->SetShear( node.GetShearing() * DEG2RAD );
		
		engNode->SetBrightness( node.GetBrightness() );
		engNode->SetContrast( node.GetContrast() );
		engNode->SetGamma( node.GetGamma() );
		engNode->SetColorize( node.GetColorize() );
		
		engNode->SetTransparency( node.GetTransparency() );
		
		if( node.GetMask() ){
			deSkinPropertyNode *engMask = NULL;
			
			try{
				engMask = CreateNode( *node.GetMask() );
				engNode->SetMask( engMask );
				
			}catch( const deException &e ){
				if( engMask ){
					delete engMask;
				}
				throw;
			}
		}
		
	}catch( const deException &e ){
		if( engNode ){
			delete engNode;
		}
		throw;
	}
	
	return engNode;
}
