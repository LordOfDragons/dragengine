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
