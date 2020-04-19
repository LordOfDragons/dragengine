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

#include "igdeActionSelectFile.h"
#include "../igdeCommonDialogs.h"
#include "../igdeContainer.h"
#include "../igdeTextField.h"

#include <dragengine/common/exceptions.h>



// Class igdeActionSelectFile
///////////////////////////////

// Constructor, destructor
////////////////////////////

igdeActionSelectFile::igdeActionSelectFile( igdeEnvironment &environment,
	igdeEnvironment::eFilePatternListTypes resourceType, igdeTextField &textField, bool useGameVFS ) :
igdeAction( "...", NULL, "Select file" ),
pEnvironment( environment ),
pFilePatterns( *pEnvironment.GetOpenFilePatternList( resourceType ) ),
pUseGameVFS( useGameVFS ),
pTextField( textField ){
}

igdeActionSelectFile::igdeActionSelectFile( igdeEnvironment &environment,
const igdeFilePatternList &filePatterns, igdeTextField &textField, bool useGameVFS ) :
igdeAction( "...", NULL, "Select file" ),
pEnvironment( environment ),
pFilePatterns( filePatterns ),
pUseGameVFS( useGameVFS ),
pTextField( textField ){
}

igdeActionSelectFile::~igdeActionSelectFile(){
}



// Management
///////////////

void igdeActionSelectFile::OnAction(){
	igdeContainer * const parent = pTextField.GetParent();
	if( ! parent ){
		return;
	}
	
	decString path( pTextField.GetText() );
	if( path.IsEmpty() ){
		path = DefaultPath();
	}
	
	const decString oldPath( path );
	
	if( pUseGameVFS ){
		if( ! igdeCommonDialogs::GetFileOpen( &pTextField, GetText(),
		*pEnvironment.GetFileSystemGame(), pFilePatterns, path ) ){
			return;
		}
		
	}else{
		if( ! igdeCommonDialogs::GetFileOpen( &pTextField, GetText(), pFilePatterns, path ) ){
			return;
		}
	}
	
	if( path == oldPath || ! AcceptFile( path ) ){
		return;
	}
	
	pTextField.SetText( path );
	pTextField.NotifyTextChanged();
}

bool igdeActionSelectFile::AcceptFile( decString& ){
	return true;
}

decString igdeActionSelectFile::DefaultPath(){
	return decString();
}
