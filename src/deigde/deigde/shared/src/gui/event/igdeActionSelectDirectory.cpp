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

#include "igdeActionSelectDirectory.h"
#include "../igdeCommonDialogs.h"
#include "../igdeContainer.h"
#include "../igdeTextField.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>



// Class igdeActionSelectDirectory
////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeActionSelectDirectory::igdeActionSelectDirectory( igdeEnvironment &environment,
	igdeTextField &textField, bool useGameVFS ) :
igdeAction( "...", NULL, "Select directory" ),
pEnvironment( environment ),
pUseGameVFS( useGameVFS ),
pTextField( textField ){
}

igdeActionSelectDirectory::~igdeActionSelectDirectory(){
}



// Management
///////////////

void igdeActionSelectDirectory::OnAction(){
	igdeContainer * const parent = pTextField.GetParent();
	if( ! parent ){
		return;
	}
	
	decString directory( pTextField.GetText() );
	if( directory.IsEmpty() ){
		directory = DefaultDirectory();
	}
	
	const decString oldDirectory( directory );
	
	if( pUseGameVFS ){
		if( ! igdeCommonDialogs::GetDirectory( &pTextField, GetText(), *pEnvironment.GetFileSystemGame(), directory ) ){
			return;
		}
		
	}else{
		if( ! igdeCommonDialogs::GetDirectory( &pTextField, GetText(), directory ) ){
			return;
		}
	}
	
	if( directory == oldDirectory || ! AcceptDirectory( directory ) ){
		return;
	}
	
	pTextField.SetText( directory );
	pTextField.NotifyTextChanged();
}

bool igdeActionSelectDirectory::AcceptDirectory( decString& ){
	return true;
}

decString igdeActionSelectDirectory::DefaultDirectory(){
	return pUseGameVFS ? "/" : decPath::CreateWorkingDirectory().GetPathNative();
}
