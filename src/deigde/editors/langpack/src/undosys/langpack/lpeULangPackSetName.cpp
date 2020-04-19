/* 
 * Drag[en]gine IGDE Language Pack Editor
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
#include <string.h>
#include <stdlib.h>

#include "lpeULangPackSetName.h"
#include "../../langpack/lpeLangPack.h"

#include <dragengine/common/exceptions.h>



// Class lpeULangPackSetName
//////////////////////////////

// Constructor, destructor
////////////////////////////

lpeULangPackSetName::lpeULangPackSetName( lpeLangPack *langpack, const decUnicodeString &newName ) :
pLangPack( NULL ),
pNewName( newName )
{
	if( ! langpack ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "LangPack set name" );
	
	pOldName = langpack->GetName();
	
	pLangPack = langpack;
	langpack->AddReference();
}

lpeULangPackSetName::~lpeULangPackSetName(){
	if( pLangPack ){
		pLangPack->FreeReference();
	}
}



// Management
///////////////

void lpeULangPackSetName::Undo(){
	pLangPack->SetName( pOldName );
}

void lpeULangPackSetName::Redo(){
	pLangPack->SetName( pNewName );
}
