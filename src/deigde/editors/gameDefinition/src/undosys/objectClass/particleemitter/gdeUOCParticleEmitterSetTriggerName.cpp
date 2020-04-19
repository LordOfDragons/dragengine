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
#include <string.h>
#include <stdlib.h>

#include "gdeUOCParticleEmitterSetTriggerName.h"
#include "../../../gamedef/objectClass/gdeObjectClass.h"

#include <dragengine/common/exceptions.h>



// Class gdeUOCParticleEmitterSetTriggerName
//////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

gdeUOCParticleEmitterSetTriggerName::gdeUOCParticleEmitterSetTriggerName( gdeObjectClass *objectClass,
gdeOCParticleEmitter *particleEmitter, gdeOCParticleEmitter::eTriggers trigger, const char *newValue ) :
pObjectClass( NULL ),
pParticleEmitter( NULL ),
pTrigger( trigger )
{
	if( ! objectClass || ! particleEmitter ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "ParticleEmitter set trigger name" );
	
	pOldValue = particleEmitter->GetTriggerName( trigger );
	pNewValue = newValue;
	
	pParticleEmitter = particleEmitter;
	particleEmitter->AddReference();
	
	pObjectClass = objectClass;
	objectClass->AddReference();
}

gdeUOCParticleEmitterSetTriggerName::~gdeUOCParticleEmitterSetTriggerName(){
	if( pParticleEmitter ){
		pParticleEmitter->FreeReference();
	}
	if( pObjectClass ){
		pObjectClass->FreeReference();
	}
}



// Management
///////////////

void gdeUOCParticleEmitterSetTriggerName::Undo(){
	pParticleEmitter->SetTriggerName( pTrigger, pOldValue );
	pObjectClass->NotifyParticleEmitterChanged( pParticleEmitter );
}

void gdeUOCParticleEmitterSetTriggerName::Redo(){
	pParticleEmitter->SetTriggerName( pTrigger, pNewValue );
	pObjectClass->NotifyParticleEmitterChanged( pParticleEmitter );
}
