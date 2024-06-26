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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "desmWritePropertyNode.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/xmlparser/decXmlWriter.h>
#include <dragengine/resources/font/deFont.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinMapped.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeGroup.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeImage.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeShape.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeText.h>



// Class desmWritePropertyNode
////////////////////////////////

// Constructor, destructor
////////////////////////////

desmWritePropertyNode::desmWritePropertyNode( decXmlWriter &writer, const deSkin &skin ) :
pWriter( writer ),
pSkin( skin ){
}

desmWritePropertyNode::~desmWritePropertyNode(){
}



// Management
///////////////

void desmWritePropertyNode::WriteNodeCommon( const deSkinPropertyNode &node ){
	const decPoint3 &position = node.GetPosition();
	if( position != decPoint3() ){
		pWriter.WriteOpeningTagStart( "position" );
		pWriter.WriteAttributeInt( "x", position.x );
		pWriter.WriteAttributeInt( "y", position.y );
		if( position.z != 0 ){
			pWriter.WriteAttributeInt( "z", position.z );
		}
		pWriter.WriteOpeningTagEnd( true );
	}
	
	const decPoint3 &size = node.GetSize();
	pWriter.WriteOpeningTagStart( "size" );
	pWriter.WriteAttributeInt( "x", size.x );
	pWriter.WriteAttributeInt( "y", size.y );
	if( size.z != 1 ){
		pWriter.WriteAttributeInt( "z", size.z );
	}
	pWriter.WriteOpeningTagEnd( true );
	
	if( fabsf( node.GetRotation() ) > FLOAT_SAFE_EPSILON ){
		pWriter.WriteDataTagFloat( "rotation", node.GetRotation() / DEG2RAD );
	}
	if( fabsf( node.GetShear() ) > FLOAT_SAFE_EPSILON ){
		pWriter.WriteDataTagFloat( "shear", node.GetShear() / DEG2RAD );
	}
	
	if( fabsf( node.GetBrightness() ) > FLOAT_SAFE_EPSILON ){
		pWriter.WriteDataTagFloat( "brightness", node.GetBrightness() );
	}
	if( fabsf( node.GetContrast() - 1.0f ) > FLOAT_SAFE_EPSILON ){
		pWriter.WriteDataTagFloat( "contrast", node.GetContrast() );
	}
	if( fabsf( node.GetGamma() - 1.0f ) > FLOAT_SAFE_EPSILON ){
		pWriter.WriteDataTagFloat( "gamma", node.GetGamma() );
	}
	
	const decColor &colorize = node.GetColorize();
	if( ! colorize.IsEqualTo( decColor( 1.0f, 1.0f, 1.0f ) ) ){
		pWriter.WriteOpeningTagStart( "colorize" );
		pWriter.WriteAttributeFloat( "r", colorize.r );
		pWriter.WriteAttributeFloat( "g", colorize.g );
		pWriter.WriteAttributeFloat( "b", colorize.b );
		pWriter.WriteOpeningTagEnd( true );
	}
	
	if( fabsf( node.GetTransparency() - 1.0f ) > FLOAT_SAFE_EPSILON ){
		pWriter.WriteDataTagFloat( "transparency", node.GetTransparency() );
	}
	
	switch( node.GetCombineMode() ){
	case deSkinPropertyNode::ecmBlend:
		//pWriter.WriteDataTagString( "combineMode", "blend" );
		break;
		
	case deSkinPropertyNode::ecmOverlay:
		pWriter.WriteDataTagString( "combineMode", "overlay" );
		break;
	}
	
	if( node.GetMask() ){
		pWriter.WriteOpeningTag( "mask" );
		node.GetMask()->Visit( *this );
		pWriter.WriteClosingTag( "mask" );
	}
	
	const struct sMapped{
		deSkinPropertyNode::eMapped mapped;
		const char *name;
	} mapped[ 15 ] = {
		{ deSkinPropertyNode::emPositionX, "positionX" },
		{ deSkinPropertyNode::emPositionY, "positionY" },
		{ deSkinPropertyNode::emPositionZ, "positionZ" },
		{ deSkinPropertyNode::emSizeX, "sizeX" },
		{ deSkinPropertyNode::emSizeY, "sizeY" },
		{ deSkinPropertyNode::emSizeZ, "sizeZ" },
		{ deSkinPropertyNode::emRotation, "rotation" },
		{ deSkinPropertyNode::emShear, "shear" },
		{ deSkinPropertyNode::emBrightness, "brightness" },
		{ deSkinPropertyNode::emContrast, "contrast" },
		{ deSkinPropertyNode::emGamma, "gamma" },
		{ deSkinPropertyNode::emColorizeRed, "colorizeR" },
		{ deSkinPropertyNode::emColorizeGreen, "colorizeG" },
		{ deSkinPropertyNode::emColorizeBlue, "colorizeB" },
		{ deSkinPropertyNode::emTransparency, "transparency" }
	};
	
	int i;
	for( i=0; i<15; i++ ){
		const int index = node.GetMappedFor( mapped[ i ].mapped );
		if( index == -1 ){
			continue;
		}
		
		pWriter.WriteOpeningTagStart( "mapped" );
		pWriter.WriteAttributeString( "name", mapped[ i ].name );
		pWriter.WriteOpeningTagEnd( false, false );
		pWriter.WriteTextString( pSkin.GetMappedAt( index )->GetName() );
		pWriter.WriteClosingTag( "mapped", false );
	}
}



// Visiting
/////////////

void desmWritePropertyNode::VisitNode( deSkinPropertyNode &node ){
}

void desmWritePropertyNode::VisitGroup( deSkinPropertyNodeGroup &node ){
	pWriter.WriteOpeningTag( "group" );
	
	WriteNodeCommon( node );
	
	const int count = node.GetNodeCount();
	int i;
	for( i=0; i<count; i++ ){
		node.GetNodeAt( i )->Visit( *this );
	}
	
	pWriter.WriteClosingTag( "group" );
}

void desmWritePropertyNode::VisitImage( deSkinPropertyNodeImage &node ){
	pWriter.WriteOpeningTag( "image" );
	
	WriteNodeCommon( node );
	
	pWriter.WriteDataTagString( "path", node.GetPath() );
	
	const decPoint &repeat = node.GetRepeat();
	if( repeat != decPoint( 1, 1 ) ){
		pWriter.WriteOpeningTagStart( "repeat" );
		if( repeat.x > 1 ){
			pWriter.WriteAttributeInt( "x", repeat.x );
		}
		if( repeat.y > 1 ){
			pWriter.WriteAttributeInt( "y", repeat.y );
		}
		pWriter.WriteOpeningTagEnd( true );
	}
	
	pWriter.WriteClosingTag( "image" );
}

void desmWritePropertyNode::VisitShape( deSkinPropertyNodeShape &node ){
	pWriter.WriteOpeningTag( "shape" );
	
	WriteNodeCommon( node );
	
	switch( node.GetShapeType() ){
	case deSkinPropertyNodeShape::estRectangle:
		pWriter.WriteDataTagString( "type", "rectangle" );
		break;
		
	case deSkinPropertyNodeShape::estEllipse:
		pWriter.WriteDataTagString( "type", "ellipse" );
		break;
		
	default:
		DETHROW( deeInvalidParam );
	};
	
	const decColor &fillColor = node.GetFillColor();
	pWriter.WriteOpeningTagStart( "fillColor" );
	pWriter.WriteAttributeFloat( "r", fillColor.r );
	pWriter.WriteAttributeFloat( "g", fillColor.g );
	pWriter.WriteAttributeFloat( "b", fillColor.b );
	if( fabsf( fillColor.a - 1.0f ) > FLOAT_SAFE_EPSILON ){
		pWriter.WriteAttributeFloat( "a", fillColor.a );
	}
	pWriter.WriteOpeningTagEnd( true );
	
	const decColor &lineColor = node.GetLineColor();
	pWriter.WriteOpeningTagStart( "lineColor" );
	pWriter.WriteAttributeFloat( "r", lineColor.r );
	pWriter.WriteAttributeFloat( "g", lineColor.g );
	pWriter.WriteAttributeFloat( "b", lineColor.b );
	if( fabsf( lineColor.a - 1.0f ) > FLOAT_SAFE_EPSILON ){
		pWriter.WriteAttributeFloat( "a", lineColor.a );
	}
	pWriter.WriteOpeningTagEnd( true );
	
	pWriter.WriteDataTagFloat( "thickness", node.GetThickness() );
	
	const struct sMapped{
		deSkinPropertyNodeShape::eShapeMapped mapped;
		const char *name;
	} mapped[ 9 ] = {
		{ deSkinPropertyNodeShape::esmFillColorRed, "fillColorR" },
		{ deSkinPropertyNodeShape::esmFillColorGreen, "fillColorG" },
		{ deSkinPropertyNodeShape::esmFillColorBlue, "fillColorB" },
		{ deSkinPropertyNodeShape::esmFillColorAlpha, "fillColorA" },
		{ deSkinPropertyNodeShape::esmLineColorRed, "fillColorR" },
		{ deSkinPropertyNodeShape::esmLineColorGreen, "fillColorG" },
		{ deSkinPropertyNodeShape::esmLineColorBlue, "fillColorB" },
		{ deSkinPropertyNodeShape::esmLineColorAlpha, "fillColorA" },
		{ deSkinPropertyNodeShape::esmThickness, "thickness" }
	};
	
	int i;
	for( i=0; i<9; i++ ){
		const int index = node.GetShapeMappedFor( mapped[ i ].mapped );
		if( index == -1 ){
			continue;
		}
		
		pWriter.WriteOpeningTagStart( "shapeMapped" );
		pWriter.WriteAttributeString( "name", mapped[ i ].name );
		pWriter.WriteOpeningTagEnd( false, false );
		pWriter.WriteTextString( pSkin.GetMappedAt( index )->GetName() );
		pWriter.WriteClosingTag( "shapeMapped", false );
	}
	
	pWriter.WriteClosingTag( "shape" );
}

void desmWritePropertyNode::VisitText( deSkinPropertyNodeText &node ){
	pWriter.WriteOpeningTag( "text" );
	
	WriteNodeCommon( node );
	
	pWriter.WriteDataTagString( "path", node.GetPath() );
	pWriter.WriteDataTagFloat( "fontSize", node.GetFontSize() );
	pWriter.WriteDataTagString( "text", node.GetText() );
	
	const decColor &color = node.GetColor();
	pWriter.WriteOpeningTagStart( "color" );
	pWriter.WriteAttributeFloat( "r", color.r );
	pWriter.WriteAttributeFloat( "g", color.g );
	pWriter.WriteAttributeFloat( "b", color.b );
	if( fabsf( color.a - 1.0f ) > FLOAT_SAFE_EPSILON ){
		pWriter.WriteAttributeFloat( "a", color.a );
	}
	pWriter.WriteOpeningTagEnd( true );
	
	const struct sMapped{
		deSkinPropertyNodeText::eTextMapped mapped;
		const char *name;
	} mapped[ 4 ] = {
		{ deSkinPropertyNodeText::etmFontSize, "fontSize" },
		{ deSkinPropertyNodeText::etmColorRed, "colorR" },
		{ deSkinPropertyNodeText::etmColorGreen, "colorG" },
		{ deSkinPropertyNodeText::etmColorBlue, "colorB" }
	};
	
	int i;
	for( i=0; i<4; i++ ){
		const int index = node.GetTextMappedFor( mapped[ i ].mapped );
		if( index == -1 ){
			continue;
		}
		
		pWriter.WriteOpeningTagStart( "textMapped" );
		pWriter.WriteAttributeString( "name", mapped[ i ].name );
		pWriter.WriteOpeningTagEnd( false, false );
		pWriter.WriteTextString( pSkin.GetMappedAt( index )->GetName() );
		pWriter.WriteClosingTag( "textMapped", false );
	}
	
	pWriter.WriteClosingTag( "text" );
}
