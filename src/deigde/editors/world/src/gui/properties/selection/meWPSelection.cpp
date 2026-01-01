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
#include <string.h>

#include "meWPSelection.h"
#include "meWPSelectionListener.h"
#include "decal/meWPSDecal.h"
#include "navspace/meWPSNavSpace.h"
#include "object/meWPSObject.h"
#include "objectshape/meWPSObjectShape.h"
#include "../meWindowProperties.h"
#include "../../meWindowMain.h"
#include "../../../world/meWorld.h"
#include "../../../world/meWorldGuiParameters.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>

#include <dragengine/common/exceptions.h>



// Class meWPSelection
/////////////////////////

// Constructor, destructor
////////////////////////////

meWPSelection::meWPSelection(meWindowProperties &windowProperties) :
igdeSwitcher(windowProperties.GetEnvironment()),
pWindowProperties(windowProperties)
{
	pListener = meWPSelectionListener::Ref::New(*this);
	
	GetEnvironment().GetUIHelper().Label(*this, "No Selection");
	
	pPanelObject = meWPSObject::Ref::New(*this);
	AddChild(pPanelObject);
	
	pPanelObjectShape = meWPSObjectShape::Ref::New(*this);
	AddChild(pPanelObjectShape);
	
	pPanelDecal = meWPSDecal::Ref::New(*this);
	AddChild(pPanelDecal);
	
	pPanelNavSpace = meWPSNavSpace::Ref::New(*this);
	AddChild(pPanelNavSpace);
	
	SetCurrent(0);  // empty
}

meWPSelection::~meWPSelection(){
	SetWorld(nullptr);
}



// Management
///////////////

void meWPSelection::SetWorld(meWorld *world){
	if(world == pWorld){
		return;
	}
	
	pPanelObject->SetWorld(nullptr);
	pPanelObjectShape->SetWorld(nullptr);
	pPanelDecal->SetWorld(nullptr);
	pPanelNavSpace->SetWorld(nullptr);
	
	if(pWorld){
		pWorld->RemoveNotifier(pListener);
	}
	
	pWorld = world;
	
	if(world){
		world->AddNotifier(pListener);
	}
	
	pPanelObject->SetWorld(world);
	pPanelObjectShape->SetWorld(world);
	pPanelDecal->SetWorld(world);
	pPanelNavSpace->SetWorld(world);
	
	ElementModeChanged();
}



void meWPSelection::ElementModeChanged(){
	if(!pWorld){
		SetCurrent(0); // none
		return;
	}
	
	switch(pWorld->GetGuiParameters().GetElementMode()){
	case meWorldGuiParameters::eemObject:
		SetCurrent(1);
		break;
		
	case meWorldGuiParameters::eemObjectShape:
		SetCurrent(2);
		break;
		
	case meWorldGuiParameters::eemDecal:
		SetCurrent(3);
		break;
		
	case meWorldGuiParameters::eemNavSpace:
		SetCurrent(4);
		break;
		
	default:
		SetCurrent(0); // none
	}
}

void meWPSelection::OnGameDefinitionChanged(){
	pPanelObject->OnGameDefinitionChanged();
	pPanelObjectShape->OnGameDefinitionChanged();
	pPanelDecal->OnGameDefinitionChanged();
}
