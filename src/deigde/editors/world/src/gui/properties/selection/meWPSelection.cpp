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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "meWPSelection.h"
#include "meWPSelectionListener.h"
#include "decal/meWPSDecal.h"
#include "navspace/meWPSNavSpace.h"
#include "object/meWPSObject.h"
#include "objectshape/meWPSObjectShape.h"
#include "../meWindowProperties.h"
#include "../../meWindowMain.h"
#include "../../../world/meWorld.h"
#include "../../../world/meWorldGuiParameters.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>

#include <dragengine/common/exceptions.h>



// Class meWPSelection
/////////////////////////

// Constructor, destructor
////////////////////////////

meWPSelection::meWPSelection( meWindowProperties &windowProperties ) :
igdeSwitcher( windowProperties.GetEnvironment() ),
pWindowProperties( windowProperties ),
pListener( NULL ),
pWorld( NULL )
{
	pListener = new meWPSelectionListener( *this );
	
	GetEnvironment().GetUIHelper().Label( *this, "No Selection" );
	
	pPanelObject.TakeOver( new meWPSObject( *this ) );
	AddChild( pPanelObject );
	
	pPanelObjectShape.TakeOver( new meWPSObjectShape( *this ) );
	AddChild( pPanelObjectShape );
	
	pPanelDecal.TakeOver( new meWPSDecal( *this ) );
	AddChild( pPanelDecal );
	
	pPanelNavSpace.TakeOver( new meWPSNavSpace( *this ) );
	AddChild( pPanelNavSpace );
	
	SetCurrent( 0 );  // empty
}

meWPSelection::~meWPSelection(){
	SetWorld( NULL );
	
	if( pListener ){
		pListener->FreeReference();
	}
}



// Management
///////////////

void meWPSelection::SetWorld( meWorld *world ){
	if( world == pWorld ){
		return;
	}
	
	( ( meWPSObject& )( igdeWidget& )pPanelObject ).SetWorld( NULL );
	( ( meWPSObjectShape& )( igdeWidget& )pPanelObjectShape ).SetWorld( NULL );
	( ( meWPSDecal& )( igdeWidget& )pPanelDecal ).SetWorld( NULL );
	( ( meWPSNavSpace& )( igdeWidget& )pPanelNavSpace ).SetWorld( NULL );
	
	if( pWorld ){
		pWorld->RemoveNotifier( pListener );
		pWorld->FreeReference();
	}
	
	pWorld = world;
	
	if( world ){
		world->AddNotifier( pListener );
		world->AddReference();
	}
	
	( ( meWPSObject& )( igdeWidget& )pPanelObject ).SetWorld( world );
	( ( meWPSObjectShape& )( igdeWidget& )pPanelObjectShape ).SetWorld( world );
	( ( meWPSDecal& )( igdeWidget& )pPanelDecal ).SetWorld( world );
	( ( meWPSNavSpace& )( igdeWidget& )pPanelNavSpace ).SetWorld( world );
	
	ElementModeChanged();
}



void meWPSelection::ElementModeChanged(){
	if( ! pWorld ){
		SetCurrent( 0 ); // none
		return;
	}
	
	switch( pWorld->GetGuiParameters().GetElementMode() ){
	case meWorldGuiParameters::eemObject:
		SetCurrent( 1 );
		break;
		
	case meWorldGuiParameters::eemObjectShape:
		SetCurrent( 2 );
		break;
		
	case meWorldGuiParameters::eemDecal:
		SetCurrent( 3 );
		break;
		
	case meWorldGuiParameters::eemNavSpace:
		SetCurrent( 4 );
		break;
		
	default:
		SetCurrent( 0 ); // none
	}
}

void meWPSelection::OnGameDefinitionChanged(){
	( ( meWPSObject& )( igdeWidget& )pPanelObject ).OnGameDefinitionChanged();
	( ( meWPSObjectShape& )( igdeWidget& )pPanelObjectShape ).OnGameDefinitionChanged();
	( ( meWPSDecal& )( igdeWidget& )pPanelDecal ).OnGameDefinitionChanged();
}
