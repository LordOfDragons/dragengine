/* 
 * Drag[en]gine OpenGL Graphic Module
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

#include "deoglRLight.h"
#include "deoglNotifyEnvMapLightChanged.h"

#include "../world/deoglWorldOctree.h"
#include "../envmap/deoglEnvironmentMap.h"

#include <dragengine/common/exceptions.h>



// Class deoglNotifyEnvMapLightChanged
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglNotifyEnvMapLightChanged::deoglNotifyEnvMapLightChanged( deoglRLight &light ) :
pLight( light )
{
	pLightBox.SetFromExtends( light.GetMinimumExtend(), light.GetMaximumExtend() );
	SetVisitEnvMaps( true );
}



// Visiting
/////////////

void deoglNotifyEnvMapLightChanged::VisitEnvMaps( deoglEnvironmentMap *envmap ){
	if( envmap->GetSkyOnly() ){
		return;
	}
	
	if( envmap->GetHasInfluenceBox() ){
		if( ! envmap->GetInfluenceCollisionBox().BoxHitsBox( &pLightBox ) ){
			return;
		}
	}
	
	envmap->LightChanged( &pLight );
}
