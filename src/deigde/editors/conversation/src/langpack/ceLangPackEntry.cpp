/* 
 * Drag[en]gine IGDE Conversation Editor
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#include "ceLangPackEntry.h"

#include <dragengine/common/exceptions.h>



// Class ceLangPackEntry
//////////////////////////

// Constructor, destructor
////////////////////////////

ceLangPackEntry::ceLangPackEntry(){
}

ceLangPackEntry::~ceLangPackEntry(){
}



// Management
///////////////

void ceLangPackEntry::SetName( const char *name ){
	pName = name;
}

void ceLangPackEntry::SetText( const decUnicodeString &text ){
	pText = text;
}