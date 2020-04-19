/* 
 * Drag[en]gine DragonScript Script Module
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

#include "dedsSTNText.h"

#include <libdscript/exceptions.h>



// Class dedsSTNText
//////////////////////

// Constructor, destructor
////////////////////////////


dedsSTNText::dedsSTNText(){
	pText = new char[ 1 ];
	if( ! pText ) DSTHROW( dueOutOfMemory );
	pText[ 0 ] = '\0';
}

dedsSTNText::dedsSTNText( const char* text ){
	if( ! pText ) DSTHROW( dueInvalidParam );
	
	pText = new char[ strlen( text ) + 1 ];
	if( ! pText ) DSTHROW( dueOutOfMemory );
	strcpy( pText, text );
}

dedsSTNText::~dedsSTNText(){
	if( pText ) delete [] pText;
}



// Management
///////////////


void dedsSTNText::SetText ( const char* text ){
	if( ! text ) DSTHROW( dueInvalidParam );
	
	char *ntext = new char[ strlen( text ) + 1 ];
	if( ! ntext ) DSTHROW( dueOutOfMemory );
	strcpy( ntext, text );
	
	delete [] pText;
	pText = ntext;
}
