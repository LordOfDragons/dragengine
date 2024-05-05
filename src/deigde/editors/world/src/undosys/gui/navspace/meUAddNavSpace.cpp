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

#include "meUAddNavSpace.h"
#include "../../../world/meWorld.h"
#include "../../../world/navspace/meNavigationSpace.h"
#include "../../../world/navspace/meNavigationSpaceSelection.h"

#include <dragengine/common/exceptions.h>



// Class meUAddNavSpace
/////////////////////////

// Constructor, destructor
////////////////////////////

meUAddNavSpace::meUAddNavSpace( meWorld *world, const decPoint3 &sector, meNavigationSpace *object ) :
pWorld( NULL ),
pNavSpace( NULL )
{
	if( ! world || ! object ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Add Navigation Space" );
	
	pWorld = world;
	world->AddReference();
	
	pNavSpace = object;
	object->AddReference();
}

meUAddNavSpace::~meUAddNavSpace(){
	pCleanUp();
}



// Management
///////////////

void meUAddNavSpace::Undo(){
	meNavigationSpaceSelection &selection = pWorld->GetSelectionNavigationSpace();
	
	selection.Remove( pNavSpace );
	if( pNavSpace->GetActive() ){
		selection.ActivateNext();
	}
	
	pWorld->RemoveNavSpace( pNavSpace );
	
	pWorld->NotifyNavSpaceCountChanged();
	pWorld->NotifyNavSpaceSelectionChanged();
}

void meUAddNavSpace::Redo(){
	meNavigationSpaceSelection &selection = pWorld->GetSelectionNavigationSpace();
	
	pWorld->AddNavSpace( pNavSpace );
	
	selection.Reset();
	selection.Add( pNavSpace );
	selection.SetActive( pNavSpace );
	
	pWorld->NotifyNavSpaceCountChanged();
	pWorld->NotifyNavSpaceSelectionChanged();
}



// Private Functions
//////////////////////

void meUAddNavSpace::pCleanUp(){
	if( pNavSpace ){
		pNavSpace->FreeReference();
	}
	if( pWorld ){
		pWorld->FreeReference();
	}
}
