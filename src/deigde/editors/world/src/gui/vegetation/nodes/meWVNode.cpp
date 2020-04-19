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

#include "meWVNode.h"
#include "meWVNodeSlot.h"
#include "../meWindowVegetation.h"
#include "../../../undosys/gui/heightterrain/vegetation/rule/meUHTVRuleMove.h"
#include "../../../world/meWorld.h"
#include "../../../world/heightterrain/meHTVegetationLayer.h"
#include "../../../world/heightterrain/rules/meHTVRule.h"
#include "../../../undosys/gui/heightterrain/vegetation/rule/meUHTVRuleAdd.h"
#include "../../../undosys/gui/heightterrain/vegetation/rule/meUHTVRuleRemove.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/menu/igdeMenuCascadeReference.h>
#include <deigde/gui/nodeview/igdeNVNodeListener.h>
#include <deigde/gui/nodeview/igdeNVNodeListenerReference.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/common/exceptions.h>



// Actions
////////////

namespace {

class cActionDeleteNode : public igdeAction {
	meWVNode &pNode;
	
public:
	cActionDeleteNode( meWVNode &node ) : igdeAction( "Delete Node",
		node.GetEnvironment().GetStockIcon( igdeEnvironment::esiMinus ), "Delete Node" ),
	pNode( node ){ }
	
	virtual void OnAction(){
		if( ! pNode.CanDelete() ){
			return;
		}
		
		meWindowVegetation &view = *( ( meWindowVegetation* )pNode.GetOwnerBoard() );
		igdeUndoReference undo;
		undo.TakeOver( new meUHTVRuleRemove( view.GetVLayer(), pNode.GetRule() ) );
		view.GetWorld()->GetUndoSystem()->Add( undo );
	}
	
	virtual void Update(){
		SetEnabled( pNode.CanDelete() );
	}
};

class cActionDuplicateNode : public igdeAction {
	meWVNode &pNode;
	
public:
	cActionDuplicateNode( meWVNode &node ) : igdeAction( "Duplicate Node",
		node.GetEnvironment().GetStockIcon( igdeEnvironment::esiDuplicate ), "Duplicate Node" ),
	pNode( node ){ }
	
	virtual void OnAction(){
		if( ! pNode.CanDuplicate() ){
			return;
		}
		
		meWindowVegetation &view = pNode.GetWindowVegetation();
		deObjectReference refRule;
		refRule.TakeOver( pNode.GetRule()->Copy() );
		
		meHTVRule &rule = ( meHTVRule& )( deObject& )refRule;
		rule.SetPosition( rule.GetPosition() + decVector2( 0.5f, 0.5f ) );
		
		igdeUndoReference undo;
		undo.TakeOver( new meUHTVRuleAdd( view.GetVLayer(), &rule ) );
		view.GetWorld()->GetUndoSystem()->Add( undo );
	}
	
	virtual void Update(){
		SetEnabled( pNode.CanDuplicate() );
	}
};


class cActivationListener : public igdeNVNodeListener{
	meWVNode &pNode;
	
public:
	cActivationListener( meWVNode &node ) : pNode( node ){ }
	
	virtual void OnActivated( igdeNVNode* ){
		meHTVegetationLayer * const vlayer = pNode.GetWindowVegetation().GetVLayer();
		if( vlayer ){
			vlayer->SetActiveRule( pNode.GetRule() );
		}
	}
	
	virtual void AddContextMenuEntries( igdeNVNode*, igdeMenuCascade &menu ){
		igdeUIHelper &helper = menu.GetEnvironment().GetUIHelper();
		
		helper.MenuCommand( menu, new cActionDeleteNode( pNode ), true );
		helper.MenuCommand( menu, new cActionDuplicateNode( pNode ), true );
	}
};

class cDragNodeListener : public igdeNVNodeListener{
	meWVNode &pNode;
	igdeUndoReference &pUndo;
	
public:
	cDragNodeListener( meWVNode &node, igdeUndoReference &undo ) :
		pNode( node ), pUndo( undo ){ }
	
	virtual void OnDragBegin( igdeNVNode* ){
		meHTVegetationLayer * const vlayer = pNode.GetWindowVegetation().GetVLayer();
		if( vlayer ){
			pUndo.TakeOver( new meUHTVRuleMove( vlayer, pNode.GetRule() ) );
		}
	}
	
	virtual void OnDraging( igdeNVNode *node ){
		if( ! pUndo ){
			return;
		}
		
		( ( meUHTVRuleMove& )( igdeUndo& )pUndo ).SetNewPosition(
			decVector2( node->GetPosition() ) * pNode.GetWindowVegetation().GetPixelToUnits() );
		pUndo->Redo();
	}
	
	virtual void OnDragEnd( igdeNVNode* ){
		if( ! pUndo ){
			return;
		}
		
		pNode.GetWindowVegetation().GetWorld()->GetUndoSystem()->Add( pUndo, false );
		pUndo = NULL;
	}
};

}



// Class meWVNode
///////////////////

// Constructor, destructor
////////////////////////////

meWVNode::meWVNode( meWindowVegetation &windowVegetation, meHTVRule *rule ) :
igdeNVNode( windowVegetation.GetEnvironment(), "Rule" ),
pWindowVegetation( windowVegetation ),
pRule( NULL )
{
	if( ! rule ){
		DETHROW( deeInvalidParam );
	}
	
	igdeNVNodeListenerReference listener;
	listener.TakeOver( new cActivationListener( *this ) );
	AddListener( listener );
	
	listener.TakeOver( new cDragNodeListener( *this, pUndoMove ) );
	AddListener( listener );
	
	pRule = rule;
	rule->AddReference();
}

meWVNode::~meWVNode(){
	if( pRule ){
		pRule->FreeReference();
	}
}



// Management
///////////////

void meWVNode::Update(){
}

void meWVNode::UpdateClassLists(){
}

void meWVNode::UpdatePositionFromRule(){
	SetPosition( decPoint( pRule->GetPosition() * pWindowVegetation.GetUnitsToPixel() ) );
}

bool meWVNode::CanDelete() const{
	return true;
}

bool meWVNode::CanDuplicate() const{
	return true;
}
