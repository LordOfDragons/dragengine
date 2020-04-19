/* 
 * Drag[en]gine Theora Video Module
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

#include "dethInfos.h"

#include <dragengine/common/exceptions.h>



// Class dethInfos
////////////////////

// Constructor, destructor
////////////////////////////

dethInfos::dethInfos() :
pSetupInfo( NULL ),
pHeaderFinished( false ),
pAudioHeaderFinished( false )
{
	th_info_init( &pInfo );
	th_comment_init( &pComment );
	
	vorbis_info_init( &pAudioInfo );
	vorbis_comment_init( &pAudioComment );
}

dethInfos::~dethInfos(){
	if( pSetupInfo ){
		th_setup_free( pSetupInfo );
	}
	th_comment_clear( &pComment );
	th_info_clear( &pInfo );
	
	vorbis_comment_clear( &pAudioComment );
	vorbis_info_clear( &pAudioInfo );
}



// Management
///////////////

void dethInfos::SetSetupInfo( th_setup_info *info ){
	pSetupInfo = info;
}

void dethInfos::SetHeaderFinished( bool headerFinished ){
	pHeaderFinished = headerFinished;
}

void dethInfos::SetAudioHeaderFinished( bool headerFinished ){
	pAudioHeaderFinished = headerFinished;
}
