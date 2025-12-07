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
#include <string.h>

#include "dedaiCostTable.h"

#include <dragengine/common/exceptions.h>



// Class dedaiCostTable
/////////////////////////

// Constructors and Destructors
/////////////////////////////////

dedaiCostTable::dedaiCostTable() :
pChanged(false){
}

dedaiCostTable::~dedaiCostTable(){
}



// Management
///////////////

void dedaiCostTable::ClearChanged(){
	pChanged = false;
}

int dedaiCostTable::GetTypeCount() const{
	return pTypes.GetCount();
}

int dedaiCostTable::GetTypeAt(int index) const{
	return pTypes.GetAt(index);
}

int dedaiCostTable::GetTypeWith(int typeNumber){
	int index = pTypes.IndexOf(typeNumber);
	
	if(index == -1){
		index = pTypes.GetCount();
		pTypes.Add(typeNumber);
	}
	
	return index;
}

int dedaiCostTable::GetTypeWith(int typeNumber, int defaultValue) const{
	const int index = pTypes.IndexOf(typeNumber);
	if(index == -1){
		return defaultValue;
	}
	return index;
}

void dedaiCostTable::RemoveAllTypes(){
	pTypes.RemoveAll();
}
