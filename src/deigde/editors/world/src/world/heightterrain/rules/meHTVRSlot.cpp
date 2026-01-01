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

#include "meHTVRule.h"
#include "meHTVRLink.h"
#include "meHTVRSlot.h"

#include <dragengine/common/exceptions.h>



// Class meHTVRSlot
/////////////////////

// Constructor, destructor
////////////////////////////

meHTVRSlot::meHTVRSlot() :
pIsInput(true){
}

meHTVRSlot::~meHTVRSlot(){
}



// Management
///////////////

void meHTVRSlot::SetIsInput(bool isInput){
	pIsInput = isInput;
}



void meHTVRSlot::AddLink(meHTVRLink *link){
	DEASSERT_NOTNULL(link)
	pLinks.Add(link);
}

void meHTVRSlot::RemoveLink(meHTVRLink *link){
	pLinks.Remove(link);
}

void meHTVRSlot::RemoveAllLinks(){
	pLinks.RemoveAll();
}



bool meHTVRSlot::HasLinkWithSource(const meHTVRule *rule, int slot) const{
	return pLinks.HasMatching([&](const meHTVRLink &link){
		return link.GetSourceRule() == rule && link.GetSourceSlot() == slot;
	});
}

meHTVRLink *meHTVRSlot::GetLinkWithSource(const meHTVRule *rule, int slot) const{
	return pLinks.FindOrDefault([&](const meHTVRLink &link){
		return link.GetSourceRule() == rule && link.GetSourceSlot() == slot;
	});
}

bool meHTVRSlot::HasLinkWithDestination(const meHTVRule *rule, int slot) const{
	return pLinks.HasMatching([&](const meHTVRLink &link){
		return link.GetDestinationRule() == rule && link.GetDestinationSlot() == slot;
	});
}

meHTVRLink *meHTVRSlot::GetLinkWithDestination(const meHTVRule *rule, int slot) const{
	return pLinks.FindOrDefault([&](const meHTVRLink &link){
		return link.GetDestinationRule() == rule && link.GetDestinationSlot() == slot;
	});
}
