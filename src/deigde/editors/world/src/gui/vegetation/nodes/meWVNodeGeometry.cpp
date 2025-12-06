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
#include "meWVNodeGeometry.h"
#include "../../../world/heightterrain/rules/meHTVRuleGeometry.h"

#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/nodeview/igdeNVSlot.h>
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

meWVNodeGeometry::meWVNodeGeometry( meWindowVegetation &windowVegetation, meHTVRuleGeometry *rule ) :
meWVNode( windowVegetation, rule ),
pRuleGeometry( rule )
{
	igdeEnvironment &env = GetEnvironment();
// 	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref formLine;
	
	SetTitle( "Geometry" );
	
	// slots
	meWVNodeSlot::Ref slot(meWVNodeSlot::Ref::NewWith(
		env, "Height", "Height of point relative to terrain position", false, *this, meWVNodeSlot::estValue, meHTVRuleGeometry::eosHeight));
	AddSlot( slot );
	
	slot.TakeOver( new meWVNodeSlot( env, "Normal", "Normalized vector relative to terrain orientation",
		false, *this, meWVNodeSlot::estVector, meHTVRuleGeometry::eosNormal ) );
	AddSlot( slot );
	
	slot.TakeOver( new meWVNodeSlot( env, "Terrain Type", "Terrain type",
		false, *this, meWVNodeSlot::estValue, meHTVRuleGeometry::eosTerrainType ) );
	AddSlot( slot );
	
	// parameters
	pFraParameters.TakeOver( new igdeContainerForm( env ) );
	AddChild( pFraParameters );
}

meWVNodeGeometry::~meWVNodeGeometry(){
}



// Management
///////////////

void meWVNodeGeometry::Update(){
	meWVNode::Update();
}
