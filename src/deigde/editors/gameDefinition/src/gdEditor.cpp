/* 
 * Drag[en]gine IGDE Game Definition Editor
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

#include "gdeIGDEModule.h"

#include <dragengine/common/exceptions.h>



// export definition
#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR igdeEditorModule *GameDefEditorCreateModule( igdeEnvironment *environment );
#ifdef  __cplusplus
}
#endif



// Entry point
////////////////

igdeEditorModule *GameDefEditorCreateModule( igdeEnvironment *environment ){
	try{
		return new gdeIGDEModule( *environment );
		
	}catch( const deException & ){
		return NULL;
	}
}
