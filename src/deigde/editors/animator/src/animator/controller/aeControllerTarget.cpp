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

#include "aeControllerTarget.h"
#include "../aeAnimator.h"
#include "../link/aeLink.h"

#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/deAnimatorLink.h>
#include <dragengine/resources/animator/controller/deAnimatorControllerTarget.h>
#include <dragengine/common/exceptions.h>



// Class aeControllerTarget
/////////////////////////////

// Constructor, destructor
////////////////////////////

aeControllerTarget::aeControllerTarget(igdeMetaPropertyObjectSetStorage<aeLink>::Storage &storage) :
pStorage(storage){
}

aeControllerTarget::aeControllerTarget(igdeMetaPropertyObjectSetStorage<aeLink>::Storage &storage, const aeControllerTarget &copy) :
pStorage(storage),
pLinks(copy.pLinks)
{
	pStorage.SetValue(copy.pStorage, false);
}

aeControllerTarget::~aeControllerTarget() = default;


// Management
///////////////

void aeControllerTarget::AddLink(aeLink *link){
	auto list = pStorage.GetValue();
	list.Add(link);
	pStorage = list;
}

void aeControllerTarget::RemoveLink(aeLink *link){
	auto list = pStorage.GetValue();
	list.Remove(link);
	pStorage = list;
}

void aeControllerTarget::RemoveAllLinks(){
	pStorage = {};
}

void aeControllerTarget::AddLinksToList(aeLink::List &list){
	list += pLinks;
}

void aeControllerTarget::OnStorageChanged(){
	pLinks.RemoveAll();
	pLinks.AddAll(pStorage.GetValue());
}


// Operators
//////////////

aeControllerTarget &aeControllerTarget::operator=(const aeControllerTarget &copy){
	pStorage = copy.pStorage;
	return *this;
}
