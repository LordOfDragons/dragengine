/* 
 * Drag[en]gine IGDE Sky Editor
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

#include "seControllerTarget.h"
#include "../seSky.h"
#include "../link/seLink.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/sky/deSkyControllerTarget.h>



// Class seControllerTarget
/////////////////////////////

// Constructor, destructor
////////////////////////////

seControllerTarget::seControllerTarget(){
}

seControllerTarget::seControllerTarget( const seControllerTarget &copy ) :
pLinks( copy.pLinks ){
}

seControllerTarget::~seControllerTarget(){
	RemoveAllLinks();
}



// Management
///////////////

void seControllerTarget::AddLink( seLink *link ){
	if( ! link ){
		DETHROW( deeInvalidParam );
	}
	pLinks.Add( link );
}

void seControllerTarget::RemoveLink( seLink *link ){
	pLinks.Remove( link );
}

void seControllerTarget::RemoveAllLinks(){
	pLinks.RemoveAll();
}



// Operators
//////////////

seControllerTarget &seControllerTarget::operator=( const seControllerTarget &copy ){
	pLinks = copy.pLinks;
	return *this;
}
