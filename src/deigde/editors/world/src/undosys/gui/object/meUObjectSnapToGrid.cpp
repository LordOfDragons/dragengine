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

#include "meUObjectSnapToGrid.h"
#include "../../../world/meWorld.h"
#include "../../../world/object/meObject.h"
#include "../../../world/object/meObjectSelection.h"

#include <dragengine/common/exceptions.h>



// Class meUObjectSnapToGrid
//////////////////////////////

// Constructor, destructor
////////////////////////////

meUObjectSnapToGrid::meUObjectSnapToGrid(meWorld *world, const meObject::List &objects, float grid) :

pGrid((double)grid)
{
	DEASSERT_NOTNULL(world)
	DEASSERT_TRUE(objects.IsNotEmpty())
	
	SetShortInfo("Snap objects to grid");
	
	meUndoDataObject::AddObjectsWithAttachments(objects, pObjects);
	
	pWorld = world;
}

meUObjectSnapToGrid::~meUObjectSnapToGrid(){
}



// Management
///////////////

void meUObjectSnapToGrid::Undo(){
	meUndoDataObject::RestoreOldGeometry(pObjects, *pWorld);
}

void meUObjectSnapToGrid::Redo(){
	pObjects.Visit([&](const meUndoDataObject &data){
		decDVector position(data.GetOldPosition());
		position.Snap(pGrid);
		
		const decDVector offset = position - data.GetOldPosition();
		
		data.GetObject()->SetPosition(position);
		pWorld->NotifyObjectGeometryChanged(data.GetObject());
		
		data.GetAttachedObjects().Visit([&](const meUndoDataObject &attachedData){
			attachedData.GetObject()->SetPosition(attachedData.GetOldPosition() + offset);
			pWorld->NotifyObjectGeometryChanged(attachedData.GetObject());
		});
	});
}
