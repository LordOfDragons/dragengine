/* 
 * Drag[en]gine IGDE Skin Editor
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

#include "seVCIKeyHandling.h"
#include "../seViewConstructedView.h"
#include "../../../skin/texture/seTexture.h"
#include "../../../skin/property/seProperty.h"
#include "../../../skin/property/node/sePropertyNodeGroup.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/menu/igdeMenuCascadeReference.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoReference.h>
#include <deigde/undo/igdeUndoSystem.h>



// Class seVCIKeyHandling
///////////////////////////

// Constructor, destructor
////////////////////////////

seVCIKeyHandling::seVCIKeyHandling( seViewConstructedView &view ) :
pView( view ),
pIsLeftMouseButtonPressed( false ){
}

seVCIKeyHandling::~seVCIKeyHandling(){
}



// Management
///////////////

void seVCIKeyHandling::OnKeyPress( igdeWidget*, deInputEvent::eKeyCodes keyCode, int ){
	if( pIsLeftMouseButtonPressed || ! pView.GetSkin() ){
		return;
	}
	
	switch( keyCode ){
	case deInputEvent::ekcPageDown:
		pView.GetActionMoveNodeDown()->OnAction();
		break;
		
	case deInputEvent::ekcPageUp:
		pView.GetActionMoveNodeUp()->OnAction();
		break;
		
	case deInputEvent::ekcHome:
		pView.GetActionMoveNodeTop()->OnAction();
		break;
		
	case deInputEvent::ekcEnd:
		pView.GetActionMoveNodeBottom()->OnAction();
		break;
		
	case deInputEvent::ekcDelete:
		pView.GetActionRemoveNode()->OnAction();
		break;
		
	default:
		break;
	}
}

void seVCIKeyHandling::OnKeyRelease( igdeWidget*, deInputEvent::eKeyCodes, int ){
}

void seVCIKeyHandling::OnButtonPress( igdeWidget*, int button, const decPoint &position, int ){
	switch( button ){
	case deInputEvent::embcLeft:
		pIsLeftMouseButtonPressed = true;
		break;
		
	case deInputEvent::embcRight:
		ShowContextMenu( position );
		break;
	}
}

void seVCIKeyHandling::OnButtonRelease( igdeWidget*, int button, const decPoint &, int ){
	if( button == deInputEvent::embcLeft ){
		pIsLeftMouseButtonPressed = false;
	}
}

void seVCIKeyHandling::OnMouseWheeled( igdeWidget*, const decPoint &, const decPoint &, int ){
}

void seVCIKeyHandling::OnDoubleClicked( igdeWidget*, int button, const decPoint &, int ){
	if( pIsLeftMouseButtonPressed || button != deInputEvent::embcLeft ){
		return;
	}
	
	seProperty * const property = pView.GetActiveProperty();
	if( ! property ){
		return;
	}
	
	sePropertyNode * const node = pView.GetActiveNode();
	if( node ){
		if( node->GetNodeType() == sePropertyNode::entGroup ){
			pView.GetActionEnterGroup()->OnAction();
		}
		return;
	}
	
	sePropertyNodeGroup * const nodeGroup = property->GetActiveNodeGroup();
	if( nodeGroup ){
		pView.GetActionExitGroup()->OnAction();
	}
}



// Protected Functions
////////////////////////

void seVCIKeyHandling::ShowContextMenu( const decPoint &position ){
	if( pIsLeftMouseButtonPressed || ! pView.GetSkin() ){
		return;
	}
	
	igdeEnvironment &env = pView.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelper();
	
	igdeMenuCascadeReference menu;
	menu.TakeOver( new igdeMenuCascade( env ) );
	
	helper.MenuCommand( menu, pView.GetActionAddShape() );
	helper.MenuCommand( menu, pView.GetActionAddImage() );
	helper.MenuCommand( menu, pView.GetActionAddText() );
	
	helper.MenuSeparator( menu );
	helper.MenuCommand( menu, pView.GetActionRemoveNode() );
	
	helper.MenuSeparator( menu );
	helper.MenuCommand( menu, pView.GetActionCopyNode() );
	helper.MenuCommand( menu, pView.GetActionCutNode() );
	helper.MenuCommand( menu, pView.GetActionPasteNode() );
	
	helper.MenuSeparator( menu );
	helper.MenuCommand( menu, pView.GetActionMoveNodeUp() );
	helper.MenuCommand( menu, pView.GetActionMoveNodeDown() );
	helper.MenuCommand( menu, pView.GetActionMoveNodeTop() );
	helper.MenuCommand( menu, pView.GetActionMoveNodeBottom() );
	
	helper.MenuSeparator( menu );
	helper.MenuCommand( menu, pView.GetActionEnterGroup() );
	helper.MenuCommand( menu, pView.GetActionExitGroup() );
	helper.MenuCommand( menu, pView.GetActionGroupNodes() );
	helper.MenuCommand( menu, pView.GetActionUngroupNodes() );
	
	helper.MenuSeparator( menu );
	helper.MenuCommand( menu, pView.GetActionSetMask() );
	helper.MenuCommand( menu, pView.GetActionRemoveMask() );
	
	// node type specific
	if( pView.GetActiveNode() ){
		switch( pView.GetActiveNode()->GetNodeType() ){
		case sePropertyNode::entImage:
			helper.MenuSeparator( menu );
			helper.MenuCommand( menu, pView.GetActionSizeFromImage() );
			break;
			
		default:
			break;
		}
	}
	
	// constructed from image
	helper.MenuSeparator( menu );
	helper.MenuCommand( menu, pView.GetActionConstructedFromImage() );
	
	menu->Popup( pView, position );
}
