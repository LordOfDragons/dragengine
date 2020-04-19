/* 
 * Drag[en]gine Game Engine
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

#include <stdlib.h>

#include "deBaseArchiveContainer.h"

#include "../../../common/file/decBaseFileReader.h"
#include "../../../common/file/decBaseFileWriter.h"
#include "../../../common/exceptions.h"



// Class deBaseArchiveContainer
/////////////////////////////////

deBaseArchiveContainer::deBaseArchiveContainer( decBaseFileReader *reader ) :
pReader( NULL )
{
	if( ! reader ){
		DETHROW( deeInvalidParam );
	}
	
	pReader = reader;
	reader->AddReference();
}

deBaseArchiveContainer::~deBaseArchiveContainer(){
	if( pReader ){
		pReader->FreeReference();
	}
}
