/* 
 * Drag[en]gine Game Engine
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

#include "deDebugDrawerShape.h"
#include "deDebugDrawerShapeFace.h"
#include "../../common/shape/decShape.h"
#include "../../common/shape/decShapeList.h"
#include "../../common/exceptions.h"



// Class deDebugDrawerShape
/////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deDebugDrawerShape::deDebugDrawerShape(){
	pScale.Set( 1.0f, 1.0f, 1.0f );
	pEdgeColor.Set( 1.0f, 1.0f, 1.0f, 1.0f );
	pFillColor.Set( 1.0f, 1.0f, 1.0f, 0.0f );
}

deDebugDrawerShape::~deDebugDrawerShape(){
	RemoveAllFaces();
}



// Management
///////////////

void deDebugDrawerShape::SetPosition( const decVector &position ){
	pPosition = position;
}

void deDebugDrawerShape::SetOrientation( const decQuaternion &orientation ){
	pOrientation = orientation;
}

void deDebugDrawerShape::SetScale( const decVector &scale ){
	pScale = scale;
}

void deDebugDrawerShape::SetEdgeColor( const decColor &color ){
	pEdgeColor = color;
}

void deDebugDrawerShape::SetFillColor( const decColor &color ){
	pFillColor = color;
}



// Faces
//////////

int deDebugDrawerShape::GetFaceCount() const{
	return pFaces.GetCount();
}

deDebugDrawerShapeFace *deDebugDrawerShape::GetFaceAt( int index ) const{
	return ( deDebugDrawerShapeFace* )pFaces.GetAt( index );
}

void deDebugDrawerShape::AddFace( deDebugDrawerShapeFace *face ){
	// no pFaces.Has(face) check. for large number of faces this becomes very slow
	if( ! face ){
		DETHROW( deeInvalidParam );
	}
	pFaces.Add( face );
}

void deDebugDrawerShape::RemoveAllFaces(){
	const int count = pFaces.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		delete ( deDebugDrawerShapeFace* )pFaces.GetAt( i );
	}
	pFaces.RemoveAll();
}
