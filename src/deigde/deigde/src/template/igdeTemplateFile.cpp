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

#include "igdeTemplateFile.h"
#include "igdeTemplateReplace.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class igdeTemplateFile
///////////////////////////

// Constructor, destructor
////////////////////////////

igdeTemplateFile::igdeTemplateFile() :
pDirectory( edData ){
}

igdeTemplateFile::igdeTemplateFile( const igdeTemplateFile &file ) :
pPath( file.pPath ),
pPattern( file.pPattern ),
pDirectory( file.pDirectory )
{
	const int count = file.pReplacements.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		pReplacements.Add( new igdeTemplateReplace( *file.pReplacements.GetAt( i ) ) );
	}
}

igdeTemplateFile::~igdeTemplateFile(){
}



// Management
///////////////

void igdeTemplateFile::SetPath( const char *path ){
	pPath = path;
}

void igdeTemplateFile::SetPattern( const char *pattern ){
	pPattern = pattern;
}

void igdeTemplateFile::SetDirectory( eDirectories directory ){
	pDirectory = directory;
}
