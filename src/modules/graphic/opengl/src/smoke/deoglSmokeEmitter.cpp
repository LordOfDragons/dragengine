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
#include <stdlib.h>
#include <string.h>

#include "deoglSmokeEmitter.h"

#include "../deoglBasics.h"
#include "../deGraphicOpenGl.h"
#include "../world/deoglWorld.h"

#include <dragengine/resources/smoke/deSmokeEmitter.h>
#include <dragengine/errortracing/deErrorTrace.h>
#include <dragengine/errortracing/deErrorTracePoint.h>
#include <dragengine/errortracing/deErrorTraceValue.h>
#include <dragengine/common/exceptions.h>



// Class deoglSmokeEmitter
////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSmokeEmitter::deoglSmokeEmitter( deGraphicOpenGl *ogl, deSmokeEmitter *smokeEmitter ){
	if( ! ogl || ! smokeEmitter ) DETHROW( deeInvalidParam );
	
	pOgl = ogl;
	pSmokeEmitter = smokeEmitter;
	pParentWorld = NULL;
	
	pDirtyExtends = true;
}

deoglSmokeEmitter::~deoglSmokeEmitter(){
	pCleanUp();
}



// Management
///////////////

void deoglSmokeEmitter::SetParentWorld( deoglWorld *world ){
	pParentWorld = world;
}



void deoglSmokeEmitter::Update( float elapsed ){
}

void deoglSmokeEmitter::PrepareForRender(){
	if( pDirtyExtends ){
		pUpdateExtends();
		
		pDirtyExtends = false;
	}
}



// Notifications
//////////////////

void deoglSmokeEmitter::VolumePositionChanged(){
}

void deoglSmokeEmitter::SkinChanged(){
}

void deoglSmokeEmitter::PointsChanged(){
}



// Private functions
//////////////////////

void deoglSmokeEmitter::pCleanUp(){
}

void deoglSmokeEmitter::pUpdateExtends(){
}
