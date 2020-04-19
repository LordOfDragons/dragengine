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

#include "gdeClipboardDataOCEnvMapProbe.h"
#include "../gamedef/objectClass/envmapprobe/gdeOCEnvMapProbe.h"

#include <dragengine/common/exceptions.h>



// Class gdeClipboardDataOCEnvMapProbe
////////////////////////////////////////

const char * const gdeClipboardDataOCEnvMapProbe::TYPE_NAME = "OCEnvMapProbe";

// Constructor, destructor
////////////////////////////

gdeClipboardDataOCEnvMapProbe::gdeClipboardDataOCEnvMapProbe( gdeOCEnvMapProbe *envMapProbe ) :
igdeClipboardData( TYPE_NAME ),
pEnvMapProbe( NULL )
{
	if( ! envMapProbe ){
		DETHROW( deeInvalidParam );
	}
	
	pEnvMapProbe = envMapProbe;
	envMapProbe->AddReference();
}

gdeClipboardDataOCEnvMapProbe::~gdeClipboardDataOCEnvMapProbe(){
	if( pEnvMapProbe ){
		pEnvMapProbe->FreeReference();
	}
}
