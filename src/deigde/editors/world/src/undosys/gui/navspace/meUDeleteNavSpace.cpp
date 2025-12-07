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

#include "meUDeleteNavSpace.h"
#include "meUndoDataNavSpace.h"
#include "../../../world/meWorld.h"
#include "../../../world/navspace/meNavigationSpace.h"
#include "../../../world/navspace/meNavigationSpaceSelection.h"

#include <dragengine/common/exceptions.h>



// Class meUDeleteNavSpace
////////////////////////////

// Constructor, destructor
////////////////////////////

meUDeleteNavSpace::meUDeleteNavSpace(meWorld *world) :
pWorld(NULL),
pNavSpaces(NULL),
pNavSpaceCount(0)
{
	if(! world){
		DETHROW(deeInvalidParam);
	}
	
	const meNavigationSpaceList &list = world->GetSelectionNavigationSpace().GetSelected();
	int count = list.GetCount();
	decString text;
	
	SetShortInfo("Delete Objects");
	if(count > 1){
		text.Format("%i objects", count);
		
	}else{
		text = "1 object";
	}
	SetLongInfo(text);
	
	try{
		pWorld = world;
		world->AddReference();
		
		if(count > 0){
			pNavSpaces = new meUndoDataNavSpace*[count];
			
			while(pNavSpaceCount < count){
				pNavSpaces[pNavSpaceCount] = new meUndoDataNavSpace(list.GetAt(pNavSpaceCount));
				pNavSpaceCount++;
			}
		}
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

meUDeleteNavSpace::~meUDeleteNavSpace(){
	pCleanUp();
}



// Management
///////////////

void meUDeleteNavSpace::Undo(){
	meNavigationSpaceSelection &selection = pWorld->GetSelectionNavigationSpace();
	int i;
	
	selection.Reset();
	
	for(i=0; i<pNavSpaceCount; i++){
		meNavigationSpace * const navspace = pNavSpaces[i]->GetNavSpace();
		
		pWorld->AddNavSpace(navspace);
		selection.Add(navspace);
		
		pWorld->NotifyNavSpaceCountChanged();
	}
	
	selection.ActivateNext();
	
	pWorld->NotifyNavSpaceSelectionChanged();
}

void meUDeleteNavSpace::Redo(){
	meNavigationSpaceSelection &selection = pWorld->GetSelectionNavigationSpace();
	int i;
	
	for(i=0; i<pNavSpaceCount; i++){
		meNavigationSpace * const navspace = pNavSpaces[i]->GetNavSpace();
		
		selection.Remove(navspace);
		if(navspace->GetActive()){
			selection.ActivateNext();
		}
		
		pWorld->RemoveNavSpace(pNavSpaces[i]->GetNavSpace());
		
		pWorld->NotifyNavSpaceCountChanged();
	}
	
	pWorld->NotifyNavSpaceSelectionChanged();
}



// Private Functions
//////////////////////

void meUDeleteNavSpace::pCleanUp(){
	if(pNavSpaces){
		while(pNavSpaceCount > 0){
			pNavSpaceCount--;
			delete pNavSpaces[pNavSpaceCount];
		}
		
		delete [] pNavSpaces;
	}
	
	if(pWorld){
		pWorld->FreeReference();
	}
}
