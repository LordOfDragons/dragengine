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

#include "deCanvasRenderWorld.h"
#include "deCanvasVisitor.h"
#include "../world/deWorld.h"
#include "../camera/deCamera.h"
#include "../../common/exceptions.h"



// Class deCanvasRenderWorld
//////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deCanvasRenderWorld::deCanvasRenderWorld( deCanvasManager *manager ) :
deCanvas( manager ){
}

deCanvasRenderWorld::~deCanvasRenderWorld(){
	SetCamera( NULL );
}



// Management
///////////////

void deCanvasRenderWorld::SetCamera( deCamera *camera ){
	if( camera == pCamera ){
		return;
	}
	
	pCamera = camera;
	
	NotifyContentChanged();
}



// Visiting
/////////////

void deCanvasRenderWorld::Visit( deCanvasVisitor &visitor ){
	visitor.VisitRenderWorld( *this );
}
