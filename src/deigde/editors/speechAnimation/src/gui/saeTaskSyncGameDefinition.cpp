/* 
 * Drag[en]gine IGDE Speech Animation Editor
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

#include "saeTaskSyncGameDefinition.h"
#include "saeWindowMain.h"
#include "../sanimation/saeSAnimation.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gui/wrapper/igdeWSky.h>

#include <dragengine/common/exceptions.h>



// Class saeTaskSyncGameDefinition
///////////////////////////////////

// Constructor, destructor
////////////////////////////

saeTaskSyncGameDefinition::saeTaskSyncGameDefinition( saeWindowMain &windowMain ) :
pWindowMain( windowMain )
{
	SetMessage( "Speech Animation Editor: Synchronize Game Definition" );
}

saeTaskSyncGameDefinition::~saeTaskSyncGameDefinition(){
}



// Management
///////////////

bool saeTaskSyncGameDefinition::Step(){
	if( ! pWindowMain.GetSAnimation() ){
		return false;
	}
	
	saeSAnimation &sanimation = *pWindowMain.GetSAnimation();
	
	sanimation.GetSky()->OnGameDefinitionChanged();
	
	return false;
}
