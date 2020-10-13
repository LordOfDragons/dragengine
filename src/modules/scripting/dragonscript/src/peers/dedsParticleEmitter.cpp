/* 
 * Drag[en]gine DragonScript Script Module
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

#include <libdscript/libdscript.h>

#include "dedsParticleEmitter.h"
#include "../deScriptingDragonScript.h"
#include "../classes/math/deClassDVector.h"
#include "../classes/particle/deClassParticleEmitterInstance.h"
#include "../classes/particle/deClassParticleEmitterInstanceListener.h"
#include "../classes/physics/deClassCollisionInfo.h"

#include <dragengine/resources/particle/deParticleEmitterType.h>
#include <dragengine/resources/particle/deParticleEmitterInstance.h>
#include <libdscript/exceptions.h>



// Class dedsParticleEmitter
//////////////////////////////

// Constructor, destructor
////////////////////////////

dedsParticleEmitter::dedsParticleEmitter( deScriptingDragonScript *ds, deParticleEmitterInstance *emitter ){
	if( ! ds || ! emitter ){
		DSTHROW( dueInvalidParam );
	}
	
	dsRunTime *rt = ds->GetScriptEngine()->GetMainRunTime();
	
	pDS = ds;
	pEmitter = emitter;
	pHasCB = false;
	pValCB = rt->CreateValue( ds->GetClassParticleEmitterInstanceListener() );
}

dedsParticleEmitter::~dedsParticleEmitter(){
	if( ! pValCB ){
		return;
	}
	
	// check if the resource is in progress of being deleted. if this is not
	// the case we can end up re-entering this destructor due to the resource
	// being deleted due to links breaking while freeing the value. if this
	// is the case delay the deletion until a safe time
	if( pEmitter && pEmitter->GetRefCount() > 0 ){
		pDS->AddValueDeleteLater( pValCB );
		
	}else{
		pDS->GetScriptEngine()->GetMainRunTime()->FreeValue( pValCB );
	}
	
	pValCB = NULL;
	pHasCB = false;
}



// Management
///////////////

dsRealObject *dedsParticleEmitter::GetCallback() const{
	return pValCB->GetRealObject();
}

void dedsParticleEmitter::SetCallback( dsRealObject *object ){
	if( pValCB ){
		dsRunTime &rt = *pDS->GetScriptEngine()->GetMainRunTime();
		
		if( object ){
			rt.SetObject( pValCB, object );
			rt.CastValueTo( pValCB, pValCB, pDS->GetClassParticleEmitterInstanceListener() );
			pHasCB = true;
			
		}else{
			rt.SetNull( pValCB, pDS->GetClassParticleEmitterInstanceListener() );
			pHasCB = false;
		}
	}
}



// Notifications
//////////////////

void dedsParticleEmitter::LastParticleDied(){
	if( ! pHasCB ){
		return;
	}
	
	const int funcIndex = pDS->GetClassParticleEmitterInstanceListener()->GetFuncIndexLastParticleDied();
	dsRunTime * const rt = pDS->GetScriptEngine()->GetMainRunTime();
	deClassParticleEmitterInstance &clsPEI = *pDS->GetClassParticleEmitterInstance();
	
	// lastParticleDied( ParticleEmitter emitter )
	try{
		clsPEI.PushInstance( rt, pEmitter );
		rt->RunFunctionFast( pValCB, funcIndex );
		
	}catch( const duException &e ){
		rt->PrintExceptionTrace();
		e.PrintError();
	}
}

void dedsParticleEmitter::CollisionResponse( deCollisionInfo *collisionInfo ){
	if( pHasCB ){
		const int funcIndex = pDS->GetClassParticleEmitterInstanceListener()->GetFuncIndexCollisionResponse();
		dsRunTime * const rt = pDS->GetScriptEngine()->GetMainRunTime();
		deClassParticleEmitterInstance &clsPEI = *pDS->GetClassParticleEmitterInstance();
		deClassCollisionInfo &clsCI = *pDS->GetClassCollisionInfo();
		
		// void collisionResponse( ParticleEmitterInstance instance, CollisionInfo collisionInfo )
		try{
			clsCI.PushInfo( rt, collisionInfo ); // CollisionInfo collisionInfo
			clsPEI.PushInstance( rt, pEmitter ); // ParticleEmitterInstance instance
			rt->RunFunctionFast( pValCB, funcIndex );
			
		}catch( const duException &e ){
			rt->PrintExceptionTrace();
			e.PrintError();
		}
	}
}
