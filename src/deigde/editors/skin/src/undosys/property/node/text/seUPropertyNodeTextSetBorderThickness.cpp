/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#include "seUPropertyNodeTextSetBorderThickness.h"
#include "../../../../skin/property/node/sePropertyNodeText.h"

#include <dragengine/common/exceptions.h>


// Class seUPropertyNodeTextSetBorderThickness
////////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

seUPropertyNodeTextSetBorderThickness::seUPropertyNodeTextSetBorderThickness(
sePropertyNodeText *node, float newValue) :
pNewValue(newValue)
{
	DEASSERT_NOTNULL(node)
	DEASSERT_NOTNULL(node->GetProperty())
	
	SetShortInfo("@Skin.Undo.NodeTextSetBorderThickness");
	
	pOldValue = node->GetBorderThickness();
	
	pNode = node;
}

seUPropertyNodeTextSetBorderThickness::~seUPropertyNodeTextSetBorderThickness() = default;


// Management
///////////////

void seUPropertyNodeTextSetBorderThickness::SetNewValue(float newValue){
	pNewValue = newValue;
}

void seUPropertyNodeTextSetBorderThickness::Undo(){
	pNode->SetBorderThickness(pOldValue);
}

void seUPropertyNodeTextSetBorderThickness::Redo(){
	pNode->SetBorderThickness(pNewValue);
}
