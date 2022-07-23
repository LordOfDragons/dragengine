/* 
 * Drag[en]gine Bullet Physics Module
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

#include "dePhysicsBullet.h"
#include "debpSkin.h"
#include "debpRig.h"
#include "debpConfiguration.h"
#include "coldet/debpCollisionDetection.h"
#include "collider/debpCollider.h"
#include "collider/debpColliderVolume.h"
#include "collider/debpColliderComponent.h"
#include "collider/debpColliderRig.h"
#include "commands/debpCommandExecuter.h"
#include "component/debpComponent.h"
#include "component/debpModel.h"
#include "decal/debpDecal.h"
#include "forcefield/debpForceField.h"
#include "smoke/debpSmokeEmitter.h"
#include "particle/debpParticleEmitter.h"
#include "particle/debpParticleEmitterInstance.h"
#include "parameters/debpParameterList.h"
#include "parameters/debpPSimulatePropFields.h"
#include "propfield/debpPropField.h"
#include "terrain/heightmap/debpHeightTerrain.h"
#include "touchsensor/debpTouchSensor.h"
#include "world/debpWorld.h"

#include <dragengine/deEngine.h>
#include <dragengine/systems/modules/deModuleParameter.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/debug/deDebugDrawerManager.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/collider/deColliderVolume.h>
#include <dragengine/resources/collider/deColliderVisitorIdentify.h>
#include <dragengine/resources/collider/deCollisionInfo.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/exceptions.h>



// export definition
#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *BulletCreateModule( deLoadableModule *loadableModule );
#ifdef  __cplusplus
}
#endif


// entry function
///////////////////

deBaseModule *BulletCreateModule( deLoadableModule *loadableModule ){
	deBaseModule *module = NULL;
	try{
		module = new dePhysicsBullet( *loadableModule );
	}catch( const deException & ){
		return NULL;
	}
	return module;
}



// Debug stuff
////////////////

const char *ShapeTypeNames[] = {
	// polyhedral convex shapes
	"BOX_SHAPE_PROXYTYPE",
	"TRIANGLE_SHAPE_PROXYTYPE",
	"TETRAHEDRAL_SHAPE_PROXYTYPE",
	"CONVEX_TRIANGLEMESH_SHAPE_PROXYTYPE",
	"CONVEX_HULL_SHAPE_PROXYTYPE",
	"CONVEX_POINT_CLOUD_SHAPE_PROXYTYPE",
	"CUSTOM_POLYHEDRAL_SHAPE_TYPE",
// implicit convex shapes
"IMPLICIT_CONVEX_SHAPES_START_HERE",
	"SPHERE_SHAPE_PROXYTYPE",
	"MULTI_SPHERE_SHAPE_PROXYTYPE",
	"CAPSULE_SHAPE_PROXYTYPE",
	"CONE_SHAPE_PROXYTYPE",
	"CONVEX_SHAPE_PROXYTYPE",
	"CYLINDER_SHAPE_PROXYTYPE",
	"UNIFORM_SCALING_SHAPE_PROXYTYPE",
	"MINKOWSKI_SUM_SHAPE_PROXYTYPE",
	"MINKOWSKI_DIFFERENCE_SHAPE_PROXYTYPE",
	"BOX_2D_SHAPE_PROXYTYPE",
	"CONVEX_2D_SHAPE_PROXYTYPE",
	"CUSTOM_CONVEX_SHAPE_TYPE",
// concave shapes
"CONCAVE_SHAPES_START_HERE",
	// keep all the convex shapetype below here, for the check IsConvexShape in broadphase proxy!
	"TRIANGLE_MESH_SHAPE_PROXYTYPE",
	"SCALED_TRIANGLE_MESH_SHAPE_PROXYTYPE",
	// used for demo integration FAST/Swift collision library and Bullet
	"FAST_CONCAVE_MESH_PROXYTYPE",
	// terrain
	"TERRAIN_SHAPE_PROXYTYPE",
// Used for GIMPACT Trimesh integration
	"GIMPACT_SHAPE_PROXYTYPE",
// Multimaterial mesh
    "MULTIMATERIAL_TRIANGLE_MESH_PROXYTYPE",
	
	"EMPTY_SHAPE_PROXYTYPE",
	"STATIC_PLANE_PROXYTYPE",
	"CUSTOM_CONCAVE_SHAPE_TYPE",
"CONCAVE_SHAPES_END_HERE",
	
	"COMPOUND_SHAPE_PROXYTYPE",
	
	"SOFTBODY_SHAPE_PROXYTYPE",
	"HFFLUID_SHAPE_PROXYTYPE",
	"HFFLUID_BUOYANT_CONVEX_SHAPE_PROXYTYPE",
	"INVALID_SHAPE_PROXYTYPE"
};



// class dePhysicsBullet
//////////////////////////

// constructor, destructor
////////////////////////////

dePhysicsBullet::dePhysicsBullet( deLoadableModule &loadableModule ) :
deBasePhysicsModule( loadableModule ),
pConfiguration( NULL ),
pDeveloperMode( *this ),
pCommandExecuter( NULL ),
pParameters( NULL ),
pColInfo( NULL ),
pCollisionDetection( NULL ),
pDebug( *this )
{
	gContactAddedCallback = debpCollisionObject::CallbackAddContact;
	
	pCommandExecuter = new debpCommandExecuter( this );
	pConfiguration = new debpConfiguration( this );
	
	// create parameters
	pParameters = new debpParameterList;
	pParameters->AddParameter( new debpPSimulatePropFields( *this ) );
}

dePhysicsBullet::~dePhysicsBullet(){
	CleanUp();
	
	if( pConfiguration ) delete pConfiguration;
	if( pCommandExecuter ) delete pCommandExecuter;
	if( pParameters ) delete pParameters;
}



// management
///////////////

bool dePhysicsBullet::Init(){
	pCollisionDetection = new debpCollisionDetection( *this );
	pColInfo = new deCollisionInfo;
	
	pConfiguration->LoadConfig();
	
	// temporarily global variables. fine with me since we do not need it local for the time being
	gDeactivationTime = 1.0f; // default 2.0f
	
	return true;
}

void dePhysicsBullet::CleanUp(){
	if( pConfiguration ){
		pConfiguration->SaveConfig();
	}
	
	if( pColInfo ){
		pColInfo->Clear(); // just to be safe in case somebody still holds a reference
		pColInfo->FreeReference();
		pColInfo = NULL;
	}
	
	if( pCollisionDetection ){
		delete pCollisionDetection;
		pCollisionDetection = NULL;
	}
}



// Parameters
///////////////

int dePhysicsBullet::GetParameterCount() const{
	return pParameters->GetParameterCount();
}

void dePhysicsBullet::GetParameterInfo( int index, deModuleParameter &info ) const{
	info = pParameters->GetParameterAt( index );
}

int dePhysicsBullet::IndexOfParameterNamed( const char *name ) const{
	return pParameters->IndexOfParameterNamed( name );
}

decString dePhysicsBullet::GetParameterValue( const char *name ) const{
	return pParameters->GetParameterNamed( name ).GetParameterValue();
}

void dePhysicsBullet::SetParameterValue( const char *name, const char *value ){
	pParameters->GetParameterNamed( name ).SetParameterValue( value );
}

void dePhysicsBullet::SendCommand( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	if( pCommandExecuter ){
		pCommandExecuter->ExecuteCommand( command, answer );
		
	}else{
		answer.SetFromUTF8( "Internal Error!" );
	}
}



// physics management
///////////////////////

deBasePhysicsComponent *dePhysicsBullet::CreateComponent( deComponent *comp ){
	return new debpComponent( *this, comp );
}

deBasePhysicsModel *dePhysicsBullet::CreateModel( deModel *model ){
	return new debpModel( *this, *model );
}

deBasePhysicsSkin *dePhysicsBullet::CreateSkin( deSkin *skin ){
	return new debpSkin( skin );
}

deBasePhysicsRig *dePhysicsBullet::CreateRig( deRig *rig ){
	return new debpRig( *this, *rig );
}

deBasePhysicsWorld *dePhysicsBullet::CreateWorld( deWorld *world ){
	return new debpWorld( *this, *world );
}

deBasePhysicsCollider *dePhysicsBullet::CreateCollider( deCollider *collider ){
	deColliderVisitorIdentify identify;
	
	collider->Visit( identify );
	
	if( identify.IsVolume() ){
		return new debpColliderVolume( this, identify.CastToVolume() );
		
	}else if( identify.IsComponent() ){
		return new debpColliderComponent( this, identify.CastToComponent() );
		
	}else if( identify.IsRigged() ){
		return new debpColliderRig( this, identify.CastToRig() );
	}
	
	DETHROW( deeOutOfMemory );
}

deBasePhysicsDecal *dePhysicsBullet::CreateDecal( deDecal *decal ){
	return new debpDecal( decal );
}

deBasePhysicsTouchSensor *dePhysicsBullet::CreateTouchSensor( deTouchSensor *touchSensor ){
	return new debpTouchSensor( *this, *touchSensor );
}

deBasePhysicsHeightTerrain *dePhysicsBullet::CreateHeightTerrain( deHeightTerrain *heightTerrain ){
	return new debpHeightTerrain( this, heightTerrain );
}

deBasePhysicsPropField *dePhysicsBullet::CreatePropField( dePropField *propField ){
	return new debpPropField( this, propField );
}

deBasePhysicsForceField *dePhysicsBullet::CreateForceField( deForceField *forceField ){
	return new debpForceField( *this, *forceField );
}

deBasePhysicsParticleEmitter *dePhysicsBullet::CreateParticleEmitter( deParticleEmitter *emitter ){
	return new debpParticleEmitter( this, emitter );
}

deBasePhysicsParticleEmitterInstance *dePhysicsBullet::CreateParticleEmitterInstance( deParticleEmitterInstance *instance ){
	return new debpParticleEmitterInstance( this, instance );
}

deBasePhysicsSmokeEmitter *dePhysicsBullet::CreateSmokeEmitter( deSmokeEmitter *smokeEmitter ){
	return new debpSmokeEmitter( this, smokeEmitter );
}
