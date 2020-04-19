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
#include <string.h>

#include "deClassParticleEmitterInstance.h"
#include "deClassParticleEmitterInstanceListener.h"
#include "../math/deClassDVector.h"
#include "../physics/deClassCollisionInfo.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <libdscript/exceptions.h>
#include <dragengine/resources/particle/deParticleEmitterType.h>
#include <dragengine/systems/modules/scripting/deBaseScriptingParticleEmitterInstance.h>



// Native Functions
/////////////////////

// public func void lastParticleDied( ParticleEmitterInstance instance )
deClassParticleEmitterInstanceListener::nfLastParticleDied::nfLastParticleDied( const sInitData &init ) :
dsFunction( init.clsPEIL, "lastParticleDied", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_ABSTRACT | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsPEI ); // instance
}
void deClassParticleEmitterInstanceListener::nfLastParticleDied::RunFunction( dsRunTime *rt, dsValue *myself ){
}

// public func void collisionResponse( ParticleEmitterInstance instance, CollisionInfo collisionInfo )
deClassParticleEmitterInstanceListener::nfCollisionResponse::nfCollisionResponse( const sInitData &init ) :
dsFunction( init.clsPEIL, "collisionResponse", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_ABSTRACT | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsPEI ); // instance
	p_AddParameter( init.clsCI ); // cinfo
}
void deClassParticleEmitterInstanceListener::nfCollisionResponse::RunFunction( dsRunTime *rt, dsValue *myself ){
}



// Class deClassParticleEmitterInstanceListener
/////////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deClassParticleEmitterInstanceListener::deClassParticleEmitterInstanceListener( deScriptingDragonScript *ds ) :
dsClass( "ParticleEmitterInstanceListener", DSCT_INTERFACE, DSTM_PUBLIC | DSTM_NATIVE | DSTM_ABSTRACT ){
	if( ! ds ){
		DSTHROW( dueInvalidParam );
	}
	
	pDS = ds;
	
	GetParserInfo()->SetParent( DENS_SCENERY );
	
	p_SetNativeDataSize( 0 );
}

deClassParticleEmitterInstanceListener::~deClassParticleEmitterInstanceListener(){
}



// Management
///////////////

void deClassParticleEmitterInstanceListener::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	init.clsPEIL = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsFlt = engine->GetClassFloat();
	init.clsInt = engine->GetClassInt();
	init.clsPEI = pDS->GetClassParticleEmitterInstance();
	init.clsCI = pDS->GetClassCollisionInfo();
	
	AddFunction( new nfLastParticleDied( init ) ); // function 0
	AddFunction( new nfCollisionResponse( init ) ); // function 1
	
	CalcMemberOffsets();
	
	const dsFuncList &funcList = *GetFuncList();
	pFuncIndexLastParticleDied = funcList.GetIndexOf( GetFunction( 0 ) );
	pFuncIndexCollisionResponse = funcList.GetIndexOf( GetFunction( 1 ) );
}
