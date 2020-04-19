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

#include "lpeULangPackSetDescription.h"
#include "../../langpack/lpeLangPack.h"

#include <dragengine/common/exceptions.h>



// Class lpeULangPackSetDescription
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

lpeULangPackSetDescription::lpeULangPackSetDescription(
	lpeLangPack *langpack, const decUnicodeString &newDescription ) :
pLangPack( NULL ),
pNewDescription( newDescription )
{
	if( ! langpack ){
		DETHROW( deeInvalidParam );
	}
	
	
	SetShortInfo( "LangPack set description" );
	
	pOldDescription = langpack->GetDescription();
	
	pLangPack = langpack;
	langpack->AddReference();
}

lpeULangPackSetDescription::~lpeULangPackSetDescription(){
	if( pLangPack ){
		pLangPack->FreeReference();
	}
}



// Management
///////////////

void lpeULangPackSetDescription::Undo(){
	pLangPack->SetDescription( pOldDescription );
}

void lpeULangPackSetDescription::Redo(){
	pLangPack->SetDescription( pNewDescription );
}
