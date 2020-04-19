/* 
 * Drag[en]gine IGDE World Editor
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

#include <stdlib.h>
#include <string.h>

#include "meView3D.h"
#include "meView3DListener.h"

#include <dragengine/common/exceptions.h>



// Class meView3DListener
///////////////////////////

// Constructor, destructor
////////////////////////////

meView3DListener::meView3DListener( meView3D &view3D ) :
pView3D( view3D ){
}

meView3DListener::~meView3DListener(){
}



// Notifications
//////////////////

void meView3DListener::ModeChanged( meWorld* ){
	pView3D.ModeChanged();
}

void meView3DListener::ActiveCameraChanged( meWorld* ){
	pView3D.ActiveCameraChanged();
}
