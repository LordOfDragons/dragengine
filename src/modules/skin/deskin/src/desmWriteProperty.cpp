/* 
 * Drag[en]gine Skin Module
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

#include "desmWriteProperty.h"
#include "desmWritePropertyNode.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/curve/decCurveBezier.h>
#include <dragengine/common/curve/decCurveBezierPoint.h>
#include <dragengine/common/xmlparser/decXmlWriter.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/skin/property/deSkinPropertyValue.h>
#include <dragengine/resources/skin/property/deSkinPropertyColor.h>
#include <dragengine/resources/skin/property/deSkinPropertyImage.h>
#include <dragengine/resources/skin/property/deSkinPropertyVideo.h>
#include <dragengine/resources/skin/property/deSkinPropertyConstructed.h>
#include <dragengine/resources/skin/property/deSkinPropertyMapped.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeGroup.h>



// Class desmWriteProperty
////////////////////////////

// Constructor, destructor
////////////////////////////

desmWriteProperty::desmWriteProperty( decXmlWriter &writer, const char *propertyType ) :
pWriter( writer ),
pPropertyType( propertyType )
{
	if( ! propertyType ){
		DETHROW( deeInvalidParam );
	}
}

desmWriteProperty::~desmWriteProperty(){
}



// Visiting
/////////////

void desmWriteProperty::VisitProperty( deSkinProperty &property ){
}

void desmWriteProperty::VisitValue( deSkinPropertyValue &property ){
	pWriter.WriteOpeningTagStart( "value" );
	pWriter.WriteAttributeString( "property", pPropertyType );
	if( ! property.GetRenderable().IsEmpty() ){
		pWriter.WriteAttributeString( "renderable", property.GetRenderable().GetString() );
	}
	pWriter.WriteOpeningTagEnd( false, false );
	
	pWriter.WriteTextFloat( property.GetValue() );
	
	pWriter.WriteClosingTag( "value", false );
}

void desmWriteProperty::VisitColor( deSkinPropertyColor &property ){
	const decColor &color = property.GetColor();
	
	pWriter.WriteOpeningTagStart( "color" );
	pWriter.WriteAttributeString( "property", pPropertyType );
	pWriter.WriteAttributeFloat( "r", color.r );
	pWriter.WriteAttributeFloat( "g", color.g );
	pWriter.WriteAttributeFloat( "b", color.b );
	pWriter.WriteAttributeFloat( "a", color.a );
	if( ! property.GetRenderable().IsEmpty() ){
		pWriter.WriteAttributeString( "renderable", property.GetRenderable().GetString() );
	}
	pWriter.WriteOpeningTagEnd( true );
}

void desmWriteProperty::VisitImage( deSkinPropertyImage &property ){
	pWriter.WriteOpeningTagStart( "image" );
	pWriter.WriteAttributeString( "property", pPropertyType );
	if( ! property.GetRenderable().IsEmpty() ){
		pWriter.WriteAttributeString( "renderable", property.GetRenderable().GetString() );
	}
	pWriter.WriteOpeningTagEnd( false, false );
	
	pWriter.WriteTextString( property.GetPath() );
	
	pWriter.WriteClosingTag( "image", false );
}

void desmWriteProperty::VisitVideo( deSkinPropertyVideo &property ){
	pWriter.WriteOpeningTagStart( "video" );
	pWriter.WriteAttributeString( "property", pPropertyType );
	if( ! property.GetRenderable().IsEmpty() ){
		pWriter.WriteAttributeString( "renderable", property.GetRenderable().GetString() );
	}
	if( ! property.GetSharedTime() ){
		pWriter.WriteAttributeBool( "sharedTime", property.GetSharedTime() );
	}
	pWriter.WriteOpeningTagEnd( false, false );
	
	pWriter.WriteTextString( property.GetPath().GetString() );
	
	pWriter.WriteClosingTag( "video", false );
}

void desmWriteProperty::VisitConstructed( deSkinPropertyConstructed &property ){
	pWriter.WriteOpeningTagStart( "constructed" );
	pWriter.WriteAttributeString( "property", pPropertyType );
	if( ! property.GetRenderable().IsEmpty() ){
		pWriter.WriteAttributeString( "renderable", property.GetRenderable() );
	}
	pWriter.WriteOpeningTagEnd();
	
	const deSkinPropertyNodeGroup &content = property.GetContent();
	const decPoint3 &size = content.GetSize();
	pWriter.WriteOpeningTagStart( "size" );
	pWriter.WriteAttributeInt( "x", size.x );
	pWriter.WriteAttributeInt( "y", size.y );
	if( size.z != 1 ){
		pWriter.WriteAttributeInt( "z", size.z );
	}
	pWriter.WriteOpeningTagEnd( true );
	
	const decColor &color = property.GetColor();
	if( ! color.IsEqualTo( decColor() ) ){
		pWriter.WriteOpeningTagStart( "color" );
		pWriter.WriteAttributeFloat( "r", color.r );
		pWriter.WriteAttributeFloat( "g", color.g );
		pWriter.WriteAttributeFloat( "b", color.b );
		if( color.a < 1.0 - FLOAT_SAFE_EPSILON ){
			pWriter.WriteAttributeFloat( "a", color.a );
		}
		pWriter.WriteOpeningTagEnd( true );
	}
	
	if( property.GetTileX() ){
		pWriter.WriteDataTagBool( "tileX", property.GetTileX() );
	}
	if( property.GetTileY() ){
		pWriter.WriteDataTagBool( "tileY", property.GetTileY() );
	}
	
	desmWritePropertyNode writer( pWriter );
	const int count = content.GetNodeCount();
	int i;
	
	for( i=0; i<count; i++ ){
		content.GetNodeAt( i )->Visit( writer );
	}
	
	pWriter.WriteClosingTag( "constructed" );
}

void desmWriteProperty::VisitMapped( deSkinPropertyMapped &property ){
	pWriter.WriteOpeningTagStart( "mapped" );
	pWriter.WriteAttributeString( "property", pPropertyType );
	if( ! property.GetRenderable().IsEmpty() ){
		pWriter.WriteAttributeString( "renderable", property.GetRenderable().GetString() );
	}
	pWriter.WriteOpeningTagEnd();
	
	const char * const names[] = { "red", "green", "blue", "alpha" };
	int i;
	for( i=0; i<4; i++ ){
		const deSkinPropertyMapped::cComponent &component = property.GetComponent( i );
		if( component.GetCurve().GetPointCount() == 0
		&& component.GetInputType() == deSkinPropertyMapped::eitTime
		&& fabsf( component.GetInputLower() ) < FLOAT_SAFE_EPSILON
		&& fabsf( component.GetInputUpper() - 1.0f ) < FLOAT_SAFE_EPSILON
		&& component.GetInputClamped()
		&& component.GetBone().IsEmpty() ){
			continue;
		}
		
		pWriter.WriteOpeningTag( names[ i ] );
		
		if( component.GetCurve().GetPointCount() > 0 ){
			const int pointCount = component.GetCurve().GetPointCount();
			int j;
			
			pWriter.WriteOpeningTag( "curve" );
			
			switch( component.GetCurve().GetInterpolationMode() ){
			case decCurveBezier::eimConstant:
				pWriter.WriteDataTagString( "interpolation", "constant" );
				break;
				
			case decCurveBezier::eimLinear:
				pWriter.WriteDataTagString( "interpolation", "linear" );
				break;
				
			case decCurveBezier::eimBezier:
				break;
			}
			
			for( j=0; j<pointCount; j++ ){
				const decCurveBezierPoint &point = component.GetCurve().GetPointAt( j );
				pWriter.WriteOpeningTag( "point" );
				
				pWriter.WriteOpeningTagStart( "coordinates" );
				pWriter.WriteAttributeFloat( "x", point.GetPoint().x );
				pWriter.WriteAttributeFloat( "y", point.GetPoint().y );
				pWriter.WriteOpeningTagEnd( true );
				
				pWriter.WriteOpeningTagStart( "handle1" );
				pWriter.WriteAttributeFloat( "x", point.GetHandle1().x );
				pWriter.WriteAttributeFloat( "y", point.GetHandle1().y );
				pWriter.WriteOpeningTagEnd( true );
				
				pWriter.WriteOpeningTagStart( "handle2" );
				pWriter.WriteAttributeFloat( "x", point.GetHandle2().x );
				pWriter.WriteAttributeFloat( "y", point.GetHandle2().y );
				pWriter.WriteOpeningTagEnd( true );
				
				pWriter.WriteClosingTag( "point" );
			}
			
			pWriter.WriteClosingTag( "curve" );
		}
		
		switch( component.GetInputType() ){
		case deSkinPropertyMapped::eitTime:
			break;
			
		case deSkinPropertyMapped::eitBonePositionX:
			pWriter.WriteDataTagString( "inputType", "bonePositionX" );
			break;
			
		case deSkinPropertyMapped::eitBonePositionY:
			pWriter.WriteDataTagString( "inputType", "bonePositionY" );
			break;
			
		case deSkinPropertyMapped::eitBonePositionZ:
			pWriter.WriteDataTagString( "inputType", "bonePositionZ" );
			break;
			
		case deSkinPropertyMapped::eitBoneRotationX:
			pWriter.WriteDataTagString( "inputType", "boneRotationX" );
			break;
			
		case deSkinPropertyMapped::eitBoneRotationY:
			pWriter.WriteDataTagString( "inputType", "boneRotationY" );
			break;
			
		case deSkinPropertyMapped::eitBoneRotationZ:
			pWriter.WriteDataTagString( "inputType", "boneRotationZ" );
			break;
			
		case deSkinPropertyMapped::eitBoneScaleX:
			pWriter.WriteDataTagString( "inputType", "boneScaleX" );
			break;
			
		case deSkinPropertyMapped::eitBoneScaleY:
			pWriter.WriteDataTagString( "inputType", "boneScaleY" );
			break;
			
		case deSkinPropertyMapped::eitBoneScaleZ:
			pWriter.WriteDataTagString( "inputType", "boneScaleZ" );
			break;
		}
		
		if( fabsf( component.GetInputLower() ) >= FLOAT_SAFE_EPSILON ){
			pWriter.WriteDataTagFloat( "inputLower", component.GetInputLower() );
		}
		
		if( fabsf( component.GetInputUpper() - 1.0f ) >= FLOAT_SAFE_EPSILON ){
			pWriter.WriteDataTagFloat( "inputUpper", component.GetInputUpper() );
		}
		
		if( component.GetInputClamped() ){
			pWriter.WriteDataTagBool( "inputClamped", component.GetInputClamped() );
		}
		
		if( ! component.GetBone().IsEmpty() ){
			pWriter.WriteDataTagString( "bone", component.GetBone() );
		}
		
		pWriter.WriteClosingTag( names[ i ] );
	}
	
	pWriter.WriteClosingTag( "mapped" );
}
