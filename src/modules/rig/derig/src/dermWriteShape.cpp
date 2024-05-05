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

#include "dermWriteShape.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/shape/decShapeBox.h>
#include <dragengine/common/shape/decShapeSphere.h>
#include <dragengine/common/shape/decShapeCylinder.h>
#include <dragengine/common/shape/decShapeCapsule.h>
#include <dragengine/common/shape/decShapeHull.h>
#include <dragengine/common/xmlparser/decXmlWriter.h>



// Class dermWriteShape
/////////////////////////

// Constructor, destructor
////////////////////////////

dermWriteShape::dermWriteShape( decXmlWriter &writer ) :
pWriter( writer ){
}

dermWriteShape::~dermWriteShape(){
}



// Management
///////////////

void dermWriteShape::SetProperty( const char *property ){
	pProperty = property;
}



// Visiting
/////////////

void dermWriteShape::VisitShape( decShape &shape ){
	// generic shapes we can not write so we do nothing here
}

void dermWriteShape::VisitShapeSphere( decShapeSphere &sphere ){
	pWriter.WriteOpeningTag( "sphere" );
	
	const decVector &position = sphere.GetPosition();
	pWriter.WriteOpeningTagStart( "position" );
	pWriter.WriteAttributeFloat( "x", position.x );
	pWriter.WriteAttributeFloat( "y", position.y );
	pWriter.WriteAttributeFloat( "z", position.z );
	pWriter.WriteOpeningTagEnd( true );
	
	pWriter.WriteDataTagFloat( "radius", sphere.GetRadius() );
	
	if( ! pProperty.IsEmpty() ){
		pWriter.WriteDataTagString( "property", pProperty );
	}
	
	pWriter.WriteClosingTag( "sphere" );
}

void dermWriteShape::VisitShapeBox( decShapeBox &box ){
	pWriter.WriteOpeningTag( "box" );
	
	const decVector &position = box.GetPosition();
	pWriter.WriteOpeningTagStart( "position" );
	pWriter.WriteAttributeFloat( "x", position.x );
	pWriter.WriteAttributeFloat( "y", position.y );
	pWriter.WriteAttributeFloat( "z", position.z );
	pWriter.WriteOpeningTagEnd( true );
	
	decVector rotation = decMatrix::CreateFromQuaternion( box.GetOrientation() ).GetEulerAngles() * RAD2DEG;
	pWriter.WriteOpeningTagStart( "rotation" );
	pWriter.WriteAttributeFloat( "x", rotation.x );
	pWriter.WriteAttributeFloat( "y", rotation.y );
	pWriter.WriteAttributeFloat( "z", rotation.z );
	pWriter.WriteOpeningTagEnd( true );
	
	const decVector &halfExtends = box.GetHalfExtends();
	pWriter.WriteOpeningTagStart( "halfExtends" );
	pWriter.WriteAttributeFloat( "x", halfExtends.x );
	pWriter.WriteAttributeFloat( "y", halfExtends.y );
	pWriter.WriteAttributeFloat( "z", halfExtends.z );
	pWriter.WriteOpeningTagEnd( true );
	
	if( ! pProperty.IsEmpty() ){
		pWriter.WriteDataTagString( "property", pProperty );
	}
	
	pWriter.WriteClosingTag( "box" );
}

void dermWriteShape::VisitShapeCylinder( decShapeCylinder &cylinder ){
	float bottomRadius = cylinder.GetBottomRadius();
	float topRadius = cylinder.GetTopRadius();
	
	pWriter.WriteOpeningTag( "cylinder" );
	
	const decVector &position = cylinder.GetPosition();
	pWriter.WriteOpeningTagStart( "position" );
	pWriter.WriteAttributeFloat( "x", position.x );
	pWriter.WriteAttributeFloat( "y", position.y );
	pWriter.WriteAttributeFloat( "z", position.z );
	pWriter.WriteOpeningTagEnd( true );
	
	decVector rotation = decMatrix::CreateFromQuaternion( cylinder.GetOrientation() ).GetEulerAngles() * RAD2DEG;
	pWriter.WriteOpeningTagStart( "rotation" );
	pWriter.WriteAttributeFloat( "x", rotation.x );
	pWriter.WriteAttributeFloat( "y", rotation.y );
	pWriter.WriteAttributeFloat( "z", rotation.z );
	pWriter.WriteOpeningTagEnd( true );
	
	pWriter.WriteDataTagFloat( "halfHeight", cylinder.GetHalfHeight() );
	
	// if the radi are near equal we can use a shorter version to specify them
	if( fabsf( topRadius - bottomRadius ) < 1e-5f ){
		pWriter.WriteDataTagFloat( "radius", topRadius );
		
	// otherwise we use the longer version to specify them
	}else{
		pWriter.WriteDataTagFloat( "topRadius", topRadius );
		pWriter.WriteDataTagFloat( "bottomRadius", bottomRadius );
	}
	
	if( ! pProperty.IsEmpty() ){
		pWriter.WriteDataTagString( "property", pProperty );
	}
	
	pWriter.WriteClosingTag( "cylinder" );
}

void dermWriteShape::VisitShapeCapsule( decShapeCapsule &capsule ){
	float bottomRadius = capsule.GetBottomRadius();
	float topRadius = capsule.GetTopRadius();
	
	pWriter.WriteOpeningTag( "capsule" );
	
	const decVector &position = capsule.GetPosition();
	pWriter.WriteOpeningTagStart( "position" );
	pWriter.WriteAttributeFloat( "x", position.x );
	pWriter.WriteAttributeFloat( "y", position.y );
	pWriter.WriteAttributeFloat( "z", position.z );
	pWriter.WriteOpeningTagEnd( true );
	
	decVector rotation = decMatrix::CreateFromQuaternion( capsule.GetOrientation() ).GetEulerAngles() * RAD2DEG;
	pWriter.WriteOpeningTagStart( "rotation" );
	pWriter.WriteAttributeFloat( "x", rotation.x );
	pWriter.WriteAttributeFloat( "y", rotation.y );
	pWriter.WriteAttributeFloat( "z", rotation.z );
	pWriter.WriteOpeningTagEnd( true );
	
	pWriter.WriteDataTagFloat( "halfHeight", capsule.GetHalfHeight() );
	
	// if the radi are near equal we can use a shorter version to specify them
	if( fabsf( topRadius - bottomRadius ) < 1e-5f ){
		pWriter.WriteDataTagFloat( "radius", topRadius );
		
	// otherwise we use the longer version to specify them
	}else{
		pWriter.WriteDataTagFloat( "topRadius", topRadius );
		pWriter.WriteDataTagFloat( "bottomRadius", bottomRadius );
	}
	
	if( ! pProperty.IsEmpty() ){
		pWriter.WriteDataTagString( "property", pProperty );
	}
	
	pWriter.WriteClosingTag( "capsule" );
}

void dermWriteShape::VisitShapeHull( decShapeHull &hull ){
	pWriter.WriteOpeningTag( "hull" );
	
	const decVector &position = hull.GetPosition();
	pWriter.WriteOpeningTagStart( "position" );
	pWriter.WriteAttributeFloat( "x", position.x );
	pWriter.WriteAttributeFloat( "y", position.y );
	pWriter.WriteAttributeFloat( "z", position.z );
	pWriter.WriteOpeningTagEnd( true );
	
	const decVector rotation( decMatrix::CreateFromQuaternion( hull.GetOrientation() ).GetEulerAngles() * RAD2DEG );
	pWriter.WriteOpeningTagStart( "rotation" );
	pWriter.WriteAttributeFloat( "x", rotation.x );
	pWriter.WriteAttributeFloat( "y", rotation.y );
	pWriter.WriteAttributeFloat( "z", rotation.z );
	pWriter.WriteOpeningTagEnd( true );
	
	const int count = hull.GetPointCount();
	int i;
	for( i=0; i<count; i++ ){
		const decVector &point = hull.GetPointAt( i );
		pWriter.WriteOpeningTagStart( "point" );
		pWriter.WriteAttributeFloat( "x", point.x );
		pWriter.WriteAttributeFloat( "y", point.y );
		pWriter.WriteAttributeFloat( "z", point.z );
		pWriter.WriteOpeningTagEnd( true );
	}
	
	if( ! pProperty.IsEmpty() ){
		pWriter.WriteDataTagString( "property", pProperty );
	}
	
	pWriter.WriteClosingTag( "hull" );
}
