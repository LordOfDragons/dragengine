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

#include "lpeULangPackSetMissingText.h"
#include "../../langpack/lpeLangPack.h"

#include <dragengine/common/exceptions.h>



// Class lpeULangPackSetMissingText
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

lpeULangPackSetMissingText::lpeULangPackSetMissingText(
	lpeLangPack *langpack, const decUnicodeString &newMissingText ) :
pLangPack( NULL ),
pNewMissingText( newMissingText )
{
	if( ! langpack ){
		DETHROW( deeInvalidParam );
	}
	
	
	SetShortInfo( "LangPack set missing text" );
	
	pOldMissingText = langpack->GetMissingText();
	
	pLangPack = langpack;
	langpack->AddReference();
}

lpeULangPackSetMissingText::~lpeULangPackSetMissingText(){
	if( pLangPack ){
		pLangPack->FreeReference();
	}
}



// Management
///////////////

void lpeULangPackSetMissingText::Undo(){
	pLangPack->SetMissingText( pOldMissingText );
}

void lpeULangPackSetMissingText::Redo(){
	pLangPack->SetMissingText( pNewMissingText );
}
