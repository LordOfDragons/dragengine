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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef OS_UNIX
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#endif

#include "igdeActionExternOpen.h"
#include "../../environment/igdeEnvironment.h"
#include "../../undo/igdeUndoSystem.h"

#include <dragengine/common/exceptions.h>

#ifdef OS_W32
#include <dragengine/app/deOSWindows.h>
#endif



// Class igdeActionExternOpen
///////////////////////////////

// Constructor, destructor
////////////////////////////

igdeActionExternOpen::igdeActionExternOpen( igdeEnvironment &environment,
	const char *text, igdeIcon *icon, const char *description, const char *path ) :
igdeAction( text, icon, description ),
pEnvironment( environment ),
pPath( path )
{
	Update();
}

igdeActionExternOpen::~igdeActionExternOpen(){
}



// Management
///////////////

void igdeActionExternOpen::SetPath( const char *path ){
	if( pPath == path ){
		return;
	}
	
	pPath = path;
	Update();
}



void igdeActionExternOpen::OnAction(){
	if( pPath.IsEmpty() ){
		return;
	}
	
	#ifdef OS_W32
	wchar_t widePath[ MAX_PATH ];
	deOSWindows::Utf8ToWide( pPath, widePath, MAX_PATH );
	ShellExecute( NULL, L"open", widePath, NULL, NULL, SW_SHOWDEFAULT );
		
	#else
	const char * const appname = "xdg-open";
	
	if( fork() == 0 ){
		// GetString() is required otherwise execlp fails to run correctly
		execlp( appname, appname, pPath.GetString(), NULL );
		printf( "Failed running '%s' (error %d)\n", appname, errno );
		exit( 0 );
	}
	#endif
}

void igdeActionExternOpen::Update(){
	SetEnabled( ! pPath.IsEmpty() );
}
