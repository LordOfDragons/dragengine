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

#include "deoglCollideList.h"
#include "deoglCollideListManager.h"

#include <dragengine/common/exceptions.h>


// Class deoglCollideListManager::sList
/////////////////////////////////////////

deoglCollideListManager::sList::sList(sList &&other) noexcept :
list(std::move(other.list)), inUse(other.inUse)
{
	other.inUse = false;
}

deoglCollideListManager::sList &deoglCollideListManager::sList::operator=(sList &&other) noexcept{
	if(this != &other){
		list = std::move(other.list);
		inUse = other.inUse;
		other.inUse = false;
	}
	return *this;
}


// class deoglCollideListManager
//////////////////////////////////

// constructor, destructor
////////////////////////////

deoglCollideListManager::deoglCollideListManager() = default;

deoglCollideListManager::~deoglCollideListManager(){
	const int inUseCount = pLists.Inject(0, [&](int accum, const sList &item){
		return accum + (item.inUse ? 1 : 0);
	});
	if(inUseCount > 0){
		printf("[OPENGL] WARNING!%i collide lists have not been released!\n", inUseCount);
	}
}



// list management
////////////////////

deoglCollideList *deoglCollideListManager::GetList(){
	int index = pLists.IndexOfMatching([](const sList &item){
		return !item.inUse;
	});
	
	if(index == -1){
		pLists.Add(deTUniqueReference<sList>::New());
		index = pLists.GetCount() - 1;
	}
	pLists[index]->inUse = true;
	return pLists[index]->list;
}

void deoglCollideListManager::ReleaseList(deoglCollideList *list){
	DEASSERT_NOTNULL(list)
	
	const int index = pLists.IndexOfMatching([list](const sList &item){
		return item.list == list;
	});
	
	if(index != -1){
		DEASSERT_TRUE(pLists[index]->inUse)
		pLists[index]->inUse = false;
	}
}
