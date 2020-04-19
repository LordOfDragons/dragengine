/* 
 * Drag[en]gine OpenGL Graphic Module
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
#include <string.h>

#include "deoglCollisionVolumeVisitor.h"



// Collision Detection Volume
///////////////////////////////

// constructors, destructors
deoglCollisionVolumeVisitor::deoglCollisionVolumeVisitor(){
}

deoglCollisionVolumeVisitor::~deoglCollisionVolumeVisitor(){
}



// Visiting
/////////////

void deoglCollisionVolumeVisitor::VisitSphere( deoglCollisionSphere *sphere ){
}

void deoglCollisionVolumeVisitor::VisitCylinder( deoglCollisionCylinder *cylinder ){
}

void deoglCollisionVolumeVisitor::VisitCapsule( deoglCollisionCapsule *capsule ){
}

void deoglCollisionVolumeVisitor::VisitBox( deoglCollisionBox *box ){
}

void deoglCollisionVolumeVisitor::VisitTriangle( deoglCollisionTriangle *triangle ){
}

void deoglCollisionVolumeVisitor::VisitFrustum( deoglCollisionFrustum *frustum ){
}
