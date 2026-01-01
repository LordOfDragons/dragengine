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

#include "gdeWPPathList.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeListBoxListener.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Actions
////////////

namespace{

class cActionAppend : public igdeAction{
	gdeWPPathList &pPanel;
	igdeEditPath::Ref &pEditPath;
	igdeListBox::Ref &pListBox;
	
public:
	typedef deTObjectReference<cActionAppend> Ref;
	cActionAppend(gdeWPPathList &panel, igdeEditPath::Ref &editPath, igdeListBox::Ref &listBox) : 
	igdeAction("Append", panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "Append path"),
	pPanel(panel), pEditPath(editPath), pListBox(listBox){}
	
	void OnAction() override{
		if(!pPanel.GetPathList() || !pPanel.GetUndoSystem()){
			return;
		}
		if(pPanel.GetPathList()->Has(pEditPath->GetPath())){
			pListBox->SetSelection(pListBox->IndexOfItem(pEditPath->GetPath()));
			return;
		}
		
		decStringList pathList(*pPanel.GetPathList());
		pathList.Add(pEditPath->GetPath());
		pPanel.GetUndoSystem()->Add(pPanel.UndoSet(pathList));
		
		pListBox->SetSelection(pListBox->IndexOfItem(pEditPath->GetPath()));
	}
};

class cActionInsert : public igdeAction{
	gdeWPPathList &pPanel;
	igdeEditPath::Ref &pEditPath;
	igdeListBox::Ref &pListBox;
	
public:
	typedef deTObjectReference<cActionInsert> Ref;
	cActionInsert(gdeWPPathList &panel, igdeEditPath::Ref &editPath, igdeListBox::Ref &listBox) : 
	igdeAction("Insert", panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "Insert path"),
	pPanel(panel), pEditPath(editPath), pListBox(listBox){}
	
	void OnAction() override{
		if(!pPanel.GetPathList() || !pPanel.GetUndoSystem()){
			return;
		}
		if(pPanel.GetPathList()->Has(pEditPath->GetPath())){
			pListBox->SetSelection(pListBox->IndexOfItem(pEditPath->GetPath()));
			return;
		}
		
		const int position = pListBox->GetSelection();
		if(position == -1){
			return;
		}
		
		igdeUndo::Ref undo;
		decStringList list(*pPanel.GetPathList());
		list.Insert(pEditPath->GetPath(), position);
		undo = pPanel.UndoSet(list);
		pPanel.GetUndoSystem()->Add(undo);
		
		pListBox->SetSelection(position);
	}
	
	void Update() override{
		SetEnabled(pListBox->GetSelection() != -1);
	}
};

class cActionRemove : public igdeAction{
	gdeWPPathList &pPanel;
	igdeListBox::Ref &pListBox;
	
public:
	typedef deTObjectReference<cActionRemove> Ref;
	cActionRemove(gdeWPPathList &panel, igdeListBox::Ref &listBox) :
	igdeAction("Remove", panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus),
		"Remove path"), pPanel(panel), pListBox(listBox){}
	
	void OnAction() override{
		if(!pPanel.GetPathList() || !pPanel.GetUndoSystem()){
			return;
		}
		
		decStringList paths(*pPanel.GetPathList());
		const int index = paths.IndexOf(pListBox->GetSelectedItem()->GetText());
		if(index != -1){
			paths.RemoveFrom(index);
		}
		
		pPanel.GetUndoSystem()->Add(pPanel.UndoSet(paths));
		
		if(pListBox->GetItems().IsNotEmpty()){
			pListBox->SetSelection(0);
		}
	}
};

class cActionClear : public igdeAction{
	gdeWPPathList &pPanel;
	igdeListBox::Ref &pListBox;
	
public:
	typedef deTObjectReference<cActionClear> Ref;
	cActionClear(gdeWPPathList &panel, igdeListBox::Ref &listBox) :
	igdeAction("Clear", nullptr, "Clear path"), pPanel(panel), pListBox(listBox){}
	
	void OnAction() override{
		if(!pPanel.GetPathList() || !pPanel.GetUndoSystem() || pListBox->GetItems().IsEmpty()){
			return;
		}
		
		pPanel.GetUndoSystem()->Add(pPanel.UndoSet(decStringList()));
	}
};

class cListPaths : public igdeListBoxListener{
	gdeWPPathList &pListBox;
	
public:
	typedef deTObjectReference<cListPaths> Ref;
	cListPaths(gdeWPPathList &listBox) : pListBox(listBox){}
	
	virtual void OnSelectionChanged(igdeListBox*){
	}
	
	virtual void AddContextMenuEntries(igdeListBox*, igdeMenuCascade &menu){
		igdeUIHelper &helper = pListBox.GetEnvironment().GetUIHelper();
		helper.MenuCommand(menu, pListBox.GetActionAppend());
		helper.MenuCommand(menu, pListBox.GetActionInsert());
		helper.MenuCommand(menu, pListBox.GetActionRemove());
		helper.MenuCommand(menu, pListBox.GetActionClear());
	}
};

}



// Class gdeWPPathList
///////////////////////

// Constructor, destructor
////////////////////////////

gdeWPPathList::gdeWPPathList(igdeUIHelper &helper, int rows, const char *description) :
igdeContainerFlow(helper.GetEnvironment(), igdeContainerFlow::eaY, igdeContainerFlow::esNone),
pPathList(nullptr),
pUndoSystem(nullptr)
{
	pActionAppend = cActionAppend::Ref::New(*this, pEditPath, pListBox);
	pActionInsert = cActionInsert::Ref::New(*this, pEditPath, pListBox);
	pActionRemove = cActionRemove::Ref::New(*this, pListBox);
	pActionClear = cActionClear::Ref::New(*this, pListBox);
	
	helper.EditPath(*this, description, igdeEnvironment::efpltAll, pEditPath, {});
	pEditPath->SetAutoValidatePath(false);
	
	helper.ListBox(*this, rows, description, pListBox, cListPaths::Ref::New(*this));
}

gdeWPPathList::~gdeWPPathList(){
}



// Management
///////////////

void gdeWPPathList::SetPathList(const decStringList *tagList){
	if(tagList == pPathList){
		return;
	}
	
	pPathList = tagList;
	
	UpdateList();
}

void gdeWPPathList::SetUndoSystem(igdeUndoSystem *undoSystem){
	pUndoSystem = undoSystem;
}



const decString &gdeWPPathList::GetSelectedPath() const{
	if(pListBox->GetSelectedItem() != nullptr){
		return pListBox->GetSelectedItem()->GetText();
		
	}else{
		static const decString noselection;
		return noselection;
	}
}

void gdeWPPathList::UpdateList(){
	const decString selection(GetSelectedPath());
	
	pListBox->RemoveAllItems();
	
	if(pPathList){
		const int count = pPathList->GetCount();
		int i;
		
		for(i=0; i<count; i++){
			pListBox->AddItem(pPathList->GetAt(i));
		}
	}
	
	SelectPath(selection);
}

void gdeWPPathList::SelectPath(const decString &tag){
	if(pPathList){
		pListBox->SetSelection(pListBox->IndexOfItem(tag));
	}
}
