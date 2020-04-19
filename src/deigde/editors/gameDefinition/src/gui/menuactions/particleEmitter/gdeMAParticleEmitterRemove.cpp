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

#include "gdeMAParticleEmitterRemove.h"
#include "../../gdeWindowMain.h"
#include "../../../gamedef/gdeGameDefinition.h"
#include "../../../undosys/particleemitter/gdeURemoveParticleEmitter.h"

#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class gdeMAParticleEmitterRemove
/////////////////////////////////////

// Constructor
////////////////

gdeMAParticleEmitterRemove::gdeMAParticleEmitterRemove( gdeWindowMain &windowMain ) :
gdeBaseAction( windowMain, "Remove Particle Emitter",
	windowMain.GetEnvironment().GetStockIcon( igdeEnvironment::esiMinus ),
	"Remove particle emitter" )
{
}



// Management
///////////////

igdeUndo *gdeMAParticleEmitterRemove::OnAction( gdeGameDefinition &gameDefinition ){
	gdeParticleEmitter * const category = gameDefinition.GetActiveParticleEmitter();
	if( ! category || gameDefinition.GetSelectedObjectType() != gdeGameDefinition::eotParticleEmitter ){
		return NULL;
	}
	
	gdeParticleEmitter * const particleEmitter = gameDefinition.GetActiveParticleEmitter();
	if( ! particleEmitter ){
		return NULL;
	}
	
	return new gdeURemoveParticleEmitter( &gameDefinition, particleEmitter );
}

void gdeMAParticleEmitterRemove::Update(){
	gdeGameDefinition * const gameDefinition = pWindowMain.GetActiveGameDefinition();
	if( ! gameDefinition ){
		SetEnabled( false );
		return;
	}
	
	SetEnabled( gameDefinition->GetSelectedObjectType() == gdeGameDefinition::eotParticleEmitter 
		&& gameDefinition->GetActiveParticleEmitter() != NULL );
}
