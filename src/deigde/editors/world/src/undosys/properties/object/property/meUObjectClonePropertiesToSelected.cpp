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

#include "meUObjectClonePropertiesToSelected.h"
#include "meUndoDataObjectProperty.h"
#include "../../../../world/meWorld.h"
#include "../../../../world/object/meObject.h"

#include <dragengine/common/exceptions.h>



// Class meUObjectClonePropertiesToSelected
/////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

meUObjectClonePropertiesToSelected::meUObjectClonePropertiesToSelected(
const meObjectList &list, const decStringDictionary &properties){
	meUndoDataObjectProperty *undoData = NULL;
	const int count = list.GetCount();
	meObject *object;
	int i;
	
	if(count < 1){
		DETHROW(deeInvalidParam);
	}
	
	try{
		SetShortInfo("Clone object properties to selected");
		pProperties = properties;
		
		for(i=0; i<count; i++){
			object = list.GetAt(i);
			if(! object->GetWorld()){
				DETHROW(deeInvalidParam);
			}
			
			undoData = new meUndoDataObjectProperty(object);
			undoData->GetOldProperties() = object->GetProperties();
			
			pList.Add(undoData);
			undoData->FreeReference();
			undoData = NULL;
		}
		
	}catch(const deException &){
		if(undoData){
			undoData->FreeReference();
		}
		pCleanUp();
		throw;
	}
}

meUObjectClonePropertiesToSelected::~meUObjectClonePropertiesToSelected(){
	pCleanUp();
}



// Management
///////////////

void meUObjectClonePropertiesToSelected::Undo(){
	const int count = pList.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		const meUndoDataObjectProperty &undoData = *((meUndoDataObjectProperty*)pList.GetAt(i));
		
		undoData.GetObject()->SetProperties(undoData.GetOldProperties());
	}
}

void meUObjectClonePropertiesToSelected::Redo(){
	const int count = pList.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		const meUndoDataObjectProperty &undoData = *((meUndoDataObjectProperty*)pList.GetAt(i));
		
		undoData.GetObject()->SetProperties(pProperties);
	}
}



// Private Functions
//////////////////////

void meUObjectClonePropertiesToSelected::pCleanUp(){
	pList.RemoveAll();
}
