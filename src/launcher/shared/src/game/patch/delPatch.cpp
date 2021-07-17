/* 
 * Drag[en]gine GUI Launcher
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

#include "delPatch.h"

#include <dragengine/common/exceptions.h>



// Class delPatch
////////////////////

// Constructors and Destructors
/////////////////////////////////

delPatch::delPatch(){
}

delPatch::~delPatch(){
}



// Management
///////////////

void delPatch::SetIdentifier( const decUuid &identifier ){
	pIdentifier = identifier;
}

void delPatch::SetName( const decUnicodeString &name ){
	pName = name;
}

void delPatch::SetDescription( const decUnicodeString &description ){
	pDescription = description;
}

void delPatch::SetCreator( const decUnicodeString &creator ){
	pCreator = creator;
}

void delPatch::SetHomepage( const char *homepage ){
	pHomepage = homepage;
}

void delPatch::SetPatchDirectory( const char *directory ){
	pPatchDirectory = directory;
}

void delPatch::SetDataDirectory( const char *directory ){
	pDataDirectory = directory;
}

void delPatch::SetGameID( const decUuid &id ){
	pGameID = id;
}

void delPatch::SetDelgaFile( const char *file ){
	pDelgaFile = file;
}
