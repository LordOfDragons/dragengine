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
#include "meWVNodeCurve.h"
#include "../meWindowVegetation.h"
#include "../../meWindowMain.h"
#include "../../../undosys/gui/heightterrain/vegetation/rule/curve/meUHTVRuleCurveSetCurve.h"
#include "../../../world/meWorld.h"
#include "../../../world/heightterrain/rules/meHTVRuleCurve.h"

#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/curveedit/igdeViewCurveBezier.h>
#include <deigde/gui/curveedit/igdeViewCurveBezierListener.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/nodeview/igdeNVSlot.h>
#include <deigde/gui/nodeview/igdeNVSlotReference.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/common/exceptions.h>



// Actions
////////////

namespace {

class cEditCurve : public igdeViewCurveBezierListener{
protected:
	meWVNodeCurve &pNode;
	igdeUndo::Ref pUndo;
	
public:
	cEditCurve( meWVNodeCurve &node ) : pNode( node ){ }
	
	virtual void OnCurveChanged( igdeViewCurveBezier *viewCurveBezier ){
		OnCurveChanging( viewCurveBezier );
		pUndo = NULL;
	}
	
	virtual void OnCurveChanging( igdeViewCurveBezier *viewCurveBezier ){
		if( pUndo ){
			( ( meUHTVRuleCurveSetCurve& )( igdeUndo& )pUndo ).SetNewCurve( viewCurveBezier->GetCurve() );
			pUndo->Redo();
			
		}else{
			pUndo.TakeOver( new meUHTVRuleCurveSetCurve( pNode.GetWindowVegetation().GetVLayer(),
				pNode.GetRuleCurve(), viewCurveBezier->GetCurve() ) );
			pNode.GetWindowVegetation().GetWorld()->GetUndoSystem()->Add( pUndo );
		}
	}
};

}



// Class meWVNode
///////////////////

// Constructor, destructor
////////////////////////////

meWVNodeCurve::meWVNodeCurve( meWindowVegetation &windowVegetation, meHTVRuleCurve *rule ) :
meWVNode( windowVegetation, rule ),
pRuleCurve( rule )
{
	igdeEnvironment &env = GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainerReference formLine;
	
	SetTitle( "Curve" );
	
	// slots
	igdeNVSlotReference slot;
	slot.TakeOver( new meWVNodeSlot( env, "Value", "Value of curve at input value",
		false, *this, meWVNodeSlot::estValue, meHTVRuleCurve::eosValue ) );
	AddSlot( slot );
	
	slot.TakeOver( new meWVNodeSlot( env, "Value", "Value to evaluate curve at",
		true, *this, meWVNodeSlot::estValue, meHTVRuleCurve::eisValue ) );
	AddSlot( slot );
	
	// parameters
	pFraParameters.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaY ) );
	AddChild( pFraParameters );
	
	helper.ViewCurveBezier( pFraParameters, pCurve, new cEditCurve( *this ) );
}

meWVNodeCurve::~meWVNodeCurve(){
}



// Management
///////////////

void meWVNodeCurve::Update(){
	meWVNode::Update();
	
	pCurve->SetCurve( pRuleCurve->GetCurve() );
}
