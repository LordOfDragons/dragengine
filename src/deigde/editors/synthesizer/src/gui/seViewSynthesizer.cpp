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

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "seViewSynthesizer.h"
#include "seWindowMain.h"
#include "properties/seWPSynthesizer.h"
#include "properties/seWPController.h"
#include "properties/seWPLink.h"
#include "properties/seWPSource.h"
#include "../synthesizer/seSynthesizer.h"
#include "../configuration/seConfiguration.h"

#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/layout/igdeContainerBox.h>
#include <deigde/gui/theme/themeNames.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Class seViewSynthesizer
////////////////////////////

// Constructor, destructor
////////////////////////////

seViewSynthesizer::seViewSynthesizer(seWindowMain &windowMain) :
igdeContainerBorder(windowMain.GetEnvironment(), 10),
pWindowMain(windowMain),
pSynthesizer(NULL),
pWPController(NULL),
pWPLink(NULL),
pWPSource(NULL),
pWPSynthesizer(NULL)
{
	igdeEnvironment &env = windowMain.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	
	SetWidgetGuiThemeName(igdeGuiThemeNames::properties);
	
	// content
	igdeContainerBox::Ref panels(igdeContainerBox::Ref::NewWith(env, igdeContainerBox::eaX, 10));
	AddChild(panels, eaCenter);
	
	pWPController.TakeOver(new seWPController(*this));
	panels->AddChild(pWPController);
	helper.Separator(panels, false);
	
	pWPLink.TakeOver(new seWPLink(*this));
	panels->AddChild(pWPLink);
	helper.Separator(panels, false);
	
	pWPSource.TakeOver(new seWPSource(*this));
	panels->AddChild(pWPSource);
	helper.Separator(panels, false);
	
	// right
	pWPSynthesizer.TakeOver(new seWPSynthesizer(*this));
	AddChild(pWPSynthesizer, eaRight);
}

seViewSynthesizer::~seViewSynthesizer(){
	SetSynthesizer(NULL);
	
}



// Management
///////////////

void seViewSynthesizer::SetSynthesizer(seSynthesizer *synthesizer){
	if(pSynthesizer == synthesizer){
		return;
	}
	
	
	pSynthesizer = synthesizer;
	
	pWPController->SetSynthesizer(synthesizer);
	pWPLink->SetSynthesizer(synthesizer);
	pWPSource->SetSynthesizer(synthesizer);
	pWPSynthesizer->SetSynthesizer(synthesizer);
}

void seViewSynthesizer::OnSynthesizerPathChanged(){
	pWPSource->OnSynthesizerPathChanged();
}



void seViewSynthesizer::ResetView(){
}

void seViewSynthesizer::OnFrameUpdate(float elapsed){
	if(pSynthesizer){
		pSynthesizer->Update(elapsed);
	}
}
