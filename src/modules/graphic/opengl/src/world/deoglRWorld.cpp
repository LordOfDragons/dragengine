/* 
 * Drag[en]gine OpenGL Graphic Module
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

#include "deoglRWorld.h"
#include "deoglWorldOctree.h"
#include "deoglWorldOctreeVisitor.h"
#include "../billboard/deoglRBillboard.h"
#include "../component/deoglRComponent.h"
#include "../debugdrawer/deoglRDebugDrawer.h"
#include "../envmap/deoglEnvironmentMap.h"
#include "../envmap/deoglREnvMapProbe.h"
#include "../light/deoglRLight.h"
#include "../particle/deoglRParticleEmitterInstance.h"
#include "../propfield/deoglRPropField.h"
#include "../rendering/plan/deoglRenderPlan.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"
#include "../sensor/deoglRLumimeter.h"
#include "../sky/deoglRSkyInstance.h"
#include "../terrain/heightmap/deoglRHeightTerrain.h"
#include "../delayedoperation/deoglDelayedDeletion.h"
#include "../delayedoperation/deoglDelayedOperations.h"

#include <dragengine/common/exceptions.h>



// Class deoglRWorld
///////////////////////

// Constructor, destructor
////////////////////////////

deoglRWorld::deoglRWorld( deoglRenderThread &renderThread, const decDVector &octreeSize ) :
pRenderThread( renderThread ),

pDirtyNotifySkyChanged( false ),
pDirtySkyOrder( false ),
pSkyEnvMap( NULL ),

pHeightTerrain( NULL ),

pRootComponent( NULL ),
pTailComponent( NULL ),
pComponentCount( 0 ),

pRootBillboard( NULL ),
pTailBillboard( NULL ),
pBillboardCount( 0 ),

pDisableLights( false ),

pValidReferenceDistance( 1e4 ), // 10km

pEnvMapUpdateCount( 0 ),
pEnvMapRenderPlan( NULL ),
pDirtyEnvMapLayout( false ),

pOctree( NULL )
{
	try{
		pOctree = new deoglWorldOctree( decDVector(), octreeSize );
		
		pEnvMapRenderPlan = new deoglRenderPlan( renderThread );
		pEnvMapRenderPlan->SetWorld( this );
		pEnvMapRenderPlan->SetUseToneMap( false );
// 		pEnvMapRenderPlan->SetIgnoreStaticComponents( true );
		pEnvMapRenderPlan->SetRenderDebugPass( false );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
	LEAK_CHECK_CREATE( renderThread, World );
}

deoglRWorld::~deoglRWorld(){
	LEAK_CHECK_FREE( pRenderThread, World );
	pCleanUp();
}



// Management
///////////////

void deoglRWorld::MarkSkyOrderDirty(){
	pDirtySkyOrder = true;
}



void deoglRWorld::SetHeightTerrain( deoglRHeightTerrain *heightTerrain ){
	if( heightTerrain == pHeightTerrain ){
		return;
	}
	
	if( pHeightTerrain ){
		pHeightTerrain->SetParentWorld( NULL );
		pHeightTerrain->FreeReference();
	}
	
	pHeightTerrain = heightTerrain;
	
	if( heightTerrain ){
		heightTerrain->AddReference();
		heightTerrain->SetParentWorld( this );
	}
}



void deoglRWorld::SetDisableLights( bool disable ){
	pDisableLights = disable;
	pDirtyNotifySkyChanged = true;
}

void deoglRWorld::SetAmbientLight( const decColor &color ){
	pAmbientLight = color;
	pDirtyNotifySkyChanged = true;
}

void deoglRWorld::SetLightColorMatrix( const decMatrix &matrix ){
	pLightColorMatrix = matrix;
	pDirtyNotifySkyChanged = true;
}



void deoglRWorld::VisitAll( deoglWorldOctreeVisitor &visitor ){
	pOctree->VisitNodes( &visitor );
}

void deoglRWorld::VisitRegion( const decDVector &boxMinExtend,
const decDVector &boxMaxExtend, deoglWorldOctreeVisitor &visitor ){
	pOctree->VisitNodesColliding( &visitor, boxMinExtend, boxMaxExtend );
}



// #define DO_SPECIAL_TIMING 1
#ifdef DO_SPECIAL_TIMING
#include <dragengine/common/utils/decTimer.h>
#define INIT_SPECIAL_TIMING decTimer sttimer;
#define SPECIAL_TIMER_PRINT_EARLY(w) if(plan.GetDebugTiming()) pRenderThread.GetLogger().LogInfoFormat("RWorld.EarlyPrepareForRender: " w "=%dys", (int)(sttimer.GetElapsedTime()*1e6f));
#define SPECIAL_TIMER_PRINT(w) if(plan.GetDebugTiming()) pRenderThread.GetLogger().LogInfoFormat("RWorld.PrepareForRender: " w "=%dys", (int)(sttimer.GetElapsedTime()*1e6f));
#else
#define INIT_SPECIAL_TIMING (void)plan;
#define SPECIAL_TIMER_PRINT_EARLY(w)
#define SPECIAL_TIMER_PRINT(w)
#endif

void deoglRWorld::EarlyPrepareForRender( deoglRenderPlan &plan ){
	INIT_SPECIAL_TIMING
	
	// early prepare lights
	const decPointerLinkedList::cListEntry *iterLight = pListPrepareForRenderLights.GetRoot();
	while( iterLight ){
		( ( deoglRLight* )iterLight->GetOwner() )->EarlyPrepareForRender();
		iterLight = iterLight->GetNext();
	}
	SPECIAL_TIMER_PRINT_EARLY("Lights")
}

void deoglRWorld::PrepareForRender( deoglRenderPlan &plan ){
	INIT_SPECIAL_TIMING
	int i, count;
	
	// remove environment map probes marked for removal by deoglWorld. this call can not be done during
	// deoglWorld::SyncToRender since deleting the environment map probe also removes deoglEnvironmentMap
	// which are interwoved with deoglRWorld in a tricky way. thus they are removed here where it is
	// safe to delete opengl objects
	RemoveRemovalMarkedEnvMapProbes();
	
	// notify environment map layout changed and update render environment maps if required
	if( pDirtyEnvMapLayout ){
		pDirtyEnvMapLayout = false;
		
		deoglRBillboard *billboard = pRootBillboard;
		while( billboard ){
			billboard->UpdateRenderEnvMap();
			billboard = billboard->GetLLWorldNext();
		}
		
		count = pParticleEmitterInstances.GetCount();
		for( i=0; i<count; i++ ){
			( ( deoglRParticleEmitterInstance* )pParticleEmitterInstances.GetAt( i ) )->UpdateRenderEnvMap();
		}
	}
		SPECIAL_TIMER_PRINT("EnvMaps")
	
	// prepare sky
	count = pSkies.GetCount();
	for( i=0; i<count; i++ ){
		( ( deoglRSkyInstance* )pSkies.GetAt( i ) )->PrepareForRender();
	}
	
	if( pDirtySkyOrder ){
		pReorderSkies();
		pDirtySkyOrder = false;
	}
	
	pCreateSkyEnvMap();
	pSkyEnvMap->PrepareForRender();
	
	if( pDirtyNotifySkyChanged ){
		EnvMapsNotifySkyChanged();
		pDirtyNotifySkyChanged = false;
	}
		SPECIAL_TIMER_PRINT("Sky")
	
	// prepare components
	decPointerLinkedList::cListEntry * const tailComponent = pListPrepareForRenderComponents.GetTail();
	while( pListPrepareForRenderComponents.GetRoot() ){
		decPointerLinkedList::cListEntry * const entry = pListPrepareForRenderComponents.GetRoot();
		deoglRComponent &component = *( ( deoglRComponent* )entry->GetOwner() );
		pListPrepareForRenderComponents.Remove( entry );
		
		component.PrepareForRender( plan ); // can potentially re-add the component
		
		if( entry == tailComponent ){
			break; // processed last component. re-added component will come next
		}
	}
		SPECIAL_TIMER_PRINT("Components")
	
	// prepare billboards
	decPointerLinkedList::cListEntry * const tailBillboard = pListPrepareForRenderBillboards.GetTail();
	while( pListPrepareForRenderBillboards.GetRoot() ){
		decPointerLinkedList::cListEntry * const entry = pListPrepareForRenderBillboards.GetRoot();
		deoglRBillboard &billboard = *( ( deoglRBillboard* )entry->GetOwner() );
		pListPrepareForRenderBillboards.Remove( entry );
		
		billboard.PrepareForRender( plan ); // can potentially re-add the billboard
		
		if( entry == tailBillboard ){
			break; // processed last billboard. re-added billboard will come next
		}
	}
		SPECIAL_TIMER_PRINT("Billboards")
	
	// prepare lights
	decPointerLinkedList::cListEntry * const tailLight = pListPrepareForRenderLights.GetTail();
	while( pListPrepareForRenderLights.GetRoot() ){
		decPointerLinkedList::cListEntry * const entry = pListPrepareForRenderLights.GetRoot();
		deoglRLight &light = *( ( deoglRLight* )entry->GetOwner() );
		pListPrepareForRenderLights.Remove( entry );
		
		light.PrepareForRender(); // can potentially re-add the light
		
		if( entry == tailLight ){
			break; // processed last light. re-added light will come next
		}
	}
		SPECIAL_TIMER_PRINT("Lights")
	
	// prepare prop fields
	decPointerLinkedList::cListEntry * const tailPropField = pListPrepareForRenderPropFields.GetTail();
	while( pListPrepareForRenderPropFields.GetRoot() ){
		decPointerLinkedList::cListEntry * const entry = pListPrepareForRenderPropFields.GetRoot();
		deoglRPropField &propField = *( ( deoglRPropField* )entry->GetOwner() );
		pListPrepareForRenderPropFields.Remove( entry );
		
		propField.PrepareForRender(); // can potentially re-add the prop field
		
		if( entry == tailPropField ){
			break; // processed last prop field. re-added prop field will come next
		}
	}
		SPECIAL_TIMER_PRINT("PropFields")
	
	
	
	
	
	// prepare debug drawers
	count = pDebugDrawers.GetCount();
	for( i=0; i<count; i++ ){
		( ( deoglRDebugDrawer* )pDebugDrawers.GetAt( i ) )->UpdateVBO();
	}
		SPECIAL_TIMER_PRINT("DebugDrawers")
}

void deoglRWorld::AddPrepareForRenderComponent( deoglRComponent *component ){
	if( ! component ){
		DETHROW( deeInvalidParam );
	}
	if( ! component->GetLLPrepareForRenderWorld().GetList() ){
		pListPrepareForRenderComponents.Add( &component->GetLLPrepareForRenderWorld() );
	}
}

void deoglRWorld::RemovePrepareForRenderComponent( deoglRComponent *component ){
	if( ! component ){
		DETHROW( deeInvalidParam );
	}
	if( component->GetLLPrepareForRenderWorld().GetList() ){
		pListPrepareForRenderComponents.Remove( &component->GetLLPrepareForRenderWorld() );
	}
}

void deoglRWorld::AddPrepareForRenderBillboard( deoglRBillboard *billboard ){
	if( ! billboard ){
		DETHROW( deeInvalidParam );
	}
	if( ! billboard->GetLLPrepareForRenderWorld().GetList() ){
		pListPrepareForRenderBillboards.Add( &billboard->GetLLPrepareForRenderWorld() );
	}
}

void deoglRWorld::RemovePrepareForRenderBillboard( deoglRBillboard *billboard ){
	if( ! billboard ){
		DETHROW( deeInvalidParam );
	}
	if( billboard->GetLLPrepareForRenderWorld().GetList() ){
		pListPrepareForRenderBillboards.Remove( &billboard->GetLLPrepareForRenderWorld() );
	}
}

void deoglRWorld::AddPrepareForRenderLight( deoglRLight *light ){
	if( ! light ){
		DETHROW( deeInvalidParam );
	}
	if( ! light->GetLLPrepareForRenderWorld().GetList() ){
		pListPrepareForRenderLights.Add( &light->GetLLPrepareForRenderWorld() );
	}
}

void deoglRWorld::RemovePrepareForRenderLight( deoglRLight *light ){
	if( ! light ){
		DETHROW( deeInvalidParam );
	}
	if( light->GetLLPrepareForRenderWorld().GetList() ){
		pListPrepareForRenderLights.Remove( &light->GetLLPrepareForRenderWorld() );
	}
}

void deoglRWorld::AddPrepareForRenderPropField( deoglRPropField *propField ){
	if( ! propField ){
		DETHROW( deeInvalidParam );
	}
	if( ! propField->GetLLPrepareForRenderWorld().GetList() ){
		pListPrepareForRenderPropFields.Add( &propField->GetLLPrepareForRenderWorld() );
	}
}

void deoglRWorld::RemovePrepareForRenderPropField( deoglRPropField *propField ){
	if( ! propField ){
		DETHROW( deeInvalidParam );
	}
	if( propField->GetLLPrepareForRenderWorld().GetList() ){
		pListPrepareForRenderPropFields.Remove( &propField->GetLLPrepareForRenderWorld() );
	}
}



// Reference position
///////////////////////

void deoglRWorld::NotifyAllReferencePositionChanged(){
	int i, count;
	
	deoglRComponent *component = pRootComponent;
	while( component ){
		component->WorldReferencePointChanged();
		component = component->GetLLWorldNext();
	}
	
	deoglRBillboard *billboard = pRootBillboard;
	while( billboard ){
		billboard->WorldReferencePointChanged();
		billboard = billboard->GetLLWorldNext();
	}
	
	count = pPropFields.GetCount();
	for( i=0; i<count; i++ ){
		( ( deoglRPropField* )pPropFields.GetAt( i ) )->WorldReferencePointChanged();
	}
	
	count = pParticleEmitterInstances.GetCount();
	for( i=0; i<count; i++ ){
		( ( deoglRParticleEmitterInstance* )pParticleEmitterInstances.GetAt( i ) )->WorldReferencePointChanged();
	}
}

void deoglRWorld::SetReferencePosition( const decDVector &position ){
	pRenderThread.GetLogger().LogInfoFormat( "World(%p) Set Reference Position: Old(%g,%g,%g) New(%g,%g,%g)",
		this, pReferencePosition.x, pReferencePosition.y, pReferencePosition.z, position.x, position.y, position.z );
	
	pReferencePosition = position;
	NotifyAllReferencePositionChanged();
}

void deoglRWorld::CheckReferencePosition( const decDVector &position ){
	if( fabs( position.x - pReferencePosition.x ) > pValidReferenceDistance
	|| fabs( position.y - pReferencePosition.y ) > pValidReferenceDistance
	|| fabs( position.z - pReferencePosition.z ) > pValidReferenceDistance ){
		SetReferencePosition( ( position + pReferencePosition ) * 0.5 );
	}
}

void deoglRWorld::SetValidReferenceDistance( double distance ){
	pValidReferenceDistance = decMath::max( distance, 1e4 );
}



// Prop fields
////////////////

int deoglRWorld::GetPropFieldCount() const{
	return pPropFields.GetCount();
}

deoglRPropField *deoglRWorld::GetPropFieldAt( int index ) const{
	return ( deoglRPropField* )pPropFields.GetAt( index );
}

void deoglRWorld::AddPropField( deoglRPropField *propField ){
	if( ! propField ){
		DETHROW( deeInvalidParam );
	}
	
	if( propField->GetParentWorld() ){
		propField->GetParentWorld()->RemovePropField( propField );
	}
	
	pPropFields.Add( propField );
	propField->SetParentWorld( this );
}

void deoglRWorld::RemovePropField( deoglRPropField *propField ){
	const int index = pPropFields.IndexOf( propField );
	if( index == -1 ){
		DETHROW( deeInvalidParam );
	}
	propField->SetParentWorld( NULL );
	pPropFields.RemoveFrom( index );
}

void deoglRWorld::RemoveAllPropFields(){
	const int count = pPropFields.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		( ( deoglRPropField* )pPropFields.GetAt( i ) )->SetParentWorld( NULL );
	}
	pPropFields.RemoveAll();
}

void deoglRWorld::RemoveRemovalMarkedPropFields(){
	int count = pPropFields.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deoglRPropField * const propField = ( deoglRPropField* )pPropFields.GetAt( i );
		if( ! propField->GetWorldMarkedRemove() ){
			continue;
		}
		
		propField->SetParentWorld( NULL );
		pPropFields.RemoveFrom( i );
		i--;
		count--;
	}
}



// Particle emitter instances
///////////////////////////////

int deoglRWorld::GetParticleEmitterInstanceCount() const{
	return pParticleEmitterInstances.GetCount();
}

deoglRParticleEmitterInstance *deoglRWorld::GetParticleEmitterInstanceAt( int index ) const{
	return ( deoglRParticleEmitterInstance* )pParticleEmitterInstances.GetAt( index );
}

void deoglRWorld::AddParticleEmitterInstance( deoglRParticleEmitterInstance *instance ){
	if( ! instance ){
		DETHROW( deeInvalidParam );
	}
	
	if( instance->GetParentWorld() ){
		instance->GetParentWorld()->RemoveParticleEmitterInstance( instance );
	}
	
	pParticleEmitterInstances.Add( instance );
	instance->SetParentWorld( this );
}

void deoglRWorld::RemoveParticleEmitterInstance( deoglRParticleEmitterInstance *instance ){
	const int index = pParticleEmitterInstances.IndexOf( instance );
	if( index == -1 ){
		DETHROW( deeInvalidParam );
	}
	instance->SetParentWorld( NULL );
	pParticleEmitterInstances.RemoveFrom( index );
}

void deoglRWorld::RemoveAllParticleEmitterInstances(){
	const int count = pParticleEmitterInstances.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		( ( deoglRParticleEmitterInstance* )pParticleEmitterInstances.GetAt( i ) )->SetParentWorld( NULL );
	}
	pParticleEmitterInstances.RemoveAll();
}

void deoglRWorld::RemoveRemovalMarkedParticleEmitterInstances(){
	int count = pParticleEmitterInstances.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deoglRParticleEmitterInstance * const instance = ( deoglRParticleEmitterInstance* )pParticleEmitterInstances.GetAt( i );
		if( ! instance->GetWorldMarkedRemove() ){
			continue;
		}
		
		instance->SetParentWorld( NULL );
		pParticleEmitterInstances.RemoveFrom( i );
		i--;
		count--;
	}
}



// Components
///////////////

void deoglRWorld::AddComponent( deoglRComponent *component ){
	if( ! component ){
		DETHROW( deeInvalidParam );
	}
	
	if( component->GetParentWorld() ){
		if( component->GetWorldMarkedRemove() ){
			component->GetParentWorld()->RemoveComponent( component );
			
		}else{
			DETHROW( deeInvalidParam );
		}
	}
	
	if( pTailComponent ){
		pTailComponent->SetLLWorldNext( component );
		component->SetLLWorldPrev( pTailComponent );
		component->SetLLWorldNext( NULL );
		pTailComponent = component;
		
	}else{
		pTailComponent = component;
		pRootComponent = component;
		component->SetLLWorldPrev( NULL );
		component->SetLLWorldNext( NULL );
	}
	
	component->AddReference();
	pComponentCount++;
	
	component->SetParentWorld( this );
	AddPrepareForRenderComponent( component );
}

void deoglRWorld::RemoveComponent( deoglRComponent *component ){
	if( component->GetParentWorld() != this ){
		DETHROW( deeInvalidParam );
	}
	
	RemovePrepareForRenderComponent( component );
	component->SetParentWorld( NULL );
	component->SetWorldMarkedRemove( false );
	
	if( component->GetLLWorldPrev() ){
		component->GetLLWorldPrev()->SetLLWorldNext( component->GetLLWorldNext() );
		
	}else{
		pRootComponent = component->GetLLWorldNext();
	}
	
	if( component->GetLLWorldNext() ){
		component->GetLLWorldNext()->SetLLWorldPrev( component->GetLLWorldPrev() );
		
	}else{
		pTailComponent = component->GetLLWorldPrev();
	}
	
	pComponentCount--;
	component->FreeReference();
}

void deoglRWorld::RemoveAllComponents(){
	while( pRootComponent ){
		deoglRComponent * const next = pRootComponent->GetLLWorldNext();
		pRootComponent->SetLLWorldPrev( NULL ); // ensure root has no prev
		
		RemovePrepareForRenderComponent( pRootComponent );
		pRootComponent->SetParentWorld( NULL );
		pRootComponent->SetWorldMarkedRemove( false );
		pComponentCount--;
		pRootComponent->FreeReference();
		
		pRootComponent = next;
	}
	
	pTailComponent = NULL;
}

void deoglRWorld::RemoveRemovalMarkedComponents(){
	deoglRComponent *component = pRootComponent;
	while( component ){
		deoglRComponent * const next = component->GetLLWorldNext();
		
		if( component->GetWorldMarkedRemove() ){
			RemoveComponent( component );
		}
		
		component = next;
	}
}



// Lights
///////////

int deoglRWorld::GetLightCount() const{
	return pLights.GetCount();
}

deoglRLight *deoglRWorld::GetLightAt( int index ) const{
	return ( deoglRLight* )pLights.GetAt( index );
}

void deoglRWorld::AddLight( deoglRLight *light ){
	if( ! light ){
		DETHROW( deeInvalidParam );
	}
	
	if( light->GetParentWorld() ){
		light->GetParentWorld()->RemoveLight( light );
	}
	
	pLights.Add( light );
	light->SetParentWorld( this );
	AddPrepareForRenderLight( light );
}

void deoglRWorld::RemoveLight( deoglRLight *light ){
	const int index = pLights.IndexOf( light );
	if( index == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	RemovePrepareForRenderLight( light );
	light->SetParentWorld( NULL );
	pLights.RemoveFrom( index );
}

void deoglRWorld::RemoveAllLights(){
	const int count = pLights.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deoglRLight * const light = ( deoglRLight* )pLights.GetAt( i );
		RemovePrepareForRenderLight( light );
		light->SetParentWorld( NULL );
	}
	
	pLights.RemoveAll();
}

void deoglRWorld::RemoveRemovalMarkedLights(){
	int count = pLights.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deoglRLight * const light = ( deoglRLight* )pLights.GetAt( i );
		if( ! light->GetWorldMarkedRemove() ){
			continue;
		}
		
		light->SetParentWorld( NULL );
		pLights.RemoveFrom( i );
		i--;
		count--;
	}
}



// EnvMapProbes
/////////////////

int deoglRWorld::GetEnvMapProbeCount() const{
	return pEnvMapProbes.GetCount();
}

deoglREnvMapProbe *deoglRWorld::GetEnvMapProbeAt( int index ) const{
	return ( deoglREnvMapProbe* )pEnvMapProbes.GetAt( index );
}

void deoglRWorld::AddEnvMapProbe( deoglREnvMapProbe *envMapProbe ){
	if( ! envMapProbe ){
		DETHROW( deeInvalidParam );
	}
	
	if( envMapProbe->GetParentWorld() ){
		envMapProbe->GetParentWorld()->RemoveEnvMapProbe( envMapProbe );
	}
	
	pEnvMapProbes.Add( envMapProbe );
	envMapProbe->SetParentWorld( this );
}

void deoglRWorld::RemoveEnvMapProbe( deoglREnvMapProbe *envMapProbe ){
	const int index = pEnvMapProbes.IndexOf( envMapProbe );
	if( index == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	envMapProbe->SetParentWorld( NULL );
	pEnvMapProbes.RemoveFrom( index );
}

void deoglRWorld::RemoveAllEnvMapProbes(){
	const int count = pEnvMapProbes.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( deoglREnvMapProbe* )pEnvMapProbes.GetAt( i ) )->SetParentWorld( NULL );
	}
	
	pEnvMapProbes.RemoveAll();
}

void deoglRWorld::RemoveRemovalMarkedEnvMapProbes(){
	int count = pEnvMapProbes.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deoglREnvMapProbe * const envMapProbe = ( deoglREnvMapProbe* )pEnvMapProbes.GetAt( i );
		if( ! envMapProbe->GetWorldMarkedRemove() ){
			continue;
		}
		
		envMapProbe->SetParentWorld( NULL );
		pEnvMapProbes.RemoveFrom( i );
		i--;
		count--;
	}
}



// Environment maps
/////////////////////

void deoglRWorld::AddEnvMap( deoglEnvironmentMap *envmap ){
	if( ! envmap || envmap->GetWorld() ){
		DETHROW( deeInvalidParam );
	}
	
	pEnvMapList.Add( envmap );
	envmap->SetWorld( this );
	
	InvalidateEnvMapLayout();
}

void deoglRWorld::RemoveEnvMap( deoglEnvironmentMap *envmap ){
	if( ! envmap || envmap->GetWorld() != this ){
		DETHROW( deeInvalidParam );
	}
	
	deoglRComponent *component = pRootComponent;
	while( component ){
		component->InvalidateRenderEnvMapIf( envmap );
		component = component->GetLLWorldNext();
	}
	
	const int peiCount = pParticleEmitterInstances.GetCount();
	int i;
	for( i=0; i<peiCount; i++ ){
		( ( deoglRParticleEmitterInstance* )pParticleEmitterInstances.GetAt( i ) )->InvalidateRenderEnvMapIf( envmap );
	}
	
	pEnvMapList.Remove( envmap );
	envmap->SetWorld( NULL );
	
	InvalidateEnvMapLayout();
}

void deoglRWorld::RemoveAllEnvMaps(){
	deoglRComponent *component = pRootComponent;
	while( component ){
		component->InvalidateRenderEnvMap();
		component = component->GetLLWorldNext();
	}
	
	const int peiCount = pParticleEmitterInstances.GetCount();
	int i;
	for( i=0; i<peiCount; i++ ){
		( ( deoglRParticleEmitterInstance* )pParticleEmitterInstances.GetAt( i ) )->InvalidateRenderEnvMap();
	}
	
	const int envMapCount = pEnvMapList.GetCount();
	for( i=0; i<envMapCount; i++ ){
		pEnvMapList.GetAt( i )->SetWorld( NULL );
	}
	pEnvMapList.RemoveAll();
	
	InvalidateEnvMapLayout();
}



void deoglRWorld::ResetEnvMapUpdateCount(){
	pEnvMapUpdateCount = 1;
}

void deoglRWorld::DecEnvMapUpdateCount(){
	pEnvMapUpdateCount--;
}

void deoglRWorld::EnvMapsNotifySkyChanged(){
	const int count = pEnvMapList.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		pEnvMapList.GetAt( i )->SkyChanged();
	}
}

void deoglRWorld::InvalidateEnvMapLayout(){
	pDirtyEnvMapLayout = true;
}



// Lumimeters
///////////////

int deoglRWorld::GetLumimeterCount() const{
	return pLumimeters.GetCount();
}

deoglRLumimeter *deoglRWorld::GetLumimeterAt( int index ) const{
	return ( deoglRLumimeter* )pLumimeters.GetAt( index );
}

void deoglRWorld::AddLumimeter( deoglRLumimeter *lumimeter ){
	if( ! lumimeter ){
		DETHROW( deeInvalidParam );
	}
	
	if( lumimeter->GetParentWorld() ){
		lumimeter->GetParentWorld()->RemoveLumimeter( lumimeter );
	}
	
	pLumimeters.Add( lumimeter );
	lumimeter->SetParentWorld( this );
}

void deoglRWorld::RemoveLumimeter( deoglRLumimeter *lumimeter ){
	const int index = pLumimeters.IndexOf( lumimeter );
	if( index == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	lumimeter->SetParentWorld( NULL );
	pLumimeters.RemoveFrom( index );
}

void deoglRWorld::RemoveAllLumimeters(){
	const int count = pLumimeters.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( deoglRLumimeter* )pLumimeters.GetAt( i ) )->SetParentWorld( NULL );
	}
	
	pLumimeters.RemoveAll();
}

void deoglRWorld::RemoveRemovalMarkedLumimeters(){
	int count = pLumimeters.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deoglRLumimeter * const lumimeter = ( deoglRLumimeter* )pLumimeters.GetAt( i );
		if( ! lumimeter->GetWorldMarkedRemove() ){
			continue;
		}
		
		lumimeter->SetParentWorld( NULL );
		pLumimeters.RemoveFrom( i );
		i--;
		count--;
	}
}



// Billboards
///////////////

void deoglRWorld::AddBillboard( deoglRBillboard *billboard ){
	if( ! billboard ){
		DETHROW( deeInvalidParam );
	}
	
	if( billboard->GetParentWorld() ){
		if( billboard->GetWorldMarkedRemove() ){
			billboard->GetParentWorld()->RemoveBillboard( billboard );
			
		}else{
			DETHROW( deeInvalidParam );
		}
	}
	
	if( pTailBillboard ){
		pTailBillboard->SetLLWorldNext( billboard );
		billboard->SetLLWorldPrev( pTailBillboard );
		billboard->SetLLWorldNext( NULL );
		pTailBillboard = billboard;
		
	}else{
		pTailBillboard = billboard;
		pRootBillboard = billboard;
		billboard->SetLLWorldPrev( NULL );
		billboard->SetLLWorldNext( NULL );
	}
	
	billboard->AddReference();
	pBillboardCount++;
	
	billboard->SetParentWorld( this );
	AddPrepareForRenderBillboard( billboard );
}

void deoglRWorld::RemoveBillboard( deoglRBillboard *billboard ){
	if( billboard->GetParentWorld() != this ){
		DETHROW( deeInvalidParam );
	}
	
	RemovePrepareForRenderBillboard( billboard );
	billboard->SetParentWorld( NULL );
	billboard->SetWorldMarkedRemove( false );
	
	if( billboard->GetLLWorldPrev() ){
		billboard->GetLLWorldPrev()->SetLLWorldNext( billboard->GetLLWorldNext() );
		
	}else{
		pRootBillboard = billboard->GetLLWorldNext();
	}
	
	if( billboard->GetLLWorldNext() ){
		billboard->GetLLWorldNext()->SetLLWorldPrev( billboard->GetLLWorldPrev() );
		
	}else{
		pTailBillboard = billboard->GetLLWorldPrev();
	}
	
	pBillboardCount--;
	billboard->FreeReference();
}

void deoglRWorld::RemoveAllBillboards(){
	while( pRootBillboard ){
		deoglRBillboard * const next = pRootBillboard->GetLLWorldNext();
		pRootBillboard->SetLLWorldPrev( NULL ); // ensure root has no prev
		
		RemovePrepareForRenderBillboard( pRootBillboard );
		pRootBillboard->SetParentWorld( NULL );
		pRootBillboard->SetWorldMarkedRemove( false );
		pBillboardCount--;
		pRootBillboard->FreeReference();
		
		pRootBillboard = next;
	}
	
	pTailBillboard = NULL;
}

void deoglRWorld::RemoveRemovalMarkedBillboards(){
	deoglRBillboard *billboard = pRootBillboard;
	while( billboard ){
		deoglRBillboard * const next = billboard->GetLLWorldNext();
		
		if( billboard->GetWorldMarkedRemove() ){
			RemoveBillboard( billboard );
		}
		
		billboard = next;
	}
}



// Skies
//////////

int deoglRWorld::GetSkyCount() const{
	return pSkies.GetCount();
}

deoglRSkyInstance *deoglRWorld::GetSkyAt( int index ) const{
	return ( deoglRSkyInstance* )pSkies.GetAt( index );
}

void deoglRWorld::AddSky( deoglRSkyInstance *sky ){
	if( ! sky ){
		DETHROW( deeInvalidParam );
	}
	
	if( sky->GetParentWorld() ){
		sky->GetParentWorld()->RemoveSky( sky );
	}
	
	pSkies.Add( sky );
	sky->SetParentWorld( this );
	pDirtyNotifySkyChanged = true;
	pDirtySkyOrder = true;
}

void deoglRWorld::RemoveSky( deoglRSkyInstance *sky ){
	const int index = pSkies.IndexOf( sky );
	if( index == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	sky->SetParentWorld( NULL );
	pSkies.RemoveFrom( index );
	pDirtyNotifySkyChanged = true;
}

void deoglRWorld::RemoveAllSkies(){
	const int count = pSkies.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( deoglRSkyInstance* )pSkies.GetAt( i ) )->SetParentWorld( NULL );
	}
	
	pSkies.RemoveAll();
	pDirtyNotifySkyChanged = true;
	pDirtySkyOrder = false;
}

void deoglRWorld::RemoveRemovalMarkedSkies(){
	int count = pSkies.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deoglRSkyInstance * const sky = ( deoglRSkyInstance* )pSkies.GetAt( i );
		if( ! sky->GetWorldMarkedRemove() ){
			continue;
		}
		
		sky->SetParentWorld( NULL );
		pSkies.RemoveFrom( i );
		i--;
		count--;
	}
}



// DebugDrawers
/////////////////

int deoglRWorld::GetDebugDrawerCount() const{
	return pDebugDrawers.GetCount();
}

deoglRDebugDrawer *deoglRWorld::GetDebugDrawerAt( int index ) const{
	return ( deoglRDebugDrawer* )pDebugDrawers.GetAt( index );
}

void deoglRWorld::AddDebugDrawer( deoglRDebugDrawer *debugDrawer ){
	if( ! debugDrawer ){
		DETHROW( deeInvalidParam );
	}
	
	if( debugDrawer->GetParentWorld() ){
		debugDrawer->GetParentWorld()->RemoveDebugDrawer( debugDrawer );
	}
	
	pDebugDrawers.Add( debugDrawer );
	debugDrawer->SetParentWorld( this );
}

void deoglRWorld::RemoveDebugDrawer( deoglRDebugDrawer *debugDrawer ){
	const int index = pDebugDrawers.IndexOf( debugDrawer );
	if( index == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	debugDrawer->SetParentWorld( NULL );
	pDebugDrawers.RemoveFrom( index );
}

void deoglRWorld::RemoveAllDebugDrawers(){
	const int count = pDebugDrawers.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( deoglRDebugDrawer* )pDebugDrawers.GetAt( i ) )->SetParentWorld( NULL );
	}
	
	pDebugDrawers.RemoveAll();
}

void deoglRWorld::RemoveRemovalMarkedDebugDrawers(){
	int count = pDebugDrawers.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deoglRDebugDrawer * const debugDrawer = ( deoglRDebugDrawer* )pDebugDrawers.GetAt( i );
		if( ! debugDrawer->GetWorldMarkedRemove() ){
			continue;
		}
		
		debugDrawer->SetParentWorld( NULL );
		pDebugDrawers.RemoveFrom( i );
		i--;
		count--;
	}
}



// Private Functions
//////////////////////

void deoglRWorld::pCleanUp(){
	// dispose of elements. this is done using a PrepareQuickDispose call which modifies some
	// internal parameters to make clean-up calls perform much faster compared to calling
	// RemoveAll* functions. this cuts down processing time for cleaning up world objects
	int i, count;
	
	count = pEnvMapProbes.GetCount();
	for( i=0; i<count; i++ ){
		( ( deoglREnvMapProbe* )pEnvMapProbes.GetAt( i ) )->PrepareQuickDispose();
	}
	pEnvMapProbes.RemoveAll();
	
	count = pParticleEmitterInstances.GetCount();
	for( i=0; i<count; i++ ){
		( ( deoglRParticleEmitterInstance* )pParticleEmitterInstances.GetAt( i ) )->PrepareQuickDispose();
	}
	pParticleEmitterInstances.RemoveAll();
	
	/*count = pPropFields.GetCount();
	for( i=0; i<count; i++ ){
		( ( deoglRPropField* )pPropFields.GetAt( i ) )->SetParentWorld( NULL );
	}*/  // deoglRPropField has no special code in SetParentWorld or destructor
	pPropFields.RemoveAll();
	
	count = pLights.GetCount();
	for( i=0; i<count; i++ ){
		( ( deoglRLight* )pLights.GetAt( i ) )->PrepareQuickDispose();
	}
	pLights.RemoveAll();
	
	while( pRootBillboard ){
		deoglRBillboard * const next = pRootBillboard->GetLLWorldNext();
		pRootBillboard->PrepareQuickDispose();
		pRootBillboard->FreeReference();
		pRootBillboard = next;
	}
	
	while( pRootComponent ){
		deoglRComponent * const next = pRootComponent->GetLLWorldNext();
		pRootComponent->PrepareQuickDispose();
		pRootComponent->FreeReference();
		pRootComponent = next;
	}
	
	count = pLumimeters.GetCount();
	for( i=0; i<count; i++ ){
		( ( deoglRLumimeter* )pLumimeters.GetAt( i ) )->PrepareQuickDispose();
	}
	pLumimeters.RemoveAll();
	
	count = pDebugDrawers.GetCount();
	for( i=0; i<count; i++ ){
		( ( deoglRDebugDrawer* )pDebugDrawers.GetAt( i ) )->PrepareQuickDispose();
	}
	pDebugDrawers.RemoveAll();
	
	count = pSkies.GetCount();
	for( i=0; i<count; i++ ){
		( ( deoglRSkyInstance* )pSkies.GetAt( i ) )->PrepareQuickDispose();
	}
	pSkies.RemoveAll();
	
	SetHeightTerrain( NULL ); // needs no special handling
	
	pFreeSkyEnvMap();
	
	pEnvMapList.RemoveAll();
	
	if( pOctree ){
		delete pOctree;
	}
	
	if( pEnvMapRenderPlan ){
		pEnvMapRenderPlan->SetWorld( NULL );
		delete pEnvMapRenderPlan;
	}
}



void deoglRWorld::pReorderSkies(){
	const int count = pSkies.GetCount();
	int i;
	
	for( i=1; i<count; i++ ){
		const int order1 = ( ( deoglRSkyInstance* )pSkies.GetAt( i - 1 ) )->GetOrder();
		const int order2 = ( ( deoglRSkyInstance* )pSkies.GetAt( i ) )->GetOrder();
		if( order2 < order1 ){
			pSkies.Move( i, i - 1 );
			if( i > 1 ){
				i--;
			}
		}
	}
}

void deoglRWorld::pCreateSkyEnvMap(){
	if( pSkyEnvMap ){
		return;
	}
	
	// 128-float: size=6*128*128*3*2 = 1179648 = 0.6MB
	// 256-float: size=6*256*256*3*2 = 4718592 = 2.4MB
	// 128-int: size=6*128*128*3*1 = 294912 = 0.3MB
	// 256-int: size=6*256*256*3*1 = 1179648 = 1.2MB
	pSkyEnvMap = new deoglEnvironmentMap( pRenderThread );
	pSkyEnvMap->SetSize( pRenderThread.GetConfiguration().GetEnvMapSize() );
	pSkyEnvMap->SetIsFloat( true );
	pSkyEnvMap->SetSkyOnly( true );
	pSkyEnvMap->SetInfluencePriority( -1 );
	pSkyEnvMap->SetDestroyIfUnused( false );
	
	AddEnvMap( pSkyEnvMap );
}

void deoglRWorld::pFreeSkyEnvMap(){
	if( pSkyEnvMap ){
		RemoveEnvMap( pSkyEnvMap );
		
		pSkyEnvMap->FreeReference();
		pSkyEnvMap = NULL;
	}
}
