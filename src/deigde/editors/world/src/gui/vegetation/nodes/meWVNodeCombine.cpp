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

#include "meWVNodeSlot.h"
#include "meWVNodeCombine.h"
#include "../meWindowVegetation.h"
#include "../../meWindowMain.h"
#include "../../../undosys/gui/heightterrain/vegetation/rule/combine/meUHTVRuleCombineSetX.h"
#include "../../../undosys/gui/heightterrain/vegetation/rule/combine/meUHTVRuleCombineSetY.h"
#include "../../../undosys/gui/heightterrain/vegetation/rule/combine/meUHTVRuleCombineSetZ.h"
#include "../../../world/meWorld.h"
#include "../../../world/heightterrain/rules/meHTVRuleCombine.h"

#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/nodeview/igdeNVSlot.h>
#include <deigde/gui/nodeview/igdeNVSlotReference.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoReference.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/common/exceptions.h>



// Actions
////////////

namespace {

class cTextX : public igdeTextFieldListener{
protected:
	meWVNodeCombine &pNode;
	
public:
	cTextX( meWVNodeCombine &node ) : pNode( node ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		const float value = textField->GetFloat();
		if( fabsf( value - pNode.GetRuleCombine()->GetX() ) <= FLOAT_SAFE_EPSILON ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new meUHTVRuleCombineSetX( pNode.GetWindowVegetation().GetVLayer(),
			pNode.GetRuleCombine(), value ) );
		pNode.GetWindowVegetation().GetWorld()->GetUndoSystem()->Add( undo );
	}
};

class cTextY : public igdeTextFieldListener{
protected:
	meWVNodeCombine &pNode;
	
public:
	cTextY( meWVNodeCombine &node ) : pNode( node ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		const float value = textField->GetFloat();
		if( fabsf( value - pNode.GetRuleCombine()->GetY() ) <= FLOAT_SAFE_EPSILON ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new meUHTVRuleCombineSetY( pNode.GetWindowVegetation().GetVLayer(),
			pNode.GetRuleCombine(), value ) );
		pNode.GetWindowVegetation().GetWorld()->GetUndoSystem()->Add( undo );
	}
};

class cTextZ : public igdeTextFieldListener{
protected:
	meWVNodeCombine &pNode;
	
public:
	cTextZ( meWVNodeCombine &node ) : pNode( node ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		const float value = textField->GetFloat();
		if( fabsf( value - pNode.GetRuleCombine()->GetZ() ) <= FLOAT_SAFE_EPSILON ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new meUHTVRuleCombineSetZ( pNode.GetWindowVegetation().GetVLayer(),
			pNode.GetRuleCombine(), value ) );
		pNode.GetWindowVegetation().GetWorld()->GetUndoSystem()->Add( undo );
	}
};

}



// Class meWVNode
///////////////////

// Constructor, destructor
////////////////////////////

meWVNodeCombine::meWVNodeCombine( meWindowVegetation &windowVegetation, meHTVRuleCombine *rule ) :
meWVNode( windowVegetation, rule ),
pRuleCombine( rule )
{
	igdeEnvironment &env = GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainerReference formLine;
	
	SetTitle( "Combine" );
	
	// slots
	igdeNVSlotReference slot;
	slot.TakeOver( new meWVNodeSlot( env, "Vector", "Vector composed of the input values",
		false, *this, meWVNodeSlot::estVector, meHTVRuleCombine::eosVector ) );
	AddSlot( slot );
	
	slot.TakeOver( new meWVNodeSlot( env, "X", "X component of vector",
		true, *this, meWVNodeSlot::estValue, meHTVRuleCombine::eisX ) );
	helper.EditFloat( slot, "X component of vector if slot is not connected.",
		pEditX, new cTextX( *this ) );
	AddSlot( slot );
	
	slot.TakeOver( new meWVNodeSlot( env, "Y", "Y component of vector",
		true, *this, meWVNodeSlot::estValue, meHTVRuleCombine::eisY ) );
	helper.EditFloat( slot, "Y component of vector if slot is not connected.",
		pEditY, new cTextY( *this ) );
	AddSlot( slot );
	
	slot.TakeOver( new meWVNodeSlot( env, "Z", "Z component of vector",
		true, *this, meWVNodeSlot::estValue, meHTVRuleCombine::eisZ ) );
	helper.EditFloat( slot, "Z component of vector if slot is not connected.",
		pEditZ, new cTextZ( *this ) );
	AddSlot( slot );
	
	// parameters
	pFraParameters.TakeOver( new igdeContainerForm( env ) );
	AddChild( pFraParameters );
}

meWVNodeCombine::~meWVNodeCombine(){
}



// Management
///////////////

void meWVNodeCombine::Update(){
	meWVNode::Update();
	
	pEditX->SetFloat( pRuleCombine->GetX() );
	pEditY->SetFloat( pRuleCombine->GetY() );
	pEditZ->SetFloat( pRuleCombine->GetZ() );
}
