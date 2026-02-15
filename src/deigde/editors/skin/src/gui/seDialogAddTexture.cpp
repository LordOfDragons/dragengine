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

#include "seWindowMain.h"
#include "seDialogAddTexture.h"
#include "../skin/seSkin.h"
#include "../skin/texture/seTexture.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeLabel.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/event/igdeListBoxListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/gui/resources/igdeIcon.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelTexture.h>



// Actions
////////////

namespace {

class cListTextureNames : public igdeListBoxListener{
	seDialogAddTexture &pDialog;
	
public:
	using Ref = deTObjectReference<cListTextureNames>;
	cListTextureNames(seDialogAddTexture &dialog) : pDialog(dialog){}
	
	void OnSelectionChanged(igdeListBox *listBox) override{
		if(listBox->GetSelectedItem()){
			pDialog.SetTextureName(listBox->GetSelectedItem()->GetText());
		}
	}
	
	void OnDoubleClickItem(igdeListBox *listBox, int index) override{
		pDialog.SetTextureName(listBox->GetItems().GetAt(index)->GetText());
		pDialog.CloseDialog(true);
	}
};

class cTextTextureName : public igdeTextFieldListener{
	seDialogAddTexture &pDialog;
	
public:
	using Ref = deTObjectReference<cTextTextureName>;
	cTextTextureName(seDialogAddTexture &dialog) : pDialog(dialog){}
	
	void OnTextChanging(igdeTextField *textField) override{
		pDialog.SetTextureName(textField->GetText());
	}
};

}


// Class seDialogAddTexture
/////////////////////////////

// Constructor, destructor
////////////////////////////

seDialogAddTexture::seDialogAddTexture(seWindowMain &windowMain) :
igdeDialog(windowMain.GetEnvironment(), "@Skin.DialogAddTexture.Title"),
pWindowMain(windowMain)
{
	igdeEnvironment &env = GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelper();
	igdeContainer::Ref content, formLine;
	
	
	igdeLabel::Ref header(igdeLabel::Ref::New(env, "@Skin.DialogAddTexture.Header"));
	
	
	content = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaY, igdeContainerFlow::esFirst, 5);
	
	helper.ListBox(content, 5, "@Skin.DialogAddTexture.TextureNames", pListModelTextureNames,
		cListTextureNames::Ref::New(*this));
	pListModelTextureNames->SetDefaultSorter();
	
	formLine = igdeContainerForm::Ref::New(env);
	helper.EditString(formLine, "@Skin.DialogAddTexture.Name", "@Skin.DialogAddTexture.Name.ToolTip", 25,
		pEditTextureName, cTextTextureName::Ref::New(*this));
	content->AddChild(formLine);
	
	
	igdeContainer::Ref buttonBar;
	CreateButtonBar(buttonBar, "@Skin.DialogAddTexture.Button.Create", "@Igde.Cancel");
	
	
	AddContent(content, header, buttonBar);
	
	
	pUpdateModelTextureList();
	
	pEditTextureName->SetText(Translate("Skin.DialogAddTexture.DefaultTextureName").ToUTF8());
	pEditTextureName->Focus();
}

seDialogAddTexture::~seDialogAddTexture(){
}



// Management
///////////////

const decString &seDialogAddTexture::GetTextureName() const{
	return pEditTextureName->GetText();
}

void seDialogAddTexture::SetTextureName(const char *name){
	pEditTextureName->SetText(name);
	
	// name could be a string reference to the list box or edit field widget text
	// and changing the text field might have invalidated such a link. by using
	// the edit field GetText() we are on the safe side
	pListModelTextureNames->SetSelection(pListModelTextureNames->IndexOfItem(pEditTextureName->GetText()));
}


// Protected Functions
////////////////////////

void seDialogAddTexture::pUpdateModelTextureList(){
	igdeIcon::Ref iconUsed(GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallPlus));
	igdeIcon::Ref iconAvailable(GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallMinus));
	const seSkin * const skin = pWindowMain.GetSkin();
	const deModel *engModel = nullptr;
	
	if(skin->GetEngineComponent()){
		engModel = skin->GetEngineComponent()->GetModel();
	}
	
	if(engModel){
		const seTexture::List &list = skin->GetTextures();
		int i, count = engModel->GetTextureCount();
		
		for(i=0; i<count; i++){
			const decString &modelTextureName = engModel->GetTextureAt(i)->GetName();
			
			if(list.HasNamed(modelTextureName)){
				pListModelTextureNames->AddItem(modelTextureName, iconUsed);
				
			}else{
				pListModelTextureNames->AddItem(modelTextureName, iconAvailable);
			}
		}
	}
	
	pListModelTextureNames->SortItems();
	
	if(pListModelTextureNames->GetItems().IsNotEmpty()){
		pListModelTextureNames->SetSelection(0);
	}
}
