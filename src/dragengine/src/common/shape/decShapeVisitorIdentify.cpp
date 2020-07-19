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

#include <string.h>
#include <stdio.h>

#include "decShape.h"
#include "decShapeBox.h"
#include "decShapeSphere.h"
#include "decShapeCapsule.h"
#include "decShapeCylinder.h"
#include "decShapeHull.h"
#include "decShapeVisitorIdentify.h"

#include "../exceptions.h"



// Class decShapeVisitorIdentify
//////////////////////////////////

// Constructors, destructors
//////////////////////////////

decShapeVisitorIdentify::decShapeVisitorIdentify() :
pShape( NULL ),
pType( estUnknown ){
}

decShapeVisitorIdentify::~decShapeVisitorIdentify(){
}



// Visiting
/////////////

decShapeSphere &decShapeVisitorIdentify::CastToSphere() const{
	if( pType != estSphere ){
		DETHROW( deeInvalidParam );
	}
	return *( ( decShapeSphere* )pShape );
}

decShapeBox &decShapeVisitorIdentify::CastToBox() const{
	if( pType != estBox ){
		DETHROW( deeInvalidParam );
	}
	return *( ( decShapeBox* )pShape );
}

decShapeCylinder &decShapeVisitorIdentify::CastToCylinder() const{
	if( pType != estCylinder ){
		DETHROW( deeInvalidParam );
	}
	return *( ( decShapeCylinder* )pShape );
}

decShapeCapsule &decShapeVisitorIdentify::CastToCapsule() const{
	if( pType != estCapsule ){
		DETHROW( deeInvalidParam );
	}
	return *( ( decShapeCapsule* )pShape );
}

decShapeHull &decShapeVisitorIdentify::CastToHull() const{
	if( pType != estHull ){
		DETHROW( deeInvalidParam );
	}
	return *( ( decShapeHull* )pShape );
}


void decShapeVisitorIdentify::Reset(){
	pShape = NULL;
	pType = estUnknown;
}


void decShapeVisitorIdentify::VisitShape( decShape &shape ){
	pShape = &shape;
	pType = estUnknown;
}

void decShapeVisitorIdentify::VisitShapeSphere( decShapeSphere &sphere ){
	pShape = &sphere;
	pType = estSphere;
}

void decShapeVisitorIdentify::VisitShapeBox( decShapeBox &box ){
	pShape = &box;
	pType = estBox;
}

void decShapeVisitorIdentify::VisitShapeCylinder( decShapeCylinder &cylinder ){
	pShape = &cylinder;
	pType = estCylinder;
}

void decShapeVisitorIdentify::VisitShapeCapsule( decShapeCapsule &capsule ){
	pShape = &capsule;
	pType = estCapsule;
}

void decShapeVisitorIdentify::VisitShapeHull( decShapeHull &hull ){
	pShape = &hull;
	pType = estHull;
}
