/* 
 * Drag[en]gine IGDE World Editor
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

#include "meIGDEModule.h"

#include <dragengine/common/exceptions.h>



// export definition
#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR igdeEditorModule *WorldEditorCreateModule( igdeEnvironment *environment );
#ifdef  __cplusplus
}
#endif



// entry point
////////////////

igdeEditorModule *WorldEditorCreateModule( igdeEnvironment *environment ){
	meIGDEModule *module = NULL;
	
	try{
		module = new meIGDEModule( *environment );
		
	}catch( const deException & ){
		return NULL;
	}
	
	return module;
}
