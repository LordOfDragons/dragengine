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

#include "deBaseScriptingCollider.h"



// Class deBaseScriptingCollider
//////////////////////////////////

deBaseScriptingCollider::deBaseScriptingCollider(){
}

deBaseScriptingCollider::~deBaseScriptingCollider(){
}



// Notifications
//////////////////

void deBaseScriptingCollider::CollisionResponse( deCollider *owner, deCollisionInfo *info ){
}

bool deBaseScriptingCollider::CanHitCollider( deCollider *owner, deCollider *collider ){
	return false;
}

void deBaseScriptingCollider::ColliderChanged( deCollider *owner ){
}



#include <stdio.h>
void deBaseScriptingCollider::ColliderConstraintBroke( deCollider *owner, int index, deColliderConstraint *constraint ){
}

void deBaseScriptingCollider::RigConstraintBroke( deCollider *owner, int bone, int index, deRigConstraint *constraint ){
}
