/* 
 * Drag[en]gine Bullet Physics Module
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#include "debpShapeToLog.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/shape/decShapeBox.h>
#include <dragengine/common/shape/decShapeSphere.h>
#include <dragengine/common/shape/decShapeCylinder.h>
#include <dragengine/common/shape/decShapeCapsule.h>
#include <dragengine/common/shape/decShapeHull.h>


// Class debpShapeToLog
/////////////////////////

// Constructor, destructor
////////////////////////////

debpShapeToLog::debpShapeToLog(){
}

debpShapeToLog::~debpShapeToLog(){
}



// Management
///////////////

void debpShapeToLog::Reset(){
	pLog.Empty();
}



// Visiting
/////////////

void debpShapeToLog::VisitShapeSphere( decShapeSphere &sphere ){
	if( pLog.IsEmpty() ){
		pLog.Append( " " );
		
	}else{
		pLog.Append( ", " );
	}
	
	const decVector &p = sphere.GetPosition();
	pLog.AppendFormat( "sphere{p=(%f,%f,%f) r=%f", p.x, p.y, p.z, sphere.GetRadius() );
	
	const decVector2 &as = sphere.GetAxisScaling();
	if( ! as.IsEqualTo( decVector2( 1.0f, 1.0f ) ) ){
		pLog.AppendFormat( " as=(%f,%f)", as.x, as.y );
	}
	
	pLog.Append( "}" );
}

void debpShapeToLog::VisitShapeBox( decShapeBox &box ){
	if( pLog.IsEmpty() ){
		pLog.Append( " " );
		
	}else{
		pLog.Append( ", " );
	}
	
	const decVector &p = box.GetPosition();
	const decVector &he = box.GetHalfExtends();
	pLog.AppendFormat( "box{p=(%f,%f,%f) he=(%f,%f,%f)", p.x, p.y, p.z, he.x, he.y, he.z );
	
	const decVector o( box.GetOrientation().GetEulerAngles() * RAD2DEG );
	if( ! o.IsZero() ){
		pLog.AppendFormat( " o=(%f,%f,%f)", o.x, o.y, o.z );
	}
	
	const decVector2 &t = box.GetTapering();
	if( ! t.IsEqualTo( decVector2( 1.0f, 1.0f ) ) ){
		pLog.AppendFormat( " t=(%f,%f)", t.x, t.y );
	}
	
	pLog.Append( "}" );
}

void debpShapeToLog::VisitShapeCylinder( decShapeCylinder &cylinder ){
	if( pLog.IsEmpty() ){
		pLog.Append( " " );
		
	}else{
		pLog.Append( ", " );
	}
	
	const decVector &p = cylinder.GetPosition();
	pLog.AppendFormat( "cylinder{p=(%f,%f,%f) hh=%f r=(%f,%f)", p.x, p.y, p.z,
		cylinder.GetHalfHeight(), cylinder.GetTopRadius(), cylinder.GetBottomRadius() );
	
	const decVector o( cylinder.GetOrientation().GetEulerAngles() * RAD2DEG );
	if( ! o.IsZero() ){
		pLog.AppendFormat( " o=(%f,%f,%f)", o.x, o.y, o.z );
	}
	
	pLog.Append( "}" );
}

void debpShapeToLog::VisitShapeCapsule( decShapeCapsule &capsule ){
	if( pLog.IsEmpty() ){
		pLog.Append( " " );
		
	}else{
		pLog.Append( ", " );
	}
	
	const decVector &p = capsule.GetPosition();
	pLog.AppendFormat( "capsule{p=(%f,%f,%f) hh=%f r=(%f,%f)", p.x, p.y, p.z,
		capsule.GetHalfHeight(), capsule.GetTopRadius(), capsule.GetBottomRadius() );
	
	const decVector o( capsule.GetOrientation().GetEulerAngles() * RAD2DEG );
	if( ! o.IsZero() ){
		pLog.AppendFormat( " o=(%f,%f,%f)", o.x, o.y, o.z );
	}
	
	pLog.Append( "}" );
}

void debpShapeToLog::VisitShapeHull( decShapeHull &hull ) {
	if( pLog.IsEmpty() ){
		pLog.Append( " " );
		
	}else{
		pLog.Append( ", " );
	}
	
	const decVector &p = hull.GetPosition();
	pLog.AppendFormat( "hull{p=(%f,%f,%f)", p.x, p.y, p.z );
	
	const decVector o( hull.GetOrientation().GetEulerAngles() * RAD2DEG );
	if( ! o.IsZero() ){
		pLog.AppendFormat( " o=(%f,%f,%f)", o.x, o.y, o.z );
	}
	
	const int count = hull.GetPointCount();
	if( count > 0 ){
		const decVector * const pts = hull.GetPoints();
		int i;
		
		pLog.Append( " pts=[" );
		
		for( i=0; i<count; i++ ){
			pLog.AppendFormat( "%s(%f,%f,%f)", i == 0 ? "" : ", ",
				pts[ i ].x, pts[ i ].y, pts[ i ].z );
		}
		
		pLog.Append( "]" );
	}
	
	pLog.Append( "}" );
}
