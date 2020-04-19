/* 
 * Drag[en]gine Python Script Module
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

#include "spScriptFile.h"
#include "spScriptFileList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class spScriptFile
///////////////////////////

// Constructor, destructor
////////////////////////////

spScriptFileList::spScriptFileList( ScriptingPython *sp ){
	if( ! sp ){
		DETHROW( deeInvalidParam );
	}
	
	pSP = sp;
}

spScriptFileList::~spScriptFileList(){
	RemoveAll();
}



// Management
///////////////

int spScriptFileList::GetCount() const{
	return pFiles.GetCount();
}

spScriptFile *spScriptFileList::GetAt( int index ) const{
	return ( spScriptFile* )pFiles.GetAt( index );
}

spScriptFile *spScriptFileList::GetWithFullName( const char *fullname ) const{
	const int count = pFiles.GetCount();
	spScriptFile *file;
	int i;
	
	for( i =0; i <count; i++ ){
		file = ( spScriptFile* )pFiles.GetAt( i );
		
		if( file->GetFullModuleName().Equals( fullname ) ){
			return file;
		}
	}
	
	return NULL;
}

int spScriptFileList::IndexOf( spScriptFile *file ) const{
	return pFiles.IndexOf( file );
}

int spScriptFileList::IndexOfWithFullName( const char *fullname ) const{
	const int count = pFiles.GetCount();
	int i;
	
	for( i =0; i <count; i++ ){
		if( ( ( spScriptFile* )pFiles.GetAt( i ) )->GetFullModuleName().Equals( fullname ) ){
			return i;
		}
	}
	
	return -1;
}

bool spScriptFileList::Has( spScriptFile *file ) const{
	return pFiles.Has( file );
}

bool spScriptFileList::HasWithFullName( const char *fullname ) const{
	const int count = pFiles.GetCount();
	int i;
	
	for( i =0; i <count; i++ ){
		if( ( ( spScriptFile* )pFiles.GetAt( i ) )->GetFullModuleName().Equals( fullname ) ){
			return true;
		}
	}
	
	return false;
}

void spScriptFileList::Add( spScriptFile *entry ){
	if( ! entry || HasWithFullName( entry->GetFullModuleName().GetString() ) ){
		DETHROW( deeInvalidParam );
	}
	
	pFiles.Add( entry );
}

void spScriptFileList::Remove( spScriptFile *entry ){
	pFiles.Remove( entry );
}

void spScriptFileList::RemoveAll(){
	pFiles.RemoveAll();
}



void spScriptFileList::PythonCleanUpAll(){
	const int count = pFiles.GetCount();
	int i;
	
	for( i =0; i <count; i++ ){
		( ( spScriptFile* )pFiles.GetAt( i ) )->PythonCleanUp();
	}
}
