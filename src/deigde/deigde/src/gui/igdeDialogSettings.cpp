/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#include "igdeDialogSettings.h"
#include "igdeWindowMain.h"
#include "../environment/igdeEnvironmentIGDE.h"
#include "../configuration/igdeConfiguration.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeApplication.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/localization/igdeTranslationManager.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>


// Actions
////////////

namespace {

class cComboLanguage : public igdeComboBoxListener {
	igdeDialogSettings &pDialog;
	
public:
	explicit cComboLanguage(igdeDialogSettings& dialog) : pDialog(dialog){}
	
	void OnTextChanged(igdeComboBox*) override{
		pDialog.UpdateLanguage();
	}
};

}


// Class igdeDialogSettings
/////////////////////////////

// Constructor, destructor
////////////////////////////

igdeDialogSettings::igdeDialogSettings(igdeWindowMain &windowMain) :
igdeDialog(windowMain.GetEnvironment(), "@Igde.Settings.Title"),
pWindowMain(windowMain)
{
	igdeEnvironment &env = windowMain.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelper();
	igdeContainer::Ref content;
	
	content = igdeContainerForm::Ref::New(env);
	
	helper.ComboBox(content, "@Igde.Settings.Language", "@Igde.Settings.Language.ToolTip", pCBLanguage, {});
	pCBLanguage->SetDefaultSorter();
	
	igdeContainer::Ref buttonBar;
	CreateButtonBar(buttonBar, "@Igde.Accept", "@Igde.Discard");
	
	AddContent(content, buttonBar);
	
	
	// initialize
	const igdeTranslationManager &tm = env.GetTranslationManager();
	tm.GetIgdeLanguagePacks().Visit([&](igdeLanguagePack *lp){
		pCBLanguage->AddItem(lp->GetName().ToUTF8(), nullptr, lp);
	});
	pCBLanguage->SetSelectionWithData(tm.GetIgdeLanguagePacks().FindByLanguage(
		windowMain.GetConfiguration().GetLanguage()));
	pCBLanguage->AddListener(deTObjectReference<cComboLanguage>::New(*this));
	
	SetSize(igdeApplication::app().DisplayScaled(decPoint(600, 500)));
}

igdeDialogSettings::~igdeDialogSettings() = default;



// Management
///////////////

bool igdeDialogSettings::CheckValidInput(){
	return true;
}

bool igdeDialogSettings::Accept(){
	if(!CheckValidInput()){
		return false;
	}
	
	pWindowMain.ChangeLanguage(reinterpret_cast<igdeLanguagePack*>(
		pCBLanguage->GetSelectedItemData())->GetLanguage());
	
	return igdeDialog::Accept();
}

void igdeDialogSettings::UpdateLanguage(){
}
