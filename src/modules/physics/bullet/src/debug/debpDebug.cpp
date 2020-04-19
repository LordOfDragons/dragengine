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
#include <string.h>
#include <stdlib.h>

#include "debpDebug.h"
#include "debpDebugInformation.h"
#include "debpDebugInformationList.h"
#include "../dePhysicsBullet.h"

#include <dragengine/common/exceptions.h>



// Class debpDebug
////////////////////

// Constructor, destructor
////////////////////////////

debpDebug::debpDebug( dePhysicsBullet &bullet ) :
pBullet( bullet ),
pEnabled( false ),
pDIColliderPrepareDetection( NULL ),
pDIColliderDetectCustomCollision( NULL ),
pDIColliderUpdateFromBody( NULL ),
pDIColliderFinishDetection( NULL ),
pDIColliderCollisionTests( NULL ),
pDIColliderUpdateOctree( NULL ),
pDITouchSensorApplyChanges( NULL ),
pDIWorldStepSimulation( NULL ),
pDIWorldUpdateOctrees( NULL ),
pDIWorldCheckDynamicCollisions( NULL )
{
	pDIColliderPrepareDetection = new debpDebugInformation( "Collider PrepareDetection:" );
	pDebugInfoList.Add( pDIColliderPrepareDetection );
	
	pDIColliderDetectCustomCollision = new debpDebugInformation( "Collider DetectCustomCollision:" );
	pDebugInfoList.Add( pDIColliderDetectCustomCollision );
	
	pDIColliderUpdateFromBody = new debpDebugInformation( "Collider UpdateFromBody:" );
	pDebugInfoList.Add( pDIColliderUpdateFromBody );
	
	pDIColliderFinishDetection = new debpDebugInformation( "Collider FinishDetection:" );
	pDebugInfoList.Add( pDIColliderFinishDetection );
	
	pDIColliderCollisionTests = new debpDebugInformation( "Collider CollisionTests:" );
	pDebugInfoList.Add( pDIColliderCollisionTests );
	
	pDIColliderUpdateOctree = new debpDebugInformation( "Collider UpdateOctreePosition:" );
	pDebugInfoList.Add( pDIColliderUpdateOctree );
	
	pDITouchSensorApplyChanges = new debpDebugInformation( "TouchSensor ApplyChanges:" );
	pDebugInfoList.Add( pDITouchSensorApplyChanges );
	
	pDIWorldStepSimulation = new debpDebugInformation( "World StepSimulation:" );
	pDebugInfoList.Add( pDIWorldStepSimulation );
	
	pDIWorldUpdateOctrees = new debpDebugInformation( "World UpdateOctrees:" );
	pDebugInfoList.Add( pDIWorldUpdateOctrees );
	
	pDIWorldCheckDynamicCollisions = new debpDebugInformation( "World CheckDynamicCollisions:" );
	pDebugInfoList.Add( pDIWorldCheckDynamicCollisions );
}

debpDebug::~debpDebug(){
	if( pDIWorldCheckDynamicCollisions ){
		pDIWorldCheckDynamicCollisions->FreeReference();
	}
	if( pDIWorldUpdateOctrees ){
		pDIWorldUpdateOctrees->FreeReference();
	}
	if( pDIWorldStepSimulation ){
		pDIWorldStepSimulation->FreeReference();
	}
	if( pDITouchSensorApplyChanges ){
		pDITouchSensorApplyChanges->FreeReference();
	}
	if( pDIColliderPrepareDetection ){
		pDIColliderPrepareDetection->FreeReference();
	}
	if( pDIColliderDetectCustomCollision ){
		pDIColliderDetectCustomCollision->FreeReference();
	}
	if( pDIColliderUpdateOctree ){
		pDIColliderUpdateOctree->FreeReference();
	}
	if( pDIColliderUpdateFromBody ){
		pDIColliderUpdateFromBody->FreeReference();
	}
	if( pDIColliderFinishDetection ){
		pDIColliderFinishDetection->FreeReference();
	}
	if( pDIColliderCollisionTests ){
		pDIColliderCollisionTests->FreeReference();
	}
}



// Management
///////////////

void debpDebug::SetEnabled( bool enabled ){
	if( enabled == pEnabled ){
		return;
	}
	
	pEnabled = enabled;
}



void debpDebug::BeginProcessPhysics( debpWorld *world ){
	if( ! pEnabled ){
		return;
	}
	
	pDIColliderPrepareDetection->Clear();
	pDIColliderDetectCustomCollision->Clear();
	pDIColliderUpdateFromBody->Clear();
	pDIColliderFinishDetection->Clear();
	pDIColliderCollisionTests->Clear();
	pDIColliderUpdateOctree->Clear();
	pDITouchSensorApplyChanges->Clear();
	pDIWorldUpdateOctrees->Clear();
	pDIWorldStepSimulation->Clear();
	pDIWorldCheckDynamicCollisions->Clear();
}

void debpDebug::EndProcessPhysics( debpWorld *world ){
	if( ! pEnabled ){
		return;
	}
	
	pUpdateDebugInformation();
	
	pBullet.LogInfoFormat( "DebugInfo World %p", world );
	pLogDebugInformation();
}



// Private Functions
//////////////////////

void debpDebug::pUpdateDebugInformation(){
	const int count = pDebugInfoList.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		pUpdateDebugInformation( *pDebugInfoList.GetAt( i ) );
	}
}

void debpDebug::pUpdateDebugInformation( debpDebugInformation &debugInfo ){
	decString text( debugInfo.GetName() );
	
	if( debugInfo.HasElapsedTime() ){
		text.AppendFormat( " %d.%01dms", ( int )( debugInfo.GetElapsedTime() * 1000.0f ),
			( int )( debugInfo.GetElapsedTime() * 10000.0f ) % 10 );
	}
	
	if( debugInfo.HasCounter() ){
		text.AppendFormat( " %dx", debugInfo.GetCounter() );
		
		if( debugInfo.GetCounter() > 0 ){
			const double average = ( double )debugInfo.GetElapsedTime() / ( double )debugInfo.GetCounter();
			text.AppendFormat( "(%dys)", ( int )( average * 1e6f ) );
		}
	}
	
	debugInfo.SetText( text );
}

void debpDebug::pLogDebugInformation(){
	const int count = pDebugInfoList.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		pBullet.LogInfo( pDebugInfoList.GetAt( i )->GetText() );
	}
}
