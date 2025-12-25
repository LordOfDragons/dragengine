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

#include "skyeWPSky.h"
#include "skyeWPSkyListener.h"
#include "skyeWindowProperties.h"
#include "../skyeWindowMain.h"
#include "../../sky/skyeSky.h"
#include "../../undosys/sky/skyeUSkySetBgColor.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeColorBox.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeGroupBox.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/event/igdeColorBoxListener.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndo.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Events
///////////


namespace{

class cColorBackground : public igdeColorBoxListener{
protected:
	skyeWPSky &pPanel;
	
public:
	typedef deTObjectReference<cColorBackground> Ref;
	cColorBackground(skyeWPSky &panel) : pPanel(panel){}
	
	virtual void OnColorChanged(igdeColorBox *colorBox){
		skyeSky * const sky = pPanel.GetSky();
		if(!sky){
			return;
		}
		
		if(colorBox->GetColor().IsEqualTo(sky->GetBgColor())){
			return;
		}
		
		skyeUSkySetBgColor::Ref undo(skyeUSkySetBgColor::Ref::New(sky, colorBox->GetColor()));
		if(undo){
			sky->GetUndoSystem()->Add(undo);
		}
	}
};

}



// Class skyeWPSky
//////////////////

// Constructor, destructor
////////////////////////////

skyeWPSky::skyeWPSky(skyeWindowProperties &windowProperties) :
igdeContainerScroll(windowProperties.GetEnvironment(), false, true),
pWindowProperties(windowProperties)
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeContainer::Ref content, groupBox, frameLine;
	igdeUIHelper &helper = env.GetUIHelperProperties();
	
	pListener = skyeWPSkyListener::Ref::New(*this);
	
	content = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaY);
	AddChild(content);
	
	helper.GroupBox(content, groupBox, "Sky:");
	
	helper.ColorBox(groupBox, "Bg Color:", "Background Color.", pClrBg, cColorBackground::Ref::New(*this));
}

skyeWPSky::~skyeWPSky(){
	SetSky(nullptr);
}



// Management
///////////////

void skyeWPSky::SetSky(skyeSky *sky){
	if(sky == pSky){
		return;
	}
	
	if(pSky){
		pSky->RemoveListener(pListener);
		pSky = nullptr;
	}
	
	pSky = sky;
	
	if(sky){
		sky->AddListener(pListener);
	}
	
	UpdateSky();
}



void skyeWPSky::UpdateSky(){
	if(pSky){
		pClrBg->SetColor(pSky->GetBgColor());
		
	}else{
		pClrBg->SetColor(decColor(1.0f, 1.0f, 1.0f));
	}
	
	const bool enabled = pSky != nullptr;
	
	pClrBg->SetEnabled(enabled);
}
