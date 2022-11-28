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

// includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libdscript/libdscript.h>

#include "deClassWorld.h"

#include "../ai/deClassNavigationSpace.h"
#include "../ai/deClassNavigator.h"
#include "../ai/deClassNavigationBlocker.h"

#include "../collider/deClassCollider.h"
#include "../collider/deClassColliderListener.h"

#include "../debug/deClassDebugDrawer.h"

#include "../graphics/deClassCamera.h"
#include "../graphics/deClassColor.h"
#include "../graphics/deClassComponent.h"
#include "../graphics/deClassLight.h"
#include "../graphics/deClassLumimeter.h"
#include "../graphics/deClassEnvMapProbe.h"
#include "../graphics/deClassBillboard.h"

#include "../heightterrain/deClassHeightTerrain.h"

#include "../math/deClassDVector.h"
#include "../math/deClassMatrix.h"
#include "../math/deClassVector.h"

#include "../network/deClassNetworkState.h"

#include "../physics/deClassCollisionInfo.h"
#include "../physics/deClassTouchSensor.h"
#include "../physics/deClassForceField.h"
#include "../physics/deClassCollisionFilter.h"

#include "../particle/deClassParticleEmitterInstance.h"

#include "../propfield/deClassPropField.h"

#include "../sky/deClassSkyInstance.h"

#include "../sound/deClassMicrophone.h"
#include "../sound/deClassSpeaker.h"
#include "../sound/deClassSoundLevelMeter.h"

#include "../../peers/dedsCollider.h"
#include "../../deClassPathes.h"
#include "../../deScriptingDragonScript.h"
#include "../../utils/dedsColliderListenerClosest.h"
#include "../../utils/dedsColliderListenerAdaptor.h"

#include <libdscript/exceptions.h>
#include <dragengine/deEngine.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/effect/deEffect.h>
#include <dragengine/resources/navigation/space/deNavigationSpace.h>
#include <dragengine/resources/navigation/navigator/deNavigator.h>
#include <dragengine/resources/navigation/blocker/deNavigationBlocker.h>
#include <dragengine/resources/collider/deCollisionInfo.h>
#include <dragengine/resources/light/deLight.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/world/deWorldManager.h>
#include <dragengine/resources/probe/deEnvMapProbe.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/billboard/deBillboard.h>



#define _SUPER_DEBUG_

#ifdef _SUPER_DEBUG_
#	define DEBUGME	pScrMgr->LogInfoFormat( "oops %s:%i", __FILE__, __LINE__)
#else
#	define DEBUGME
#endif


// native structure
struct sWorldNatDat{
	deWorld *world;
};


// native functions
/////////////////////

// #define ENABLE_SPECIAL_DEBUG_TIMING 1

#ifdef ENABLE_SPECIAL_DEBUG_TIMING
static dsVariable *vDebug1 = NULL;
static void vDebugStartTiming( dsRunTime *rt, dsFunction *f ){
	if( ! vDebug1 ){
		dsClass * const cls = ( (deClassWorld*)f->GetOwnerClass() )->GetDS()->GetScriptEngine()->
			GetClass( "Dragengine" )->GetInnerClass( "Scenery" )->GetInnerClass( "GameWorld" );
		const int count = cls->GetVariableCount();
		for( int i=0; i<count; i++ ){
			dsVariable * const var = cls->GetVariable( i );
			if( strcmp( var->GetName(), "rtm" ) == 0 ){
				vDebug1 = var;
				break;
			}
		}
	}
	
	dsValue * const value = vDebug1->GetStaticValue();
	if( value->GetRealObject() ){
		rt->PushInt( 1 );
		rt->PushInt( 3 );
		rt->RunFunction( value, "reset", 2 );
	}
}

static void vDebugCaptureTiming( dsRunTime *rt, int counter ){
	dsValue * const value = vDebug1->GetStaticValue();
	if( value->GetRealObject() ){
		rt->PushInt( 9 + counter );
		rt->PushInt( 3 );
		rt->RunFunction( value, "counterAdd", 2 );
	}
}

#define SPECIAL_DEBUG_TIMING_START(rt, f) vDebugStartTiming(rt, f);
#define SPECIAL_DEBUG_TIMING_CAPTURE(rt, counter) vDebugCaptureTiming(rt, counter);

// 0: component
// 1: ligh 
// 2: collider
// 3: touch sensor
// 4: navigation space
// 5: navigation blocker
// 6: env-map probe

#else
#define SPECIAL_DEBUG_TIMING_START(rt, f)
#define SPECIAL_DEBUG_TIMING_CAPTURE(rt, counter)
#endif



// constructor, destructors
/////////////////////////////

// public func new()
deClassWorld::nfNew::nfNew(const sInitData &init) : dsFunction( init.clsWorld,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassWorld::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	sWorldNatDat *nd = ( sWorldNatDat* )p_GetNativeData( myself );
	deClassWorld *clsWorld = (deClassWorld*)GetOwnerClass();
	deWorldManager *worldMgr = clsWorld->GetGameEngine()->GetWorldManager();
	
	// prepare
	nd->world = NULL;
	
	// create world
	nd->world = worldMgr->CreateWorld();
	if(!nd->world) DSTHROW(dueOutOfMemory);
}

// public func destructor()
deClassWorld::nfDestructor::nfDestructor(const sInitData &init) : dsFunction( init.clsWorld,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassWorld::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sWorldNatDat *nd = ( sWorldNatDat* )p_GetNativeData( myself );
	
	if(nd->world){
		nd->world->FreeReference();
		nd->world = NULL;
	}
}



// management
///////////////

// public func void detectCollisions( float elapsed )
deClassWorld::nfDetectCollisions::nfDetectCollisions( const sInitData &init ) : dsFunction( init.clsWorld,
"detectCollisions", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFlt ); // elapsed
}
void deClassWorld::nfDetectCollisions::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld &world = *( ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world );
	world.ProcessPhysics( rt->GetValue( 0 )->GetFloat() );
}

// public func void update( float elapsed )
deClassWorld::nfUpdate::nfUpdate( const sInitData &init ) : dsFunction( init.clsWorld,
"update", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFlt ); // elapsed
}
void deClassWorld::nfUpdate::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld *world = ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world;
	float elapsed = rt->GetValue( 0 )->GetFloat();
	
	world->Update( elapsed );
}

// public func DVector getSize()
deClassWorld::nfGetSize::nfGetSize( const sInitData &init ) : dsFunction( init.clsWorld,
"getSize", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDVec ){
}
void deClassWorld::nfGetSize::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld *world = ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world;
	deClassWorld *clsWorld = ( deClassWorld* )GetOwnerClass();
	
	clsWorld->GetClassDVector()->PushDVector( rt, world->GetSize() );
}

// public func void setSize( Vector size )
deClassWorld::nfSetSize::nfSetSize( const sInitData &init ) : dsFunction( init.clsWorld,
"setSize", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsDVec ); // size
}
void deClassWorld::nfSetSize::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld *world = ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world;
	deClassWorld *clsWorld = ( deClassWorld* )GetOwnerClass();
	
	// check arguments
	dsRealObject *object = rt->GetValue( 0 )->GetRealObject();
	if( ! object ) DSTHROW( dueNullPointer );
	
	// set size
	world->SetSize( clsWorld->GetClassDVector()->GetDVector( object ) );
}



// Cameras
/////////////

// public func int getCameraCount()
deClassWorld::nfGetCameraCount::nfGetCameraCount( const sInitData &init ) :
dsFunction( init.clsWorld, "getCameraCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassWorld::nfGetCameraCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deWorld &world = *( ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world );
	rt->PushInt( world.GetCameraCount() );
}

// public func void addCamera( Camera camera )
deClassWorld::nfAddCamera::nfAddCamera( const sInitData &init ) : dsFunction(init.clsWorld,
"addCamera", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsCam ); // camera
}
void deClassWorld::nfAddCamera::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld *world = ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world;
	deClassWorld *clsWorld = ( deClassWorld* )GetOwnerClass();
	
	dsRealObject *object = rt->GetValue( 0 )->GetRealObject();
	if( ! object ) DSTHROW( dueNullPointer );
	
	world->AddCamera( clsWorld->GetClassCamera()->GetCamera( object ) );
}

// public func void removeCamera(Camera camera)
deClassWorld::nfRemoveCamera::nfRemoveCamera( const sInitData &init ) : dsFunction(init.clsWorld,
"removeCamera", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsCam); // camera
}
void deClassWorld::nfRemoveCamera::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld *world = ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world;
	deClassWorld *clsWorld = ( deClassWorld* )GetOwnerClass();
	
	dsRealObject *object = rt->GetValue(0)->GetRealObject();
	if( ! object ) DSTHROW( dueNullPointer );
	
	world->RemoveCamera( clsWorld->GetClassCamera()->GetCamera( object ) );
}

// public func void removeAllCameras()
deClassWorld::nfRemoveAllCameras::nfRemoveAllCameras( const sInitData &init ) : dsFunction(init.clsWorld,
"removeAllCameras", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassWorld::nfRemoveAllCameras::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld *world = ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world;
	
	world->RemoveAllCameras();
}



// Speakers
/////////////

// public func int getSpeakerCount()
deClassWorld::nfGetSpeakerCount::nfGetSpeakerCount( const sInitData &init ) :
dsFunction( init.clsWorld, "getSpeakerCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassWorld::nfGetSpeakerCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deWorld &world = *( ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world );
	rt->PushInt( world.GetSpeakerCount() );
}

// public func void addSpeaker( Speaker speaker )
deClassWorld::nfAddSpeaker::nfAddSpeaker( const sInitData &init ) : dsFunction(init.clsWorld,
"addSpeaker", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsSpk ); // speaker
}
void deClassWorld::nfAddSpeaker::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld *world = ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world;
	deClassWorld *clsWorld = ( deClassWorld* )GetOwnerClass();
	
	dsRealObject *object = rt->GetValue( 0 )->GetRealObject();
	if( ! object ) DSTHROW( dueNullPointer );
	
	world->AddSpeaker( clsWorld->GetClassSpeaker()->GetSpeaker( object ) );
}

// public func void removeSpeaker(Speaker speaker)
deClassWorld::nfRemoveSpeaker::nfRemoveSpeaker( const sInitData &init ) : dsFunction(init.clsWorld,
"removeSpeaker", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter( init.clsSpk ); // speaker
}
void deClassWorld::nfRemoveSpeaker::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld *world = ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world;
	deClassWorld *clsWorld = ( deClassWorld* )GetOwnerClass();
	
	dsRealObject *object = rt->GetValue(0)->GetRealObject();
	if( ! object ) DSTHROW( dueNullPointer );
	
	world->RemoveSpeaker( clsWorld->GetClassSpeaker()->GetSpeaker( object ) );
}

// public func void removeAllSpeakers()
deClassWorld::nfRemoveAllSpeakers::nfRemoveAllSpeakers( const sInitData &init ) : dsFunction(init.clsWorld,
"removeAllSpeakers", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassWorld::nfRemoveAllSpeakers::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld *world = ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world;
	
	world->RemoveAllSpeakers();
}



// Microphones
////////////////

// public func int getMicrophoneCount()
deClassWorld::nfGetMicrophoneCount::nfGetMicrophoneCount( const sInitData &init ) :
dsFunction( init.clsWorld, "getMicrophoneCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassWorld::nfGetMicrophoneCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deWorld &world = *( ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world );
	rt->PushInt( world.GetMicrophoneCount() );
}

// public func void addMicrophone( Microphone microphone )
deClassWorld::nfAddMicrophone::nfAddMicrophone( const sInitData &init ) : dsFunction(init.clsWorld,
"addMicrophone", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsMic ); // microphone
}
void deClassWorld::nfAddMicrophone::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld *world = ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world;
	deClassWorld *clsWorld = ( deClassWorld* )GetOwnerClass();
	
	dsRealObject *object = rt->GetValue( 0 )->GetRealObject();
	if( ! object ) DSTHROW( dueNullPointer );
	
	world->AddMicrophone( clsWorld->GetClassMicrophone()->GetMicrophone( object ) );
}

// public func void removeMicrophone(Microphone microphone)
deClassWorld::nfRemoveMicrophone::nfRemoveMicrophone( const sInitData &init ) : dsFunction(init.clsWorld,
"removeMicrophone", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter( init.clsMic ); // microphone
}
void deClassWorld::nfRemoveMicrophone::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld *world = ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world;
	deClassWorld *clsWorld = ( deClassWorld* )GetOwnerClass();
	
	dsRealObject *object = rt->GetValue(0)->GetRealObject();
	if( ! object ) DSTHROW( dueNullPointer );
	
	world->RemoveMicrophone( clsWorld->GetClassMicrophone()->GetMicrophone( object ) );
}

// public func void removeAllMicrophones()
deClassWorld::nfRemoveAllMicrophones::nfRemoveAllMicrophones( const sInitData &init ) : dsFunction(init.clsWorld,
"removeAllMicrophones", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassWorld::nfRemoveAllMicrophones::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld *world = ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world;
	
	world->RemoveAllMicrophones();
}



// NetworkStates
//////////////////

// public func int getNetworkStateCount()
deClassWorld::nfGetNetworkStateCount::nfGetNetworkStateCount( const sInitData &init ) :
dsFunction( init.clsWorld, "getNetworkStateCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassWorld::nfGetNetworkStateCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deWorld &world = *( ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world );
	rt->PushInt( world.GetNetworkStateCount() );
}

// public func void addNetworkState( NetworkState networkState )
deClassWorld::nfAddNetworkState::nfAddNetworkState( const sInitData &init ) : dsFunction(init.clsWorld,
"addNetworkState", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsNS ); // networkState
}
void deClassWorld::nfAddNetworkState::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld *world = ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world;
	deClassWorld *clsWorld = ( deClassWorld* )GetOwnerClass();
	
	dsRealObject *object = rt->GetValue( 0 )->GetRealObject();
	if( ! object ) DSTHROW( dueNullPointer );
	
	world->AddNetworkState( clsWorld->GetClassNetworkState()->GetNetworkState( object ) );
}

// public func void removeNetworkState(NetworkState networkState)
deClassWorld::nfRemoveNetworkState::nfRemoveNetworkState( const sInitData &init ) : dsFunction(init.clsWorld,
"removeNetworkState", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter( init.clsNS ); // networkState
}
void deClassWorld::nfRemoveNetworkState::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld *world = ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world;
	deClassWorld *clsWorld = ( deClassWorld* )GetOwnerClass();
	
	dsRealObject *object = rt->GetValue(0)->GetRealObject();
	if( ! object ) DSTHROW( dueNullPointer );
	
	world->RemoveNetworkState( clsWorld->GetClassNetworkState()->GetNetworkState( object ) );
}

// public func void removeAllNetworkStates()
deClassWorld::nfRemoveAllNetworkStates::nfRemoveAllNetworkStates( const sInitData &init ) : dsFunction(init.clsWorld,
"removeAllNetworkStates", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassWorld::nfRemoveAllNetworkStates::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld *world = ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world;
	
	world->RemoveAllNetworkStates();
}



// Components
///////////////

// public func int getComponentCount()
deClassWorld::nfGetComponentCount::nfGetComponentCount( const sInitData &init ) :
dsFunction( init.clsWorld, "getComponentCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassWorld::nfGetComponentCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deWorld &world = *( ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world );
	rt->PushInt( world.GetComponentCount() );
}

// public func void addComponent(Component comp)
deClassWorld::nfAddComponent::nfAddComponent( const sInitData &init ) : dsFunction(init.clsWorld,
"addComponent", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsComp); // comp
}
void deClassWorld::nfAddComponent::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld *world = ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world;
	deClassWorld *clsWorld = ( deClassWorld* )GetOwnerClass();
	
	dsRealObject *object = rt->GetValue(0)->GetRealObject();
	if( ! object ) DSTHROW( dueNullPointer );
	
	world->AddComponent( clsWorld->GetClassComponent()->GetComponent(object) );
}

// public func void removeComponent(Component comp)
deClassWorld::nfRemoveComponent::nfRemoveComponent( const sInitData &init ) : dsFunction(init.clsWorld,
"removeComponent", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsComp); // comp
}
void deClassWorld::nfRemoveComponent::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld *world = ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world;
	deClassWorld *clsWorld = ( deClassWorld* )GetOwnerClass();
	
	dsRealObject *object = rt->GetValue(0)->GetRealObject();
	if( ! object ) DSTHROW( dueNullPointer );
	
	SPECIAL_DEBUG_TIMING_START(rt, this)
	world->RemoveComponent( clsWorld->GetClassComponent()->GetComponent(object) );
	SPECIAL_DEBUG_TIMING_CAPTURE(rt, 0)
}

// public func void removeAllComponents()
deClassWorld::nfRemoveAllComponents::nfRemoveAllComponents( const sInitData &init ) : dsFunction(init.clsWorld,
"removeAllComponents", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassWorld::nfRemoveAllComponents::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld *world = ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world;
	
	world->RemoveAllComponents();
}



// Lights
///////////

// public func int getLightCount()
deClassWorld::nfGetLightCount::nfGetLightCount( const sInitData &init ) :
dsFunction( init.clsWorld, "getLightCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassWorld::nfGetLightCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deWorld &world = *( ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world );
	rt->PushInt( world.GetLightCount() );
}

// public func void addLight(Light light)
deClassWorld::nfAddLight::nfAddLight( const sInitData &init ) : dsFunction(init.clsWorld,
"addLight", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsLig); // light
}
void deClassWorld::nfAddLight::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld *world = ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world;
	deClassWorld *clsWorld = ( deClassWorld* )GetOwnerClass();
	
	dsRealObject *object = rt->GetValue(0)->GetRealObject();
	if( ! object ) DSTHROW( dueNullPointer );
	
	world->AddLight( clsWorld->GetClassLight()->GetLight(object) );
}

// public func void removeLight(Light light)
deClassWorld::nfRemoveLight::nfRemoveLight( const sInitData &init ) : dsFunction(init.clsWorld,
"removeLight", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsLig); // light
}
void deClassWorld::nfRemoveLight::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld *world = ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world;
	deClassWorld *clsWorld = ( deClassWorld* )GetOwnerClass();
	
	dsRealObject *object = rt->GetValue(0)->GetRealObject();
	if( ! object ) DSTHROW( dueNullPointer );
	
	SPECIAL_DEBUG_TIMING_START(rt, this)
	world->RemoveLight( clsWorld->GetClassLight()->GetLight(object) );
	SPECIAL_DEBUG_TIMING_CAPTURE(rt, 1)
}

// public func void removeAllLights()
deClassWorld::nfRemoveAllLights::nfRemoveAllLights( const sInitData &init ) : dsFunction(init.clsWorld,
"removeAllLights", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassWorld::nfRemoveAllLights::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld *world = ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world;
	
	world->RemoveAllLights();
}



// Colliders
//////////////

// public func int getColliderCount()
deClassWorld::nfGetColliderCount::nfGetColliderCount( const sInitData &init ) :
dsFunction( init.clsWorld, "getColliderCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassWorld::nfGetColliderCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deWorld &world = *( ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world );
	rt->PushInt( world.GetColliderCount() );
}

// public func void addCollider(Collider collider)
deClassWorld::nfAddCollider::nfAddCollider( const sInitData &init ) : dsFunction(init.clsWorld,
"addCollider", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsCol); // collider
}
void deClassWorld::nfAddCollider::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld *world = ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world;
	deClassWorld *clsWorld = ( deClassWorld* )GetOwnerClass();
	
	dsRealObject *object = rt->GetValue(0)->GetRealObject();
	if( ! object ) DSTHROW( dueNullPointer );
	
	world->AddCollider( clsWorld->GetClassCollider()->GetCollider(object) );
}

// public func void removeCollider(Collider collider)
deClassWorld::nfRemoveCollider::nfRemoveCollider( const sInitData &init ) : dsFunction(init.clsWorld,
"removeCollider", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsCol); // collider
}
void deClassWorld::nfRemoveCollider::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld *world = ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world;
	deClassWorld *clsWorld = ( deClassWorld* )GetOwnerClass();
	
	dsRealObject *object = rt->GetValue(0)->GetRealObject();
	if( ! object ) DSTHROW( dueNullPointer );
	
	SPECIAL_DEBUG_TIMING_START(rt, this)
	world->RemoveCollider( clsWorld->GetClassCollider()->GetCollider(object) );
	SPECIAL_DEBUG_TIMING_CAPTURE(rt, 2)
}

// public func void removeAllColliders()
deClassWorld::nfRemoveAllColliders::nfRemoveAllColliders( const sInitData &init ) : dsFunction(init.clsWorld,
"removeAllColliders", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassWorld::nfRemoveAllColliders::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld *world = ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world;
	
	world->RemoveAllColliders();
}



// Lumimeters
///////////////

// public func int getLumimeterCount()
deClassWorld::nfGetLumimeterCount::nfGetLumimeterCount( const sInitData &init ) :
dsFunction( init.clsWorld, "getLumimeterCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassWorld::nfGetLumimeterCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deWorld &world = *( ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world );
	rt->PushInt( world.GetLumimeterCount() );
}

// public func void addLumimeter( Lumimeter lumimeter )
deClassWorld::nfAddLumimeter::nfAddLumimeter( const sInitData &init ) : dsFunction(init.clsWorld,
"addLumimeter", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsLum ); // lumimeter
}
void deClassWorld::nfAddLumimeter::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld *world = ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world;
	deScriptingDragonScript &ds = *( ( ( deClassWorld* )GetOwnerClass() )->GetDS() );
	
	dsRealObject *object = rt->GetValue( 0 )->GetRealObject();
	if( ! object ) DSTHROW( dueNullPointer );
	
	world->AddLumimeter( ds.GetClassLumimeter()->GetLumimeter( object ) );
}

// public func void removeLumimeter(Lumimeter lumimeter)
deClassWorld::nfRemoveLumimeter::nfRemoveLumimeter( const sInitData &init ) : dsFunction(init.clsWorld,
"removeLumimeter", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter( init.clsLum ); // lumimeter
}
void deClassWorld::nfRemoveLumimeter::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld *world = ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world;
	deScriptingDragonScript &ds = *( ( ( deClassWorld* )GetOwnerClass() )->GetDS() );
	
	dsRealObject *object = rt->GetValue(0)->GetRealObject();
	if( ! object ) DSTHROW( dueNullPointer );
	
	world->RemoveLumimeter( ds.GetClassLumimeter()->GetLumimeter( object ) );
}

// public func void removeAllLumimeters()
deClassWorld::nfRemoveAllLumimeters::nfRemoveAllLumimeters( const sInitData &init ) : dsFunction(init.clsWorld,
"removeAllLumimeters", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassWorld::nfRemoveAllLumimeters::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld *world = ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world;
	
	world->RemoveAllLumimeters();
}



// TouchSensors
/////////////////

// public func int getTouchSensorCount()
deClassWorld::nfGetTouchSensorCount::nfGetTouchSensorCount( const sInitData &init ) :
dsFunction( init.clsWorld, "getTouchSensorCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassWorld::nfGetTouchSensorCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deWorld &world = *( ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world );
	rt->PushInt( world.GetTouchSensorCount() );
}

// public func void addTouchSensor( TouchSensor touchSensor )
deClassWorld::nfAddTouchSensor::nfAddTouchSensor( const sInitData &init ) : dsFunction(init.clsWorld,
"addTouchSensor", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsTS ); // touchSensor
}
void deClassWorld::nfAddTouchSensor::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld *world = ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world;
	deClassWorld *clsWorld = ( deClassWorld* )GetOwnerClass();
	
	dsRealObject *object = rt->GetValue( 0 )->GetRealObject();
	if( ! object ) DSTHROW( dueNullPointer );
	
	world->AddTouchSensor( clsWorld->GetClassTouchSensor()->GetTouchSensor( object ) );
}

// public func void removeTouchSensor(TouchSensor touchSensor)
deClassWorld::nfRemoveTouchSensor::nfRemoveTouchSensor( const sInitData &init ) : dsFunction(init.clsWorld,
"removeTouchSensor", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter( init.clsTS ); // touchSensor
}
void deClassWorld::nfRemoveTouchSensor::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld *world = ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world;
	deClassWorld *clsWorld = ( deClassWorld* )GetOwnerClass();
	
	dsRealObject *object = rt->GetValue(0)->GetRealObject();
	if( ! object ) DSTHROW( dueNullPointer );
	
	SPECIAL_DEBUG_TIMING_START(rt, this)
	world->RemoveTouchSensor( clsWorld->GetClassTouchSensor()->GetTouchSensor( object ) );
	SPECIAL_DEBUG_TIMING_CAPTURE(rt, 3)
}

// public func void removeAllTouchSensors()
deClassWorld::nfRemoveAllTouchSensors::nfRemoveAllTouchSensors( const sInitData &init ) : dsFunction(init.clsWorld,
"removeAllTouchSensors", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassWorld::nfRemoveAllTouchSensors::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld *world = ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world;
	
	world->RemoveAllTouchSensors();
}



// Prop Fields
////////////////

// public func int getPropFieldCount()
deClassWorld::nfGetPropFieldCount::nfGetPropFieldCount( const sInitData &init ) :
dsFunction( init.clsWorld, "getPropFieldCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassWorld::nfGetPropFieldCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deWorld &world = *( ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world );
	rt->PushInt( world.GetPropFieldCount() );
}

// public func void addPropField( PropField propfield )
deClassWorld::nfAddPropField::nfAddPropField( const sInitData &init ) : dsFunction(init.clsWorld,
"addPropField", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsPF ); // propfield
}
void deClassWorld::nfAddPropField::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld *world = ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world;
	deScriptingDragonScript *ds = ( ( deClassWorld* )GetOwnerClass() )->GetDS();
	
	dsRealObject *objPF = rt->GetValue( 0 )->GetRealObject();
	if( ! objPF ) DSTHROW( dueNullPointer );
	
	world->AddPropField( ds->GetClassPropField()->GetPropField( objPF ) );
}

// public func void removePropField( PropField propfield )
deClassWorld::nfRemovePropField::nfRemovePropField( const sInitData &init ) : dsFunction(init.clsWorld,
"removePropField", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter( init.clsPF ); // propfield
}
void deClassWorld::nfRemovePropField::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld *world = ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world;
	deScriptingDragonScript *ds = ( ( deClassWorld* )GetOwnerClass() )->GetDS();
	
	dsRealObject *objPF = rt->GetValue( 0 )->GetRealObject();
	if( ! objPF ) DSTHROW( dueNullPointer );
	
	world->RemovePropField( ds->GetClassPropField()->GetPropField( objPF ) );
}

// public func void removeAllPropFields()
deClassWorld::nfRemoveAllPropFields::nfRemoveAllPropFields( const sInitData &init ) : dsFunction(init.clsWorld,
"removeAllPropFields", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassWorld::nfRemoveAllPropFields::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld *world = ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world;
	
	world->RemoveAllPropFields();
}



// Force Fields
/////////////////

// public func int getForceFieldCount()
deClassWorld::nfGetForceFieldCount::nfGetForceFieldCount( const sInitData &init ) :
dsFunction( init.clsWorld, "getForceFieldCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassWorld::nfGetForceFieldCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deWorld &world = *( ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world );
	rt->PushInt( world.GetForceFieldCount() );
}

// public func void addForceField( ForceField forcefield )
deClassWorld::nfAddForceField::nfAddForceField( const sInitData &init ) : dsFunction(init.clsWorld,
"addForceField", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFF ); // forcefield
}
void deClassWorld::nfAddForceField::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld *world = ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world;
	deScriptingDragonScript *ds = ( ( deClassWorld* )GetOwnerClass() )->GetDS();
	
	dsRealObject *objPF = rt->GetValue( 0 )->GetRealObject();
	if( ! objPF ) DSTHROW( dueNullPointer );
	
	world->AddForceField( ds->GetClassForceField()->GetForceField( objPF ) );
}

// public func void removeForceField( ForceField forcefield )
deClassWorld::nfRemoveForceField::nfRemoveForceField( const sInitData &init ) : dsFunction(init.clsWorld,
"removeForceField", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter( init.clsFF ); // forcefield
}
void deClassWorld::nfRemoveForceField::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld *world = ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world;
	deScriptingDragonScript *ds = ( ( deClassWorld* )GetOwnerClass() )->GetDS();
	
	dsRealObject *objPF = rt->GetValue( 0 )->GetRealObject();
	if( ! objPF ) DSTHROW( dueNullPointer );
	
	world->RemoveForceField( ds->GetClassForceField()->GetForceField( objPF ) );
}

// public func void removeAllForceFields()
deClassWorld::nfRemoveAllForceFields::nfRemoveAllForceFields( const sInitData &init ) : dsFunction(init.clsWorld,
"removeAllForceFields", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassWorld::nfRemoveAllForceFields::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld *world = ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world;
	
	world->RemoveAllForceFields();
}



// Particle Emitters
//////////////////////

// public func int getParticleEmitterCount()
deClassWorld::nfGetParticleEmitterCount::nfGetParticleEmitterCount( const sInitData &init ) :
dsFunction( init.clsWorld, "getParticleEmitterCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassWorld::nfGetParticleEmitterCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deWorld &world = *( ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world );
	rt->PushInt( world.GetParticleEmitterCount() );
}

// public func void addParticleEmitter( ParticleEmitterInstance emitter )
deClassWorld::nfAddParticleEmitter::nfAddParticleEmitter( const sInitData &init ) : dsFunction( init.clsWorld,
"addParticleEmitter", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsPEI ); // emitter
}
void deClassWorld::nfAddParticleEmitter::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld &world = *( ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world );
	const deScriptingDragonScript &ds = *( ( ( deClassWorld* )GetOwnerClass() )->GetDS() );
	deParticleEmitterInstance *emitter;
	
	emitter = ds.GetClassParticleEmitterInstance()->GetInstance( rt->GetValue( 0 )->GetRealObject() );
	if( ! emitter ){
		DSTHROW( dueNullPointer );
	}
	
	world.AddParticleEmitter( emitter );
}

// public func void removeParticleEmitter( ParticleEmitterInstance emitter )
deClassWorld::nfRemoveParticleEmitter::nfRemoveParticleEmitter( const sInitData &init ) : dsFunction( init.clsWorld,
"removeParticleEmitter", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsPEI ); // emitter
}
void deClassWorld::nfRemoveParticleEmitter::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld &world = *( ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world );
	const deScriptingDragonScript &ds = *( ( ( deClassWorld* )GetOwnerClass() )->GetDS() );
	deParticleEmitterInstance *emitter;
	
	emitter = ds.GetClassParticleEmitterInstance()->GetInstance( rt->GetValue( 0 )->GetRealObject() );
	if( ! emitter ){
		DSTHROW( dueNullPointer );
	}
	
	world.RemoveParticleEmitter( emitter );
}

// public func void removeAllParticleEmitters()
deClassWorld::nfRemoveAllParticleEmitters::nfRemoveAllParticleEmitters( const sInitData &init ) : dsFunction( init.clsWorld,
"removeAllParticleEmitters", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassWorld::nfRemoveAllParticleEmitters::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld &world = *( ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world );
	
	world.RemoveAllParticleEmitters();
}



// Debug Drawers
//////////////////

// public func int getDebugDrawerCount()
deClassWorld::nfGetDebugDrawerCount::nfGetDebugDrawerCount( const sInitData &init ) :
dsFunction( init.clsWorld, "getDebugDrawerCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassWorld::nfGetDebugDrawerCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deWorld &world = *( ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world );
	rt->PushInt( world.GetDebugDrawerCount() );
}

// public func void addDebugDrawer( DebugDrawer debugDrawer )
deClassWorld::nfAddDebugDrawer::nfAddDebugDrawer( const sInitData &init ) : dsFunction(init.clsWorld,
"addDebugDrawer", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsDD ); // debugDrawer
}
void deClassWorld::nfAddDebugDrawer::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld &world = *( ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world );
	const deScriptingDragonScript &ds = *( ( ( deClassWorld* )GetOwnerClass() )->GetDS() );
	deDebugDrawer * const ddrawer = ds.GetClassDebugDrawer()->GetDebugDrawer( rt->GetValue( 0 )->GetRealObject() );
	
	world.AddDebugDrawer( ddrawer );
}

// public func void removeDebugDrawer( DebugDrawer debugDrawer )
deClassWorld::nfRemoveDebugDrawer::nfRemoveDebugDrawer( const sInitData &init ) : dsFunction(init.clsWorld,
"removeDebugDrawer", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter( init.clsDD ); // debugDrawer
}
void deClassWorld::nfRemoveDebugDrawer::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld &world = *( ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world );
	const deScriptingDragonScript &ds = *( ( ( deClassWorld* )GetOwnerClass() )->GetDS() );
	deDebugDrawer * const ddrawer = ds.GetClassDebugDrawer()->GetDebugDrawer( rt->GetValue( 0 )->GetRealObject() );
	
	world.RemoveDebugDrawer( ddrawer );
}

// public func void removeAllDebugDrawers()
deClassWorld::nfRemoveAllDebugDrawers::nfRemoveAllDebugDrawers( const sInitData &init ) : dsFunction(init.clsWorld,
"removeAllDebugDrawers", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassWorld::nfRemoveAllDebugDrawers::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld &world = *( ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world );
	
	world.RemoveAllDebugDrawers();
}



// Navigation Spaces
//////////////////////

// public func int getNavigationSpaceCount()
deClassWorld::nfGetNavigationSpaceCount::nfGetNavigationSpaceCount( const sInitData &init ) :
dsFunction( init.clsWorld, "getNavigationSpaceCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassWorld::nfGetNavigationSpaceCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deWorld &world = *( ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world );
	rt->PushInt( world.GetNavigationSpaceCount() );
}

// public func void addNavigationSpace( NavigationSpace navspace )
deClassWorld::nfAddNavigationSpace::nfAddNavigationSpace( const sInitData &init ) : dsFunction(init.clsWorld,
"addNavigationSpace", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsNavSpace ); // navspace
}
void deClassWorld::nfAddNavigationSpace::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld *world = ( ( const sWorldNatDat * )p_GetNativeData( myself ) )->world;
	const deScriptingDragonScript &ds = *( ( ( const deClassWorld * )GetOwnerClass() )->GetDS() );
	deNavigationSpace *navspace = ds.GetClassNavigationSpace()->GetNavigationSpace( rt->GetValue( 0 )->GetRealObject() );
	
	if( ! navspace ){
		DSTHROW( dueNullPointer );
	}
	
	if( ! navspace->GetParentWorld() ){
		world->AddNavigationSpace( navspace );
	}
}

// public func void removeNavigationSpace( NavigationSpace navspace )
deClassWorld::nfRemoveNavigationSpace::nfRemoveNavigationSpace( const sInitData &init ) : dsFunction(init.clsWorld,
"removeNavigationSpace", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter( init.clsNavSpace ); // navspace
}
void deClassWorld::nfRemoveNavigationSpace::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld *world = ( ( const sWorldNatDat * )p_GetNativeData( myself ) )->world;
	const deScriptingDragonScript &ds = *( ( ( const deClassWorld * )GetOwnerClass() )->GetDS() );
	deNavigationSpace *navspace = ds.GetClassNavigationSpace()->GetNavigationSpace( rt->GetValue( 0 )->GetRealObject() );
	
	if( ! navspace ){
		DSTHROW( dueNullPointer );
	}
	
	SPECIAL_DEBUG_TIMING_START(rt, this)
	if( navspace->GetParentWorld() == world ){
		world->RemoveNavigationSpace( navspace );
	}
	SPECIAL_DEBUG_TIMING_CAPTURE(rt, 4)
}

// public func void removeAllNavigationSpaces()
deClassWorld::nfRemoveAllNavigationSpaces::nfRemoveAllNavigationSpaces( const sInitData &init ) : dsFunction(init.clsWorld,
"removeAllNavigationSpaces", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassWorld::nfRemoveAllNavigationSpaces::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld &world = *( ( ( const sWorldNatDat * )p_GetNativeData( myself ) )->world );
	
	world.RemoveAllNavigationSpaces();
}



// Navigation Blockers
////////////////////////

// public func int getNavigationBlockerCount()
deClassWorld::nfGetNavigationBlockerCount::nfGetNavigationBlockerCount( const sInitData &init ) :
dsFunction( init.clsWorld, "getNavigationBlockerCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassWorld::nfGetNavigationBlockerCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deWorld &world = *( ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world );
	rt->PushInt( world.GetNavigationBlockerCount() );
}

// public func void addNavigationBlocker( NavigationBlocker navblocker )
deClassWorld::nfAddNavigationBlocker::nfAddNavigationBlocker( const sInitData &init ) : dsFunction(init.clsWorld,
"addNavigationBlocker", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsNavBlocker ); // navblocker
}
void deClassWorld::nfAddNavigationBlocker::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld *world = ( ( const sWorldNatDat * )p_GetNativeData( myself ) )->world;
	const deScriptingDragonScript &ds = *( ( ( const deClassWorld * )GetOwnerClass() )->GetDS() );
	deNavigationBlocker *navblocker = ds.GetClassNavigationBlocker()->GetNavigationBlocker( rt->GetValue( 0 )->GetRealObject() );
	
	if( ! navblocker ){
		DSTHROW( dueNullPointer );
	}
	
	if( ! navblocker->GetParentWorld() ){
		world->AddNavigationBlocker( navblocker );
	}
}

// public func void removeNavigationBlocker( NavigationBlocker navblocker )
deClassWorld::nfRemoveNavigationBlocker::nfRemoveNavigationBlocker( const sInitData &init ) : dsFunction(init.clsWorld,
"removeNavigationBlocker", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter( init.clsNavBlocker ); // navblocker
}
void deClassWorld::nfRemoveNavigationBlocker::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld *world = ( ( const sWorldNatDat * )p_GetNativeData( myself ) )->world;
	const deScriptingDragonScript &ds = *( ( ( const deClassWorld * )GetOwnerClass() )->GetDS() );
	deNavigationBlocker *navblocker = ds.GetClassNavigationBlocker()->GetNavigationBlocker( rt->GetValue( 0 )->GetRealObject() );
	
	if( ! navblocker ){
		DSTHROW( dueNullPointer );
	}
	
	SPECIAL_DEBUG_TIMING_START(rt, this)
	if( navblocker->GetParentWorld() == world ){
		world->RemoveNavigationBlocker( navblocker );
	}
	SPECIAL_DEBUG_TIMING_CAPTURE(rt, 5)
}

// public func void removeAllNavigationBlockers()
deClassWorld::nfRemoveAllNavigationBlockers::nfRemoveAllNavigationBlockers( const sInitData &init ) : dsFunction(init.clsWorld,
"removeAllNavigationBlockers", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassWorld::nfRemoveAllNavigationBlockers::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld &world = *( ( ( const sWorldNatDat * )p_GetNativeData( myself ) )->world );
	
	world.RemoveAllNavigationBlockers();
}



// Navigators
///////////////

// public func int getNavigatorCount()
deClassWorld::nfGetNavigatorCount::nfGetNavigatorCount( const sInitData &init ) :
dsFunction( init.clsWorld, "getNavigatorCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassWorld::nfGetNavigatorCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deWorld &world = *( ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world );
	rt->PushInt( world.GetNavigatorCount() );
}

// public func void addNavigator( Navigator navigator )
deClassWorld::nfAddNavigator::nfAddNavigator( const sInitData &init ) : dsFunction( init.clsWorld,
"addNavigator", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsNavigator ); // navigator
}
void deClassWorld::nfAddNavigator::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld *world = ( ( const sWorldNatDat * )p_GetNativeData( myself ) )->world;
	const deScriptingDragonScript &ds = *( ( ( const deClassWorld * )GetOwnerClass() )->GetDS() );
	deNavigator *navigator = ds.GetClassNavigator()->GetNavigator( rt->GetValue( 0 )->GetRealObject() );
	
	if( ! navigator ){
		DSTHROW( dueNullPointer );
	}
	
	if( ! navigator->GetParentWorld() ){
		world->AddNavigator( navigator );
	}
}

// public func void removeNavigator( Navigator navigator )
deClassWorld::nfRemoveNavigator::nfRemoveNavigator( const sInitData &init ) : dsFunction( init.clsWorld,
"removeNavigator", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsNavigator ); // navigator
}
void deClassWorld::nfRemoveNavigator::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld *world = ( ( const sWorldNatDat * )p_GetNativeData( myself ) )->world;
	const deScriptingDragonScript &ds = *( ( ( const deClassWorld * )GetOwnerClass() )->GetDS() );
	deNavigator *navigator = ds.GetClassNavigator()->GetNavigator( rt->GetValue( 0 )->GetRealObject() );
	
	if( ! navigator ){
		DSTHROW( dueNullPointer );
	}
	
	if( navigator->GetParentWorld() == world ){
		world->RemoveNavigator( navigator );
	}
}

// public func void removeAllNavigators()
deClassWorld::nfRemoveAllNavigators::nfRemoveAllNavigators( const sInitData &init ) : dsFunction( init.clsWorld,
"removeAllNavigators", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassWorld::nfRemoveAllNavigators::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld &world = *( ( ( const sWorldNatDat * )p_GetNativeData( myself ) )->world );
	
	world.RemoveAllNavigators();
}



// Environment map probes
///////////////////////////

// public func int getEnvMapProbeCount()
deClassWorld::nfGetEnvMapProbeCount::nfGetEnvMapProbeCount( const sInitData &init ) :
dsFunction( init.clsWorld, "getEnvMapProbeCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassWorld::nfGetEnvMapProbeCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deWorld &world = *( ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world );
	rt->PushInt( world.GetEnvMapProbeCount() );
}

// public func void addEnvMapProbe( EnvMapProbe envMapProbe )
deClassWorld::nfAddEnvMapProbe::nfAddEnvMapProbe( const sInitData &init ) : dsFunction(init.clsWorld,
"addEnvMapProbe", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsEmp ); // envMapProbe
}
void deClassWorld::nfAddEnvMapProbe::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld *world = ( ( const sWorldNatDat * )p_GetNativeData( myself ) )->world;
	const deScriptingDragonScript &ds = *( ( ( const deClassWorld * )GetOwnerClass() )->GetDS() );
	deEnvMapProbe * const envMapProbe = ds.GetClassEnvMapProbe()->GetEnvMapProbe( rt->GetValue( 0 )->GetRealObject() );
	
	if( ! envMapProbe ){
		DSTHROW( dueNullPointer );
	}
	
	if( ! envMapProbe->GetParentWorld() ){
		world->AddEnvMapProbe( envMapProbe );
	}
}

// public func void removeEnvMapProbe( EnvMapProbe envMapProbe )
deClassWorld::nfRemoveEnvMapProbe::nfRemoveEnvMapProbe( const sInitData &init ) : dsFunction(init.clsWorld,
"removeEnvMapProbe", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter( init.clsEmp ); // envMapProbe
}
void deClassWorld::nfRemoveEnvMapProbe::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld *world = ( ( const sWorldNatDat * )p_GetNativeData( myself ) )->world;
	const deScriptingDragonScript &ds = *( ( ( const deClassWorld * )GetOwnerClass() )->GetDS() );
	deEnvMapProbe * const envMapProbe = ds.GetClassEnvMapProbe()->GetEnvMapProbe( rt->GetValue( 0 )->GetRealObject() );
	
	if( ! envMapProbe ){
		DSTHROW( dueNullPointer );
	}
	
	SPECIAL_DEBUG_TIMING_START(rt, this)
	if( envMapProbe->GetParentWorld() == world ){
		world->RemoveEnvMapProbe( envMapProbe );
	}
	SPECIAL_DEBUG_TIMING_CAPTURE(rt, 6)
}

// public func void removeAllEnvMapProbes()
deClassWorld::nfRemoveAllEnvMapProbes::nfRemoveAllEnvMapProbes( const sInitData &init ) : dsFunction(init.clsWorld,
"removeAllEnvMapProbes", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassWorld::nfRemoveAllEnvMapProbes::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld &world = *( ( ( const sWorldNatDat * )p_GetNativeData( myself ) )->world );
	
	world.RemoveAllEnvMapProbes();
}



// Sound level meters
///////////////////////

// public func int getSoundLevelMeterCount()
deClassWorld::nfGetSoundLevelMeterCount::nfGetSoundLevelMeterCount( const sInitData &init ) :
dsFunction( init.clsWorld, "getSoundLevelMeterCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassWorld::nfGetSoundLevelMeterCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deWorld &world = *( ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world );
	rt->PushInt( world.GetSoundLevelMeterCount() );
}

// public func void addSoundLevelMeter( SoundLevelMeter soundLevelMeter )
deClassWorld::nfAddSoundLevelMeter::nfAddSoundLevelMeter( const sInitData &init ) :
dsFunction(init.clsWorld, "addSoundLevelMeter", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsSoundLevelMeter ); // soundLevelMeter
}
void deClassWorld::nfAddSoundLevelMeter::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld &world = *( ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world );
	const deScriptingDragonScript &ds = *( ( deClassWorld* )GetOwnerClass() )->GetDS();
	
	world.AddSoundLevelMeter( ds.GetClassSoundLevelMeter()
		->GetSoundLevelMeter( rt->GetValue( 0 )->GetRealObject() ) );
}

// public func void removeSoundLevelMeter(SoundLevelMeter soundLevelMeter)
deClassWorld::nfRemoveSoundLevelMeter::nfRemoveSoundLevelMeter( const sInitData &init ) :
dsFunction( init.clsWorld, "removeSoundLevelMeter", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsSoundLevelMeter ); // soundLevelMeter
}
void deClassWorld::nfRemoveSoundLevelMeter::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld &world = *( ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world );
	deScriptingDragonScript &ds = *( ( deClassWorld* )GetOwnerClass() )->GetDS();
	
	world.RemoveSoundLevelMeter( ds.GetClassSoundLevelMeter()
		->GetSoundLevelMeter( rt->GetValue( 0 )->GetRealObject() ) );
}

// public func void removeAllSoundLevelMeters()
deClassWorld::nfRemoveAllSoundLevelMeters::nfRemoveAllSoundLevelMeters( const sInitData &init ) :
dsFunction( init.clsWorld, "removeAllSoundLevelMeters", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassWorld::nfRemoveAllSoundLevelMeters::RunFunction( dsRunTime*, dsValue *myself ){
	deWorld &world = *( ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world );
	
	world.RemoveAllSoundLevelMeters();
}



// Billboards
/////////////

// public func int getBillboardCount()
deClassWorld::nfGetBillboardCount::nfGetBillboardCount( const sInitData &init ) :
dsFunction( init.clsWorld, "getBillboardCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassWorld::nfGetBillboardCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deWorld &world = *( ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world );
	rt->PushInt( world.GetBillboardCount() );
}

// public func void addBillboard( Billboard billboard )
deClassWorld::nfAddBillboard::nfAddBillboard( const sInitData &init ) :
dsFunction( init.clsWorld, "addBillboard", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsBillboard ); // billboard
}
void deClassWorld::nfAddBillboard::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld &world = *( ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world );
	deScriptingDragonScript &ds = *( ( ( deClassWorld* )GetOwnerClass() )->GetDS() );
	
	deBillboard * const billboard = ds.GetClassBillboard()->GetBillboard( rt->GetValue( 0 )->GetRealObject() );
	if( ! billboard ){
		DSTHROW( dueNullPointer );
	}
	
	world.AddBillboard( billboard );
}

// public func void removeBillboard( Billboard billboard )
deClassWorld::nfRemoveBillboard::nfRemoveBillboard( const sInitData &init ) :
dsFunction( init.clsWorld, "removeBillboard", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsBillboard ); // billboard
}
void deClassWorld::nfRemoveBillboard::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld &world = *( ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world );
	deScriptingDragonScript &ds = *( ( ( deClassWorld* )GetOwnerClass() )->GetDS() );
	
	deBillboard * const billboard = ds.GetClassBillboard()->GetBillboard( rt->GetValue( 0 )->GetRealObject() );
	if( ! billboard ){
		DSTHROW( dueNullPointer );
	}
	
	world.RemoveBillboard( billboard );
}

// public func void removeAllBillboards()
deClassWorld::nfRemoveAllBillboards::nfRemoveAllBillboards( const sInitData &init ) :
dsFunction( init.clsWorld, "removeAllBillboards", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassWorld::nfRemoveAllBillboards::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld &world = *( ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world );
	world.RemoveAllBillboards();
}



// Skies
///////////

// public func int getSkyCount()
deClassWorld::nfGetSkyCount::nfGetSkyCount( const sInitData &init ) :
dsFunction( init.clsWorld, "getSkyCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassWorld::nfGetSkyCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deWorld &world = *( ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world );
	rt->PushInt( world.GetSkyCount() );
}

// public func void addSky( SkyInstance sky )
deClassWorld::nfAddSky::nfAddSky( const sInitData &init ) :
dsFunction( init.clsWorld, "addSky", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsSkyInst ); // sky
}
void deClassWorld::nfAddSky::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld *world = ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world;
	deScriptingDragonScript &ds = *( ( ( deClassWorld* )GetOwnerClass() )->GetDS() );
	
	dsRealObject *object = rt->GetValue(0)->GetRealObject();
	if( ! object ) DSTHROW( dueNullPointer );
	
	world->AddSky( ds.GetClassSkyInstance()->GetInstance(object) );
}

// public func void removeSky( SkyInstance sky )
deClassWorld::nfRemoveSky::nfRemoveSky( const sInitData &init ) :
dsFunction( init.clsWorld, "removeSky", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsSkyInst ); // sky
}
void deClassWorld::nfRemoveSky::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld *world = ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world;
	deScriptingDragonScript &ds = *( ( ( deClassWorld* )GetOwnerClass() )->GetDS() );
	
	dsRealObject *object = rt->GetValue(0)->GetRealObject();
	if( ! object ) DSTHROW( dueNullPointer );
	
	SPECIAL_DEBUG_TIMING_START(rt, this)
	world->RemoveSky( ds.GetClassSkyInstance()->GetInstance(object) );
	SPECIAL_DEBUG_TIMING_CAPTURE(rt, 1)
}

// public func void removeAllSkies()
deClassWorld::nfRemoveAllSkies::nfRemoveAllSkies( const sInitData &init ) : dsFunction(init.clsWorld,
"removeAllSkies", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassWorld::nfRemoveAllSkies::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld *world = ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world;
	
	world->RemoveAllSkies();
}



// Physics
////////////

// public func Vector getGravity()
deClassWorld::nfGetGravity::nfGetGravity( const sInitData &init ) : dsFunction( init.clsWorld,
"getGravity", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec ){
}
void deClassWorld::nfGetGravity::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld *world = ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world;
	deClassWorld *clsWorld = ( deClassWorld* )GetOwnerClass();
	
	clsWorld->GetClassVector()->PushVector( rt, world->GetGravity() );
}

// public func void setGravity( Vector gravity )
deClassWorld::nfSetGravity::nfSetGravity( const sInitData &init ) : dsFunction( init.clsWorld,
"setGravity", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsVec ); // gravity
}
void deClassWorld::nfSetGravity::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld *world = ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world;
	deClassWorld *clsWorld = ( deClassWorld* )GetOwnerClass();
	
	dsRealObject *object = rt->GetValue( 0 )->GetRealObject();
	if( ! object ) DSTHROW( dueNullPointer );
	
	world->SetGravity( clsWorld->GetClassVector()->GetVector( object ) );
}



// Lighting
/////////////

// public func bool getDisableLights()
deClassWorld::nfGetDisableLights::nfGetDisableLights(const sInitData &init) : dsFunction( init.clsWorld,
"getDisableLights", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}
void deClassWorld::nfGetDisableLights::RunFunction(dsRunTime *rt, dsValue *myself){
	deWorld *world = ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world;
	
	rt->PushBool( world->GetDisableLights() );
}

// public func void setDisableLights(bool disable)
deClassWorld::nfSetDisableLights::nfSetDisableLights(const sInitData &init) : dsFunction( init.clsWorld,
"setDisableLights", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // disable
}
void deClassWorld::nfSetDisableLights::RunFunction(dsRunTime *rt, dsValue *myself){
	deWorld *world = ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world;
	
	world->SetDisableLights( rt->GetValue(0)->GetBool() );
}

// public func Color getAmbientLight()
deClassWorld::nfGetAmbientLight::nfGetAmbientLight(const sInitData &init) : dsFunction( init.clsWorld,
"getAmbientLight", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsClr){
}
void deClassWorld::nfGetAmbientLight::RunFunction(dsRunTime *rt, dsValue *myself){
	deWorld *world = ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world;
	deClassWorld *clsWorld = (deClassWorld*)GetOwnerClass();
	
	clsWorld->GetClassColor()->PushColor( rt, world->GetAmbientLight() );
}

// public func void setAmbientLight(Color light)
deClassWorld::nfSetAmbientLight::nfSetAmbientLight(const sInitData &init) : dsFunction( init.clsWorld,
"setAmbientLight", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsClr); // color
}
void deClassWorld::nfSetAmbientLight::RunFunction(dsRunTime *rt, dsValue *myself){
	deWorld *world = ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world;
	deClassWorld *clsWorld = (deClassWorld*)GetOwnerClass();
	
	dsRealObject *object = rt->GetValue(0)->GetRealObject();
	if( ! object ) DSTHROW(dueNullPointer);
	
	world->SetAmbientLight( clsWorld->GetClassColor()->GetColor( object ) );
}

// public func Matrix getLightColorMatrix()
deClassWorld::nfGetLightColorMatrix::nfGetLightColorMatrix( const sInitData &init ) : dsFunction( init.clsWorld,
"getLightColorMatrix", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsMat ){
}
void deClassWorld::nfGetLightColorMatrix::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld *world = ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world;
	deClassWorld *clsWorld = (deClassWorld*)GetOwnerClass();
	
	clsWorld->GetClassMatrix()->PushMatrix( rt, world->GetLightColorMatrix() );
}

// public func void setLightColorMatrix( Matrix matrix )
deClassWorld::nfSetLightColorMatrix::nfSetLightColorMatrix( const sInitData &init ) : dsFunction( init.clsWorld,
"setLightColorMatrix", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsMat ); // matrix
}
void deClassWorld::nfSetLightColorMatrix::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld *world = ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world;
	deClassWorld *clsWorld = ( deClassWorld* )GetOwnerClass();
	
	dsRealObject *object = rt->GetValue( 0 )->GetRealObject();
	if( ! object ) DSTHROW( dueNullPointer );
	
	world->SetLightColorMatrix( clsWorld->GetClassMatrix()->GetMatrix( object ) );
}

// public func float getSpeakerGain()
deClassWorld::nfGetSpeakerGain::nfGetSpeakerGain( const sInitData &init ) :
dsFunction( init.clsWorld, "getSpeakerGain", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassWorld::nfGetSpeakerGain::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deWorld &world = *( ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world );
	rt->PushFloat( world.GetSpeakerGain() );
}

// public func void setSpeakerGain(float gain)
deClassWorld::nfSetSpeakerGain::nfSetSpeakerGain(const sInitData &init) :
dsFunction( init.clsWorld, "setSpeakerGain", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFlt ); // gain
}
void deClassWorld::nfSetSpeakerGain::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld &world = *( ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world );
	world.SetSpeakerGain( rt->GetValue( 0 )->GetFloat() );
}



// Height Terrain
///////////////////

// public func HeightTerrain getHeightTerrain()
deClassWorld::nfGetHeightTerrain::nfGetHeightTerrain( const sInitData &init ) : dsFunction( init.clsWorld,
"getHeightTerrain", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsHT ){
}
void deClassWorld::nfGetHeightTerrain::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld *world = ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world;
	deClassWorld *clsWorld = ( deClassWorld* )GetOwnerClass();
	
	if( world->GetHeightTerrain() ){
		clsWorld->GetClassHeightTerrain()->PushHeightTerrain( rt, world->GetHeightTerrain() );
		
	}else{
		rt->PushObject( NULL, clsWorld->GetClassHeightTerrain() );
	}
}

// public func void setHeightTerrain( HeightTerrain heightTerrain )
deClassWorld::nfSetHeightTerrain::nfSetHeightTerrain( const sInitData &init ) : dsFunction( init.clsWorld,
"setHeightTerrain", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsHT ); // heightTerrain
}
void deClassWorld::nfSetHeightTerrain::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld *world = ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world;
	deClassWorld *clsWorld = ( deClassWorld* )GetOwnerClass();
	
	dsRealObject *object = rt->GetValue( 0 )->GetRealObject();
	if( object ){
		world->SetHeightTerrain( clsWorld->GetClassHeightTerrain()->GetHeightTerrain( object ) );
		
	}else{
		world->SetHeightTerrain( NULL );
	}
}



// Collision Detection
////////////////////////

// public func void pointHits( DVector point, ColliderListener listener,
// CollisionFilter collisionFilter )
deClassWorld::nfPointHits::nfPointHits( const sInitData &init ) : dsFunction( init.clsWorld,
"pointHits", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsDVec ); // point
	p_AddParameter( init.clsCLL ); // listener
	p_AddParameter( init.clsCF ); // collisionFilter
}
void deClassWorld::nfPointHits::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld &world = *( ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world );
	const deScriptingDragonScript &ds = *( ( ( deClassWorld* )GetOwnerClass() )->GetDS() );
	
	const decDVector &point = ds.GetClassDVector()->GetDVector(
		rt->GetValue( 0 )->GetRealObject() );
	const decCollisionFilter &collisionFilter = ds.GetClassCollisionFilter()->GetCollisionFilter(
		rt->GetValue( 2 )->GetRealObject() );
	
	dedsColliderListenerAdaptor &adaptor = ds.GetColliderListenerAdaptor();
	adaptor.SetListener( rt->GetValue( 1 )->GetRealObject() );
	adaptor.Reset();
	
	world.PointHits( point, &adaptor, collisionFilter );
	
	adaptor.SetListener( NULL );
	adaptor.Reset();
}

// public func void colliderHits( Collider collider, ColliderListener listener )
deClassWorld::nfColliderHits::nfColliderHits( const sInitData &init ) :
dsFunction( init.clsWorld, "colliderHits", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsCol ); // collider
	p_AddParameter( init.clsCLL ); // listener
}
void deClassWorld::nfColliderHits::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld &world = *( ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world );
	const deScriptingDragonScript &ds = *( ( ( deClassWorld* )GetOwnerClass() )->GetDS() );
	
	deCollider * const collider = ds.GetClassCollider()->GetCollider( rt->GetValue( 0 )->GetRealObject() );
	dsRealObject * const listener = rt->GetValue( 1 )->GetRealObject();
	if( ! collider || ! listener ){
		DSTHROW( dueNullPointer );
	}
	
	dedsColliderListenerAdaptor &adaptor = ds.GetColliderListenerAdaptor();
	adaptor.SetListener( listener );
	adaptor.Reset();
	
	world.ColliderHits( collider, &adaptor );
	
	adaptor.SetListener( NULL );
	adaptor.Reset();
}

// public func void colliderMoveHits( Collider collider, Vector displacement, ColliderListener listener )
deClassWorld::nfColliderMoveHits::nfColliderMoveHits( const sInitData &init ) :
dsFunction( init.clsWorld, "colliderMoveHits", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsCol ); // collider
	p_AddParameter( init.clsVec ); // displacement
	p_AddParameter( init.clsCLL ); // listener
}
void deClassWorld::nfColliderMoveHits::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld &world = *( ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world );
	const deScriptingDragonScript &ds = *( ( ( deClassWorld* )GetOwnerClass() )->GetDS() );
	
	deCollider * const collider = ds.GetClassCollider()->GetCollider( rt->GetValue( 0 )->GetRealObject() );
	const decVector &displacement = ds.GetClassVector()->GetVector( rt->GetValue( 1 )->GetRealObject() );
	dsRealObject * const listener = rt->GetValue( 2 )->GetRealObject();
	if( ! collider || ! listener ){
		DSTHROW( dueNullPointer );
	}
	
	dedsColliderListenerAdaptor &adaptor = ds.GetColliderListenerAdaptor();
	adaptor.SetListener( listener );
	adaptor.Reset();
	
	world.ColliderMoveHits( collider, displacement, &adaptor );
	
	adaptor.SetListener( NULL );
	adaptor.Reset();
}

// public func void colliderMoveHitsClosest( Collider collider, Vector displacement, ColliderListener listener )
deClassWorld::nfColliderMoveHitsClosest::nfColliderMoveHitsClosest( const sInitData &init ) :
dsFunction( init.clsWorld, "colliderMoveHitsClosest", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsCol ); // collider
	p_AddParameter( init.clsVec ); // displacement
	p_AddParameter( init.clsCLL ); // listener
}
void deClassWorld::nfColliderMoveHitsClosest::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld &world = *( ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world );
	const deScriptingDragonScript &ds = *( ( ( deClassWorld* )GetOwnerClass() )->GetDS() );
	
	deCollider * const collider = ds.GetClassCollider()->GetCollider( rt->GetValue( 0 )->GetRealObject() );
	const decVector &displacement = ds.GetClassVector()->GetVector( rt->GetValue( 1 )->GetRealObject() );
	dsRealObject * const listener = rt->GetValue( 2 )->GetRealObject();
	if( ! collider || ! listener ){
		DSTHROW( dueNullPointer );
	}
	
	dedsColliderListenerClosest &hitsClosest = ds.GetColliderListenerClosest();
	hitsClosest.SetListener( listener );
	hitsClosest.Reset();
	
	world.ColliderMoveHits( collider, displacement, &hitsClosest );
	
	hitsClosest.NotifyListener();
	hitsClosest.SetListener( NULL );
	hitsClosest.Reset();
}

// public func void rayHits( DVector rayOrigin, Vector rayDirection,
// ColliderListener listener, CollisionFilter collisionFilter )
deClassWorld::nfRayHits::nfRayHits( const sInitData &init ) : dsFunction( init.clsWorld,
"rayHits", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsDVec ); // rayOrigin
	p_AddParameter( init.clsVec ); // rayDirection
	p_AddParameter( init.clsCLL ); // listener
	p_AddParameter( init.clsCF ); // collisionFilter
}
void deClassWorld::nfRayHits::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld &world = *( ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world );
	const deScriptingDragonScript &ds = *( ( ( deClassWorld* )GetOwnerClass() )->GetDS() );
	
	const decDVector &rayOrigin = ds.GetClassDVector()->GetDVector( rt->GetValue( 0 )->GetRealObject() );
	const decVector &rayDirection = ds.GetClassVector()->GetVector( rt->GetValue( 1 )->GetRealObject() );
	dsRealObject * const objListener = rt->GetValue( 2 )->GetRealObject();
	const decCollisionFilter &collisionFilter = ds.GetClassCollisionFilter()->GetCollisionFilter( rt->GetValue( 3 )->GetRealObject() );
	
	dedsColliderListenerAdaptor &adaptor = ds.GetColliderListenerAdaptor();
	adaptor.SetListener( objListener );
	adaptor.Reset();
	
	world.RayHits( rayOrigin, rayDirection, &adaptor, collisionFilter );
	
	adaptor.SetListener( NULL );
	adaptor.Reset();
}

// public func void rayHitsClosest( DVector rayOrigin, Vector rayDirection,
// ColliderListener listener, CollisionFilter collisionFilter )
deClassWorld::nfRayHitsClosest::nfRayHitsClosest( const sInitData &init ) : dsFunction( init.clsWorld,
"rayHitsClosest", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsDVec ); // rayOrigin
	p_AddParameter( init.clsVec ); // rayDirection
	p_AddParameter( init.clsCLL ); // listener
	p_AddParameter( init.clsCF ); // collisionFilter
}
void deClassWorld::nfRayHitsClosest::RunFunction( dsRunTime *rt, dsValue *myself ){
	deWorld &world = *( ( ( sWorldNatDat* )p_GetNativeData( myself ) )->world );
	const deScriptingDragonScript &ds = *( ( ( deClassWorld* )GetOwnerClass() )->GetDS() );
	
	const decDVector &rayOrigin = ds.GetClassDVector()->GetDVector( rt->GetValue( 0 )->GetRealObject() );
	const decVector &rayDirection = ds.GetClassVector()->GetVector( rt->GetValue( 1 )->GetRealObject() );
	dsRealObject * const objListener = rt->GetValue( 2 )->GetRealObject();
	const decCollisionFilter &collisionFilter = ds.GetClassCollisionFilter()->GetCollisionFilter( rt->GetValue( 3 )->GetRealObject() );
	
	dedsColliderListenerClosest &hitsClosest = ds.GetColliderListenerClosest();
	hitsClosest.SetListener( objListener );
	hitsClosest.Reset();
	
	world.RayHits( rayOrigin, rayDirection, &hitsClosest, collisionFilter );
	
	hitsClosest.NotifyListener();
	hitsClosest.SetListener( NULL );
	hitsClosest.Reset();
}



// Class deClassWorld
///////////////////////

// Constructor, destructor
////////////////////////////

deClassWorld::deClassWorld( deEngine *gameEngine, deScriptingDragonScript *scriptManager ) :
dsClass( "World", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED ){
	if( ! gameEngine || ! scriptManager ) DSTHROW( dueInvalidParam );
	
	// prepare
	pGameEngine = gameEngine;
	pDS = scriptManager;
	
	// set parser infos
	GetParserInfo()->SetParent( DENS_SCENERY );
	GetParserInfo()->SetBase( "Object" );
	
	// do the rest
	p_SetNativeDataSize( sizeof( sWorldNatDat ) );
}

deClassWorld::~deClassWorld(){
}



// Management
///////////////

void deClassWorld::CreateClassMembers( dsEngine *engine ){
	pClsCam = pDS->GetClassCamera();
	pClsClr = pDS->GetClassColor();
	pClsCol = pDS->GetClassCollider();
	pClsCI = pDS->GetClassCollisionInfo();
	pClsComp = pDS->GetClassComponent();
	pClsDVec = pDS->GetClassDVector();
	pClsHT = pDS->GetClassHeightTerrain();
	pClsLig = pDS->GetClassLight();
	pClsMat = pDS->GetClassMatrix();
	pClsMic = pDS->GetClassMicrophone();
	pClsNS = pDS->GetClassNetworkState();
	pClsSkyInst = pDS->GetClassSkyInstance();
	pClsSpk = pDS->GetClassSpeaker();
	pClsTS = pDS->GetClassTouchSensor();
	pClsVec = pDS->GetClassVector();
	
	sInitData init;
	init.clsWorld = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsBool = engine->GetClassBool();
	init.clsCam = pClsCam;
	init.clsClr = pClsClr;
	init.clsCol = pClsCol;
	init.clsCI = pClsCI;
	init.clsComp = pClsComp;
	init.clsDVec = pClsDVec;
	init.clsHT = pClsHT;
	init.clsLig = pClsLig;
	init.clsLum = pDS->GetClassLumimeter();
	init.clsMat = pClsMat;
	init.clsMic = pClsMic;
	init.clsNS = pClsNS;
	init.clsSkyInst = pClsSkyInst;
	init.clsSpk = pClsSpk;
	init.clsTS = pClsTS;
	init.clsVec = pClsVec;
	init.clsPF = pDS->GetClassPropField();
	init.clsFF = pDS->GetClassForceField();
	init.clsDD = pDS->GetClassDebugDrawer();
	init.clsNavSpace = pDS->GetClassNavigationSpace();
	init.clsNavBlocker = pDS->GetClassNavigationBlocker();
	init.clsNavigator = pDS->GetClassNavigator();
	init.clsPEI = pDS->GetClassParticleEmitterInstance();
	init.clsEmp = pDS->GetClassEnvMapProbe();
	init.clsCLL = pDS->GetClassColliderListener();
	init.clsCF = pDS->GetClassCollisionFilter();
	init.clsSoundLevelMeter = pDS->GetClassSoundLevelMeter();
	init.clsBillboard = pDS->GetClassBillboard();
	
	// add functions
	AddFunction( new nfNew( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfUpdate( init ) );
	
	AddFunction( new nfDetectCollisions( init ) );
	
	AddFunction( new nfGetGravity( init ) );
	AddFunction( new nfSetGravity( init ) );
	
	AddFunction( new nfGetDisableLights( init ) );
	AddFunction( new nfSetDisableLights( init ) );
	AddFunction( new nfGetAmbientLight( init ) );
	AddFunction( new nfSetAmbientLight( init ) );
	AddFunction( new nfGetLightColorMatrix( init ) );
	AddFunction( new nfSetLightColorMatrix( init ) );
	
	AddFunction( new nfGetSpeakerGain( init ) );
	AddFunction( new nfSetSpeakerGain( init ) );
	
	AddFunction( new nfGetHeightTerrain( init ) );
	AddFunction( new nfSetHeightTerrain( init ) );
	
	AddFunction( new nfGetSize( init ) );
	AddFunction( new nfSetSize( init ) );
	
	AddFunction( new nfGetSkyCount( init ) );
	AddFunction( new nfAddSky( init ) );
	AddFunction( new nfRemoveSky( init ) );
	AddFunction( new nfRemoveAllSkies( init ) );
	
	AddFunction( new nfGetCameraCount( init ) );
	AddFunction( new nfAddCamera( init ) );
	AddFunction( new nfRemoveCamera( init ) );
	AddFunction( new nfRemoveAllCameras( init ) );
	
	AddFunction( new nfGetSpeakerCount( init ) );
	AddFunction( new nfAddSpeaker( init ) );
	AddFunction( new nfRemoveSpeaker( init ) );
	AddFunction( new nfRemoveAllSpeakers( init ) );
	
	AddFunction( new nfGetMicrophoneCount( init ) );
	AddFunction( new nfAddMicrophone( init ) );
	AddFunction( new nfRemoveMicrophone( init ) );
	AddFunction( new nfRemoveAllMicrophones( init ) );
	
	AddFunction( new nfGetNetworkStateCount( init ) );
	AddFunction( new nfAddNetworkState( init ) );
	AddFunction( new nfRemoveNetworkState( init ) );
	AddFunction( new nfRemoveAllNetworkStates( init ) );
	
	AddFunction( new nfGetComponentCount( init ) );
	AddFunction( new nfAddComponent( init ) );
	AddFunction( new nfRemoveComponent( init ) );
	AddFunction( new nfRemoveAllComponents( init ) );
	
	AddFunction( new nfGetLightCount( init ) );
	AddFunction( new nfAddLight( init ) );
	AddFunction( new nfRemoveLight( init ) );
	AddFunction( new nfRemoveAllLights( init ) );
	
	AddFunction( new nfGetColliderCount( init ) );
	AddFunction( new nfAddCollider( init ) );
	AddFunction( new nfRemoveCollider( init ) );
	AddFunction( new nfRemoveAllColliders( init ) );
	
	AddFunction( new nfGetLumimeterCount( init ) );
	AddFunction( new nfAddLumimeter( init ) );
	AddFunction( new nfRemoveLumimeter( init ) );
	AddFunction( new nfRemoveAllLumimeters( init ) );
	
	AddFunction( new nfGetTouchSensorCount( init ) );
	AddFunction( new nfAddTouchSensor( init ) );
	AddFunction( new nfRemoveTouchSensor( init ) );
	AddFunction( new nfRemoveAllTouchSensors( init ) );
	
	AddFunction( new nfGetPropFieldCount( init ) );
	AddFunction( new nfAddPropField( init ) );
	AddFunction( new nfRemovePropField( init ) );
	AddFunction( new nfRemoveAllPropFields( init ) );
	
	AddFunction( new nfGetForceFieldCount( init ) );
	AddFunction( new nfAddForceField( init ) );
	AddFunction( new nfRemoveForceField( init ) );
	AddFunction( new nfRemoveAllForceFields( init ) );
	
	AddFunction( new nfGetParticleEmitterCount( init ) );
	AddFunction( new nfAddParticleEmitter( init ) );
	AddFunction( new nfRemoveParticleEmitter( init ) );
	AddFunction( new nfRemoveAllParticleEmitters( init ) );
	
	AddFunction( new nfGetDebugDrawerCount( init ) );
	AddFunction( new nfAddDebugDrawer( init ) );
	AddFunction( new nfRemoveDebugDrawer( init ) );
	AddFunction( new nfRemoveAllDebugDrawers( init ) );
	
	AddFunction( new nfGetNavigationSpaceCount( init ) );
	AddFunction( new nfAddNavigationSpace( init ) );
	AddFunction( new nfRemoveNavigationSpace( init ) );
	AddFunction( new nfRemoveAllNavigationSpaces( init ) );
	
	AddFunction( new nfGetNavigationBlockerCount( init ) );
	AddFunction( new nfAddNavigationBlocker( init ) );
	AddFunction( new nfRemoveNavigationBlocker( init ) );
	AddFunction( new nfRemoveAllNavigationBlockers( init ) );
	
	AddFunction( new nfGetNavigatorCount( init ) );
	AddFunction( new nfAddNavigator( init ) );
	AddFunction( new nfRemoveNavigator( init ) );
	AddFunction( new nfRemoveAllNavigators( init ) );
	
	AddFunction( new nfGetEnvMapProbeCount( init ) );
	AddFunction( new nfAddEnvMapProbe( init ) );
	AddFunction( new nfRemoveEnvMapProbe( init ) );
	AddFunction( new nfRemoveAllEnvMapProbes( init ) );
	
	AddFunction( new nfGetSoundLevelMeterCount( init ) );
	AddFunction( new nfAddSoundLevelMeter( init ) );
	AddFunction( new nfRemoveSoundLevelMeter( init ) );
	AddFunction( new nfRemoveAllSoundLevelMeters( init ) );
	
	AddFunction( new nfGetBillboardCount( init ) );
	AddFunction( new nfAddBillboard( init ) );
	AddFunction( new nfRemoveBillboard( init ) );
	AddFunction( new nfRemoveAllBillboards( init ) );
	
	AddFunction( new nfPointHits( init ) );
	AddFunction( new nfColliderHits( init ) );
	AddFunction( new nfColliderMoveHits( init ) );
	AddFunction( new nfColliderMoveHitsClosest( init ) );
	AddFunction( new nfRayHits( init ) );
	AddFunction( new nfRayHitsClosest( init ) );
}



deWorld *deClassWorld::GetWorld( dsRealObject *myself ) const{
	if( ! myself ){
		return NULL;
	}
	
	return ( ( sWorldNatDat* )p_GetNativeData( myself->GetBuffer() ) )->world;
}

void deClassWorld::PushWorld( dsRunTime *rt, deWorld *world ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	if( ! world ){
		rt->PushObject( NULL, this );
		return;
	}
	
	rt->CreateObjectNakedOnStack( this );
	( ( sWorldNatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) )->world = world;
	world->AddReference();
}
