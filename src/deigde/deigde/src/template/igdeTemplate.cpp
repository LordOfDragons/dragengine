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

#include "igdeTemplate.h"
#include "igdeTemplateFile.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class igdeTemplate
///////////////////////

// Constructor, destructor
////////////////////////////

igdeTemplate::igdeTemplate(){
}

igdeTemplate::igdeTemplate( const igdeTemplate &atemplate ) :
pBasePath( atemplate.pBasePath ),
pName( atemplate.pName ),
pDescription( atemplate.pDescription ),
pScriptModule( atemplate.pScriptModule ),
pBaseGameDefinitions( atemplate.pBaseGameDefinitions )
{
	const int fileCount = atemplate.pFiles.GetCount();
	igdeTemplateFile *file = NULL;
	int i;
	
	try{
		for( i=0; i<fileCount; i++ ){
			file = new igdeTemplateFile( *atemplate.pFiles.GetAt( i ) );
			pFiles.Add( file );
			file->FreeReference();
			file = NULL;
		}
		
	}catch( const deException & ){
		if( file ){
			file->FreeReference();
		}
		throw;
	}
}

igdeTemplate::~igdeTemplate(){
}



// Management
///////////////

void igdeTemplate::SetBasePath( const char *path ){
	pBasePath = path;
}

void igdeTemplate::SetName( const char *name ){
	pName = name;
}

void igdeTemplate::SetDescription( const char *description ){
	pDescription = description;
}

void igdeTemplate::SetScriptModule( const char* moduleName ){
	pScriptModule = moduleName;
}
