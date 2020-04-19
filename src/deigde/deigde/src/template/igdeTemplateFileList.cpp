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
#include "igdeTemplateFileList.h"

#include <dragengine/common/exceptions.h>



// Class igdeTemplateFile
///////////////////////////

// Constructor, destructor
////////////////////////////

igdeTemplateFileList::igdeTemplateFileList(){
}

igdeTemplateFileList::igdeTemplateFileList( const igdeTemplateFileList &list ) :
pFiles( list.pFiles ){
}

igdeTemplateFileList::~igdeTemplateFileList(){
}



// Management
///////////////

int igdeTemplateFileList::GetCount() const{
	return pFiles.GetCount();
}

igdeTemplateFile *igdeTemplateFileList::GetAt( int index ) const{
	return ( igdeTemplateFile* )pFiles.GetAt( index );
}

int igdeTemplateFileList::IndexOf( igdeTemplateFile *file ) const{
	return pFiles.IndexOf( file );
}

bool igdeTemplateFileList::Has( igdeTemplateFile *file ) const{
	return pFiles.Has( file );
}

void igdeTemplateFileList::Add( igdeTemplateFile *file ){
	if( ! file ){
		DETHROW( deeInvalidParam );
	}
	pFiles.Add( file );
}

void igdeTemplateFileList::Remove( igdeTemplateFile *file ){
	pFiles.Remove( file );
}

void igdeTemplateFileList::RemoveAll(){
	pFiles.RemoveAll();
}



// Operators
//////////////

igdeTemplateFileList &igdeTemplateFileList::operator=( const igdeTemplateFileList &list ){
	pFiles = list.pFiles;
	return *this;
}
