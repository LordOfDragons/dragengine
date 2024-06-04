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
