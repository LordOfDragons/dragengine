/* 
 * Drag[en]gine Rig Module
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
