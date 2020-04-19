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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "igdeStepableTask.h"

#include <dragengine/common/exceptions.h>



// Class igdeStepableTask
///////////////////////////

// Constructor, destructor
////////////////////////////

igdeStepableTask::igdeStepableTask() :
pProgress( 0.0f ),
pDirtyMessage( false ),
pDirtyProgress( false ){
}

igdeStepableTask::~igdeStepableTask(){
}



// Management
///////////////

void igdeStepableTask::SetMessage( const char *message ){
	pMessage = message;
	pDirtyMessage = true;
}

void igdeStepableTask::SetProgress( float progress ){
	pProgress = progress;
	pDirtyProgress = true;
}

void igdeStepableTask::SetDirtyMessage( bool dirty ){
	pDirtyMessage = dirty;
}

void igdeStepableTask::SetDirtyProgress( bool dirty ){
	pDirtyProgress = dirty;
}
