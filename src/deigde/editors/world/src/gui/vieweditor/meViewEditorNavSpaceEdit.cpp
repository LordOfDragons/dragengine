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

#include "meViewEditorNavSpaceEdit.h"
#include "../meView3D.h"
#include "../meWindowMain.h"
#include "../../world/meCamera.h"
#include "../../world/meWorld.h"
#include "../../world/meWorldGuiParameters.h"
#include "../../world/terrain/meHeightTerrainSector.h"
#include "../../world/terrain/meHeightTerrain.h"
#include "../../collisions/meCLSelect.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/utils/decLayerMask.h>
#include <dragengine/common/utils/decCollisionFilter.h>
#include <dragengine/resources/collider/deColliderVolume.h>
#include <dragengine/resources/collider/deColliderManager.h>



// Class meViewEditorNavSpaceEdit
///////////////////////////////////

// Constructor, destructor
////////////////////////////

meViewEditorNavSpaceEdit::meViewEditorNavSpaceEdit( meView3D &view ) :
meViewEditorNavigation( view ),
pCLSelect( NULL ),
pSelectDistance( 500.0f )
{
	pColVol.TakeOver( view.GetEngine()->GetColliderManager()->CreateColliderVolume() );
	
	try{
		pCLSelect = new meCLSelect( *view.GetWorld() );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

meViewEditorNavSpaceEdit::~meViewEditorNavSpaceEdit(){
	pCleanUp();
}



// Management
///////////////



// Callbacks
//////////////

void meViewEditorNavSpaceEdit::OnResize(){
}

void meViewEditorNavSpaceEdit::OnLeftMouseButtonPress( int x, int y, bool shift, bool control ){
	meViewEditorNavigation::OnLeftMouseButtonPress( x, y, shift, control );
	
	if( ! GetDragLeftMouseButton() || ! pCLSelect ){
		return;
	}
	
	pCLSelect->SetSingleSelect( ! shift );
	pCLSelect->SetRectSelect( false );
	
	decLayerMask collisionCategory;
	collisionCategory.SetBit( meWorld::eclmEditing );
	
	decLayerMask collisionFilter;
	collisionFilter.SetBit( meWorld::eclmAI ); // for individual navigation spaces
	collisionFilter.SetBit( meWorld::eclmHeightTerrains ); // for height terrain navigation spaces
	
	pCLSelect->SetCanSelectAll( false );
	pCLSelect->SetCanSelectHTNavPoints( true );
	
	pCLSelect->SetCanHitAll( false );
	pCLSelect->SetCanHitHTNavPoints( true );
	
	pCLSelect->Reset();
	const decDVector rayPosition( GetMatrixView().GetPosition() );
	const decVector rayDirection( GetActiveCamera().GetDirectionFor( GetViewWidth(), GetViewHeight(), x, y ) * pSelectDistance );
	pCLSelect->SetRay( rayPosition, rayDirection );
	RayTestCollision( pCLSelect, rayPosition, rayDirection, decCollisionFilter( collisionCategory, collisionFilter ) );
	pCLSelect->RunAction();
}

void meViewEditorNavSpaceEdit::OnLeftMouseButtonRelease( int x, int y, bool shift, bool control ){
	meViewEditorNavigation::OnLeftMouseButtonRelease( x, y, shift, control );
	
	if( pCLSelect ){
		pCLSelect->Reset();
	}
	
	// DEBUG
	/*
	const meHeightTerrainSector * const sector = GetWorld().GetHeightTerrain()->GetActiveSector();
	if( sector ){
		const decIntList &selNavPoints = sector->GetSelectedNavPoints();
		const int count = selNavPoints.GetCount();
		decString text;
		int i;
		
		for( i=0; i<count; i++ ){
			const int navPoint = selNavPoints.GetAt( i );
			const decPoint pos( navPoint % sector->GetImageDimension(), navPoint / sector->GetImageDimension() );
			if( i > 0 ){
				text.Append( ", " );
			}
			text.AppendFormat( "%i (%i,%i)", navPoint, pos.x, pos.y );
		}
		
		printf( "SelNavPoints: %s\n", text.GetString() );
	}
	*/
	// DEBUG
}

void meViewEditorNavSpaceEdit::OnMouseMove( int x, int y, bool shift, bool control ){
	meViewEditorNavigation::OnMouseMove( x, y, shift, control );
	
	if( ! GetDragLeftMouseButton() || ! pCLSelect ){
		return;
	}
}

void meViewEditorNavSpaceEdit::OnMouseWheel( int steps, bool shift, bool control ){
}



// Private functions
//////////////////////

void meViewEditorNavSpaceEdit::pCleanUp(){
	if( pCLSelect ){
		delete pCLSelect;
	}
}
