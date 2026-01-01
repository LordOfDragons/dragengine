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
#include <string.h>

#include "skyeWPLink.h"
#include "skyeWPLinkListener.h"
#include "../../sky/skyeSky.h"
#include "../../sky/link/skyeLink.h"

#include <dragengine/common/exceptions.h>



// Class skyeWPLinkListener
///////////////////////////

// Constructor, destructor
////////////////////////////

skyeWPLinkListener::skyeWPLinkListener(skyeWPLink &panel) :
pPanel(panel){
}

skyeWPLinkListener::~skyeWPLinkListener(){
}



// Management
///////////////

void skyeWPLinkListener::ControllerStructureChanged(skyeSky *sky){
	pPanel.UpdateControllerList();
}

void skyeWPLinkListener::ControllerNameChanged(skyeSky *sky, skyeController *controller){
	pPanel.UpdateControllerList();
}



void skyeWPLinkListener::LinkStructureChanged(skyeSky *sky){
	pPanel.UpdateLinkList();
}

void skyeWPLinkListener::LinkChanged(skyeSky *sky, skyeLink *link){
	if(link->GetActive()){
		pPanel.UpdateLink();
	}
}

void skyeWPLinkListener::LinkNameChanged(skyeSky* sky, skyeLink* link){
	pPanel.UpdateLinkList();
}

void skyeWPLinkListener::ActiveLinkChanged(skyeSky *sky){
	pPanel.SelectActiveLink();
}
