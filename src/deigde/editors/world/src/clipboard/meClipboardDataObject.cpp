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

#include <stdlib.h>

#include "meClipboardDataObject.h"
#include "../world/meWorld.h"
#include "../world/object/meObject.h"
#include "../world/object/meObjectSelection.h"

#include <dragengine/common/exceptions.h>



// Class meClipboardDataObject
////////////////////////////////

const char * const meClipboardDataObject::TYPE_NAME = "object";

// Constructor, destructor
////////////////////////////

meClipboardDataObject::meClipboardDataObject(const meWorld &world) :
igdeClipboardData(TYPE_NAME)
{
	const meObject::List &list = world.GetSelectionObject().GetSelected();
	DEASSERT_TRUE(list.IsNotEmpty())
	
	list.Visit([&](const meObject &object){
		const meCDOObject::Ref cdo(meCDOObject::Ref::New(object));
		
		if(object.GetAttachedTo()){
			// if the object is attached to another copied object store the relative index
			cdo->SetAttachToIndex(list.IndexOf(object.GetAttachedTo()));
			
			// otherwise the object is attached to a not copied object.
			// in this case store the hex-string id of the object
			if(cdo->GetAttachToIndex() == -1){
				cdo->SetAttachToID(object.GetAttachedTo()->GetID().ToHexString());
			}
		}
		
		pObjects.Add(cdo);
	});
}

meClipboardDataObject::~meClipboardDataObject(){
}
