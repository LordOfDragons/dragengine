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

#include "skyeWindowCurves.h"
#include "skyeWindowCurvesListener.h"
#include "../skyeWindowMain.h"
#include "../../sky/skyeSky.h"
#include "../../sky/link/skyeLink.h"
#include "../../undosys/link/skyeULinkSetCurve.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeApplication.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/curveedit/igdeViewCurveBezier.h>
#include <deigde/gui/curveedit/igdeViewCurveBezierListener.h>
#include <deigde/gui/event/igdeListBoxListener.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/common/exceptions.h>



// Events
///////////

namespace{

class cListLinks : public igdeListBoxListener{
	skyeWindowCurves &pPanel;
public:
	typedef deTObjectReference<cListLinks> Ref;
	cListLinks(skyeWindowCurves &panel) : pPanel(panel){}
	
	virtual void OnSelectionChanged(igdeListBox *listBox){
		skyeSky * const sky = pPanel.GetSky();
		if(!sky){
			return;
		}
		
		const igdeListItem * const selection = listBox->GetSelectedItem();
		if(selection){
			sky->SetActiveLink((skyeLink*)selection->GetData());
			
		}else{
			sky->SetActiveLink(nullptr);
		}
	}
	
	virtual void AddContextMenuEntries(igdeListBox*, igdeMenuCascade&){
	}
};

class cViewCurve : public igdeViewCurveBezierListener{
	skyeWindowCurves &pPanel;
	igdeUndo::Ref &pUndo;
public:
	typedef deTObjectReference<cViewCurve> Ref;
	cViewCurve(skyeWindowCurves &panel, igdeUndo::Ref &undo) :
	pPanel(panel),
	pUndo(undo){
	}
	
	virtual void OnCurveChanged(igdeViewCurveBezier *viewCurveBezier){
		if(pUndo){
			((skyeULinkSetCurve&)(igdeUndo&)pUndo).SetNewCurve(viewCurveBezier->GetCurve());
			
		}else if(!pPanel.GetLink() || pPanel.GetLink()->GetCurve() == viewCurveBezier->GetCurve()){
			return;
			
		}else{
			pUndo = skyeULinkSetCurve::Ref::New(pPanel.GetLink(), viewCurveBezier->GetCurve());
		}
		
		pPanel.GetSky()->GetUndoSystem()->Add(pUndo);
		pUndo = nullptr;
	}
	
	virtual void OnCurveChanging(igdeViewCurveBezier *viewCurveBezier){
		if(pUndo){
			((skyeULinkSetCurve&)(igdeUndo&)pUndo).SetNewCurve(viewCurveBezier->GetCurve());
			pUndo->Redo();
			
		}else if(pPanel.GetLink() && pPanel.GetLink()->GetCurve() != viewCurveBezier->GetCurve()){
			pUndo = skyeULinkSetCurve::Ref::New(pPanel.GetLink(), viewCurveBezier->GetCurve());
		}
	}
};

}



// Class skyeWindowCurves
/////////////////////////

// Constructor, destructor
////////////////////////////

skyeWindowCurves::skyeWindowCurves(skyeWindowMain &windowMain) :
igdeContainerSplitted(windowMain.GetEnvironment(), igdeContainerSplitted::espRight,
	igdeApplication::app().DisplayScaled(200)),
pWindowMain(windowMain)
{
	igdeEnvironment &env = windowMain.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	
	pListener = skyeWindowCurvesListener::Ref::New(*this);
	
	helper.ListBox(5, "Link to edit", pListLinks, cListLinks::Ref::New(*this));
	pListLinks->SetDefaultSorter();
	AddChild(pListLinks, igdeContainerSplitted::eaSide);
	
	helper.ViewCurveBezier(pEditCurve, cViewCurve::Ref::New(*this, pUndoSetCurve));
	pEditCurve->SetClampMin(decVector2(0.0f, 0.0f));
	pEditCurve->SetClampMax(decVector2(1.0f, 1.0f));
	pEditCurve->SetClamp(true);
	AddChild(pEditCurve, igdeContainerSplitted::eaCenter);
}

skyeWindowCurves::~skyeWindowCurves(){
	SetSky(nullptr);
}



// Management
///////////////

void skyeWindowCurves::SetSky(skyeSky *sky){
	if(sky == pSky){
		return;
	}
	
	pUndoSetCurve = nullptr;
	
	if(pSky){
		pSky->RemoveListener(pListener);
	}
	
	pSky = sky;
	
	if(sky){
		sky->AddListener(pListener);
	}
	
	UpdateLinkList();
}



skyeLink *skyeWindowCurves::GetLink() const{
	return pSky ? pSky->GetActiveLink() : nullptr;
}

void skyeWindowCurves::UpdateLinkList(){
	pListLinks->RemoveAllItems();
	
	if(pSky){
		pSky->GetLinks().Visit([&](skyeLink *link){
			pListLinks->AddItem(link->GetName(), nullptr, link);
		});
		
		pListLinks->SortItems();
	}
	
	SelectActiveLink();
}

void skyeWindowCurves::SelectActiveLink(){
	pListLinks->SetSelectionWithData(GetLink());
	pListLinks->MakeSelectionVisible();
	UpdateCurve();
}

void skyeWindowCurves::UpdateCurve(){
	const skyeLink * const link = GetLink();
	
	if(link){
		pEditCurve->SetCurve(link->GetCurve());
		
	}else{
		pEditCurve->SetDefaultBezier();
	}
	
	pEditCurve->SetEnabled(link != nullptr);
}
