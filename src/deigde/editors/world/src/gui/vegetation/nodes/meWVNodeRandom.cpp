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
#include "meWVNodeRandom.h"
#include "../../../world/heightterrain/rules/meHTVRuleRandom.h"

#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeContainer.h>
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
	
}



// Class meWVNode
///////////////////

// Constructor, destructor
////////////////////////////

meWVNodeRandom::meWVNodeRandom( meWindowVegetation &windowVegetation, meHTVRuleRandom *rule ) :
meWVNode( windowVegetation, rule ),
pRuleRandom( rule )
{
	igdeEnvironment &env = GetEnvironment();
// 	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainerReference formLine;
	
	SetTitle( "Random" );
	
	// slots
	igdeNVSlotReference slot;
	slot.TakeOver( new meWVNodeSlot( env, "Random", "Random value",
		false, *this, meWVNodeSlot::estValue, meHTVRuleRandom::eosRandom ) );
	AddSlot( slot );
	
	// parameters
	pFraParameters.TakeOver( new igdeContainerForm( env ) );
	AddChild( pFraParameters );
}

meWVNodeRandom::~meWVNodeRandom(){
}



// Management
///////////////

void meWVNodeRandom::Update(){
	meWVNode::Update();
}
