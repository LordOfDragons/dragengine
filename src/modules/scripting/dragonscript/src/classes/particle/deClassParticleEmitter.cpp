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
#include <stdint.h>
#include <string.h>

#include "deClassParticleEmitter.h"
#include "deClassParticleEmitterController.h"
#include "../curve/deClassCurveBezier.h"
#include "../graphics/deClassComponent.h"
#include "../world/deClassSkin.h"
#include "../world/deClassModel.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/particle/deParticleEmitter.h>
#include <dragengine/resources/particle/deParticleEmitterParameter.h>
#include <dragengine/resources/particle/deParticleEmitterManager.h>
#include <dragengine/resources/particle/deParticleEmitterController.h>
#include <dragengine/resources/particle/deParticleEmitterType.h>

#include <libdscript/exceptions.h>
#include <libdscript/packages/default/dsClassEnumeration.h>



// Native Structure
/////////////////////

struct sPENatDat{
	deParticleEmitter *emitter;
};



// Native functions
/////////////////////

// public func new()
deClassParticleEmitter::nfNew::nfNew( const sInitData &init ) : dsFunction( init.clsPE,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassParticleEmitter::nfNew::RunFunction( dsRunTime*, dsValue *myself ){
	deScriptingDragonScript &ds = *( ( ( deClassParticleEmitter* )GetOwnerClass() )->GetDS() );
	deParticleEmitterManager &pemgr = *ds.GetGameEngine()->GetParticleEmitterManager();
	sPENatDat *nd = ( sPENatDat* )p_GetNativeData( myself );
	
	// clear ( important )
	nd->emitter = NULL;
	
	// create emitter
	nd->emitter = pemgr.CreateParticleEmitter();
}

// public func destructor()
deClassParticleEmitter::nfDestructor::nfDestructor( const sInitData &init ) : dsFunction( init.clsPE,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassParticleEmitter::nfDestructor::RunFunction( dsRunTime*, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sPENatDat *nd = ( sPENatDat* )p_GetNativeData( myself );
	
	// free emitter
	if( nd->emitter ){
		nd->emitter->FreeReference();
		nd->emitter = NULL;
	}
}



// Management
///////////////

// public func int getControllerCount()
deClassParticleEmitter::nfGetControllerCount::nfGetControllerCount( const sInitData &init ) :
dsFunction( init.clsPE, "getControllerCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassParticleEmitter::nfGetControllerCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deParticleEmitter &emitter = *( ( ( sPENatDat* )p_GetNativeData( myself ) )->emitter );
	rt->PushInt( emitter.GetControllerCount() );
}

// public func void setControllerCount( int count )
deClassParticleEmitter::nfSetControllerCount::nfSetControllerCount( const sInitData &init ) :
dsFunction( init.clsPE, "setControllerCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // count
}
void deClassParticleEmitter::nfSetControllerCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	deParticleEmitter *emitter = ( ( sPENatDat* )p_GetNativeData( myself ) )->emitter;
	int count = rt->GetValue( 0 )->GetInt();
	deParticleEmitterController *controller = NULL;
	
	if( count < 0 ) DSTHROW( dueInvalidParam );
	
	while( emitter->GetControllerCount() > count ){
		emitter->RemoveController( emitter->GetControllerAt( emitter->GetControllerCount() - 1 ) );
	}
	
	try{
		while( emitter->GetControllerCount() < count ){
			controller = new deParticleEmitterController;
			if( ! controller ) DSTHROW( dueOutOfMemory );
			emitter->AddController( controller );
			controller = NULL;
		}
	}catch( ... ){
		if( controller ) delete controller;
		throw;
	}
}

// public func ParticleEmitterController getControllerAt( int index )
deClassParticleEmitter::nfGetControllerAt::nfGetControllerAt( const sInitData &init ) :
dsFunction( init.clsPE, "getControllerAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsPEController ){
	p_AddParameter( init.clsInt ); // controller
}
void deClassParticleEmitter::nfGetControllerAt::RunFunction( dsRunTime *rt, dsValue *myself ){
	deParticleEmitter * const emitter = ( ( sPENatDat* )p_GetNativeData( myself ) )->emitter;
	const deScriptingDragonScript &ds = *( ( ( deClassParticleEmitter* )GetOwnerClass() )->GetDS() );
	const int index = rt->GetValue( 0 )->GetInt();
	
	if( index < 0 ){
		ds.GetClassParticleEmitterController()->PushController( rt, emitter,
			emitter->GetControllerCount() + index );
		
	}else{
		ds.GetClassParticleEmitterController()->PushController( rt, emitter, index );
	}
}

// public func ParticleEmitterController getControllerNamed( String name )
deClassParticleEmitter::nfGetControllerNamed::nfGetControllerNamed( const sInitData &init ) :
dsFunction( init.clsPE, "getControllerNamed", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsPEController ){
	p_AddParameter( init.clsStr ); // name
}
void deClassParticleEmitter::nfGetControllerNamed::RunFunction( dsRunTime *rt, dsValue *myself ){
	deParticleEmitter * const emitter = ( ( sPENatDat* )p_GetNativeData( myself ) )->emitter;
	const deScriptingDragonScript &ds = *( ( ( deClassParticleEmitter* )GetOwnerClass() )->GetDS() );
	const int index = emitter->IndexOfControllerNamed( rt->GetValue( 0 )->GetString() );
	
	if( index == -1 ){
		rt->PushObject( NULL, ds.GetClassParticleEmitterController() );
		
	}else{
		ds.GetClassParticleEmitterController()->PushController( rt, emitter, index );
	}
}

// public func int indexOfControllerNamed( String name )
deClassParticleEmitter::nfIndexOfControllerNamed::nfIndexOfControllerNamed( const sInitData &init ) :
dsFunction( init.clsPE, "indexOfControllerNamed", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
	p_AddParameter( init.clsStr ); // name
}
void deClassParticleEmitter::nfIndexOfControllerNamed::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deParticleEmitter &emitter = *( ( ( sPENatDat* )p_GetNativeData( myself ) )->emitter );
	rt->PushInt( emitter.IndexOfControllerNamed( rt->GetValue( 0 )->GetString() ) );
}



// public func float getBurstLifetime()
deClassParticleEmitter::nfGetBurstLifetime::nfGetBurstLifetime( const sInitData &init ) : dsFunction( init.clsPE,
"getBurstLifetime", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}
void deClassParticleEmitter::nfGetBurstLifetime::RunFunction( dsRunTime *rt, dsValue *myself ){
	deParticleEmitter &emitter = *( ( ( sPENatDat* )p_GetNativeData( myself ) )->emitter );
	
	rt->PushFloat( emitter.GetBurstLifetime() );
}

// public func void setBurstLifetime( float lifetime )
deClassParticleEmitter::nfSetBurstLifetime::nfSetBurstLifetime( const sInitData &init ) : dsFunction( init.clsPE,
"setBurstLifetime", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // lifetime
}
void deClassParticleEmitter::nfSetBurstLifetime::RunFunction( dsRunTime *rt, dsValue *myself ){
	deParticleEmitter &emitter = *( ( ( sPENatDat* )p_GetNativeData( myself ) )->emitter );
	
	emitter.SetBurstLifetime( rt->GetValue( 0 )->GetFloat() );
}

// public func bool getEmitBurst()
deClassParticleEmitter::nfGetEmitBurst::nfGetEmitBurst( const sInitData &init ) : dsFunction( init.clsPE,
"getEmitBurst", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
}
void deClassParticleEmitter::nfGetEmitBurst::RunFunction( dsRunTime *rt, dsValue *myself ){
	deParticleEmitter &emitter = *( ( ( sPENatDat* )p_GetNativeData( myself ) )->emitter );
	
	rt->PushBool( emitter.GetEmitBurst() );
}

// public func void setEmitBurst( bool emitBurst )
deClassParticleEmitter::nfSetEmitBurst::nfSetEmitBurst( const sInitData &init ) : dsFunction( init.clsPE,
"setEmitBurst", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsBool ); // emitBurst
}
void deClassParticleEmitter::nfSetEmitBurst::RunFunction( dsRunTime *rt, dsValue *myself ){
	deParticleEmitter &emitter = *( ( ( sPENatDat* )p_GetNativeData( myself ) )->emitter );
	
	emitter.SetEmitBurst( rt->GetValue( 0 )->GetBool() );
}



// public func int getTypeCount()
deClassParticleEmitter::nfGetTypeCount::nfGetTypeCount( const sInitData &init ) : dsFunction( init.clsPE,
"getTypeCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassParticleEmitter::nfGetTypeCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deParticleEmitter &emitter = *( ( ( sPENatDat* )p_GetNativeData( myself ) )->emitter );
	
	rt->PushInt( emitter.GetTypeCount() );
}

// public func void setTypeCount( int count )
deClassParticleEmitter::nfSetTypeCount::nfSetTypeCount( const sInitData &init ) : dsFunction( init.clsPE,
"setTypeCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // count
}
void deClassParticleEmitter::nfSetTypeCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	deParticleEmitter &emitter = *( ( ( sPENatDat* )p_GetNativeData( myself ) )->emitter );
	
	emitter.SetTypeCount( rt->GetValue( 0 )->GetInt() );
}

// public func Skin getTypeSkin( int type )
deClassParticleEmitter::nfGetTypeSkin::nfGetTypeSkin( const sInitData &init ) : dsFunction( init.clsPE,
"getTypeSkin", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsSkin ){
	p_AddParameter( init.clsInt ); // type
}
void deClassParticleEmitter::nfGetTypeSkin::RunFunction( dsRunTime *rt, dsValue *myself ){
	deParticleEmitter &emitter = *( ( ( sPENatDat* )p_GetNativeData( myself ) )->emitter );
	deScriptingDragonScript &ds = *( ( ( deClassParticleEmitter* )GetOwnerClass() )->GetDS() );
	
	const deParticleEmitterType &type = emitter.GetTypeAt( rt->GetValue( 0 )->GetInt() );
	
	ds.GetClassSkin()->PushSkin( rt, type.GetSkin() );
}

// public func void setTypeSkin( int type, Skin skin )
deClassParticleEmitter::nfSetTypeSkin::nfSetTypeSkin( const sInitData &init ) : dsFunction( init.clsPE,
"setTypeSkin", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // type
	p_AddParameter( init.clsSkin ); // skin
}
void deClassParticleEmitter::nfSetTypeSkin::RunFunction( dsRunTime *rt, dsValue *myself ){
	deParticleEmitter &emitter = *( ( ( sPENatDat* )p_GetNativeData( myself ) )->emitter );
	deScriptingDragonScript &ds = *( ( ( deClassParticleEmitter* )GetOwnerClass() )->GetDS() );
	
	deParticleEmitterType &type = emitter.GetTypeAt( rt->GetValue( 0 )->GetInt() );
	
	type.SetSkin( ds.GetClassSkin()->GetSkin( rt->GetValue( 1 )->GetRealObject() ) );
}

// public func Model getTypeModel( int type )
deClassParticleEmitter::nfGetTypeModel::nfGetTypeModel( const sInitData &init ) : dsFunction( init.clsPE,
"getTypeModel", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsModel ){
	p_AddParameter( init.clsInt ); // type
}
void deClassParticleEmitter::nfGetTypeModel::RunFunction( dsRunTime *rt, dsValue *myself ){
	deParticleEmitter &emitter = *( ( ( sPENatDat* )p_GetNativeData( myself ) )->emitter );
	deScriptingDragonScript &ds = *( ( ( deClassParticleEmitter* )GetOwnerClass() )->GetDS() );
	
	const deParticleEmitterType &type = emitter.GetTypeAt( rt->GetValue( 0 )->GetInt() );
	
	ds.GetClassModel()->PushModel( rt, type.GetModel() );
}

// public func void setTypeModel( int type, Model model )
deClassParticleEmitter::nfSetTypeModel::nfSetTypeModel( const sInitData &init ) : dsFunction( init.clsPE,
"setTypeModel", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // type
	p_AddParameter( init.clsModel ); // model
}
void deClassParticleEmitter::nfSetTypeModel::RunFunction( dsRunTime *rt, dsValue *myself ){
	deParticleEmitter &emitter = *( ( ( sPENatDat* )p_GetNativeData( myself ) )->emitter );
	deScriptingDragonScript &ds = *( ( ( deClassParticleEmitter* )GetOwnerClass() )->GetDS() );
	
	deParticleEmitterType &type = emitter.GetTypeAt( rt->GetValue( 0 )->GetInt() );
	
	type.SetModel( ds.GetClassModel()->GetModel( rt->GetValue( 1 )->GetRealObject() ) );
}

// public func Skin getTypeModelSkin( int type )
deClassParticleEmitter::nfGetTypeModelSkin::nfGetTypeModelSkin( const sInitData &init ) : dsFunction( init.clsPE,
"getTypeModelSkin", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsSkin ){
	p_AddParameter( init.clsInt ); // type
}
void deClassParticleEmitter::nfGetTypeModelSkin::RunFunction( dsRunTime *rt, dsValue *myself ){
	deParticleEmitter &emitter = *( ( ( sPENatDat* )p_GetNativeData( myself ) )->emitter );
	deScriptingDragonScript &ds = *( ( ( deClassParticleEmitter* )GetOwnerClass() )->GetDS() );
	
	const deParticleEmitterType &type = emitter.GetTypeAt( rt->GetValue( 0 )->GetInt() );
	
	ds.GetClassSkin()->PushSkin( rt, type.GetModelSkin() );
}

// public func void setTypeModelSkin( int type, Skin skin )
deClassParticleEmitter::nfSetTypeModelSkin::nfSetTypeModelSkin( const sInitData &init ) : dsFunction( init.clsPE,
"setTypeModelSkin", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // type
	p_AddParameter( init.clsSkin ); // skin
}
void deClassParticleEmitter::nfSetTypeModelSkin::RunFunction( dsRunTime *rt, dsValue *myself ){
	deParticleEmitter &emitter = *( ( ( sPENatDat* )p_GetNativeData( myself ) )->emitter );
	deScriptingDragonScript &ds = *( ( ( deClassParticleEmitter* )GetOwnerClass() )->GetDS() );
	
	deParticleEmitterType &type = emitter.GetTypeAt( rt->GetValue( 0 )->GetInt() );
	
	type.SetModelSkin( ds.GetClassSkin()->GetSkin( rt->GetValue( 1 )->GetRealObject() ) );
}

// public func ParticleEmitterCastFrom getTypeCastFrom( int type )
deClassParticleEmitter::nfGetTypeCastFrom::nfGetTypeCastFrom( const sInitData &init ) : dsFunction( init.clsPE,
"getTypeCastFrom", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
	p_AddParameter( init.clsParticleEmitterCastFrom ); // type
}
void deClassParticleEmitter::nfGetTypeCastFrom::RunFunction( dsRunTime *rt, dsValue *myself ){
	deParticleEmitter &emitter = *( ( ( sPENatDat* )p_GetNativeData( myself ) )->emitter );
	const deParticleEmitterType &type = emitter.GetTypeAt( rt->GetValue( 0 )->GetInt() );
	
	rt->PushValue( ( ( deClassParticleEmitter* )GetOwnerClass() )->GetClassParticleEmitterCastFrom()
		->GetVariable( type.GetCastFrom() )->GetStaticValue() );
}

// public func void setTypeCastFrom( int type, ParticleEmitterCastFrom castFrom )
deClassParticleEmitter::nfSetTypeCastFrom::nfSetTypeCastFrom( const sInitData &init ) : dsFunction( init.clsPE,
"setTypeCastFrom", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // type
	p_AddParameter( init.clsParticleEmitterCastFrom ); // castFrom
}
void deClassParticleEmitter::nfSetTypeCastFrom::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( ! rt->GetValue( 1 )->GetRealObject() ){
		DSTHROW( dueNullPointer );
	}
	
	deParticleEmitter &emitter = *( ( ( sPENatDat* )p_GetNativeData( myself ) )->emitter );
	deParticleEmitterType &type = emitter.GetTypeAt( rt->GetValue( 0 )->GetInt() );
	
	type.SetCastFrom( ( deParticleEmitterType::eCastFrom )
		( ( dsClassEnumeration* )rt->GetEngine()->GetClassEnumeration() )->GetConstantOrder(
			*rt->GetValue( 1 )->GetRealObject() ) );
}

// public func ParticleEmitterSimulation getTypeSimulationType( int type )
deClassParticleEmitter::nfGetTypeSimulationType::nfGetTypeSimulationType( const sInitData &init ) : dsFunction( init.clsPE,
"getTypeSimulationType", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsParticleEmitterSimulation ){
	p_AddParameter( init.clsInt ); // type
}
void deClassParticleEmitter::nfGetTypeSimulationType::RunFunction( dsRunTime *rt, dsValue *myself ){
	deParticleEmitter &emitter = *( ( ( sPENatDat* )p_GetNativeData( myself ) )->emitter );
	const deParticleEmitterType &type = emitter.GetTypeAt( rt->GetValue( 0 )->GetInt() );
	
	rt->PushValue( ( ( deClassParticleEmitter* )GetOwnerClass() )->GetClassParticleEmitterSimulation()
		->GetVariable( type.GetSimulationType() )->GetStaticValue() );
}

// public func void setTypeSimulationType( int type, ParticleEmitterSimulation simulationType )
deClassParticleEmitter::nfSetTypeSimulationType::nfSetTypeSimulationType( const sInitData &init ) : dsFunction( init.clsPE,
"setTypeSimulationType", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // type
	p_AddParameter( init.clsParticleEmitterSimulation ); // simulationType
}
void deClassParticleEmitter::nfSetTypeSimulationType::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( ! rt->GetValue( 1 )->GetRealObject() ){
		DSTHROW( dueNullPointer );
	}
	
	deParticleEmitter &emitter = *( ( ( sPENatDat* )p_GetNativeData( myself ) )->emitter );
	deParticleEmitterType &type = emitter.GetTypeAt( rt->GetValue( 0 )->GetInt() );
	
	type.SetSimulationType( ( deParticleEmitterType::eSimulationTypes )
		( ( dsClassEnumeration* )rt->GetEngine()->GetClassEnumeration() )->GetConstantOrder(
			*rt->GetValue( 1 )->GetRealObject() )  );
}

// public func bool getTypeIntervalAsDistance( int type )
deClassParticleEmitter::nfGetTypeIntervalAsDistance::nfGetTypeIntervalAsDistance( const sInitData &init ) : dsFunction( init.clsPE,
"getTypeIntervalAsDistance", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
	p_AddParameter( init.clsInt ); // type
}
void deClassParticleEmitter::nfGetTypeIntervalAsDistance::RunFunction( dsRunTime *rt, dsValue *myself ){
	deParticleEmitter &emitter = *( ( ( sPENatDat* )p_GetNativeData( myself ) )->emitter );
	const deParticleEmitterType &type = emitter.GetTypeAt( rt->GetValue( 0 )->GetInt() );
	
	rt->PushBool( type.GetIntervalAsDistance() );
}

// public func void setTypeIntervalAsDistance( int type, bool intervalAsDistance )
deClassParticleEmitter::nfSetTypeIntervalAsDistance::nfSetTypeIntervalAsDistance( const sInitData &init ) : dsFunction( init.clsPE,
"setTypeIntervalAsDistance", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // type
	p_AddParameter( init.clsBool ); // intervalAsDistance
}
void deClassParticleEmitter::nfSetTypeIntervalAsDistance::RunFunction( dsRunTime *rt, dsValue *myself ){
	deParticleEmitter &emitter = *( ( ( sPENatDat* )p_GetNativeData( myself ) )->emitter );
	deParticleEmitterType &type = emitter.GetTypeAt( rt->GetValue( 0 )->GetInt() );
	
	type.SetIntervalAsDistance( rt->GetValue( 1 )->GetBool() );
}

// public func float getTypePhysicsSize( int type )
deClassParticleEmitter::nfGetTypePhysicsSize::nfGetTypePhysicsSize( const sInitData &init ) : dsFunction( init.clsPE,
"getTypePhysicsSize", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
	p_AddParameter( init.clsInt ); // type
}
void deClassParticleEmitter::nfGetTypePhysicsSize::RunFunction( dsRunTime *rt, dsValue *myself ){
	deParticleEmitter &emitter = *( ( ( sPENatDat* )p_GetNativeData( myself ) )->emitter );
	const deParticleEmitterType &type = emitter.GetTypeAt( rt->GetValue( 0 )->GetInt() );
	
	rt->PushFloat( type.GetPhysicsSize() );
}

// public func void setTypePhysicsSize( int type, float size )
deClassParticleEmitter::nfSetTypePhysicsSize::nfSetTypePhysicsSize( const sInitData &init ) : dsFunction( init.clsPE,
"setTypePhysicsSize", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // type
	p_AddParameter( init.clsFloat ); // size
}
void deClassParticleEmitter::nfSetTypePhysicsSize::RunFunction( dsRunTime *rt, dsValue *myself ){
	deParticleEmitter &emitter = *( ( ( sPENatDat* )p_GetNativeData( myself ) )->emitter );
	deParticleEmitterType &type = emitter.GetTypeAt( rt->GetValue( 0 )->GetInt() );
	
	type.SetPhysicsSize( rt->GetValue( 1 )->GetFloat() );
}

// public func ParticleEmitter getTypeCollisionEmitter( int type )
deClassParticleEmitter::nfGetTypeTrailEmitter::nfGetTypeTrailEmitter( const sInitData &init ) : dsFunction( init.clsPE,
"getTypeTrailEmitter", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsPE ){
	p_AddParameter( init.clsInt ); // type
}
void deClassParticleEmitter::nfGetTypeTrailEmitter::RunFunction( dsRunTime *rt, dsValue *myself ){
	deParticleEmitter &emitter = *( ( ( sPENatDat* )p_GetNativeData( myself ) )->emitter );
	deClassParticleEmitter *clsPE = ( deClassParticleEmitter* )GetOwnerClass();
	
	const deParticleEmitterType &type = emitter.GetTypeAt( rt->GetValue( 0 )->GetInt() );
	
	clsPE->PushParticleEmitter( rt, type.GetTrailEmitter() );
}

// public func void setTypeTrailEmitter( int type, ParticleEmitter emitter )
deClassParticleEmitter::nfSetTypeTrailEmitter::nfSetTypeTrailEmitter( const sInitData &init ) : dsFunction( init.clsPE,
"setTypeTrailEmitter", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // type
	p_AddParameter( init.clsPE ); // emitter
}
void deClassParticleEmitter::nfSetTypeTrailEmitter::RunFunction( dsRunTime *rt, dsValue *myself ){
	deParticleEmitter &emitter = *( ( ( sPENatDat* )p_GetNativeData( myself ) )->emitter );
	deClassParticleEmitter *clsPE = ( deClassParticleEmitter* )GetOwnerClass();
	
	deParticleEmitterType &type = emitter.GetTypeAt( rt->GetValue( 0 )->GetInt() );
	
	type.SetTrailEmitter( clsPE->GetParticleEmitter( rt->GetValue( 1 )->GetRealObject() ) );
}

// public func int getTypeTrailController( int type, ParticleEmitterEmitController controller )
deClassParticleEmitter::nfGetTypeTrailController::nfGetTypeTrailController( const sInitData &init ) : dsFunction( init.clsPE,
"getTypeTrailController", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
	p_AddParameter( init.clsInt ); // type
	p_AddParameter( init.clsParticleEmitterEmitController ); // controller
}
void deClassParticleEmitter::nfGetTypeTrailController::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( ! rt->GetValue( 1 )->GetRealObject() ){
		DSTHROW( dueNullPointer );
	}
	
	deParticleEmitter &emitter = *( ( ( sPENatDat* )p_GetNativeData( myself ) )->emitter );
	const deParticleEmitterType &type = emitter.GetTypeAt( rt->GetValue( 0 )->GetInt() );
	const deParticleEmitterType::eEmitControllers controller = ( deParticleEmitterType::eEmitControllers )
		( ( dsClassEnumeration* )rt->GetEngine()->GetClassEnumeration() )->GetConstantOrder(
			*rt->GetValue( 1 )->GetRealObject() );
	
	rt->PushInt( type.GetTrailController( controller ) );
}

// public func void setTypeTrailController( int type, ParticleEmitterEmitController controller, int targetController )
deClassParticleEmitter::nfSetTypeTrailController::nfSetTypeTrailController( const sInitData &init ) : dsFunction( init.clsPE,
"setTypeTrailController", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // type
	p_AddParameter( init.clsParticleEmitterEmitController ); // controller
	p_AddParameter( init.clsInt ); // targetController
}
void deClassParticleEmitter::nfSetTypeTrailController::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( ! rt->GetValue( 1 )->GetRealObject() ){
		DSTHROW( dueNullPointer );
	}
	
	deParticleEmitter &emitter = *( ( ( sPENatDat* )p_GetNativeData( myself ) )->emitter );
	deParticleEmitterType &type = emitter.GetTypeAt( rt->GetValue( 0 )->GetInt() );
	const deParticleEmitterType::eEmitControllers controller = ( deParticleEmitterType::eEmitControllers )
		( ( dsClassEnumeration* )rt->GetEngine()->GetClassEnumeration() )->GetConstantOrder(
			*rt->GetValue( 1 )->GetRealObject() );
	const int target = rt->GetValue( 2 )->GetInt();
	
	type.SetTrailController( controller, target );
}

// public func ParticleCollisionResponse getTypeCollisionResponse( int type )
deClassParticleEmitter::nfGetTypeCollisionResponse::nfGetTypeCollisionResponse( const sInitData &init ) : dsFunction( init.clsPE,
"getTypeCollisionResponse", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsParticleCollisionResponse ){
	p_AddParameter( init.clsInt ); // type
}
void deClassParticleEmitter::nfGetTypeCollisionResponse::RunFunction( dsRunTime *rt, dsValue *myself ){
	deParticleEmitter &emitter = *( ( ( sPENatDat* )p_GetNativeData( myself ) )->emitter );
	const deParticleEmitterType &type = emitter.GetTypeAt( rt->GetValue( 0 )->GetInt() );
	
	rt->PushValue( ( ( deClassParticleEmitter* )GetOwnerClass() )->GetClassParticleCollisionResponse()
		->GetVariable( type.GetCollisionResponse() )->GetStaticValue() );
}

// public func void setTypeCollisionResponse( int type, ParticleCollisionResponse response )
deClassParticleEmitter::nfSetTypeCollisionResponse::nfSetTypeCollisionResponse( const sInitData &init ) : dsFunction( init.clsPE,
"setTypeCollisionResponse", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // type
	p_AddParameter( init.clsParticleCollisionResponse ); // response
}
void deClassParticleEmitter::nfSetTypeCollisionResponse::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( ! rt->GetValue( 1 )->GetRealObject() ){
		DSTHROW( dueNullPointer );
	}
	
	deParticleEmitter &emitter = *( ( ( sPENatDat* )p_GetNativeData( myself ) )->emitter );
	deParticleEmitterType &type = emitter.GetTypeAt( rt->GetValue( 0 )->GetInt() );
	
	type.SetCollisionResponse( ( deParticleEmitterType::eCollisionResponses )
		( ( dsClassEnumeration* )rt->GetEngine()->GetClassEnumeration() )->GetConstantOrder(
			*rt->GetValue( 1 )->GetRealObject() )  );
}

// public func ParticleEmitter getTypeCollisionEmitter( int type )
deClassParticleEmitter::nfGetTypeCollisionEmitter::nfGetTypeCollisionEmitter( const sInitData &init ) : dsFunction( init.clsPE,
"getTypeCollisionEmitter", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsPE ){
	p_AddParameter( init.clsInt ); // type
}
void deClassParticleEmitter::nfGetTypeCollisionEmitter::RunFunction( dsRunTime *rt, dsValue *myself ){
	deParticleEmitter &emitter = *( ( ( sPENatDat* )p_GetNativeData( myself ) )->emitter );
	deClassParticleEmitter *clsPE = ( deClassParticleEmitter* )GetOwnerClass();
	
	const deParticleEmitterType &type = emitter.GetTypeAt( rt->GetValue( 0 )->GetInt() );
	
	clsPE->PushParticleEmitter( rt, type.GetCollisionEmitter() );
}

// public func void setTypeCollisionEmitter( int type, ParticleEmitter emitter )
deClassParticleEmitter::nfSetTypeCollisionEmitter::nfSetTypeCollisionEmitter( const sInitData &init ) : dsFunction( init.clsPE,
"setTypeCollisionEmitter", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // type
	p_AddParameter( init.clsPE ); // emitter
}
void deClassParticleEmitter::nfSetTypeCollisionEmitter::RunFunction( dsRunTime *rt, dsValue *myself ){
	deParticleEmitter &emitter = *( ( ( sPENatDat* )p_GetNativeData( myself ) )->emitter );
	deClassParticleEmitter *clsPE = ( deClassParticleEmitter* )GetOwnerClass();
	
	deParticleEmitterType &type = emitter.GetTypeAt( rt->GetValue( 0 )->GetInt() );
	
	type.SetCollisionEmitter( clsPE->GetParticleEmitter( rt->GetValue( 1 )->GetRealObject() ) );
}

// public func float getTypeEmitMinImpulse( int type )
deClassParticleEmitter::nfGetTypeEmitMinImpulse::nfGetTypeEmitMinImpulse( const sInitData &init ) : dsFunction( init.clsPE,
"getTypeEmitMinImpulse", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
	p_AddParameter( init.clsInt ); // type
}
void deClassParticleEmitter::nfGetTypeEmitMinImpulse::RunFunction( dsRunTime *rt, dsValue *myself ){
	deParticleEmitter &emitter = *( ( ( sPENatDat* )p_GetNativeData( myself ) )->emitter );
	const deParticleEmitterType &type = emitter.GetTypeAt( rt->GetValue( 0 )->GetInt() );
	
	rt->PushFloat( type.GetEmitMinImpulse() );
}

// public func void setTypeEmitMinImpulse( int type, float impulse )
deClassParticleEmitter::nfSetTypeEmitMinImpulse::nfSetTypeEmitMinImpulse( const sInitData &init ) : dsFunction( init.clsPE,
"setTypeEmitMinImpulse", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // type
	p_AddParameter( init.clsFloat ); // impulse
}
void deClassParticleEmitter::nfSetTypeEmitMinImpulse::RunFunction( dsRunTime *rt, dsValue *myself ){
	deParticleEmitter &emitter = *( ( ( sPENatDat* )p_GetNativeData( myself ) )->emitter );
	deParticleEmitterType &type = emitter.GetTypeAt( rt->GetValue( 0 )->GetInt() );
	
	type.SetEmitMinImpulse( rt->GetValue( 1 )->GetFloat() );
}

// public func int getTypeEmitController( int type, ParticleEmitterEmitController controller )
deClassParticleEmitter::nfGetTypeEmitController::nfGetTypeEmitController( const sInitData &init ) : dsFunction( init.clsPE,
"getTypeEmitController", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
	p_AddParameter( init.clsInt ); // type
	p_AddParameter( init.clsParticleEmitterEmitController ); // controller
}
void deClassParticleEmitter::nfGetTypeEmitController::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( ! rt->GetValue( 1 )->GetRealObject() ){
		DSTHROW( dueNullPointer );
	}
	
	deParticleEmitter &emitter = *( ( ( sPENatDat* )p_GetNativeData( myself ) )->emitter );
	const deParticleEmitterType &type = emitter.GetTypeAt( rt->GetValue( 0 )->GetInt() );
	const deParticleEmitterType::eEmitControllers controller = ( deParticleEmitterType::eEmitControllers )
		( ( dsClassEnumeration* )rt->GetEngine()->GetClassEnumeration() )->GetConstantOrder(
			*rt->GetValue( 1 )->GetRealObject() );
	
	rt->PushInt( type.GetEmitController( controller ) );
}

// public func void setTypeEmitController( int type, ParticleEmitterEmitController controller, int targetController )
deClassParticleEmitter::nfSetTypeEmitController::nfSetTypeEmitController( const sInitData &init ) : dsFunction( init.clsPE,
"setTypeEmitController", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // type
	p_AddParameter( init.clsParticleEmitterEmitController ); // controller
	p_AddParameter( init.clsInt ); // targetController
}
void deClassParticleEmitter::nfSetTypeEmitController::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( ! rt->GetValue( 1 )->GetRealObject() ){
		DSTHROW( dueNullPointer );
	}
	
	deParticleEmitter &emitter = *( ( ( sPENatDat* )p_GetNativeData( myself ) )->emitter );
	deParticleEmitterType &type = emitter.GetTypeAt( rt->GetValue( 0 )->GetInt() );
	const deParticleEmitterType::eEmitControllers controller = ( deParticleEmitterType::eEmitControllers )
		( ( dsClassEnumeration* )rt->GetEngine()->GetClassEnumeration() )->GetConstantOrder(
			*rt->GetValue( 1 )->GetRealObject() );
	const int target = rt->GetValue( 2 )->GetInt();
	
	type.SetEmitController( controller, target );
}



// public func void setTypeParameterValue( int type, ParticleEmitterParameter parameter, float value )
deClassParticleEmitter::nfSetTypeParameterValue::nfSetTypeParameterValue( const sInitData &init ) : dsFunction( init.clsPE,
"setTypeParameterValue", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // type
	p_AddParameter( init.clsParticleEmitterParameter ); // parameter
	p_AddParameter( init.clsFloat ); // value
}
void deClassParticleEmitter::nfSetTypeParameterValue::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( ! rt->GetValue( 1 )->GetRealObject() ){
		DSTHROW( dueNullPointer );
	}
	
	deParticleEmitter &emitter = *( ( ( sPENatDat* )p_GetNativeData( myself ) )->emitter );
	const int typeIndex = rt->GetValue( 0 )->GetInt();
	const float value = rt->GetValue( 2 )->GetFloat();
	
	deParticleEmitterType &type = emitter.GetTypeAt( typeIndex );
	deParticleEmitterParameter &parameter = type.GetParameter( ( deParticleEmitterType::eParameters )
		( ( dsClassEnumeration* )rt->GetEngine()->GetClassEnumeration() )->GetConstantOrder(
			*rt->GetValue( 1 )->GetRealObject() ) );
	
	parameter.SetValue( value );
	emitter.NotifyTypeChangedAt( typeIndex );
}

// public func void setTypeParameterSpread( int type, ParticleEmitterParameter parameter, float spread )
deClassParticleEmitter::nfSetTypeParameterSpread::nfSetTypeParameterSpread( const sInitData &init ) : dsFunction( init.clsPE,
"setTypeParameterSpread", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // type
	p_AddParameter( init.clsParticleEmitterParameter ); // parameter
	p_AddParameter( init.clsFloat ); // spread
}
void deClassParticleEmitter::nfSetTypeParameterSpread::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( ! rt->GetValue( 1 )->GetRealObject() ){
		DSTHROW( dueNullPointer );
	}
	
	deParticleEmitter &emitter = *( ( ( sPENatDat* )p_GetNativeData( myself ) )->emitter );
	const int typeIndex = rt->GetValue( 0 )->GetInt();
	const float value = rt->GetValue( 2 )->GetFloat();
	
	deParticleEmitterType &type = emitter.GetTypeAt( typeIndex );
	deParticleEmitterParameter &parameter = type.GetParameter( ( deParticleEmitterType::eParameters )
		( ( dsClassEnumeration* )rt->GetEngine()->GetClassEnumeration() )->GetConstantOrder(
			*rt->GetValue( 1 )->GetRealObject() ) );
	
	parameter.SetSpread( value );
	emitter.NotifyTypeChangedAt( typeIndex );
}

// public func void setTypeParameterControllerValue( int type, ParticleEmitterParameter parameter, int controller )
deClassParticleEmitter::nfSetTypeParameterControllerValue::nfSetTypeParameterControllerValue( const sInitData &init ) : dsFunction( init.clsPE,
"setTypeParameterControllerValue", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // type
	p_AddParameter( init.clsParticleEmitterParameter ); // parameter
	p_AddParameter( init.clsInt ); // controller
}
void deClassParticleEmitter::nfSetTypeParameterControllerValue::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( ! rt->GetValue( 1 )->GetRealObject() ){
		DSTHROW( dueNullPointer );
	}
	
	deParticleEmitter &emitter = *( ( ( sPENatDat* )p_GetNativeData( myself ) )->emitter );
	const int typeIndex = rt->GetValue( 0 )->GetInt();
	const int controller = rt->GetValue( 2 )->GetInt();
	
	deParticleEmitterType &type = emitter.GetTypeAt( typeIndex );
	deParticleEmitterParameter &parameter = type.GetParameter( ( deParticleEmitterType::eParameters )
		( ( dsClassEnumeration* )rt->GetEngine()->GetClassEnumeration() )->GetConstantOrder(
			*rt->GetValue( 1 )->GetRealObject() ) );
	
	parameter.SetControllerValue( controller );
	emitter.NotifyTypeChangedAt( typeIndex );
}

// public func void setTypeParameterCurveValue( int type, ParticleEmitterParameter parameter, CurveBezier curve )
deClassParticleEmitter::nfSetTypeParameterCurveValue::nfSetTypeParameterCurveValue( const sInitData &init ) : dsFunction( init.clsPE,
"setTypeParameterCurveValue", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // type
	p_AddParameter( init.clsParticleEmitterParameter ); // parameter
	p_AddParameter( init.clsCurveBezier ); // curve
}
void deClassParticleEmitter::nfSetTypeParameterCurveValue::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( ! rt->GetValue( 1 )->GetRealObject() ){
		DSTHROW( dueNullPointer );
	}
	
	deParticleEmitter &emitter = *( ( ( sPENatDat* )p_GetNativeData( myself ) )->emitter );
	deScriptingDragonScript &ds = *( ( ( deClassParticleEmitter* )GetOwnerClass() )->GetDS() );
	const int typeIndex = rt->GetValue( 0 )->GetInt();
	dsRealObject *objCurve = rt->GetValue( 2 )->GetRealObject();
	
	deParticleEmitterType &type = emitter.GetTypeAt( typeIndex );
	deParticleEmitterParameter &parameter = type.GetParameter( ( deParticleEmitterType::eParameters )
		( ( dsClassEnumeration* )rt->GetEngine()->GetClassEnumeration() )->GetConstantOrder(
			*rt->GetValue( 1 )->GetRealObject() ) );
	
	if( objCurve ){
		parameter.GetCurveValue() = ds.GetClassCurveBezier()->GetCurve( objCurve );
		
	}else{
		parameter.GetCurveValue().RemoveAllPoints();
	}
	emitter.NotifyTypeChangedAt( typeIndex );
}

// public func void setTypeParameterControllerSpread( int type, ParticleEmitterParameter parameter, int controller )
deClassParticleEmitter::nfSetTypeParameterControllerSpread::nfSetTypeParameterControllerSpread( const sInitData &init ) : dsFunction( init.clsPE,
"setTypeParameterControllerSpread", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // type
	p_AddParameter( init.clsParticleEmitterParameter ); // parameter
	p_AddParameter( init.clsInt ); // controller
}
void deClassParticleEmitter::nfSetTypeParameterControllerSpread::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( ! rt->GetValue( 1 )->GetRealObject() ){
		DSTHROW( dueNullPointer );
	}
	
	deParticleEmitter &emitter = *( ( ( sPENatDat* )p_GetNativeData( myself ) )->emitter );
	const int typeIndex = rt->GetValue( 0 )->GetInt();
	const int controller = rt->GetValue( 2 )->GetInt();
	
	deParticleEmitterType &type = emitter.GetTypeAt( typeIndex );
	deParticleEmitterParameter &parameter = type.GetParameter( ( deParticleEmitterType::eParameters )
		( ( dsClassEnumeration* )rt->GetEngine()->GetClassEnumeration() )->GetConstantOrder(
			*rt->GetValue( 1 )->GetRealObject() ) );
	
	parameter.SetControllerSpread( controller );
	emitter.NotifyTypeChangedAt( typeIndex );
}

// public func void setTypeParameterCurveSpread( int type, ParticleEmitterParameter parameter, CurveBezier curve )
deClassParticleEmitter::nfSetTypeParameterCurveSpread::nfSetTypeParameterCurveSpread( const sInitData &init ) : dsFunction( init.clsPE,
"setTypeParameterCurveSpread", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // type
	p_AddParameter( init.clsParticleEmitterParameter ); // parameter
	p_AddParameter( init.clsCurveBezier ); // curve
}
void deClassParticleEmitter::nfSetTypeParameterCurveSpread::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( ! rt->GetValue( 1 )->GetRealObject() ){
		DSTHROW( dueNullPointer );
	}
	
	deParticleEmitter &emitter = *( ( ( sPENatDat* )p_GetNativeData( myself ) )->emitter );
	deScriptingDragonScript &ds = *( ( ( deClassParticleEmitter* )GetOwnerClass() )->GetDS() );
	const int typeIndex = rt->GetValue( 0 )->GetInt();
	dsRealObject *objCurve = rt->GetValue( 2 )->GetRealObject();
	
	deParticleEmitterType &type = emitter.GetTypeAt( typeIndex );
	deParticleEmitterParameter &parameter = type.GetParameter( ( deParticleEmitterType::eParameters )
		( ( dsClassEnumeration* )rt->GetEngine()->GetClassEnumeration() )->GetConstantOrder(
			*rt->GetValue( 1 )->GetRealObject() ) );
	
	if( objCurve ){
		parameter.GetCurveSpread() = ds.GetClassCurveBezier()->GetCurve( objCurve );
		
	}else{
		parameter.GetCurveSpread().RemoveAllPoints();
	}
	emitter.NotifyTypeChangedAt( typeIndex );
}

// public func void setTypeParameterCurveProgress( int type, ParticleEmitterParameter parameter, CurveBezier curve )
deClassParticleEmitter::nfSetTypeParameterCurveProgress::nfSetTypeParameterCurveProgress( const sInitData &init ) : dsFunction( init.clsPE,
"setTypeParameterCurveProgress", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // type
	p_AddParameter( init.clsParticleEmitterParameter ); // parameter
	p_AddParameter( init.clsCurveBezier ); // curve
}
void deClassParticleEmitter::nfSetTypeParameterCurveProgress::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( ! rt->GetValue( 1 )->GetRealObject() ){
		DSTHROW( dueNullPointer );
	}
	
	deParticleEmitter &emitter = *( ( ( sPENatDat* )p_GetNativeData( myself ) )->emitter );
	deScriptingDragonScript &ds = *( ( ( deClassParticleEmitter* )GetOwnerClass() )->GetDS() );
	const int typeIndex = rt->GetValue( 0 )->GetInt();
	dsRealObject *objCurve = rt->GetValue( 2 )->GetRealObject();
	
	deParticleEmitterType &type = emitter.GetTypeAt( typeIndex );
	deParticleEmitterParameter &parameter = type.GetParameter( ( deParticleEmitterType::eParameters )
		( ( dsClassEnumeration* )rt->GetEngine()->GetClassEnumeration() )->GetConstantOrder(
			*rt->GetValue( 1 )->GetRealObject() ) );
	
	if( objCurve ){
		parameter.GetCurveProgress() = ds.GetClassCurveBezier()->GetCurve( objCurve );
		
	}else{
		parameter.GetCurveProgress().RemoveAllPoints();
	}
	emitter.NotifyTypeChangedAt( typeIndex );
}

// public func void setTypeParameterCurveBeam( int type, ParticleEmitterParameter parameter, CurveBezier curve )
deClassParticleEmitter::nfSetTypeParameterCurveBeam::nfSetTypeParameterCurveBeam( const sInitData &init ) : dsFunction( init.clsPE,
"setTypeParameterCurveBeam", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // type
	p_AddParameter( init.clsParticleEmitterParameter ); // parameter
	p_AddParameter( init.clsCurveBezier ); // curve
}
void deClassParticleEmitter::nfSetTypeParameterCurveBeam::RunFunction( dsRunTime *rt, dsValue *myself ){
	deParticleEmitter &emitter = *( ( ( sPENatDat* )p_GetNativeData( myself ) )->emitter );
	if( ! rt->GetValue( 1 )->GetRealObject() ){
		DSTHROW( dueNullPointer );
	}
	
	deScriptingDragonScript &ds = *( ( ( deClassParticleEmitter* )GetOwnerClass() )->GetDS() );
	const int typeIndex = rt->GetValue( 0 )->GetInt();
	dsRealObject *objCurve = rt->GetValue( 2 )->GetRealObject();
	
	deParticleEmitterType &type = emitter.GetTypeAt( typeIndex );
	deParticleEmitterParameter &parameter = type.GetParameter( ( deParticleEmitterType::eParameters )
		( ( dsClassEnumeration* )rt->GetEngine()->GetClassEnumeration() )->GetConstantOrder(
			*rt->GetValue( 1 )->GetRealObject() ) );
	
	if( objCurve ){
		parameter.GetCurveBeam() = ds.GetClassCurveBezier()->GetCurve( objCurve );
		
	}else{
		parameter.GetCurveBeam().RemoveAllPoints();
	}
	emitter.NotifyTypeChangedAt( typeIndex );
}



// public func int hashCode()
deClassParticleEmitter::nfHashCode::nfHashCode( const sInitData &init ) :
dsFunction( init.clsPE, "hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}

void deClassParticleEmitter::nfHashCode::RunFunction( dsRunTime *rt, dsValue *myself ){
	deParticleEmitter *emitter = ( ( sPENatDat* )p_GetNativeData( myself ) )->emitter;
	
	rt->PushInt( ( intptr_t )emitter );
}

// public func bool equals( Object obj )
deClassParticleEmitter::nfEquals::nfEquals( const sInitData &init ) :
dsFunction( init.clsPE, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsObj ); // obj
}
void deClassParticleEmitter::nfEquals::RunFunction( dsRunTime *rt, dsValue *myself ){
	deParticleEmitter *emitter = ( ( sPENatDat* )p_GetNativeData( myself ) )->emitter;
	deClassParticleEmitter *clsPE = ( deClassParticleEmitter* )GetOwnerClass();
	dsValue *obj = rt->GetValue( 0 );
	
	if( ! p_IsObjOfType( obj, clsPE ) ){
		rt->PushBool( false );
		
	}else{
		deParticleEmitter *other = ( ( sPENatDat* )p_GetNativeData( obj ) )->emitter;
		
		rt->PushBool( emitter == other );
	}
}




// Class deClassParticleEmitter
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deClassParticleEmitter::deClassParticleEmitter( deScriptingDragonScript *ds ) :
dsClass( "ParticleEmitter", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED ){
	if( ! ds ){
		DSTHROW( dueInvalidParam );
	}
	
	pDS = ds;
	
	GetParserInfo()->SetParent( DENS_SCENERY );
	GetParserInfo()->SetBase( "Object" );
	
	p_SetNativeDataSize( sizeof( sPENatDat ) );
}

deClassParticleEmitter::~deClassParticleEmitter(){
}



// Management
///////////////

void deClassParticleEmitter::CreateClassMembers( dsEngine *engine ){
	pClsParticleEmitterParameter = engine->GetClass( "Dragengine.Scenery.ParticleEmitterParameter" );
	pClsParticleEmitterEmitController = engine->GetClass( "Dragengine.Scenery.ParticleEmitterEmitController" );
	pClsParticleEmitterCastFrom = engine->GetClass( "Dragengine.Scenery.ParticleEmitterCastFrom" );
	pClsParticleCollisionResponse = engine->GetClass( "Dragengine.Scenery.ParticleCollisionResponse" );
	pClsParticleEmitterSimulation = engine->GetClass( "Dragengine.Scenery.ParticleEmitterSimulation" );
	
	sInitData init;
	init.clsPE = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFloat = engine->GetClassFloat();
	init.clsStr = engine->GetClassString();
	init.clsBool = engine->GetClassBool();
	init.clsObj = engine->GetClassObject();
	init.clsCurveBezier = pDS->GetClassCurveBezier();
	init.clsModel = pDS->GetClassModel();
	init.clsSkin = pDS->GetClassSkin();
	init.clsPEController = pDS->GetClassParticleEmitterController();
	init.clsParticleEmitterParameter = pClsParticleEmitterParameter;
	init.clsParticleEmitterEmitController = pClsParticleEmitterEmitController;
	init.clsParticleEmitterCastFrom = pClsParticleEmitterCastFrom;
	init.clsParticleCollisionResponse = pClsParticleCollisionResponse;
	init.clsParticleEmitterSimulation = pClsParticleEmitterSimulation;
	
	AddFunction( new nfNew( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfGetControllerCount( init ) );
	AddFunction( new nfSetControllerCount( init ) );
	AddFunction( new nfGetControllerAt( init ) );
	AddFunction( new nfGetControllerNamed( init ) );
	AddFunction( new nfIndexOfControllerNamed( init ) );
	
	AddFunction( new nfGetBurstLifetime( init ) );
	AddFunction( new nfSetBurstLifetime( init ) );
	AddFunction( new nfGetEmitBurst( init ) );
	AddFunction( new nfSetEmitBurst( init ) );
	
	AddFunction( new nfGetTypeCount( init ) );
	AddFunction( new nfSetTypeCount( init ) );
	AddFunction( new nfGetTypeSkin( init ) );
	AddFunction( new nfSetTypeSkin( init ) );
	AddFunction( new nfGetTypeModel( init ) );
	AddFunction( new nfSetTypeModel( init ) );
	AddFunction( new nfGetTypeModelSkin( init ) );
	AddFunction( new nfSetTypeModelSkin( init ) );
	AddFunction( new nfGetTypeCastFrom( init ) );
	AddFunction( new nfSetTypeCastFrom( init ) );
	AddFunction( new nfGetTypeSimulationType( init ) );
	AddFunction( new nfSetTypeSimulationType( init ) );
	AddFunction( new nfGetTypeIntervalAsDistance( init ) );
	AddFunction( new nfSetTypeIntervalAsDistance( init ) );
	AddFunction( new nfGetTypeTrailEmitter( init ) );
	AddFunction( new nfSetTypeTrailEmitter( init ) );
	AddFunction( new nfGetTypeTrailController( init ) );
	AddFunction( new nfSetTypeTrailController( init ) );
	AddFunction( new nfGetTypePhysicsSize( init ) );
	AddFunction( new nfSetTypePhysicsSize( init ) );
	AddFunction( new nfGetTypeCollisionResponse( init ) );
	AddFunction( new nfSetTypeCollisionResponse( init ) );
	AddFunction( new nfGetTypeCollisionEmitter( init ) );
	AddFunction( new nfSetTypeCollisionEmitter( init ) );
	AddFunction( new nfGetTypeEmitMinImpulse( init ) );
	AddFunction( new nfSetTypeEmitMinImpulse( init ) );
	AddFunction( new nfGetTypeEmitController( init ) );
	AddFunction( new nfSetTypeEmitController( init ) );
	
	AddFunction( new nfSetTypeParameterValue( init ) );
	AddFunction( new nfSetTypeParameterSpread( init ) );
	AddFunction( new nfSetTypeParameterControllerValue( init ) );
	AddFunction( new nfSetTypeParameterCurveValue( init ) );
	AddFunction( new nfSetTypeParameterControllerSpread( init ) );
	AddFunction( new nfSetTypeParameterCurveSpread( init ) );
	AddFunction( new nfSetTypeParameterCurveProgress( init ) );
	AddFunction( new nfSetTypeParameterCurveBeam( init ) );
	
	AddFunction( new nfHashCode( init ) );
	AddFunction( new nfEquals( init ) );
	
	CalcMemberOffsets();
}

deParticleEmitter *deClassParticleEmitter::GetParticleEmitter( dsRealObject *myself ) const{
	if( ! myself ){
		return NULL;
	}
	
	return ( ( sPENatDat* )p_GetNativeData( myself->GetBuffer() ) )->emitter;
}

void deClassParticleEmitter::PushParticleEmitter( dsRunTime *rt, deParticleEmitter *emitter ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	if( ! emitter ){
		rt->PushObject( NULL, this );
		return;
	}
	
	rt->CreateObjectNakedOnStack( this );
	( ( sPENatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) )->emitter = emitter;
	emitter->AddReference();
}
