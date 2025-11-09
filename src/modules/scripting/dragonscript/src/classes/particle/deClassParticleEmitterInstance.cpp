/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "deClassParticleEmitter.h"
#include "deClassParticleEmitterController.h"
#include "deClassParticleEmitterInstance.h"
#include "deClassParticleEmitterInstanceListener.h"
#include "../collider/deClassCollider.h"
#include "../curve/deClassCurveBezier.h"
#include "../graphics/deClassComponent.h"
#include "../math/deClassVector.h"
#include "../math/deClassDVector.h"
#include "../math/deClassQuaternion.h"
#include "../physics/deClassCollisionFilter.h"
#include "../physics/deClassLayerMask.h"
#include "../world/deClassModel.h"
#include "../world/deClassSkin.h"
#include "../world/deClassDynamicSkin.h"
#include "../world/deClassWorld.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"
#include "../../peers/dedsParticleEmitter.h"

#include <dragengine/deEngine.h>
#include <libdscript/exceptions.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/particle/deParticleEmitter.h>
#include <dragengine/resources/particle/deParticleEmitterInstance.h>
#include <dragengine/resources/particle/deParticleEmitterInstanceManager.h>
#include <dragengine/resources/particle/deParticleEmitterController.h>
#include <dragengine/resources/particle/deParticleEmitterInstanceType.h>
#include "dragengine/resources/skin/deSkin.h"



// Native Structure
/////////////////////

struct sPEINatDat{
	deParticleEmitterInstance *instance;
};



// Native functions
/////////////////////

// public func new()
deClassParticleEmitterInstance::nfNew::nfNew( const sInitData &init ) : dsFunction( init.clsPEI,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassParticleEmitterInstance::nfNew::RunFunction( dsRunTime*, dsValue *myself ){
	deScriptingDragonScript &ds = *( ( ( deClassParticleEmitterInstance* )GetOwnerClass() )->GetDS() );
	deParticleEmitterInstanceManager &peimgr = *ds.GetGameEngine()->GetParticleEmitterInstanceManager();
	sPEINatDat *nd = ( sPEINatDat* )p_GetNativeData( myself );
	
	// clear ( important )
	nd->instance = NULL;
	
	// create emitter
	nd->instance = peimgr.CreateInstance();
}

// public func destructor()
deClassParticleEmitterInstance::nfDestructor::nfDestructor( const sInitData &init ) : dsFunction( init.clsPEI,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassParticleEmitterInstance::nfDestructor::RunFunction( dsRunTime*, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sPEINatDat *nd = ( sPEINatDat* )p_GetNativeData( myself );
	
	if( nd->instance ){
		nd->instance->FreeReference();
		nd->instance = NULL;
	}
}



// Management
///////////////

// public func ParticleEmitter getEmitter()
deClassParticleEmitterInstance::nfGetEmitter::nfGetEmitter( const sInitData &init ) : dsFunction( init.clsPEI,
"getEmitter", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsPE ){
}
void deClassParticleEmitterInstance::nfGetEmitter::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deParticleEmitterInstance &instance = *( ( ( sPEINatDat* )p_GetNativeData( myself ) )->instance );
	deScriptingDragonScript &ds = *( ( ( deClassParticleEmitterInstance* )GetOwnerClass() )->GetDS() );
	
	ds.GetClassParticleEmitter()->PushParticleEmitter( rt, instance.GetEmitter() );
}

// public func void setEmitter( ParticleEmitter emitter )
deClassParticleEmitterInstance::nfSetEmitter::nfSetEmitter( const sInitData &init ) : dsFunction( init.clsPEI,
"setEmitter", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsPE ); // emitter
}
void deClassParticleEmitterInstance::nfSetEmitter::RunFunction( dsRunTime *rt, dsValue *myself ){
	deParticleEmitterInstance &instance = *( ( ( sPEINatDat* )p_GetNativeData( myself ) )->instance );
	deScriptingDragonScript &ds = *( ( ( deClassParticleEmitterInstance* )GetOwnerClass() )->GetDS() );
	
	instance.SetEmitter( ds.GetClassParticleEmitter()->GetParticleEmitter( rt->GetValue( 0 )->GetRealObject() ) );
}



// public func DVector getPosition()
deClassParticleEmitterInstance::nfGetPosition::nfGetPosition( const sInitData &init ) : dsFunction( init.clsPEI,
"getPosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDVec ){
}
void deClassParticleEmitterInstance::nfGetPosition::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deParticleEmitterInstance &instance = *( ( ( sPEINatDat* )p_GetNativeData( myself ) )->instance );
	deScriptingDragonScript &ds = *( ( ( deClassParticleEmitterInstance* )GetOwnerClass() )->GetDS() );
	
	ds.GetClassDVector()->PushDVector( rt, instance.GetPosition() );
}

// public func void setPosition( DVector position )
deClassParticleEmitterInstance::nfSetPosition::nfSetPosition( const sInitData &init ) : dsFunction( init.clsPEI,
"setPosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsDVec ); // position
}
void deClassParticleEmitterInstance::nfSetPosition::RunFunction( dsRunTime *rt, dsValue *myself ){
	deParticleEmitterInstance &instance = *( ( ( sPEINatDat* )p_GetNativeData( myself ) )->instance );
	deScriptingDragonScript &ds = *( ( ( deClassParticleEmitterInstance* )GetOwnerClass() )->GetDS() );
	
	instance.SetPosition( ds.GetClassDVector()->GetDVector( rt->GetValue( 0 )->GetRealObject() ) );
}

// public func Quaternion getOrientation()
deClassParticleEmitterInstance::nfGetOrientation::nfGetOrientation( const sInitData &init ) : dsFunction( init.clsPEI,
"getOrientation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsQuat ){
}
void deClassParticleEmitterInstance::nfGetOrientation::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deParticleEmitterInstance &instance = *( ( ( sPEINatDat* )p_GetNativeData( myself ) )->instance );
	deScriptingDragonScript &ds = *( ( ( deClassParticleEmitterInstance* )GetOwnerClass() )->GetDS() );
	
	ds.GetClassQuaternion()->PushQuaternion( rt, instance.GetOrientation() );
}

// public func void setOrientation( Quaternion orientation )
deClassParticleEmitterInstance::nfSetOrientation::nfSetOrientation( const sInitData &init ) : dsFunction( init.clsPEI,
"setOrientation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsQuat ); // orientation
}
void deClassParticleEmitterInstance::nfSetOrientation::RunFunction( dsRunTime *rt, dsValue *myself ){
	deParticleEmitterInstance &instance = *( ( ( sPEINatDat* )p_GetNativeData( myself ) )->instance );
	deScriptingDragonScript &ds = *( ( ( deClassParticleEmitterInstance* )GetOwnerClass() )->GetDS() );
	
	instance.SetOrientation( ds.GetClassQuaternion()->GetQuaternion( rt->GetValue( 0 )->GetRealObject() ) );
}

// public func bool getEnableCasting()
deClassParticleEmitterInstance::nfGetEnableCasting::nfGetEnableCasting( const sInitData &init ) : dsFunction( init.clsPEI,
"getEnableCasting", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
}
void deClassParticleEmitterInstance::nfGetEnableCasting::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deParticleEmitterInstance &instance = *( ( ( sPEINatDat* )p_GetNativeData( myself ) )->instance );
	
	rt->PushBool( instance.GetEnableCasting() );
}

// public func void setEnableCasting( bool enable )
deClassParticleEmitterInstance::nfSetEnableCasting::nfSetEnableCasting( const sInitData &init ) : dsFunction( init.clsPEI,
"setEnableCasting", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsBool ); // enable
}
void deClassParticleEmitterInstance::nfSetEnableCasting::RunFunction( dsRunTime *rt, dsValue *myself ){
	deParticleEmitterInstance &instance = *( ( ( sPEINatDat* )p_GetNativeData( myself ) )->instance );
	
	instance.SetEnableCasting( rt->GetValue( 0 )->GetBool() );
}

// public func bool getRemoveAfterLastParticleDied()
deClassParticleEmitterInstance::nfGetRemoveAfterLastParticleDied::nfGetRemoveAfterLastParticleDied( const sInitData &init ) : dsFunction( init.clsPEI,
"getRemoveAfterLastParticleDied", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
}
void deClassParticleEmitterInstance::nfGetRemoveAfterLastParticleDied::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deParticleEmitterInstance &instance = *( ( ( sPEINatDat* )p_GetNativeData( myself ) )->instance );
	
	rt->PushBool( instance.GetRemoveAfterLastParticleDied() );
}

// public func void setRemoveAfterLastParticleDied( bool remove )
deClassParticleEmitterInstance::nfSetRemoveAfterLastParticleDied::nfSetRemoveAfterLastParticleDied( const sInitData &init ) : dsFunction( init.clsPEI,
"setRemoveAfterLastParticleDied", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsBool ); // remove
}
void deClassParticleEmitterInstance::nfSetRemoveAfterLastParticleDied::RunFunction( dsRunTime *rt, dsValue *myself ){
	deParticleEmitterInstance &instance = *( ( ( sPEINatDat* )p_GetNativeData( myself ) )->instance );
	
	instance.SetRemoveAfterLastParticleDied( rt->GetValue( 0 )->GetBool() );
}

// public float getTimeScale()
deClassParticleEmitterInstance::nfGetTimeScale::nfGetTimeScale( const sInitData &init ) : dsFunction( init.clsPEI,
"getTimeScale", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}
void deClassParticleEmitterInstance::nfGetTimeScale::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deParticleEmitterInstance &instance = *( ( ( sPEINatDat* )p_GetNativeData( myself ) )->instance );
	
	rt->PushFloat( instance.GetTimeScale() );
}

// public func void setTimeScale( float scale )
deClassParticleEmitterInstance::nfSetTimeScale::nfSetTimeScale( const sInitData &init ) : dsFunction( init.clsPEI,
"setTimeScale", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // scale
}
void deClassParticleEmitterInstance::nfSetTimeScale::RunFunction( dsRunTime *rt, dsValue *myself ){
	deParticleEmitterInstance &instance = *( ( ( sPEINatDat* )p_GetNativeData( myself ) )->instance );
	
	instance.SetTimeScale( rt->GetValue( 0 )->GetFloat() );
}

// public float getWarmUpTime()
deClassParticleEmitterInstance::nfGetWarmUpTime::nfGetWarmUpTime( const sInitData &init ) : dsFunction( init.clsPEI,
"getWarmUpTime", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}
void deClassParticleEmitterInstance::nfGetWarmUpTime::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deParticleEmitterInstance &instance = *( ( ( sPEINatDat* )p_GetNativeData( myself ) )->instance );
	
	rt->PushFloat( instance.GetWarmUpTime() );
}

// public func void setWarmUpTime( float warmUpTime )
deClassParticleEmitterInstance::nfSetWarmUpTime::nfSetWarmUpTime( const sInitData &init ) : dsFunction( init.clsPEI,
"setWarmUpTime", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // warmUpTime
}
void deClassParticleEmitterInstance::nfSetWarmUpTime::RunFunction( dsRunTime *rt, dsValue *myself ){
	deParticleEmitterInstance &instance = *( ( ( sPEINatDat* )p_GetNativeData( myself ) )->instance );
	
	instance.SetWarmUpTime( rt->GetValue( 0 )->GetFloat() );
}

// public float getBurstTime()
deClassParticleEmitterInstance::nfGetBurstTime::nfGetBurstTime( const sInitData &init ) : dsFunction( init.clsPEI,
"getBurstTime", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}
void deClassParticleEmitterInstance::nfGetBurstTime::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deParticleEmitterInstance &instance = *( ( ( sPEINatDat* )p_GetNativeData( myself ) )->instance );
	
	rt->PushFloat( instance.GetBurstTime() );
}

// public func void setBurstTime( float burstTime )
deClassParticleEmitterInstance::nfSetBurstTime::nfSetBurstTime( const sInitData &init ) : dsFunction( init.clsPEI,
"setBurstTime", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // burstTime
}
void deClassParticleEmitterInstance::nfSetBurstTime::RunFunction( dsRunTime *rt, dsValue *myself ){
	deParticleEmitterInstance &instance = *( ( ( sPEINatDat* )p_GetNativeData( myself ) )->instance );
	
	instance.SetBurstTime( rt->GetValue( 0 )->GetFloat() );
}

// func World getParentWorld()
deClassParticleEmitterInstance::nfGetParentWorld::nfGetParentWorld(const sInitData &init) :
dsFunction(init.clsPEI, "getParentWorld", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsWorld){
}

void deClassParticleEmitterInstance::nfGetParentWorld::RunFunction(dsRunTime *rt, dsValue *myself){
	const deParticleEmitterInstance &instance = *((sPEINatDat*)p_GetNativeData(myself))->instance;
	deScriptingDragonScript &ds = *((deClassParticleEmitterInstance*)GetOwnerClass())->GetDS();
	ds.GetClassWorld()->PushWorld(rt, instance.GetParentWorld());
}

// public func int getControllerCount()
deClassParticleEmitterInstance::nfGetControllerCount::nfGetControllerCount( const sInitData &init ) :
dsFunction( init.clsPEI, "getControllerCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassParticleEmitterInstance::nfGetControllerCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deParticleEmitterInstance &instance = *( ( ( sPEINatDat* )p_GetNativeData( myself ) )->instance );
	
	rt->PushInt( instance.GetControllerCount() );
}

// public func ParticleEmitterController getControllerAt( int index )
deClassParticleEmitterInstance::nfGetControllerAt::nfGetControllerAt( const sInitData &init ) :
dsFunction( init.clsPEI, "getControllerAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsPEController ){
	p_AddParameter( init.clsInt ); // controller
}
void deClassParticleEmitterInstance::nfGetControllerAt::RunFunction( dsRunTime *rt, dsValue *myself ){
	deParticleEmitterInstance * const instance = ( ( sPEINatDat* )p_GetNativeData( myself ) )->instance;
	const deScriptingDragonScript &ds = *( ( ( deClassParticleEmitterInstance* )GetOwnerClass() )->GetDS() );
	const int index = rt->GetValue( 0 )->GetInt();
	
	if( index < 0 ){
		ds.GetClassParticleEmitterController()->PushController( rt, instance,
			instance->GetControllerCount() + index );
		
	}else{
		ds.GetClassParticleEmitterController()->PushController( rt, instance, index );
	}
}

// public func ParticleEmitterController getControllerNamed( String name )
deClassParticleEmitterInstance::nfGetControllerNamed::nfGetControllerNamed( const sInitData &init ) :
dsFunction( init.clsPEI, "getControllerNamed", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsPEController ){
	p_AddParameter( init.clsStr ); // name
}
void deClassParticleEmitterInstance::nfGetControllerNamed::RunFunction( dsRunTime *rt, dsValue *myself ){
	deParticleEmitterInstance * const instance = ( ( sPEINatDat* )p_GetNativeData( myself ) )->instance;
	const deScriptingDragonScript &ds = *( ( ( deClassParticleEmitterInstance* )GetOwnerClass() )->GetDS() );
	const int index = instance->IndexOfControllerNamed( rt->GetValue( 0 )->GetString() );
	
	if( index == -1 ){
		rt->PushObject( NULL, ds.GetClassParticleEmitterController() );
		
	}else{
		ds.GetClassParticleEmitterController()->PushController( rt, instance, index );
	}
}

// public func int indexOfControllerNamed( String name )
deClassParticleEmitterInstance::nfIndexOfControllerNamed::nfIndexOfControllerNamed( const sInitData &init ) :
dsFunction( init.clsPEI, "indexOfControllerNamed", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
	p_AddParameter( init.clsStr ); // name
}
void deClassParticleEmitterInstance::nfIndexOfControllerNamed::RunFunction( dsRunTime *rt, dsValue *myself ){
	deParticleEmitterInstance * const instance = ( ( sPEINatDat* )p_GetNativeData( myself ) )->instance;
	rt->PushInt( instance->IndexOfControllerNamed( rt->GetValue( 0 )->GetString() ) );
}

// public func void copyControllerStates( ParticleEmitterInstance instance, int count )
deClassParticleEmitterInstance::nfCopyControllerStates::nfCopyControllerStates( const sInitData &init ) :
dsFunction( init.clsPEI, "copyControllerStates", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsPEI ); // instance
	p_AddParameter( init.clsInt ); // count
}
void deClassParticleEmitterInstance::nfCopyControllerStates::RunFunction( dsRunTime *rt, dsValue *myself ){
	deParticleEmitterInstance &instance = *( ( ( sPEINatDat* )p_GetNativeData( myself ) )->instance );
	const deClassParticleEmitterInstance &clsPEI = *( ( deClassParticleEmitterInstance* )GetOwnerClass() );
	
	const deParticleEmitterInstance * const sourceInstance = clsPEI.GetInstance( rt->GetValue( 0 )->GetRealObject() );
	int count = rt->GetValue( 1 )->GetInt();
	
	if( count < 0 ){
		DSTHROW( dueInvalidParam );
	}
	
	if( ! sourceInstance ){
		DSTHROW( dueNullPointer );
	}
	
	if( sourceInstance->GetControllerCount() < count ){
		count = sourceInstance->GetControllerCount();
	}
	if( instance.GetControllerCount() < count ){
		count = instance.GetControllerCount();
	}
	
	int i;
	
	for( i=0; i<count; i++ ){
		const deParticleEmitterController &sourceController = sourceInstance->GetControllerAt( i );
		deParticleEmitterController &controller = instance.GetControllerAt( i );
		
		controller.SetValue( sourceController.GetValue() );
		
		instance.NotifyControllerChangedAt( i );
	}
}

// public func void copyControllerStates( ParticleEmitterInstance instance, int offset, int count )
deClassParticleEmitterInstance::nfCopyControllerStates2::nfCopyControllerStates2( const sInitData &init ) :
dsFunction( init.clsPEI, "copyControllerStates", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsPEI ); // instance
	p_AddParameter( init.clsInt ); // offset
	p_AddParameter( init.clsInt ); // count
}
void deClassParticleEmitterInstance::nfCopyControllerStates2::RunFunction( dsRunTime *rt, dsValue *myself ){
	deParticleEmitterInstance &instance = *( ( ( sPEINatDat* )p_GetNativeData( myself ) )->instance );
	const deClassParticleEmitterInstance &clsPEI = *( ( deClassParticleEmitterInstance* )GetOwnerClass() );
	
	const deParticleEmitterInstance * const sourceInstance = clsPEI.GetInstance( rt->GetValue( 0 )->GetRealObject() );
	const int offset = rt->GetValue( 1 )->GetInt();
	int count = rt->GetValue( 2 )->GetInt();
	
	if( offset < 0 ){
		DSTHROW( dueInvalidParam );
	}
	if( count < 0 ){
		DSTHROW( dueInvalidParam );
	}
	
	if( ! sourceInstance ){
		DSTHROW( dueNullPointer );
	}
	
	if( sourceInstance->GetControllerCount() < count ){
		count = sourceInstance->GetControllerCount();
	}
	if( instance.GetControllerCount() - offset < count ){
		count = instance.GetControllerCount() - offset;
	}
	
	int i;
	
	for( i=0; i<count; i++ ){
		const deParticleEmitterController &sourceController = sourceInstance->GetControllerAt( i );
		deParticleEmitterController &controller = instance.GetControllerAt( offset + i );
		
		controller.SetValue( sourceController.GetValue() );
		
		instance.NotifyControllerChangedAt( i );
	}
}

// public func void copyNamedControllerStates( ParticleEmitterInstance instance )
deClassParticleEmitterInstance::nfCopyNamedControllerStates::nfCopyNamedControllerStates( const sInitData &init ) :
dsFunction( init.clsPEI, "copyNamedControllerStates", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsPEI ); // instance
}
void deClassParticleEmitterInstance::nfCopyNamedControllerStates::RunFunction( dsRunTime *rt, dsValue *myself ){
	deParticleEmitterInstance &instance = *( ( ( sPEINatDat* )p_GetNativeData( myself ) )->instance );
	const deClassParticleEmitterInstance &clsPEI = *( ( deClassParticleEmitterInstance* )GetOwnerClass() );
	
	const deParticleEmitterInstance * const sourceInstance = clsPEI.GetInstance( rt->GetValue( 0 )->GetRealObject() );
	if( ! sourceInstance ){
		DSTHROW( dueNullPointer );
	}
	
	const int count = sourceInstance->GetControllerCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const deParticleEmitterController &sourceController = sourceInstance->GetControllerAt( i );
		const int index = instance.IndexOfControllerNamed( sourceController.GetName() );
		if( index == -1 ){
			continue;
		}
		
		deParticleEmitterController &controller = instance.GetControllerAt( index );
		
		controller.SetValue( sourceController.GetValue() );
		
		instance.NotifyControllerChangedAt( index );
	}
}



// public func LayerMask getLayerMask()
deClassParticleEmitterInstance::nfGetLayerMask::nfGetLayerMask( const sInitData &init ) :
dsFunction( init.clsPEI, "getLayerMask", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsLayerMask ){
}
void deClassParticleEmitterInstance::nfGetLayerMask::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deParticleEmitterInstance &instance = *( ( ( sPEINatDat* )p_GetNativeData( myself ) )->instance );
	deScriptingDragonScript &ds = *( ( ( deClassParticleEmitterInstance* )GetOwnerClass() )->GetDS() );
	
	ds.GetClassLayerMask()->PushLayerMask( rt, instance.GetLayerMask() );
}

// public func void setLayerMask( LayerMask layerMask )
deClassParticleEmitterInstance::nfSetLayerMask::nfSetLayerMask( const sInitData &init ) :
dsFunction( init.clsPEI, "setLayerMask", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsLayerMask ); // layerMask
}
void deClassParticleEmitterInstance::nfSetLayerMask::RunFunction( dsRunTime *rt, dsValue *myself ){
	deParticleEmitterInstance &instance = *( ( ( sPEINatDat* )p_GetNativeData( myself ) )->instance );
	deScriptingDragonScript &ds = *( ( ( deClassParticleEmitterInstance* )GetOwnerClass() )->GetDS() );
	
	const decLayerMask &layerMask = ds.GetClassLayerMask()->GetLayerMask( rt->GetValue( 0 )->GetRealObject() );
	
	instance.SetLayerMask( layerMask );
}

// public func CollisionFilter getCollisionFilter()
deClassParticleEmitterInstance::nfGetCollisionFilter::nfGetCollisionFilter( const sInitData &init ) : dsFunction( init.clsPEI,
"getCollisionFilter", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsCF ){
}
void deClassParticleEmitterInstance::nfGetCollisionFilter::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deParticleEmitterInstance &instance = *( ( ( sPEINatDat* )p_GetNativeData( myself ) )->instance );
	deScriptingDragonScript &ds = *( ( ( deClassParticleEmitterInstance* )GetOwnerClass() )->GetDS() );
	
	ds.GetClassCollisionFilter()->PushCollisionFilter( rt, instance.GetCollisionFilter() );
}

// public func void setCollisionFilter( CollisionFilter collisionFilter )
deClassParticleEmitterInstance::nfSetCollisionFilter::nfSetCollisionFilter( const sInitData &init ) : dsFunction( init.clsPEI,
"setCollisionFilter", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsCF ); // collisionFilter
}
void deClassParticleEmitterInstance::nfSetCollisionFilter::RunFunction( dsRunTime *rt, dsValue *myself ){
	deParticleEmitterInstance &instance = *( ( ( sPEINatDat* )p_GetNativeData( myself ) )->instance );
	deScriptingDragonScript &ds = *( ( ( deClassParticleEmitterInstance* )GetOwnerClass() )->GetDS() );
	
	const decCollisionFilter &collisionFilter = ds.GetClassCollisionFilter()->GetCollisionFilter( rt->GetValue( 0 )->GetRealObject() );
	
	instance.SetCollisionFilter( collisionFilter );
}



// public func Component getTypeComponent( int type )
deClassParticleEmitterInstance::nfGetTypeComponent::nfGetTypeComponent( const sInitData &init ) : dsFunction( init.clsPEI,
"getTypeComponent", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsComp ){
	p_AddParameter( init.clsInt ); // type
}
void deClassParticleEmitterInstance::nfGetTypeComponent::RunFunction( dsRunTime *rt, dsValue *myself ){
	deParticleEmitterInstance &instance = *( ( ( sPEINatDat* )p_GetNativeData( myself ) )->instance );
	deScriptingDragonScript &ds = *( ( ( deClassParticleEmitterInstance* )GetOwnerClass() )->GetDS() );
	
	const deParticleEmitterInstanceType &type = instance.GetTypeAt( rt->GetValue( 0 )->GetInt() );
	
	ds.GetClassComponent()->PushComponent( rt, type.GetComponent() );
}

// public func void setTypeComponent( int type, Component component )
deClassParticleEmitterInstance::nfSetTypeComponent::nfSetTypeComponent( const sInitData &init ) : dsFunction( init.clsPEI,
"setTypeComponent", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // type
	p_AddParameter( init.clsComp ); // component
}
void deClassParticleEmitterInstance::nfSetTypeComponent::RunFunction( dsRunTime *rt, dsValue *myself ){
	deParticleEmitterInstance &instance = *( ( ( sPEINatDat* )p_GetNativeData( myself ) )->instance );
	deScriptingDragonScript &ds = *( ( ( deClassParticleEmitterInstance* )GetOwnerClass() )->GetDS() );
	
	const int index = rt->GetValue( 0 )->GetInt();
	deParticleEmitterInstanceType &type = instance.GetTypeAt( index );
	
	type.SetComponent( ds.GetClassComponent()->GetComponent( rt->GetValue( 1 )->GetRealObject() ) );
	instance.NotifyTypeChangedAt( index );
}

// public func DynamicSkin getTypeDynamicSkin( int type )
deClassParticleEmitterInstance::nfGetTypeDynamicSkin::nfGetTypeDynamicSkin( const sInitData &init ) : dsFunction( init.clsPEI,
"getTypeDynamicSkin", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDynSkin ){
	p_AddParameter( init.clsInt ); // type
}
void deClassParticleEmitterInstance::nfGetTypeDynamicSkin::RunFunction( dsRunTime *rt, dsValue *myself ){
	deParticleEmitterInstance &instance = *( ( ( sPEINatDat* )p_GetNativeData( myself ) )->instance );
	deScriptingDragonScript &ds = *( ( ( deClassParticleEmitterInstance* )GetOwnerClass() )->GetDS() );
	
	const deParticleEmitterInstanceType &type = instance.GetTypeAt( rt->GetValue( 0 )->GetInt() );
	
	ds.GetClassDynamicSkin()->PushDynamicSkin( rt, type.GetDynamicSkin() );
}

// public func void setTypeDynamicSkin( int type, DynamicSkin dynamicSkin )
deClassParticleEmitterInstance::nfSetTypeDynamicSkin::nfSetTypeDynamicSkin( const sInitData &init ) : dsFunction( init.clsPEI,
"setTypeDynamicSkin", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // type
	p_AddParameter( init.clsDynSkin ); // dynamicSkin
}
void deClassParticleEmitterInstance::nfSetTypeDynamicSkin::RunFunction( dsRunTime *rt, dsValue *myself ){
	deParticleEmitterInstance &instance = *( ( ( sPEINatDat* )p_GetNativeData( myself ) )->instance );
	deScriptingDragonScript &ds = *( ( ( deClassParticleEmitterInstance* )GetOwnerClass() )->GetDS() );
	
	const int index = rt->GetValue( 0 )->GetInt();
	deParticleEmitterInstanceType &type = instance.GetTypeAt( index );
	
	type.SetDynamicSkin( ds.GetClassDynamicSkin()->GetDynamicSkin( rt->GetValue( 1 )->GetRealObject() ) );
	instance.NotifyTypeChangedAt( index );
}



// public func void resetBurst()
deClassParticleEmitterInstance::nfResetBurst::nfResetBurst( const sInitData &init ) : dsFunction( init.clsPEI,
"resetBurst", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassParticleEmitterInstance::nfResetBurst::RunFunction( dsRunTime*, dsValue *myself ){
	deParticleEmitterInstance &instance = *( ( ( sPEINatDat* )p_GetNativeData( myself ) )->instance );
	
	instance.ResetBurst();
}

// public func void killAllParticles()
deClassParticleEmitterInstance::nfKillAllParticles::nfKillAllParticles( const sInitData &init ) : dsFunction( init.clsPEI,
"killAllParticles", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassParticleEmitterInstance::nfKillAllParticles::RunFunction( dsRunTime*, dsValue *myself ){
	deParticleEmitterInstance &instance = *( ( ( sPEINatDat* )p_GetNativeData( myself ) )->instance );
	
	instance.KillAllParticles();
}



// Listening
//////////////

// public func PropFieldListener getListener()
deClassParticleEmitterInstance::nfGetListener::nfGetListener( const sInitData &init ) : dsFunction( init.clsPEI,
"getListener", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsPEIL ){
}
void deClassParticleEmitterInstance::nfGetListener::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deParticleEmitterInstance &instance = *( ( ( sPEINatDat* )p_GetNativeData( myself ) )->instance );
	deScriptingDragonScript &ds = *( ( ( deClassParticleEmitterInstance* )GetOwnerClass() )->GetDS() );
	dedsParticleEmitter *peer = ( dedsParticleEmitter* )instance.GetPeerScripting();
	
	if( peer ){
		rt->PushObject( peer->GetCallback(), ds.GetClassParticleEmitterInstanceListener() );
		
	}else{
		rt->PushObject( NULL, ds.GetClassParticleEmitterInstanceListener() );
	}
}

// public func void setListener( PropFieldListener listener )
deClassParticleEmitterInstance::nfSetListener::nfSetListener( const sInitData &init ) : dsFunction( init.clsPEI,
"setListener", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsPEIL ); // listener
}
void deClassParticleEmitterInstance::nfSetListener::RunFunction( dsRunTime *rt, dsValue *myself ){
	deParticleEmitterInstance &instance = *( ( ( sPEINatDat* )p_GetNativeData( myself ) )->instance );
	dedsParticleEmitter *peer = ( dedsParticleEmitter* )instance.GetPeerScripting();
	if( peer ){
		peer->SetCallback( rt->GetValue( 0 )->GetRealObject() );
	}
}



// Ignore colliders
/////////////////////

// public func int getIgnoreColliderCount()
deClassParticleEmitterInstance::nfGetIgnoreColliderCount::nfGetIgnoreColliderCount( const sInitData &init ) :
dsFunction( init.clsPEI, "getIgnoreColliderCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassParticleEmitterInstance::nfGetIgnoreColliderCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sPEINatDat &nd = *( ( sPEINatDat* )p_GetNativeData( myself ) );
	if( ! nd.instance ){
		DSTHROW( dueNullPointer );
	}
	
	rt->PushInt( nd.instance->GetIgnoreColliderCount() );
}

// public func Collider getIgnoreColliderAt( int index )
deClassParticleEmitterInstance::nfGetIgnoreColliderAt::nfGetIgnoreColliderAt( const sInitData &init ) :
dsFunction( init.clsPEI, "getIgnoreColliderAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsCollider ){
	p_AddParameter( init.clsInt ); // index
}
void deClassParticleEmitterInstance::nfGetIgnoreColliderAt::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sPEINatDat &nd = *( ( sPEINatDat* )p_GetNativeData( myself ) );
	if( ! nd.instance ){
		DSTHROW( dueNullPointer );
	}
	
	deScriptingDragonScript &ds = *( ( ( deClassParticleEmitterInstance* )GetOwnerClass() )->GetDS() );
	const int index = rt->GetValue( 0 )->GetInt();
	ds.GetClassCollider()->PushCollider( rt, nd.instance->GetIgnoreColliderAt( index ) );
}

// public func bool hasIgnoreCollider( Collider collider )
deClassParticleEmitterInstance::nfHasIgnoreCollider::nfHasIgnoreCollider( const sInitData &init ) :
dsFunction( init.clsPEI, "hasIgnoreCollider", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsCollider ); // collider
}
void deClassParticleEmitterInstance::nfHasIgnoreCollider::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sPEINatDat &nd = *( ( sPEINatDat* )p_GetNativeData( myself ) );
	if( ! nd.instance ){
		DSTHROW( dueNullPointer );
	}
	
	deScriptingDragonScript &ds = *( ( ( deClassParticleEmitterInstance* )GetOwnerClass() )->GetDS() );
	
	deCollider * const collider = ds.GetClassCollider()->GetCollider( rt->GetValue( 0 )->GetRealObject() );
	rt->PushBool( nd.instance->HasIgnoreCollider( collider ) );
}

// public func void addIgnoreCollider( Collider collider )
deClassParticleEmitterInstance::nfAddIgnoreCollider::nfAddIgnoreCollider( const sInitData &init ) :
dsFunction( init.clsPEI, "addIgnoreCollider", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsCollider ); // collider
}
void deClassParticleEmitterInstance::nfAddIgnoreCollider::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sPEINatDat &nd = *( ( sPEINatDat* )p_GetNativeData( myself ) );
	if( ! nd.instance ){
		DSTHROW( dueNullPointer );
	}
	
	deScriptingDragonScript &ds = *( ( ( deClassParticleEmitterInstance* )GetOwnerClass() )->GetDS() );
	deCollider * const collider = ds.GetClassCollider()->GetCollider( rt->GetValue( 0 )->GetRealObject() );
	nd.instance->AddIgnoreCollider( collider );
}

// public func void removeIgnoreCollider( Collider collider )
deClassParticleEmitterInstance::nfRemoveIgnoreCollider::nfRemoveIgnoreCollider( const sInitData &init ) :
dsFunction( init.clsPEI, "removeIgnoreCollider", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsCollider ); // collider
}
void deClassParticleEmitterInstance::nfRemoveIgnoreCollider::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sPEINatDat &nd = *( ( sPEINatDat* )p_GetNativeData( myself ) );
	if( ! nd.instance ){
		DSTHROW( dueNullPointer );
	}
	
	deScriptingDragonScript &ds = *( ( ( deClassParticleEmitterInstance* )GetOwnerClass() )->GetDS() );
	deCollider * const collider = ds.GetClassCollider()->GetCollider( rt->GetValue( 0 )->GetRealObject() );
	nd.instance->RemoveIgnoreCollider( collider );
}

// public func void removeAllIgnoreColliders()
deClassParticleEmitterInstance::nfRemoveAllIgnoreColliders::nfRemoveAllIgnoreColliders( const sInitData &init ) :
dsFunction( init.clsPEI, "removeAllIgnoreColliders", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassParticleEmitterInstance::nfRemoveAllIgnoreColliders::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sPEINatDat &nd = *( ( sPEINatDat* )p_GetNativeData( myself ) );
	if( ! nd.instance ){
		DSTHROW( dueNullPointer );
	}
	
	nd.instance->RemoveAllIgnoreColliders();
}



// public func int hashCode()
deClassParticleEmitterInstance::nfHashCode::nfHashCode( const sInitData &init ) :
dsFunction( init.clsPEI, "hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}

void deClassParticleEmitterInstance::nfHashCode::RunFunction( dsRunTime *rt, dsValue *myself ){
	deParticleEmitterInstance *instance = ( ( sPEINatDat* )p_GetNativeData( myself ) )->instance;
	
	rt->PushInt( ( int )( intptr_t )instance );
}

// public func bool equals( Object obj )
deClassParticleEmitterInstance::nfEquals::nfEquals( const sInitData &init ) :
dsFunction( init.clsPEI, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsObj ); // obj
}
void deClassParticleEmitterInstance::nfEquals::RunFunction( dsRunTime *rt, dsValue *myself ){
	deParticleEmitterInstance *instance = ( ( sPEINatDat* )p_GetNativeData( myself ) )->instance;
	deClassParticleEmitterInstance *clsPEI = ( deClassParticleEmitterInstance* )GetOwnerClass();
	dsValue *obj = rt->GetValue( 0 );
	
	if( ! p_IsObjOfType( obj, clsPEI ) ){
		rt->PushBool( false );
		
	}else{
		deParticleEmitterInstance *other = ( ( sPEINatDat* )p_GetNativeData( obj ) )->instance;
		
		rt->PushBool( instance == other );
	}
}




// Class deClassParticleEmitterInstance
/////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deClassParticleEmitterInstance::deClassParticleEmitterInstance( deScriptingDragonScript *ds ) :
dsClass( "ParticleEmitterInstance", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED ){
	if( ! ds ){
		DSTHROW( dueInvalidParam );
	}
	
	pDS = ds;
	
	GetParserInfo()->SetParent( DENS_SCENERY );
	GetParserInfo()->SetBase( "Object" );
	
	p_SetNativeDataSize( sizeof( sPEINatDat ) );
}

deClassParticleEmitterInstance::~deClassParticleEmitterInstance(){
}



// Management
///////////////

void deClassParticleEmitterInstance::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	init.clsPEI = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFloat = engine->GetClassFloat();
	init.clsStr = engine->GetClassString();
	init.clsBool = engine->GetClassBool();
	init.clsObj = engine->GetClassObject();
	init.clsVec = pDS->GetClassVector();
	init.clsDVec = pDS->GetClassDVector();
	init.clsComp = pDS->GetClassComponent();
	init.clsMdl = pDS->GetClassModel();
	init.clsSkin = pDS->GetClassSkin();
	init.clsCurveBezier = pDS->GetClassCurveBezier();
	init.clsCF = pDS->GetClassCollisionFilter();
	init.clsQuat = pDS->GetClassQuaternion();
	init.clsPE = pDS->GetClassParticleEmitter();
	init.clsPEIL = pDS->GetClassParticleEmitterInstanceListener();
	init.clsDynSkin = pDS->GetClassDynamicSkin();
	init.clsPEController = pDS->GetClassParticleEmitterController();
	init.clsLayerMask = pDS->GetClassLayerMask();
	init.clsCollider = pDS->GetClassCollider();
	init.clsWorld = pDS->GetClassWorld();
	
	AddFunction( new nfNew( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfGetEmitter( init ) );
	AddFunction( new nfSetEmitter( init ) );
	AddFunction( new nfGetPosition( init ) );
	AddFunction( new nfSetPosition( init ) );
	AddFunction( new nfGetOrientation( init ) );
	AddFunction( new nfSetOrientation( init ) );
	AddFunction( new nfGetEnableCasting( init ) );
	AddFunction( new nfSetEnableCasting( init ) );
	AddFunction( new nfGetRemoveAfterLastParticleDied( init ) );
	AddFunction( new nfSetRemoveAfterLastParticleDied( init ) );
	AddFunction( new nfGetTimeScale( init ) );
	AddFunction( new nfSetTimeScale( init ) );
	AddFunction( new nfGetWarmUpTime( init ) );
	AddFunction( new nfSetWarmUpTime( init ) );
	AddFunction( new nfGetBurstTime( init ) );
	AddFunction( new nfSetBurstTime( init ) );
	AddFunction(new nfGetParentWorld(init));
	
	AddFunction( new nfGetControllerCount( init ) );
	AddFunction( new nfGetControllerAt( init ) );
	AddFunction( new nfGetControllerNamed( init ) );
	AddFunction( new nfIndexOfControllerNamed( init ) );
	AddFunction( new nfCopyControllerStates( init ) );
	AddFunction( new nfCopyControllerStates2( init ) );
	AddFunction( new nfCopyNamedControllerStates( init ) );
	
	AddFunction( new nfGetLayerMask( init ) );
	AddFunction( new nfSetLayerMask( init ) );
	AddFunction( new nfGetCollisionFilter( init ) );
	AddFunction( new nfSetCollisionFilter( init ) );
	
	AddFunction( new nfGetTypeComponent( init ) );
	AddFunction( new nfSetTypeComponent( init ) );
	AddFunction( new nfGetTypeDynamicSkin( init ) );
	AddFunction( new nfSetTypeDynamicSkin( init ) );
	
	AddFunction( new nfResetBurst( init ) );
	AddFunction( new nfKillAllParticles( init ) );
	
	AddFunction( new nfGetListener( init ) );
	AddFunction( new nfSetListener( init ) );
	
	AddFunction( new nfGetIgnoreColliderCount( init ) );
	AddFunction( new nfGetIgnoreColliderAt( init ) );
	AddFunction( new nfHasIgnoreCollider( init ) );
	AddFunction( new nfAddIgnoreCollider( init ) );
	AddFunction( new nfRemoveIgnoreCollider( init ) );
	AddFunction( new nfRemoveAllIgnoreColliders( init ) );
	
	AddFunction( new nfHashCode( init ) );
	AddFunction( new nfEquals( init ) );
	
	CalcMemberOffsets();
}

deParticleEmitterInstance *deClassParticleEmitterInstance::GetInstance( dsRealObject *myself ) const{
	if( ! myself ){
		return NULL;
	}
	
	return ( ( sPEINatDat* )p_GetNativeData( myself->GetBuffer() ) )->instance;
}

void deClassParticleEmitterInstance::PushInstance( dsRunTime *rt, deParticleEmitterInstance *instance ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	if( ! instance ){
		rt->PushObject( NULL, this );
		return;
	}
	
	rt->CreateObjectNakedOnStack( this );
	( ( sPEINatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) )->instance = instance;
	instance->AddReference();
}
