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

#include "meUDecalClonePropertiesToSelected.h"
#include "meUndoDataDecalProperty.h"
#include "../../../../world/meWorld.h"
#include "../../../../world/decal/meDecal.h"

#include <dragengine/common/exceptions.h>



// Class meUDecalClonePropertiesToSelected
////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

meUDecalClonePropertiesToSelected::meUDecalClonePropertiesToSelected(
const meDecalList &list, const decStringDictionary &properties){
	meUndoDataDecalProperty *undoData = NULL;
	const int count = list.GetCount();
	meDecal *decal;
	int i;
	
	if(count < 1){
		DETHROW(deeInvalidParam);
	}
	
	try{
		SetShortInfo("Clone decal properties to selected");
		pPropertyList = properties;
		
		for(i=0; i<count; i++){
			decal = list.GetAt(i);
			if(!decal->GetWorld()){
				DETHROW(deeInvalidParam);
			}
			
			undoData = new meUndoDataDecalProperty(decal);
			undoData->GetOldProperties() = decal->GetProperties();
			
			pList.Add(undoData);
			undoData->FreeReference();
			undoData = NULL;
		}
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

meUDecalClonePropertiesToSelected::~meUDecalClonePropertiesToSelected(){
	pCleanUp();
}



// Management
///////////////

void meUDecalClonePropertiesToSelected::Undo(){
	const int count = pList.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		const meUndoDataDecalProperty &undoData = *((meUndoDataDecalProperty*)pList.GetAt(i));
		undoData.GetDecal()->SetProperties(undoData.GetOldProperties());
	}
}

void meUDecalClonePropertiesToSelected::Redo(){
	const int count = pList.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		const meUndoDataDecalProperty &undoData = *((meUndoDataDecalProperty*)pList.GetAt(i));
		undoData.GetDecal()->SetProperties(pPropertyList);
	}
}



// Private Functions
//////////////////////

void meUDecalClonePropertiesToSelected::pCleanUp(){
	pList.RemoveAll();
}
