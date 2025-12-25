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

#include "seWindowProperties.h"
#include "seWPSky.h"
#include "seWPLink.h"
#include "seWPLayer.h"
#include "seWPController.h"
#include "seWPUndoHistory.h"
#include "seWPView.h"
#include "../seWindowMain.h"
#include "../../sky/seSky.h"

#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/layout/igdeContainerBox.h>
#include <deigde/gui/theme/themeNames.h>

#include <dragengine/common/exceptions.h>



// Class seWindowProperties
/////////////////////////////

// Constructor, destructor
////////////////////////////

seWindowProperties::seWindowProperties(seWindowMain &windowMain) :
igdeTabBook(windowMain.GetEnvironment()),
pWindowMain(windowMain)
{
	SetWidgetGuiThemeName(igdeGuiThemeNames::properties);
	
	pPanelController = seWPController::Ref::New(*this);
	AddChild(pPanelController, "Controller");
	
	pPanelLink = seWPLink::Ref::New(*this);
	AddChild(pPanelLink, "Link");
	
	pPanelLayer = seWPLayer::Ref::New(*this);
	AddChild(pPanelLayer, "Layer");
	
	pPanelSky = seWPSky::Ref::New(*this);
	AddChild(pPanelSky, "Sky");
	
	pPanelView = seWPView::Ref::New(*this);
	AddChild(pPanelView, "View");
	
	pPanelUndoHistory = seWPUndoHistory::Ref::New(GetEnvironment());
	AddChild(pPanelUndoHistory, "Undo");
	
	SetActivePanel(0); // controller
}

seWindowProperties::~seWindowProperties(){
}



// Management
///////////////

void seWindowProperties::SetSky(seSky *sky){
	pPanelController->SetSky(sky);
	pPanelLink->SetSky(sky);
	pPanelLayer->SetSky(sky);
	pPanelSky->SetSky(sky);
	pPanelView->SetSky(sky);
	pPanelUndoHistory->SetSky(sky);
}

void seWindowProperties::OnSkyPathChanged(){
	pPanelLayer->OnSkyPathChanged();
}
