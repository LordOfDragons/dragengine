/* 
 * Drag[en]gine IGDE Project Editor
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
#include <stdio.h>

#include "projDownSizer.h"
#include "../projProfile.h"
#include "../../projProject.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decMath.h>



// Class projDownSizer
////////////////////////

// Constructor, destructor
////////////////////////////

projDownSizer::projDownSizer() :
pProfile( NULL ),
pQuality( 100 ),
pScale( 0 ){
}

projDownSizer::projDownSizer( const projDownSizer &downsizer ) :
pProfile( NULL ),
pDescription( downsizer.pDescription ),
pPatterns( downsizer.pPatterns ),
pQuality( downsizer.pQuality ),
pScale( downsizer.pScale ){
}

projDownSizer::~projDownSizer(){
	pCleanUp();
}



// Management
///////////////

void projDownSizer::SetProfile( projProfile *profile ){
	pProfile = profile;
}

void projDownSizer::SetDescription( const char *description ){
	if( pDescription == description ){
		return;
	}
	
	pDescription = description;
	NotifyChanged();
}

void projDownSizer::SetPatterns( const decStringSet &patterns ){
	if( patterns == pPatterns ){
		return;
	}
	
	pPatterns = patterns;
	NotifyChanged();
}

void projDownSizer::SetQuality( int quality ){
	quality = decMath::clamp( quality, 0, 100 );
	if( quality == pQuality ){
		return;
	}
	
	pQuality = quality;
	NotifyChanged();
}

void projDownSizer::SetScale( int scale ){
	if( scale < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( scale == pScale ){
		return;
	}
	
	pScale = scale;
	NotifyChanged();
}



void projDownSizer::NotifyChanged(){
	if( pProfile && pProfile->GetProject() ){
		//pProfile->GetProject()->NotifyDownSizerChanged( pProfile, this );
	}
}



// Private Functions
//////////////////////

void projDownSizer::pCleanUp(){
}
