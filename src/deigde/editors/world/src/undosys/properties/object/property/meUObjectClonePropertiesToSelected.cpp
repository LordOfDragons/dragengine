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
const meObject::List &list, const decStringDictionary &properties){
	DEASSERT_TRUE(list.IsNotEmpty())
	
	SetShortInfo("Clone object properties to selected");
	pProperties = properties;
	
	list.Visit([&](meObject *object){
		DEASSERT_NOTNULL(object->GetWorld())
		
		const meUndoDataObjectProperty:: Ref udata(meUndoDataObjectProperty::Ref::New(object));
		udata->GetOldProperties() = object->GetProperties();
		
		pList.Add(udata);
	});
}

meUObjectClonePropertiesToSelected::~meUObjectClonePropertiesToSelected(){
}



// Management
///////////////

void meUObjectClonePropertiesToSelected::Undo(){
	pList.Visit([&](const meUndoDataObjectProperty &data){
		data.GetObject()->SetProperties(data.GetOldProperties());
	});
}

void meUObjectClonePropertiesToSelected::Redo(){
	pList.Visit([&](const meUndoDataObjectProperty &data){
		data.GetObject()->SetProperties(pProperties);
	});
}
