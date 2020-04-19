/* 
 * Drag[en]gine IGDE
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

#include "igdeGDParticleEmitter.h"
#include "igdeGDParticleEmitterManager.h"
#include "../igdeGDCategory.h"
#include "../igdeGameDefinition.h"

#include <dragengine/common/exceptions.h>



// Class igdeGDParticleEmitterManager
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeGDParticleEmitterManager::igdeGDParticleEmitterManager() :
pDefaultPath( "default" )
{
	pCategories.TakeOver( new igdeGDCategory( "Particle Emitters" ) );
}

igdeGDParticleEmitterManager::~igdeGDParticleEmitterManager(){
}



// Management
///////////////

void igdeGDParticleEmitterManager::AddEmitter( igdeGDParticleEmitter *emitter ){
	if( ! emitter ){
		DETHROW( deeInvalidParam );
	}
	pEmitterList.Add( emitter );
}

void igdeGDParticleEmitterManager::RemoveEmitter( igdeGDParticleEmitter *emitter ){
	pEmitterList.Remove( emitter );
}

void igdeGDParticleEmitterManager::RemoveAllEmitters(){
	pEmitterList.RemoveAll();
}

void igdeGDParticleEmitterManager::SetDefaultPath( const char *path ){
	pDefaultPath = path;
}



void igdeGDParticleEmitterManager::UpdateWith( const igdeGDParticleEmitterManager &particleEmitterManager ){
	const int count = particleEmitterManager.GetEmitterList().GetCount();
	igdeGDParticleEmitter *emitter = NULL;
	igdeGDParticleEmitter *emitterCheck;
	int i;
	
	try{
		for( i=0; i<count; i++ ){
			emitter = new igdeGDParticleEmitter( *particleEmitterManager.GetEmitterList().GetAt( i ) );
			emitterCheck = pEmitterList.GetWithPath( emitter->GetPath().GetString() );
			if( emitterCheck ){
				RemoveEmitter( emitterCheck );
			}
			AddEmitter( emitter );
			emitter = NULL;
		}
		
	}catch( const deException & ){
		if( emitter ){
			emitter->FreeReference();
		}
		throw;
	}
	
	pCategories->UpdateWith( particleEmitterManager.pCategories );
	
	pDefaultPath = particleEmitterManager.pDefaultPath;
}
