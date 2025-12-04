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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "meWVNodeSlot.h"
#include "meWVNodeConstant.h"
#include "../meWindowVegetation.h"
#include "../../meWindowMain.h"
#include "../../../undosys/gui/heightterrain/vegetation/rule/const/meUHTVRuleConstSetVector.h"
#include "../../../world/meWorld.h"
#include "../../../world/heightterrain/rules/meHTVRuleConstant.h"

#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/composed/igdeEditVectorListener.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/nodeview/igdeNVSlot.h>
#include <deigde/gui/nodeview/igdeNVSlotReference.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/common/exceptions.h>



// Actions
////////////

namespace {

class cEditVector : public igdeEditVectorListener{
protected:
	meWVNodeConstant &pNode;
	
public:
	cEditVector( meWVNodeConstant &node ) : pNode( node ){ }
	
	virtual void OnVectorChanged( igdeEditVector *editVector ){
		if( editVector->GetVector().IsEqualTo( pNode.GetRuleConstant()->GetVector() ) ){
			return;
		}
		
		igdeUndo::Ref undo;
		undo.TakeOver( new meUHTVRuleConstSetVector( pNode.GetWindowVegetation().GetVLayer(),
			pNode.GetRuleConstant(), editVector->GetVector() ) );
		pNode.GetWindowVegetation().GetWorld()->GetUndoSystem()->Add( undo );
	}
};

}



// Class meWVNode
///////////////////

// Constructor, destructor
////////////////////////////

meWVNodeConstant::meWVNodeConstant( meWindowVegetation &windowVegetation, meHTVRuleConstant *rule ) :
meWVNode( windowVegetation, rule ),
pRuleConstant( rule )
{
	igdeEnvironment &env = GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainerReference formLine;
	
	SetTitle( "Constant" );
	
	// slots
	igdeNVSlotReference slot;
	slot.TakeOver( new meWVNodeSlot( env, "Vector", "Vector value",
		false, *this, meWVNodeSlot::estVector, meHTVRuleConstant::eosVector ) );
	AddSlot( slot );
	
	slot.TakeOver( new meWVNodeSlot( env, "X", "X value",
		false, *this, meWVNodeSlot::estValue, meHTVRuleConstant::eosX ) );
	AddSlot( slot );
	
	slot.TakeOver( new meWVNodeSlot( env, "Y", "Y value",
		false, *this, meWVNodeSlot::estValue, meHTVRuleConstant::eosY ) );
	AddSlot( slot );
	
	slot.TakeOver( new meWVNodeSlot( env, "Z", "Z value",
		false, *this, meWVNodeSlot::estValue, meHTVRuleConstant::eosZ ) );
	AddSlot( slot );
	
	// parameters
	pFraParameters.TakeOver( new igdeContainerForm( env ) );
	AddChild( pFraParameters );
	
	helper.EditVector( pFraParameters, "Vector:", "Vector value.", pEditVector, new cEditVector( *this ) );
}

meWVNodeConstant::~meWVNodeConstant(){
}



// Management
///////////////

void meWVNodeConstant::Update(){
	meWVNode::Update();
	
	pEditVector->SetVector( pRuleConstant->GetVector() );
}
