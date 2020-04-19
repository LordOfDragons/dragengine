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
#include "decShapeVisitor.h"



// Class decShapeVisitor
//////////////////////////

// Constructors, destructors
//////////////////////////////

decShapeVisitor::decShapeVisitor(){
}

decShapeVisitor::~decShapeVisitor(){
}



// Visiting
/////////////

void decShapeVisitor::VisitShape( decShape &shape ){
}

void decShapeVisitor::VisitShapeSphere( decShapeSphere &sphere ){
	VisitShape( sphere );
}

void decShapeVisitor::VisitShapeBox( decShapeBox &box ){
	VisitShape( box );
}

void decShapeVisitor::VisitShapeCylinder( decShapeCylinder &cylinder ){
	VisitShape( cylinder );
}

void decShapeVisitor::VisitShapeCapsule( decShapeCapsule &capsule ){
	VisitShape( capsule );
}

void decShapeVisitor::VisitShapeHull( decShapeHull &hull ){
	VisitShape( hull );
}
