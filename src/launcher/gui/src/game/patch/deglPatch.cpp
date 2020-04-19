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

#include "deglPatch.h"

#include <dragengine/common/exceptions.h>



// Class deglPatch
////////////////////

// Constructors and Destructors
/////////////////////////////////

deglPatch::deglPatch(){
}

deglPatch::~deglPatch(){
}



// Management
///////////////

void deglPatch::SetIdentifier( const decUuid &identifier ){
	pIdentifier = identifier;
}

void deglPatch::SetName( const decUnicodeString &name ){
	pName = name;
}

void deglPatch::SetDescription( const decUnicodeString &description ){
	pDescription = description;
}

void deglPatch::SetCreator( const decUnicodeString &creator ){
	pCreator = creator;
}

void deglPatch::SetHomepage( const char *homepage ){
	pHomepage = homepage;
}

void deglPatch::SetPatchDirectory( const char *directory ){
	pPatchDirectory = directory;
}

void deglPatch::SetDataDirectory( const char *directory ){
	pDataDirectory = directory;
}

void deglPatch::SetGameID( const decUuid &id ){
	pGameID = id;
}

void deglPatch::SetDelgaFile( const char *file ){
	pDelgaFile = file;
}
