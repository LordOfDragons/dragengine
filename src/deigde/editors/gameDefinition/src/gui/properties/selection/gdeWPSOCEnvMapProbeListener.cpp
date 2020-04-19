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

#include "gdeWPSOCEnvMapProbe.h"
#include "gdeWPSOCEnvMapProbeListener.h"
#include "../../../gamedef/gdeGameDefinition.h"
#include "../../../gamedef/objectClass/envmapprobe/gdeOCEnvMapProbe.h"

#include <dragengine/common/exceptions.h>



// Class gdeWPSOCEnvMapProbeListener
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

gdeWPSOCEnvMapProbeListener::gdeWPSOCEnvMapProbeListener( gdeWPSOCEnvMapProbe &panel ) :
pPanel( panel ){
}

gdeWPSOCEnvMapProbeListener::~gdeWPSOCEnvMapProbeListener(){
}



// Management
///////////////

void gdeWPSOCEnvMapProbeListener::OCPropertyNameChanged( gdeGameDefinition*,
gdeObjectClass *objectClass, gdeProperty* ){
	if( pPanel.GetObjectClass() != objectClass ){
		return;
	}
	pPanel.UpdatePropertyList();
}

void gdeWPSOCEnvMapProbeListener::OCPropertiesChanged( gdeGameDefinition*,
gdeObjectClass *objectClass ){
	if( pPanel.GetObjectClass() != objectClass ){
		return;
	}
	pPanel.UpdatePropertyList();
}

void gdeWPSOCEnvMapProbeListener::ActiveObjectClassChanged( gdeGameDefinition* ){
	pPanel.UpdatePropertyList();
	pPanel.UpdateEnvMapProbe();
}



void gdeWPSOCEnvMapProbeListener::OCEnvMapProbesChanged( gdeGameDefinition*,
gdeObjectClass *objectClass ){
	if( pPanel.GetObjectClass() != objectClass ){
		return;
	}
	pPanel.UpdateEnvMapProbe();
}

void gdeWPSOCEnvMapProbeListener::OCEnvMapProbeChanged( gdeGameDefinition*,
gdeObjectClass *objectClass, gdeOCEnvMapProbe *envMapProbe ){
	if( pPanel.GetObjectClass() != objectClass ){
		return;
	}
	if( pPanel.GetEnvMapProbe() != envMapProbe ){
		return;
	}
	pPanel.UpdateEnvMapProbe();
}

void gdeWPSOCEnvMapProbeListener::ActiveOCEnvMapProbeChanged( gdeGameDefinition* ){
	pPanel.UpdateEnvMapProbe();
}
