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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gdeMAOCEnvMapProbeAdd.h"
#include "../../../gdeWindowMain.h"
#include "../../../../gamedef/gdeGameDefinition.h"
#include "../../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../../gamedef/objectClass/envmapprobe/gdeOCEnvMapProbe.h"
#include "../../../../undosys/objectClass/envmapprobe/gdeUOCAddEnvMapProbe.h"

#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/deEngine.h>
#include <dragengine/deObjectReference.h>
#include <dragengine/common/exceptions.h>



// Class gdeMAOCEnvMapProbeAdd
////////////////////////////////

// Constructor
////////////////

gdeMAOCEnvMapProbeAdd::gdeMAOCEnvMapProbeAdd( gdeWindowMain &windowMain ) :
gdeBaseMAOCSubObject( windowMain, "Add Object Class Environment Map Probe...",
	windowMain.GetEnvironment().GetStockIcon( igdeEnvironment::esiPlus ),
	"Add object class environment map probe" )
{
}



// Management
///////////////

igdeUndo *gdeMAOCEnvMapProbeAdd::OnActionSubObject( gdeGameDefinition&, gdeObjectClass &objectClass ){
	deObjectReference envMapProbe;
	envMapProbe.TakeOver( new gdeOCEnvMapProbe );
	return new gdeUOCAddEnvMapProbe( &objectClass, ( gdeOCEnvMapProbe* )( deObject* )envMapProbe );
}

void gdeMAOCEnvMapProbeAdd::Update(){
	SetEnabled( GetActiveObjectClass() != NULL );
}
