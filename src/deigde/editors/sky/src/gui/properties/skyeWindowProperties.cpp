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

#include "skyeWindowProperties.h"
#include "skyeWPSky.h"
#include "skyeWPLink.h"
#include "skyeWPLayer.h"
#include "skyeWPController.h"
#include "skyeWPUndoHistory.h"
#include "skyeWPView.h"
#include "../skyeWindowMain.h"
#include "../../sky/skyeSky.h"

#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/layout/igdeContainerBox.h>
#include <deigde/gui/theme/themeNames.h>

#include <dragengine/common/exceptions.h>



// Class skyeWindowProperties
/////////////////////////////

// Constructor, destructor
////////////////////////////

skyeWindowProperties::skyeWindowProperties(skyeWindowMain &windowMain) :
igdeTabBook(windowMain.GetEnvironment()),
pWindowMain(windowMain)
{
	SetWidgetGuiThemeName(igdeGuiThemeNames::properties);
	
	pPanelController = skyeWPController::Ref::New(*this);
	AddChild(pPanelController, "Controller");
	
	pPanelLink = skyeWPLink::Ref::New(*this);
	AddChild(pPanelLink, "Link");
	
	pPanelLayer = skyeWPLayer::Ref::New(*this);
	AddChild(pPanelLayer, "Layer");
	
	pPanelSky = skyeWPSky::Ref::New(*this);
	AddChild(pPanelSky, "Sky");
	
	pPanelView = skyeWPView::Ref::New(*this);
	AddChild(pPanelView, "View");
	
	pPanelUndoHistory = skyeWPUndoHistory::Ref::New(GetEnvironment());
	AddChild(pPanelUndoHistory, "Undo");
	
	SetActivePanel(0); // controller
}

skyeWindowProperties::~skyeWindowProperties(){
}



// Management
///////////////

void skyeWindowProperties::SetSky(skyeSky *sky){
	pPanelController->SetSky(sky);
	pPanelLink->SetSky(sky);
	pPanelLayer->SetSky(sky);
	pPanelSky->SetSky(sky);
	pPanelView->SetSky(sky);
	pPanelUndoHistory->SetSky(sky);
}

void skyeWindowProperties::OnSkyPathChanged(){
	pPanelLayer->OnSkyPathChanged();
}
