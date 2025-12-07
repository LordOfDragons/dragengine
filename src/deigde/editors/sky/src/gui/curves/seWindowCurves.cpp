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

#include "seWindowCurves.h"
#include "seWindowCurvesListener.h"
#include "../seWindowMain.h"
#include "../../sky/seSky.h"
#include "../../sky/link/seLink.h"
#include "../../undosys/link/seULinkSetCurve.h"

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
	seWindowCurves &pPanel;
public:
	cListLinks(seWindowCurves &panel) : pPanel(panel){}
	
	virtual void OnSelectionChanged(igdeListBox *listBox){
		seSky * const sky = pPanel.GetSky();
		if(! sky){
			return;
		}
		
		const igdeListItem * const selection = listBox->GetSelectedItem();
		if(selection){
			sky->SetActiveLink((seLink*)selection->GetData());
			
		}else{
			sky->SetActiveLink(NULL);
		}
	}
	
	virtual void AddContextMenuEntries(igdeListBox*, igdeMenuCascade&){
	}
};

class cViewCurve : public igdeViewCurveBezierListener{
	seWindowCurves &pPanel;
	igdeUndo::Ref &pUndo;
public:
	cViewCurve(seWindowCurves &panel, igdeUndo::Ref &undo) :
	pPanel(panel),
	pUndo(undo){
	}
	
	virtual void OnCurveChanged(igdeViewCurveBezier *viewCurveBezier){
		if(pUndo){
			((seULinkSetCurve&)(igdeUndo&)pUndo).SetNewCurve(viewCurveBezier->GetCurve());
			
		}else if(! pPanel.GetLink() || pPanel.GetLink()->GetCurve() == viewCurveBezier->GetCurve()){
			return;
			
		}else{
			pUndo.TakeOver(new seULinkSetCurve(pPanel.GetLink(), viewCurveBezier->GetCurve()));
		}
		
		pPanel.GetSky()->GetUndoSystem()->Add(pUndo);
		pUndo = NULL;
	}
	
	virtual void OnCurveChanging(igdeViewCurveBezier *viewCurveBezier){
		if(pUndo){
			((seULinkSetCurve&)(igdeUndo&)pUndo).SetNewCurve(viewCurveBezier->GetCurve());
			pUndo->Redo();
			
		}else if(pPanel.GetLink() && pPanel.GetLink()->GetCurve() != viewCurveBezier->GetCurve()){
			pUndo.TakeOver(new seULinkSetCurve(pPanel.GetLink(), viewCurveBezier->GetCurve()));
		}
	}
};

}



// Class seWindowCurves
/////////////////////////

// Constructor, destructor
////////////////////////////

seWindowCurves::seWindowCurves(seWindowMain &windowMain) :
igdeContainerSplitted(windowMain.GetEnvironment(), igdeContainerSplitted::espRight,
	igdeApplication::app().DisplayScaled(200)),
pWindowMain(windowMain),
pListener(NULL),
pSky(NULL)
{
	igdeEnvironment &env = windowMain.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	
	pListener = new seWindowCurvesListener(*this);
	
	helper.ListBox(5, "Link to edit", pListLinks, new cListLinks(*this));
	pListLinks->SetDefaultSorter();
	AddChild(pListLinks, igdeContainerSplitted::eaSide);
	
	helper.ViewCurveBezier(pEditCurve, new cViewCurve(*this, pUndoSetCurve));
	pEditCurve->SetClampMin(decVector2(0.0f, 0.0f));
	pEditCurve->SetClampMax(decVector2(1.0f, 1.0f));
	pEditCurve->SetClamp(true);
	AddChild(pEditCurve, igdeContainerSplitted::eaCenter);
}

seWindowCurves::~seWindowCurves(){
	SetSky(NULL);
	if(pListener){
		pListener->FreeReference();
	}
}



// Management
///////////////

void seWindowCurves::SetSky(seSky *sky){
	if(sky == pSky){
		return;
	}
	
	pUndoSetCurve = NULL;
	
	if(pSky){
		pSky->RemoveListener(pListener);
		pSky->FreeReference();
	}
	
	pSky = sky;
	
	if(sky){
		sky->AddListener(pListener);
		sky->AddReference();
	}
	
	UpdateLinkList();
}



seLink *seWindowCurves::GetLink() const{
	return pSky ? pSky->GetActiveLink() : NULL;
}

void seWindowCurves::UpdateLinkList(){
	pListLinks->RemoveAllItems();
	
	if(pSky){
		const seLinkList &links = pSky->GetLinks();
		const int count = links.GetCount();
		int i;
		
		for(i=0; i<count; i++){
			seLink * const link = links.GetAt(i);
			pListLinks->AddItem(link->GetName(), NULL, link);
		}
		
		pListLinks->SortItems();
	}
	
	SelectActiveLink();
}

void seWindowCurves::SelectActiveLink(){
	pListLinks->SetSelectionWithData(GetLink());
	pListLinks->MakeSelectionVisible();
	UpdateCurve();
}

void seWindowCurves::UpdateCurve(){
	const seLink * const link = GetLink();
	
	if(link){
		pEditCurve->SetCurve(link->GetCurve());
		
	}else{
		pEditCurve->SetDefaultBezier();
	}
	
	pEditCurve->SetEnabled(link != NULL);
}
