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

#include "seTexture.h"
#include "seTextureSkinBuilder.h"
#include "../seSkin.h"
#include "../mapped/seMapped.h"
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

seTextureSkinBuilder::seTextureSkinBuilder( const seSkin &skin, const seTexture &texture ) :
pSkin( skin ),
pTexture( texture ){
}

seTextureSkinBuilder::~seTextureSkinBuilder(){
}



// Management
///////////////

void seTextureSkinBuilder::BuildSkin( deSkin *engSkin ){
	DEASSERT_NOTNULL( engSkin )
	
	pAddMapped( *engSkin );
	pAddTexture( *engSkin );
}

void seTextureSkinBuilder::CreateNodeGroup( deSkinPropertyNodeGroup &engNodeGroup,
const sePropertyNodeGroup &nodeGroup ){
	const int count = nodeGroup.GetNodeCount();
	deSkinPropertyNode *engNode = nullptr;
	int i;
	
	try{
		for( i=0; i<count; i++ ){
			engNode = CreateNode( *nodeGroup.GetNodeAt( i ) );
			engNodeGroup.AddNode( engNode );
			engNode = nullptr;
		}
		
	}catch( const deException & ){
		if( engNode ){
			delete engNode;
		}
		throw;
	}
}

deSkinPropertyNode *seTextureSkinBuilder::CreateNode( const sePropertyNode &node ){
	deSkinPropertyNode *engNode = nullptr;
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
		
		if( node.GetMask() ){
			deSkinPropertyNode *engMask = nullptr;
			
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

void seTextureSkinBuilder::pAddMapped( deSkin &engSkin ){
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
		engMapped->SetRenderable( mapped.GetRenderable() );
		engMapped->SetRenderableComponent( mapped.GetRenderableComponent() );
		
		engSkin.AddMapped( engMapped );
	}
}

void seTextureSkinBuilder::pAddTexture( deSkin &engSkin ) {
	const seMappedList &mappedList = pSkin.GetMappedList();
	deSkinPropertyConstructed *engPropertyConstructed = nullptr;
	deSkinPropertyValue *engPropertyValue = nullptr;
	deSkinPropertyColor *engPropertyColor = nullptr;
	deSkinPropertyImage *engPropertyImage = nullptr;
	deSkinPropertyVideo *engPropertyVideo = nullptr;
	deSkinPropertyMapped *engPropertyMapped = nullptr;
	deSkinTexture *engTexture = nullptr;
	int i;
	
	try{
		const sePropertyList &propertyList = pTexture.GetPropertyList();
		const int propertyCount = propertyList.GetCount();
		
		engTexture = new deSkinTexture( "texture" );
		
		for( i=0; i<propertyCount; i++ ){
			const seProperty &property = *propertyList.GetAt( i );
			
			switch( property.GetValueType() ){
			case seProperty::evtValue:
				engPropertyValue = new deSkinPropertyValue( property.GetName() );
				engPropertyValue->SetValue( property.GetValue() );
				engPropertyValue->SetRenderable( property.GetRenderableName() );
				engPropertyValue->SetBone( property.GetBoneName() );
				
				engTexture->AddProperty( engPropertyValue );
				engPropertyValue = nullptr;
				break;
				
			case seProperty::evtColor:
				engPropertyColor = new deSkinPropertyColor( property.GetName() );
				engPropertyColor->SetColor( property.GetColor() );
				engPropertyColor->SetRenderable( property.GetRenderableName() );
				engPropertyColor->SetBone( property.GetBoneName() );
				
				engTexture->AddProperty( engPropertyColor );
				engPropertyColor = nullptr;
				break;
				
			case seProperty::evtImage:
				engPropertyImage = new deSkinPropertyImage( property.GetName() );
				engPropertyImage->SetPath( property.GetImagePath() );
				engPropertyImage->SetImage( property.GetEngineImage() );
				engPropertyImage->SetRenderable( property.GetRenderableName() );
				engPropertyImage->SetBone( property.GetBoneName() );
				
				engTexture->AddProperty( engPropertyImage );
				engPropertyImage = nullptr;
				break;
				
			case seProperty::evtVideo:
				engPropertyVideo = new deSkinPropertyVideo( property.GetName() );
				engPropertyVideo->SetPath( property.GetVideoPath() );
				engPropertyVideo->SetVideo( property.GetEngineVideo() );
				engPropertyVideo->SetRenderable( property.GetRenderableName() );
				engPropertyVideo->SetBone( property.GetBoneName() );
				
				engTexture->AddProperty( engPropertyVideo );
				engPropertyVideo = nullptr;
				break;
				
			case seProperty::evtMapped:
				engPropertyMapped = new deSkinPropertyMapped( property.GetName() );
				
				if( property.GetMappedComponent( 0 ) ){
					engPropertyMapped->SetRed( mappedList.IndexOf( property.GetMappedComponent( 0 ) ) );
				}
				if( property.GetMappedComponent( 1 ) ){
					engPropertyMapped->SetGreen( mappedList.IndexOf( property.GetMappedComponent( 1 ) ) );
				}
				if( property.GetMappedComponent( 2 ) ){
					engPropertyMapped->SetBlue( mappedList.IndexOf( property.GetMappedComponent( 2 ) ) );
				}
				if( property.GetMappedComponent( 3 ) ){
					engPropertyMapped->SetAlpha( mappedList.IndexOf( property.GetMappedComponent( 3 ) ) );
				}
				
				engPropertyMapped->SetRenderable( property.GetRenderableName() );
				engPropertyMapped->SetBone( property.GetBoneName() );
				
				engTexture->AddProperty( engPropertyMapped );
				engPropertyMapped = nullptr;
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
				engPropertyConstructed->SetBone( property.GetBoneName() );
				
				engTexture->AddProperty( engPropertyConstructed );
				engPropertyConstructed = nullptr;
				break;
			}
		}
		
		engSkin.AddTexture( engTexture );
		engTexture = nullptr;
		
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
	}
}
