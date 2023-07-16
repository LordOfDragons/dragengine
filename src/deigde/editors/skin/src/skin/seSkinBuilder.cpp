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

#include "seSkin.h"
#include "seSkinBuilder.h"
#include "mapped/seMapped.h"
#include "texture/seTexture.h"
#include "property/seProperty.h"
#include "property/node/sePropertyNodeImage.h"
#include "property/node/sePropertyNodeGroup.h"
#include "property/node/sePropertyNodeShape.h"
#include "property/node/sePropertyNodeText.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinTexture.h>
#include <dragengine/resources/skin/property/deSkinProperty.h>
#include <dragengine/resources/skin/property/deSkinPropertyColor.h>
#include <dragengine/resources/skin/property/deSkinPropertyConstructed.h>
#include <dragengine/resources/skin/property/deSkinPropertyImage.h>
#include <dragengine/resources/skin/property/deSkinPropertyValue.h>
#include <dragengine/resources/skin/property/deSkinPropertyVideo.h>
#include <dragengine/resources/skin/property/deSkinPropertyMapped.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeImage.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeGroup.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeShape.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeText.h>



// Class seSkinBuilder
///////////////////////

// Constructor, destructor
////////////////////////////

seSkinBuilder::seSkinBuilder( const seSkin &skin ) :
pSkin( skin ){
}

seSkinBuilder::~seSkinBuilder(){
}



// Management
///////////////

void seSkinBuilder::BuildSkin( deSkin *engSkin ){
	DEASSERT_NOTNULL( engSkin )
	
	pAddMapped( *engSkin );
	pAddTextures( *engSkin );
}

void seSkinBuilder::CreateNodeGroup( deSkinPropertyNodeGroup &engNodeGroup,
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
		
	}catch( const deException & ){
		if( engNode ){
			delete engNode;
		}
		throw;
	}
}

deSkinPropertyNode *seSkinBuilder::CreateNode( const sePropertyNode &node ){
	deSkinPropertyNode *engNode = NULL;
	int i;
	
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
			
			const struct sMappedShape{
				sePropertyNodeShape::eShapeMapped from;
				deSkinPropertyNodeShape::eShapeMapped to;
			} mappingsShape[ 9 ] = {
				{ sePropertyNodeShape::esmFillColorRed, deSkinPropertyNodeShape::esmFillColorRed },
				{ sePropertyNodeShape::esmFillColorGreen, deSkinPropertyNodeShape::esmFillColorGreen },
				{ sePropertyNodeShape::esmFillColorBlue, deSkinPropertyNodeShape::esmFillColorBlue },
				{ sePropertyNodeShape::esmFillColorAlpha, deSkinPropertyNodeShape::esmFillColorAlpha },
				{ sePropertyNodeShape::esmLineColorRed, deSkinPropertyNodeShape::esmLineColorRed },
				{ sePropertyNodeShape::esmLineColorGreen, deSkinPropertyNodeShape::esmLineColorGreen },
				{ sePropertyNodeShape::esmLineColorBlue, deSkinPropertyNodeShape::esmLineColorBlue },
				{ sePropertyNodeShape::esmLineColorAlpha, deSkinPropertyNodeShape::esmLineColorAlpha },
				{ sePropertyNodeShape::esmThickness, deSkinPropertyNodeShape::esmThickness }
			};
			
			for( i=0; i<9; i++ ){
				seMapped * const mapped = nodeShape.GetMappedFor( mappingsShape[ i ].from );
				if( mapped ){
					engNodeShape->SetShapeMappedFor( mappingsShape[ i ].to, pSkin.GetMappedList().IndexOf( mapped ) );
				}
			}
			
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
			
			const struct sMappedText{
				sePropertyNodeText::eTextMapped from;
				deSkinPropertyNodeText::eTextMapped to;
			} mappingsText[ 4 ] = {
				{ sePropertyNodeText::etmFontSize, deSkinPropertyNodeText::etmFontSize },
				{ sePropertyNodeText::etmColorRed, deSkinPropertyNodeText::etmColorRed },
				{ sePropertyNodeText::etmColorGreen, deSkinPropertyNodeText::etmColorGreen },
				{ sePropertyNodeText::etmColorBlue, deSkinPropertyNodeText::etmColorBlue }
			};
			
			for( i=0; i<4; i++ ){
				seMapped * const mapped = nodeText.GetMappedFor( mappingsText[ i ].from );
				if( mapped ){
					engNodeText->SetTextMappedFor( mappingsText[ i ].to, pSkin.GetMappedList().IndexOf( mapped ) );
				}
			}
			
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
		engNode->SetCombineMode( node.GetCombineMode() );
		
		if( node.GetMask() ){
			deSkinPropertyNode *engMask = NULL;
			
			try{
				engMask = CreateNode( *node.GetMask() );
				engNode->SetMask( engMask );
				
			}catch( const deException & ){
				if( engMask ){
					delete engMask;
				}
				throw;
			}
		}
		
		const struct sMapped{
			sePropertyNode::eMapped from;
			deSkinPropertyNode::eMapped to;
		} mappings[ 15 ] = {
			{ sePropertyNode::emPositionX, deSkinPropertyNode::emPositionX },
			{ sePropertyNode::emPositionY, deSkinPropertyNode::emPositionY },
			{ sePropertyNode::emPositionZ, deSkinPropertyNode::emPositionZ },
			{ sePropertyNode::emSizeX, deSkinPropertyNode::emSizeX },
			{ sePropertyNode::emSizeY, deSkinPropertyNode::emSizeY },
			{ sePropertyNode::emSizeZ, deSkinPropertyNode::emSizeZ },
			{ sePropertyNode::emRotation, deSkinPropertyNode::emRotation },
			{ sePropertyNode::emShear, deSkinPropertyNode::emShear },
			{ sePropertyNode::emBrightness, deSkinPropertyNode::emBrightness },
			{ sePropertyNode::emContrast, deSkinPropertyNode::emContrast },
			{ sePropertyNode::emGamma, deSkinPropertyNode::emGamma },
			{ sePropertyNode::emColorizeRed, deSkinPropertyNode::emColorizeRed },
			{ sePropertyNode::emColorizeGreen, deSkinPropertyNode::emColorizeGreen },
			{ sePropertyNode::emColorizeBlue, deSkinPropertyNode::emColorizeBlue },
			{ sePropertyNode::emTransparency, deSkinPropertyNode::emTransparency }
		};
		
		for( i=0; i<15; i++ ){
			seMapped * const mapped = node.GetMappedFor( mappings[ i ].from );
			if( mapped ){
				engNode->SetMappedFor( mappings[ i ].to, pSkin.GetMappedList().IndexOf( mapped ) );
			}
		}
		
	}catch( const deException & ){
		if( engNode ){
			delete engNode;
		}
		throw;
	}
	
	return engNode;
}



// Private Functions
//////////////////////

void seSkinBuilder::pAddMapped( deSkin &engSkin ){
	const seMappedList &list = pSkin.GetMappedList();
	const int count = list.GetCount();
	deSkinMapped::Ref engMapped;
	int i;
	
	for( i=0; i<count; i++ ){
		const seMapped &mapped = *list.GetAt( i );
		
		engMapped.TakeOver( new deSkinMapped( mapped.GetName() ) );
		
		engMapped->GetCurve() = mapped.GetCurve();
		engMapped->SetInputType( mapped.GetInputType() );
		engMapped->SetInputLower( mapped.GetInputLower() );
		engMapped->SetInputUpper( mapped.GetInputUpper() );
		engMapped->SetInputClamped( mapped.GetInputClamped() );
		engMapped->SetOutputLower( mapped.GetOutputLower() );
		engMapped->SetOutputUpper( mapped.GetOutputUpper() );
		engMapped->SetBone( mapped.GetBone() );
		
		engSkin.AddMapped( engMapped );
	}
}

void seSkinBuilder::pAddTextures( deSkin &engSkin ){
	const seMappedList &listMapped = pSkin.GetMappedList();
	const seTextureList &textureList = pSkin.GetTextureList();
	const int textureCount = textureList.GetCount();
	deSkinPropertyConstructed *engPropertyConstructed = NULL;
	deSkinPropertyColor *engPropertyColor = NULL;
	deSkinPropertyImage *engPropertyImage = NULL;
	deSkinPropertyValue *engPropertyValue = NULL;
	deSkinPropertyVideo *engPropertyVideo = NULL;
	deSkinPropertyMapped *engPropertyMapped = NULL;
	deSkinTexture *engTexture = NULL;
	int t, p;
	
	try{
		for( t=0; t<textureCount; t++ ){
			const seTexture &texture = *textureList.GetAt( t );
			const sePropertyList &propertyList = texture.GetPropertyList();
			const int propertyCount = propertyList.GetCount();
			
			engTexture = new deSkinTexture( texture.GetName() );
			
			for( p=0; p<propertyCount; p++ ){
				const seProperty &property = *propertyList.GetAt( p );
				
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
					engPropertyVideo->SetSharedTime( property.GetVideoSharedTime() );
					
					engPropertyVideo->SetRenderable( property.GetRenderableName() );
					
					engTexture->AddProperty( engPropertyVideo );
					engPropertyVideo = NULL;
					break;
					
				case seProperty::evtMapped:
					engPropertyMapped = new deSkinPropertyMapped( property.GetName() );
					
					engPropertyMapped->SetRed( listMapped.IndexOf( property.GetMappedComponent( 0 ) ) );
					engPropertyMapped->SetGreen( listMapped.IndexOf( property.GetMappedComponent( 1 ) ) );
					engPropertyMapped->SetBlue( listMapped.IndexOf( property.GetMappedComponent( 2 ) ) );
					engPropertyMapped->SetAlpha( listMapped.IndexOf( property.GetMappedComponent( 3 ) ) );
					
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
					
				default:
					DETHROW( deeInvalidParam );
				}
			}
			
			engSkin.AddTexture( engTexture );
			engTexture = NULL;
		}
		
	}catch( const deException & ){
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
		throw;
	}
}
