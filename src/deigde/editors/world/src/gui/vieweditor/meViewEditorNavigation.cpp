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

#include "meViewEditorNavigation.h"
#include "../../configuration/meConfiguration.h"
#include "../../world/meCamera.h"
#include "../../world/meWorld.h"
#include "../../undosys/gui/camera/meUCameraMoveObject.h"
#include "../../undosys/gui/camera/meUCameraRotateObject.h"

#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/common/exceptions.h>



// Definitions
////////////////

#define ME_DRAG_MOVE	( 1.0f / 50.0f )
#define ME_DRAG_TURN	( 1.0f / 5.0f )



// Class meViewEditorNavigation
/////////////////////////////////

// Constructor, destructor
////////////////////////////

meViewEditorNavigation::meViewEditorNavigation( meView3D &view ) : meViewEditor( view ){
	pNavigating = false;
}

meViewEditorNavigation::~meViewEditorNavigation(){
}



// Management
///////////////



// Callbacks
//////////////

void meViewEditorNavigation::OnRightMouseButtonPress( int x, int y, bool shift, bool control ){
	meViewEditor::OnRightMouseButtonPress( x, y, shift, control );
	
	meWorld &world = GetWorld();
	meCamera * const activeCamera = world.GetActiveCamera();
	
	pUndoCameraMove = NULL;
	pUndoCameraRotate = NULL;
	
	// distance
	if( shift && control ){
		pOldDist = activeCamera->GetDistance();
		pNavigating = true;
		
		if( activeCamera->HasHostObject() ){
			pUndoCameraMove.TakeOver( new meUCameraMoveObject( activeCamera->GetHostObject() ) );
		}
		
	// move
	}else if( shift || control ){
		pNavigating = true;
		
		if( activeCamera->HasHostObject() ){
			pUndoCameraMove.TakeOver( new meUCameraMoveObject( activeCamera->GetHostObject() ) );
		}
		
	// rotate
	}else{
		pOldOrientation = activeCamera->GetOrientation();
		pNavigating = true;
		
		if( activeCamera->HasHostObject() ){
			pUndoCameraRotate.TakeOver( new meUCameraRotateObject( activeCamera->GetHostObject() ) );
		}
	}
}

void meViewEditorNavigation::OnRightMouseButtonRelease( int x, int y, bool shift, bool control ){
	meViewEditor::OnRightMouseButtonRelease( x, y, shift, control );
	
	if( pUndoCameraMove ){
		if( ( ( meUCameraMoveObject& )( igdeUndo& )pUndoCameraMove ).HasChanged() ){
			GetWorld().GetUndoSystem()->Add( pUndoCameraMove, false );
		}
		pUndoCameraMove = NULL;
	}
	
	if( pUndoCameraRotate ){
		if( ( ( meUCameraRotateObject& )( igdeUndo& )pUndoCameraRotate ).HasChanged() ){
			GetWorld().GetUndoSystem()->Add( pUndoCameraRotate, false );
		}
		pUndoCameraRotate = NULL;
	}
	
	if( pNavigating ){
		pNavigating = false;
	}
}

void meViewEditorNavigation::OnMouseMove( int x, int y, bool shift, bool control ){
	meViewEditor::OnMouseMove( x, y, shift, control );
	
	meWorld &world = GetWorld();
	
	if( GetDragRightMouseButton() ){
		const float sensitivity = GetConfiguration().GetSensitivity();
		const decPoint &dragDistStart = GetDragDistanceStart();
		const decPoint &dragDistLast = GetDragDistanceLast();
		const decDMatrix &matrixView = GetMatrixView();
		const bool controlStart = GetControlStart();
		const bool shiftStart = GetShiftStart();
		meCamera *camera;
		
		// distance
		if( shiftStart && controlStart ){
			const float newDistance = decMath::max( 0.0f,
				pOldDist - ME_DRAG_MOVE * ( float )dragDistStart.y * sensitivity );
			
			camera = world.GetFreeRoamingCamera();
			camera->SetDistance( newDistance );
			
			world.NotifyCameraChanged( camera );
			
		// move left-right
		}else if( shiftStart ){
			camera = world.GetActiveCamera();
			
			if( camera->HasHostObject() ){
				if( pUndoCameraMove ){
					( ( meUCameraMoveObject& )( igdeUndo& )pUndoCameraMove ).SetNewPosition( camera->GetPosition()
						- matrixView.TransformRight() * ( ME_DRAG_MOVE * ( float )dragDistLast.x * sensitivity )
						- matrixView.TransformUp() * ( ME_DRAG_MOVE * ( float )dragDistLast.y ) * sensitivity );
					pUndoCameraMove->Redo();
				}
				
			}else{
				camera->SetPosition( camera->GetPosition()
					- matrixView.TransformRight() * ( ME_DRAG_MOVE * ( float )dragDistLast.x * sensitivity )
					- matrixView.TransformUp() * ( ME_DRAG_MOVE * ( float )dragDistLast.y ) * sensitivity );
			}
			
			world.NotifyCameraChanged( camera );
			
		// move forward-backward
		}else if( controlStart ){
			camera = world.GetActiveCamera();
			
			if( camera->HasHostObject() ){
				if( pUndoCameraMove ){
					( ( meUCameraMoveObject& )( igdeUndo& )pUndoCameraMove ).SetNewPosition( camera->GetPosition()
						+ matrixView.TransformView() * ( ME_DRAG_MOVE * ( float )dragDistLast.y * sensitivity ) );
					pUndoCameraMove->Redo();
				}
				
			}else{
				camera->SetPosition( camera->GetPosition()
					+ matrixView.TransformView() * ( ME_DRAG_MOVE * ( float )dragDistLast.y * sensitivity ) );
			}
			
			world.NotifyCameraChanged( camera );
			
		// rotate
		}else{
			const decVector vector(
				decMath::normalize( pOldOrientation.x - ME_DRAG_TURN * ( float )dragDistStart.y, 0.0f, 360.0f ),
				decMath::normalize( pOldOrientation.y - ME_DRAG_TURN * ( float )dragDistStart.x, 0.0f, 360.0f ),
				0.0f );
			
			camera = world.GetActiveCamera();
			
			if( camera->HasHostObject() ){
				if( pUndoCameraRotate ){
					( ( meUCameraRotateObject& )( igdeUndo& )pUndoCameraRotate ).SetNewRotation( vector );
					pUndoCameraRotate->Redo();
				}
				
			}else{
				camera->SetOrientation( vector );
				world.NotifyCameraChanged( camera );
			}
		}
	}
}

void meViewEditorNavigation::OnMouseWheel( int steps, bool shift, bool control ){
	meViewEditor::OnMouseWheel( steps, shift, control );
}
