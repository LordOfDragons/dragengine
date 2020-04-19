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

#include "meViewEditorScale.h"
#include "../meView3D.h"
#include "../meWindowMain.h"
#include "../../configuration/meConfiguration.h"
#include "../../undosys/gui/object/meUScaleObject.h"
#include "../../undosys/gui/decal/meUDecalScale.h"
#include "../../world/meCamera.h"
#include "../../world/meWorld.h"
#include "../../world/meWorldGuiParameters.h"
#include "../../world/object/meObject.h"
#include "../../world/decal/meDecal.h"
#include "../../world/objectshape/meObjectShape.h"

#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/shape/decShape.h>



// Definitions
////////////////

#define ME_DRAG_SCALE	( 1.0 / 250.0 )



// Class meViewEditorScale
////////////////////////////

// Constructor, destructor
////////////////////////////

meViewEditorScale::meViewEditorScale( meView3D &view ) : meViewEditorNavigation( view ){
}

meViewEditorScale::~meViewEditorScale(){
}



// Management
///////////////



// Callbacks
//////////////

bool meViewEditorScale::OnKeyPress( deInputEvent::eKeyCodes key, bool shift, bool control ){
	if( pUndoScale ){
		return CheckAxisLocking( key );
		
	}else{
		return meViewEditorNavigation::OnKeyPress( key, shift, control );
	}
}

void meViewEditorScale::OnLeftMouseButtonPress( int x, int y, bool shift, bool control ){
	meViewEditorNavigation::OnLeftMouseButtonPress( x, y, shift, control );
	
	const int elementMode = GetElementMode();
	meWorld &world = GetWorld();
	
	pUndoScale = NULL;
	
	if( elementMode == meWorldGuiParameters::eemObject ){
		const meObjectSelection &selection = world.GetSelectionObject();
		const meObjectList &listSelected = selection.GetSelected();
		
		if( listSelected.GetCount() > 0 ){
			meObjectList list;
			GetSelectedObjectsWithAttached( list );
			pUndoScale.TakeOver( new meUScaleObject( &world, list ) );
		}
		
	}else if( elementMode == meWorldGuiParameters::eemDecal ){
		const meDecalSelection &selection = world.GetSelectionDecal();
		
		if( selection.GetSelected().GetCount() > 0 ){
			pUndoScale.TakeOver( new meUDecalScale( &world ) );
		}
	}
	
	if( pUndoScale ){
		( ( meBaseUndoScale& )( igdeUndo& )pUndoScale ).SetModifyPosition( false );
	}
}

void meViewEditorScale::OnLeftMouseButtonRelease( int x, int y, bool shift, bool control ){
	meViewEditor::OnLeftMouseButtonRelease( x, y, shift, control );
	
	if( pUndoScale ){
		if( ! ( ( ( meBaseUndoScale& )( igdeUndo& )pUndoScale ).GetFactors().IsEqualTo( decVector( 1.0f, 1.0f, 1.0f ) ) ) ){
			GetWorld().GetUndoSystem()->Add( pUndoScale, false );
		}
		pUndoScale = NULL;
	}
}

void meViewEditorScale::OnMouseMove( int x, int y, bool shift, bool control ){
	meViewEditorNavigation::OnMouseMove( x, y, shift, control );
	
	if( pUndoScale ){
		const meWorldGuiParameters &guiparams = GetWorldGuiParameters();
		const meConfiguration &configuration = GetConfiguration();
		const float sensitivity = configuration.GetSensitivity();
		const int halfHeight = GetViewHeight() >> 1;
		const int halfWidth = GetViewWidth() >> 1;
		const decPoint &dragOrigin = GetDragOrigin();
		const decPoint &dragCurrent = GetDragCurrent();
		const decPoint &dragDist = GetDragDistanceStart();
		const float dx1 = ( float )( dragOrigin.x - halfWidth );
		const float dy1 = ( float )( dragOrigin.y - halfHeight );
		const float dx2 = ( float )( dragCurrent.x - halfWidth );
		const float dy2 = ( float )( dragCurrent.y - halfHeight );
		const decDMatrix &matrixView = GetMatrixView();
		float uniformFactor = 1.0f;
		decDVector vector;
		
		// determine new scaling
		/*
		float fx = 1.0f, fy = 1.0f;
		
		if( dx1 != 0.0f ){
			fx = dx2 / dx1;
		}
		if( dy1 != 0.0f ){
			fy = dy2 / dy1;
		}
		*/
		
		// determine new scaling
		vector = matrixView.TransformRight() * ( ME_DRAG_SCALE * ( double )dragDist.x * ( double )sensitivity );
		vector += matrixView.TransformUp() * ( ME_DRAG_SCALE * ( double )dragDist.y * ( double )sensitivity );
		
		if( guiparams.GetLockAxisX() ){
			vector.x = 0.0;
		}
		if( guiparams.GetLockAxisY() ){
			vector.y = 0.0;
		}
		if( guiparams.GetLockAxisZ() ){
			vector.z = 0.0;
		}
		if( configuration.GetScaleSnap() != shift ){
			vector.Snap( ( double )configuration.GetScaleStep() );
		}
		
		const float oldDistance = sqrtf( dx1 * dx1 + dy1 * dy1 );
		if( oldDistance > 0.0f ){
			uniformFactor = sqrtf( dx2 * dx2 + dy2 * dy2 ) / oldDistance;
		}
		
		if( configuration.GetScaleSnap() != shift ){
			const float grid = configuration.GetScaleStep();
			const float halfGrid = grid * 0.5f;
			const float integral = grid * ( float )floorf( uniformFactor / grid );
			
			if( uniformFactor - integral < halfGrid ){
				uniformFactor = integral;
				
			}else{
				uniformFactor = integral + grid;
			}
		}
		
		meBaseUndoScale &undo = ( meBaseUndoScale& )( igdeUndo& )pUndoScale;
		undo.SetFactors( decVector( 1.0f, 1.0f, 1.0f ) + vector );
		undo.SetUniformFactor( uniformFactor );
		undo.SetScaleUniform( control );
		undo.ProgressiveRedo();
	}
}
