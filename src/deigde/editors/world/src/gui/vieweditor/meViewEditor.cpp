/* 
 * Drag[en]gine IGDE World Editor
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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "meViewEditor.h"
#include "../meView3D.h"
#include "../meWindowMain.h"
#include "../../configuration/meConfiguration.h"
#include "../../world/meCamera.h"
#include "../../world/meWorld.h"
#include "../../world/meWorldGuiParameters.h"
#include "../../world/decal/meDecal.h"
#include "../../world/object/meObject.h"

#include <deigde/module/igdeEditorModule.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/common/utils/decCollisionFilter.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/systems/modules/physics/deBasePhysicsWorld.h>



// Class meViewEditor
///////////////////////

// Constructor, destructor
////////////////////////////

meViewEditor::meViewEditor( meView3D &view ) :
pView( view ),

pShiftStart( false ),
pControlStart( false ),

pDragLMB( false ),
pDragRMB( false ){
}

meViewEditor::~meViewEditor(){
}



// Management
///////////////

int meViewEditor::GetViewWidth() const{
	return pView.GetRenderAreaSize().x;
}

int meViewEditor::GetViewHeight() const{
	return pView.GetRenderAreaSize().y;
}

meWorld &meViewEditor::GetWorld() const{
	return *pView.GetWorld();
}

meWorldGuiParameters &meViewEditor::GetWorldGuiParameters() const{
	return GetWorld().GetGuiParameters();
}

int meViewEditor::GetElementMode() const{
	return GetWorldGuiParameters().GetElementMode();
}

meCamera &meViewEditor::GetActiveCamera() const{
	return *GetWorld().GetActiveCamera();
}

meConfiguration &meViewEditor::GetConfiguration() const{
	return pView.GetWindowMain().GetConfiguration();
}



void meViewEditor::UpdateMatrices(){
	pMatrixView = CalculateViewMatrix();
	pMatrixViewLocal = CalculateViewLocalMatrix();
	pMatrixViewLocalInverse = pMatrixViewLocal.Invert();
}

bool meViewEditor::CheckAxisLocking( deInputEvent::eKeyCodes key ){
	meWorldGuiParameters &guiparams = GetWorldGuiParameters();
	const meConfiguration &config = GetConfiguration();
	
	if( key == config.GetHotKeyAt( meConfiguration::ehkEditLockX ) ){
		guiparams.SetLockAxisX( ! guiparams.GetLockAxisX() );
		return true;
		
	}else if( key == config.GetHotKeyAt( meConfiguration::ehkEditLockY ) ){
		guiparams.SetLockAxisY( ! guiparams.GetLockAxisY() );
		return true;
		
	}else if( key == config.GetHotKeyAt( meConfiguration::ehkEditLockZ ) ){
		guiparams.SetLockAxisZ( ! guiparams.GetLockAxisZ() );
		return true;
		
	}else if( key == config.GetHotKeyAt( meConfiguration::ehkEditLocalCFrame ) ){
		guiparams.SetUseLocal( ! guiparams.GetUseLocal() );
		return true;
		
	}else if( key == config.GetHotKeyAt( meConfiguration::ehkEditSnapToSnapPoints ) ){
		guiparams.SetSnapToSnapPoints( ! guiparams.GetSnapToSnapPoints() );
		return true;
	}
	
	return false;
}

void meViewEditor::LogException( const deException &exception ) const{
	pView.GetWindowMain().GetEditorModule().LogException( exception );
}



void meViewEditor::GetSelectedObjectsWithAttached( meObjectList &list ){
	const meObjectList &listSelected = GetWorld().GetSelectionObject().GetSelected();
	const int selectedCount = listSelected.GetCount();
	meObject *object;
	int i, j;
	
	list.RemoveAll();
	
	for( i=0; i<selectedCount; i++ ){
		list.Add( listSelected.GetAt( i ) );
	}
	
	for( i=0; i<list.GetCount(); i++ ){
		object = list.GetAt( i );
		
		const meObjectList &listChildren = object->GetAttachedObjectsList();
		const int childrenCount = listChildren.GetCount();
		
		for( j=0; j<childrenCount; j++ ){
			list.AddIfAbsent( listChildren.GetAt( j ) );
		}
	}
}



void meViewEditor::RayTestCollision( deBaseScriptingCollider *listener, const decDVector &rayPosition,
const decVector &rayDirection, const decCollisionFilter &collisionFilter ){
	deBasePhysicsWorld * const peer = GetWorld().GetEngineWorld()->GetPeerPhysics();
	if( peer ){
		peer->RayHits( rayPosition, rayDirection, listener, collisionFilter );
	}
}

void meViewEditor::ColliderTestCollision( deBaseScriptingCollider *listener, deCollider *collider ){
	deBasePhysicsWorld * const peer = GetWorld().GetEngineWorld()->GetPeerPhysics();
	if( peer ){
		peer->ColliderHits( collider, listener );
	}
}



decDMatrix meViewEditor::CalculateViewMatrix() const{
	return GetActiveCamera().GetViewMatrix();
}

decDMatrix meViewEditor::CalculateViewLocalMatrix() const{
	const int elementMode = GetElementMode();
	meWorld &world = GetWorld();
	
	if( elementMode == meWorldGuiParameters::eemObject ){
		meObject * const object = world.GetSelectionObject().GetActive();
		
		if( object ){
			return decDMatrix::CreateRotation( object->GetRotation() * DEG2RAD );
		}
		
	}else if( elementMode == meWorldGuiParameters::eemDecal ){
		meDecal * const decal = world.GetSelectionDecal().GetActive();
		
		if( decal ){
			return decDMatrix::CreateRotation( decal->GetRotation() * DEG2RAD );
		}
	}
	
	return decDMatrix();
}



void meViewEditor::OnFrameUpdate( float elapsed ){
}



// Callbacks
//////////////

void meViewEditor::OnResize(){
}

bool meViewEditor::OnKeyPress( deInputEvent::eKeyCodes key, bool shift, bool control ){
	return false;
}

bool meViewEditor::OnKeyRelease( deInputEvent::eKeyCodes key, bool shift, bool control ){
	return false;
}

void meViewEditor::OnLeftMouseButtonPress( int x, int y, bool shift, bool control ){
	if( ! pDragLMB && ! pDragRMB ){
		pDragOrgin.Set( x, y );
		pShiftStart = shift;
		pControlStart = control;
		
		pDragCurrent.Set( x, y );
		
		pDragDistanceLast.SetZero();
		pDragDistanceStart.SetZero();
		
		pDragLMB = true;
	}
	
	UpdateMatrices();
}

void meViewEditor::OnLeftMouseButtonRelease( int x, int y, bool shift, bool control ){
	pDragLMB = false;
}

void meViewEditor::OnRightMouseButtonPress( int x, int y, bool shift, bool control ){
	if( ! pDragLMB && ! pDragRMB ){
		pDragOrgin.Set( x, y );
		pShiftStart = shift;
		pControlStart = control;
		
		pDragCurrent.Set( x, y );
		
		pDragDistanceLast.SetZero();
		pDragDistanceStart.SetZero();
		
		pDragRMB = true;
	}
	
	UpdateMatrices();
}

void meViewEditor::OnRightMouseButtonRelease( int x, int y, bool shift, bool control ){
	pDragRMB = false;
}

void meViewEditor::OnMouseMove( int x, int y, bool shift, bool control ){
	if( pDragLMB || pDragRMB ){
		pDragDistanceLast.x = x - pDragCurrent.x;
		pDragDistanceLast.y = pDragCurrent.y - y;
		
		pDragCurrent.Set( x, y );
		pDragDistanceStart.x = pDragCurrent.x - pDragOrgin.x;
		pDragDistanceStart.y = pDragOrgin.y - pDragCurrent.y;
	}
}

void meViewEditor::OnMouseWheel( int steps, bool shift, bool control ){
}
