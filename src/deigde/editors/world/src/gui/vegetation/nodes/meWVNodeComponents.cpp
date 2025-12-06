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
#include "meWVNodeComponents.h"
#include "../meWindowVegetation.h"
#include "../../meWindowMain.h"
#include "../../../undosys/gui/heightterrain/vegetation/rule/comp/meUHTVRuleCompSetVector.h"
#include "../../../world/meWorld.h"
#include "../../../world/heightterrain/rules/meHTVRuleComponents.h"

#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/composed/igdeEditVectorListener.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/nodeview/igdeNVSlot.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/common/exceptions.h>



// Actions
////////////

namespace {

class cEditVector : public igdeEditVectorListener{
protected:
	meWVNodeComponents &pNode;
	
public:
	cEditVector( meWVNodeComponents &node ) : pNode( node ){ }
	
	virtual void OnVectorChanged( igdeEditVector *editVector ){
		if( editVector->GetVector().IsEqualTo( pNode.GetRuleComponents()->GetVector() ) ){
			return;
		}
		
		meUHTVRuleCompSetVector::Ref undo(meUHTVRuleCompSetVector::Ref::New(new meUHTVRuleCompSetVector( pNode.GetWindowVegetation().GetVLayer(),
			pNode.GetRuleComponents(), editVector->GetVector() )));
		pNode.GetWindowVegetation().GetWorld()->GetUndoSystem()->Add( undo );
	}
};

}



// Class meWVNode
///////////////////

// Constructor, destructor
////////////////////////////

meWVNodeComponents::meWVNodeComponents( meWindowVegetation &windowVegetation, meHTVRuleComponents *rule ) :
meWVNode( windowVegetation, rule ),
pRuleComponents( rule )
{
	igdeEnvironment &env = GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref formLine;
	
	SetTitle( "Components" );
	
	// slots
	meWVNodeSlot::Ref slot(meWVNodeSlot::Ref::New(new meWVNodeSlot( env, "X", "X component of vector",
		false, *this, meWVNodeSlot::estValue, meHTVRuleComponents::eosX )));
	AddSlot( slot );
	
	slot.TakeOver( new meWVNodeSlot( env, "Y", "Y component of vector",
		false, *this, meWVNodeSlot::estValue, meHTVRuleComponents::eosY ) );
	AddSlot( slot );
	
	slot.TakeOver( new meWVNodeSlot( env, "Z", "Z component of vector",
		false, *this, meWVNodeSlot::estValue, meHTVRuleComponents::eosZ ) );
	AddSlot( slot );
	
	slot.TakeOver( new meWVNodeSlot( env, "Vector", "Vector to decompose",
		true, *this, meWVNodeSlot::estVector, meHTVRuleComponents::eisVector ) );
	helper.EditVector( slot, "Input vector.", pEditVector, new cEditVector( *this ) );
	AddSlot( slot );
	
	// parameters
	pFraParameters.TakeOver( new igdeContainerForm( env ) );
	AddChild( pFraParameters );
}

meWVNodeComponents::~meWVNodeComponents(){
}



// Management
///////////////

void meWVNodeComponents::Update(){
	meWVNode::Update();
	
	pEditVector->SetVector( pRuleComponents->GetVector() );
}
