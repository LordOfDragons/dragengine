/* 
 * Drag[en]gine IGDE
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

#include "igdePropertyStringEncodeShapeList.h"
#include <dragengine/common/shape/decShapeHull.h>
#include <dragengine/common/shape/decShapeCapsule.h>
#include <dragengine/common/shape/decShapeCylinder.h>
#include <dragengine/common/shape/decShapeBox.h>
#include <dragengine/common/shape/decShapeSphere.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/decStringList.h>



// Class igdePropertyStringEncodeShapeList
////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdePropertyStringEncodeShapeList::igdePropertyStringEncodeShapeList( decString *string ){
	if( ! string ){
		DETHROW( deeInvalidParam );
	}
	
	pString = string;
	pRequiresSeparator = false;
}

igdePropertyStringEncodeShapeList::~igdePropertyStringEncodeShapeList(){
}



// Management
///////////////

void igdePropertyStringEncodeShapeList::Reset(){
	pRequiresSeparator = false;
}



// Visiting
/////////////

void igdePropertyStringEncodeShapeList::VisitShape( decShape &shape ){
	DETHROW( deeInvalidAction );
}

void igdePropertyStringEncodeShapeList::VisitShapeSphere( decShapeSphere &sphere ){
	const decVector &position = sphere.GetPosition();
	const float radius = sphere.GetRadius();
	
	pAddSeparator();
	pString->Append( "sphere" );
	pString->AppendFormat( ":position,%g,%g,%g", position.x, position.y, position.z );
	pString->AppendFormat( ":radius,%g", radius );
}

void igdePropertyStringEncodeShapeList::VisitShapeBox( decShapeBox &box ){
	const decVector &position = box.GetPosition();
	const decVector rotation = decMatrix::CreateFromQuaternion( box.GetOrientation() ).GetEulerAngles() / DEG2RAD;
	const decVector &extends = box.GetHalfExtends();
	
	pAddSeparator();
	pString->Append( "box" );
	pString->AppendFormat( ":position,%g,%g,%g", position.x, position.y, position.z );
	if( ! rotation.IsEqualTo( decVector() ) ){
		pString->AppendFormat( ":rotation,%g,%g,%g", rotation.x, rotation.y, rotation.z );
	}
	pString->AppendFormat( ":extends,%g,%g,%g", extends.x, extends.y, extends.z );
}

void igdePropertyStringEncodeShapeList::VisitShapeCylinder( decShapeCylinder &cylinder ){
	const decVector &position = cylinder.GetPosition();
	const decVector rotation = decMatrix::CreateFromQuaternion( cylinder.GetOrientation() ).GetEulerAngles() / DEG2RAD;
	const float height = cylinder.GetHalfHeight();
	const float topRadius = cylinder.GetTopRadius();
	const float bottomRadius = cylinder.GetBottomRadius();
	const decVector2 &topAxisScaling = cylinder.GetTopAxisScaling();
	const decVector2 &bottomAxisScaling = cylinder.GetBottomAxisScaling();
	
	pAddSeparator();
	pString->Append( "cylinder" );
	pString->AppendFormat( ":position,%g,%g,%g", position.x, position.y, position.z );
	if( ! rotation.IsEqualTo( decVector() ) ){
		pString->AppendFormat( ":rotation,%g,%g,%g", rotation.x, rotation.y, rotation.z );
	}
	pString->AppendFormat( ":height,%g:radius,%g,%g", height, topRadius, bottomRadius );
	if( ! topAxisScaling.IsEqualTo( decVector2( 1.0f, 1.0f ) ) ){
		pString->AppendFormat( ":topAxisScaling,%g,%g", topAxisScaling.x, topAxisScaling.y );
	}
	if( ! bottomAxisScaling.IsEqualTo( decVector2( 1.0f, 1.0f ) ) ){
		pString->AppendFormat( ":bottomAxisScaling,%g,%g", bottomAxisScaling.x, bottomAxisScaling.y );
	}
}

void igdePropertyStringEncodeShapeList::VisitShapeCapsule( decShapeCapsule &capsule ){
	const decVector &position = capsule.GetPosition();
	const decVector rotation = decMatrix::CreateFromQuaternion( capsule.GetOrientation() ).GetEulerAngles() / DEG2RAD;
	const float height = capsule.GetHalfHeight();
	const float topRadius = capsule.GetTopRadius();
	const float bottomRadius = capsule.GetBottomRadius();
	const decVector2 &topAxisScaling = capsule.GetTopAxisScaling();
	const decVector2 &bottomAxisScaling = capsule.GetBottomAxisScaling();
	
	pAddSeparator();
	pString->Append( "capsule" );
	pString->AppendFormat( ":position,%g,%g,%g", position.x, position.y, position.z );
	if( ! rotation.IsEqualTo( decVector() ) ){
		pString->AppendFormat( ":rotation,%g,%g,%g", rotation.x, rotation.y, rotation.z );
	}
	pString->AppendFormat( ":height,%g:radius,%g,%g", height, topRadius, bottomRadius );
	if( ! topAxisScaling.IsEqualTo( decVector2( 1.0f, 1.0f ) ) ){
		pString->AppendFormat( ":topAxisScaling,%g,%g", topAxisScaling.x, topAxisScaling.y );
	}
	if( ! bottomAxisScaling.IsEqualTo( decVector2( 1.0f, 1.0f ) ) ){
		pString->AppendFormat( ":bottomAxisScaling,%g,%g", bottomAxisScaling.x, bottomAxisScaling.y );
	}
}

void igdePropertyStringEncodeShapeList::VisitShapeHull( decShapeHull &hull ){
	const decVector &position = hull.GetPosition();
	const decVector rotation( decMatrix::CreateFromQuaternion( hull.GetOrientation() ).GetEulerAngles() / DEG2RAD );
	const int pointCount = hull.GetPointCount();
	
	pAddSeparator();
	pString->Append( "hull" );
	pString->AppendFormat( ":position,%g,%g,%g", position.x, position.y, position.z );
	if( ! rotation.IsEqualTo( decVector() ) ){
		pString->AppendFormat( ":rotation,%g,%g,%g", rotation.x, rotation.y, rotation.z );
	}
	if( pointCount > 0 ){
		int i;
		pString->Append( ":points" );
		for( i=0; i<pointCount; i++ ){
			const decVector &point = hull.GetPointAt( i );
			pString->AppendFormat( ",%g,%g,%g", point.x, point.y, point.z );
		}
	}
}



// Private Functions
//////////////////////

void igdePropertyStringEncodeShapeList::pAddSeparator(){
	if( pRequiresSeparator ){
		pString->AppendCharacter( ' ' );
	}
	pRequiresSeparator = true;
}
