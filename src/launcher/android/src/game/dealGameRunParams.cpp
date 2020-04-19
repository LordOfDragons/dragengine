/* 
 * Drag[en]gine Android Launcher
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

#include "dealGameRunParams.h"
#include "profile/dealGameProfile.h"

#include "../common/exceptions.h"



// Class dealGameRunParams
////////////////////////////

// Constructors and Destructors
/////////////////////////////////

dealGameRunParams::dealGameRunParams(){
	pGameProfile = NULL;
	pWidth = 0;
	pHeight = 0;
	pFullScreen = false;
}

dealGameRunParams::~dealGameRunParams(){
	SetGameProfile( NULL );
}



// Management
///////////////

void dealGameRunParams::SetGameProfile( dealGameProfile *profile ){
	if( profile != pGameProfile ){
		if( pGameProfile ) pGameProfile->FreeReference();
		pGameProfile = profile;
		if( profile ) profile->AddReference();
	}
}

void dealGameRunParams::SetRunArguments( const char *arguments ){
	if( ! arguments ) DETHROW( deeInvalidParam );
	pRunArguments = arguments;
}

void dealGameRunParams::SetWidth( int width ){
	if( width < 1 ) DETHROW( deeInvalidParam );
	
	pWidth = width;
}

void dealGameRunParams::SetHeight( int height ){
	if( height < 1 ) DETHROW( deeInvalidParam );
	
	pHeight = height;
}

void dealGameRunParams::SetFullScreen( bool fullScreen ){
	pFullScreen = fullScreen;
}
