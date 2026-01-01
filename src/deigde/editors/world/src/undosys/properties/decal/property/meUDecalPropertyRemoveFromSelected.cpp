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

#include "meUDecalPropertyRemoveFromSelected.h"
#include "meUndoDataDecalProperty.h"
#include "../../../../world/meWorld.h"
#include "../../../../world/decal/meDecal.h"

#include <dragengine/common/exceptions.h>



// Class meUDecalPropertyRemoveFromSelected
/////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

meUDecalPropertyRemoveFromSelected::meUDecalPropertyRemoveFromSelected(const meDecal::List &list, const char *key){
	DEASSERT_TRUE(list.IsNotEmpty())
	
	SetShortInfo("Remove decal property from selected");
	pKey = key;
	
	list.Visit([&](meDecal *decal){
		DEASSERT_NOTNULL(decal->GetWorld())
		
		const decStringDictionary &properties = decal->GetProperties();
		
		const meUndoDataDecalProperty::Ref udata(meUndoDataDecalProperty::Ref::New(decal));
		udata->SetPropertyExists(properties.Has(key));
		if(udata->GetPropertyExists()){
			udata->SetOldValue(properties.GetAt(key));
		}
		
		pList.Add(udata);
	});
}

meUDecalPropertyRemoveFromSelected::~meUDecalPropertyRemoveFromSelected(){
}



// Management
///////////////

void meUDecalPropertyRemoveFromSelected::Undo(){
	pList.Visit([&](const meUndoDataDecalProperty &data){
		if(data.GetPropertyExists()){
			data.GetDecal()->SetProperty(pKey, data.GetOldValue());
		}
	});
}

void meUDecalPropertyRemoveFromSelected::Redo(){
	pList.Visit([&](const meUndoDataDecalProperty &data){
		if(data.GetPropertyExists()){
			data.GetDecal()->RemoveProperty(pKey);
		}
	});
}
