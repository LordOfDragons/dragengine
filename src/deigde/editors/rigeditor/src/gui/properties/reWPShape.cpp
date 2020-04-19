/* 
 * Drag[en]gine IGDE Rig Editor
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

#include "reWPShape.h"
#include "reWPShapeListener.h"
#include "reWindowProperties.h"
#include "panels/reWPPanelShape.h"
#include "panels/reWPPanelShapeSphere.h"
#include "panels/reWPPanelShapeBox.h"
#include "panels/reWPPanelShapeCylinder.h"
#include "panels/reWPPanelShapeCapsule.h"
#include "../reWindowMain.h"
#include "../../rig/reRig.h"
#include "../../rig/shape/reRigShape.h"
#include "../../rig/shape/reRigShapeBox.h"
#include "../../rig/shape/reRigShapeSphere.h"
#include "../../rig/shape/reRigShapeCapsule.h"
#include "../../rig/shape/reRigShapeCylinder.h"
#include "../../rig/shape/reSelectionShapes.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeSwitcher.h>

#include <dragengine/common/exceptions.h>



// Class reWPShape
////////////////////

// Constructor, destructor
////////////////////////////

reWPShape::reWPShape( reWindowProperties &windowProperties ) :
igdeContainerScroll( windowProperties.GetEnvironment(), false, true ),
pWindowProperties( windowProperties ),
pShape( NULL ),
pRig( NULL ),
pListener( NULL ),

pPanelSphere( NULL ),
pPanelBox( NULL ),
pPanelCylinder( NULL ),
pPanelCapsule( NULL ),
pActivePanel( NULL )
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	
	pListener = new reWPShapeListener( *this );
	
	pSwitcher.TakeOver( new igdeSwitcher( env ) );
	AddChild( pSwitcher );
	
	helper.Label( pSwitcher, "No Active Shape" );
	
	pPanelSphere = new reWPPanelShapeSphere( *this );
	pSwitcher->AddChild( pPanelSphere );
	
	pPanelBox = new reWPPanelShapeBox( *this );
	pSwitcher->AddChild( pPanelBox );
	
	pPanelCylinder = new reWPPanelShapeCylinder( *this );
	pSwitcher->AddChild( pPanelCylinder );
	
	pPanelCapsule = new reWPPanelShapeCapsule( *this );
	pSwitcher->AddChild( pPanelCapsule );
	
	pSwitcher->SetCurrent( 0 );
}

reWPShape::~reWPShape(){
	SetRig( NULL );
	
	if( pListener ){
		pListener->FreeReference();
	}
}



// Management
///////////////

void reWPShape::SetRig( reRig *rig ){
	if( rig == pRig ){
		return;
	}
	
	SetShape( NULL );
	
	if( pRig ){
		pRig->RemoveNotifier( pListener );
		pRig->FreeReference();
		pRig = NULL;
	}
	
	pRig = rig;
	
	if( rig ){
		rig->AddNotifier( pListener );
		rig->AddReference();
		
		SetShape( rig->GetSelectionShapes()->GetActiveShape() );
	}
}

void reWPShape::SetShape( reRigShape *shape ){
	if( shape == pShape ){
		return;
	}
	
	if( pActivePanel ){
		pActivePanel->SetShape( NULL, NULL );
		pActivePanel = NULL;
	}
	
	if( pShape ){
		pShape->FreeReference();
	}
	
	pShape = shape;
	
	if( ! shape ){
		pSwitcher->SetCurrent( 0 );
		return;
	}
	
	shape->AddReference();
	
	const reRigShape::eShapeTypes shapeType = shape->GetShapeType();
	
	if( shapeType == pPanelSphere->GetRequiredShapeType() ){
		pSwitcher->SetCurrent( 1 );
		pActivePanel = pPanelSphere;
		
	}else if( shapeType == pPanelBox->GetRequiredShapeType() ){
		pSwitcher->SetCurrent( 2 );
		pActivePanel = pPanelBox;
		
	}else if( shapeType == pPanelCylinder->GetRequiredShapeType() ){
		pSwitcher->SetCurrent( 3 );
		pActivePanel = pPanelCylinder;
		
	}else if( shapeType == pPanelCapsule->GetRequiredShapeType() ){
		pSwitcher->SetCurrent( 4 );
		pActivePanel = pPanelCapsule;
		
	}else{
		pSwitcher->SetCurrent( 0 );
	}
	
	if( pActivePanel ){
		pActivePanel->SetShape( pRig, pShape );
		UpdateShape();
	}
}



void reWPShape::UpdateShape(){
	if( pActivePanel ){
		pActivePanel->UpdateShape();
	}
}
