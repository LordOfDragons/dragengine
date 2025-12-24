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

#include "projPanelProfiles.h"
#include "projPanelProfilesListener.h"
#include "projWindowMain.h"
#include "../project/projProject.h"
#include "../project/profile/projProfile.h"
#include "../undosys/profile/projUProfileSetName.h"
#include "../undosys/profile/projUProfileSetDescription.h"
#include "../undosys/profile/projUProfileSetScriptDirectory.h"
#include "../undosys/profile/projUProfileSetGameObject.h"
#include "../undosys/profile/projUProfileSetPathConfig.h"
#include "../undosys/profile/projUProfileSetPathCapture.h"
#include "../undosys/profile/projUProfileSetIdentifier.h"
#include "../undosys/profile/projUProfileSetAliasIdentifier.h"
#include "../undosys/profile/projUProfileSetTitle.h"
#include "../undosys/profile/projUProfileSetGameDescription.h"
#include "../undosys/profile/projUProfileSetWindowSize.h"
#include "../undosys/profile/projUProfileSetDelgaPath.h"
#include "../undosys/profile/projUProfileSetRunArguments.h"
#include "../undosys/profile/projUProfileSetIcons.h"
#include "../undosys/profile/projUProfileSetExcludePatterns.h"
#include "../undosys/profile/projUProfileSetRequiredExtensions.h"
#include "../undosys/profile/projUProfileSetCreator.h"
#include "../undosys/profile/projUProfileSetWebsite.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeApplication.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeTextArea.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeGroupBox.h>
#include <deigde/gui/composed/igdeEditPoint.h>
#include <deigde/gui/composed/igdeEditPointListener.h>
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/composed/igdeEditPathListener.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionSelectFile.h>
#include <deigde/gui/event/igdeActionExternOpen.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeListBoxListener.h>
#include <deigde/gui/event/igdeTextAreaListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/layout/igdeContainerBox.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/layout/igdeContainerScroll.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Events
///////////

namespace{

class cActionBase : public igdeAction{
protected:
	projPanelProfiles &pPanel;
	
public:
	typedef deTObjectReference<cActionBase> Ref;
	cActionBase(projPanelProfiles &panel, const char *text, igdeIcon *icon, const char *description) :
	igdeAction(text, icon, description),
	pPanel(panel){}
	
	void OnAction() override{
		projProject * const project = pPanel.GetProject();
		if(!project || !project->GetActiveProfile()){
			return;
		}
		
		igdeUndo::Ref undo(OnAction(project, project->GetActiveProfile()));
		if(undo){
			project->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnAction(projProject *project, projProfile *profile) = 0;
	
	void Update() override{
		const projProject * const project = pPanel.GetProject();
		SetEnabled(project && project->GetActiveProfile());
	}
};

class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	projPanelProfiles &pPanel;
	
public:
	typedef deTObjectReference<cBaseTextFieldListener> Ref;
	cBaseTextFieldListener(projPanelProfiles &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		projProject * const project = pPanel.GetProject();
		if(!project){
			return;
		}
		
		projProfile * const profile = project->GetActiveProfile();
		if(!profile){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(textField, project, profile));
		if(undo){
			project->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField *textField, projProject *project,
		projProfile *profile) = 0;
};

class cBaseTextAreaListener : public igdeTextAreaListener{
protected:
	projPanelProfiles &pPanel;
	
public:
	typedef deTObjectReference<cBaseTextAreaListener> Ref;
	cBaseTextAreaListener(projPanelProfiles &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextArea *textArea){
		projProject * const project = pPanel.GetProject();
		if(!project){
			return;
		}
		
		projProfile * const profile = project->GetActiveProfile();
		if(!profile){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(textArea, project, profile));
		if(undo){
			project->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeTextArea *textArea, projProject *project,
		projProfile *profile) = 0;
};

class cBaseEditPointListener : public igdeEditPointListener{
protected:
	projPanelProfiles &pPanel;
	
public:
	typedef deTObjectReference<cBaseEditPointListener> Ref;
	cBaseEditPointListener(projPanelProfiles &panel) : pPanel(panel){}
	
	virtual void OnPointChanged(igdeEditPoint *editPoint){
		projProject * const project = pPanel.GetProject();
		if(!project){
			return;
		}
		
		projProfile * const profile = project->GetActiveProfile();
		if(!profile){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(editPoint, project, profile));
		if(undo){
			project->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeEditPoint *editPoint, projProject *project,
		projProfile *profile) = 0;
};



class cListProfile : public igdeListBoxListener{
protected:
	projPanelProfiles &pPanel;
	
public:
	typedef deTObjectReference<cListProfile> Ref;
	cListProfile(projPanelProfiles &panel) : pPanel(panel){}
	
	virtual void OnSelectionChanged(igdeListBox *listBox){
		projProject * const project = pPanel.GetProject();
		if(!project){
			return;
		}
		
		const igdeListItem * const selection = listBox->GetSelectedItem();
		if(selection){
			project->SetActiveProfile((projProfile*)selection->GetData());
			
		}else{
			project->SetActiveProfile(nullptr);
		}
	}
};



class cTextName : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextName> Ref;
	cTextName(projPanelProfiles &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField *textField, projProject *project,
	projProfile *profile){
		const decString &name = textField->GetText();
		if(name == profile->GetName()){
			return {};
		}
		
		if(project->GetProfiles().HasNamed(name)){
			igdeCommonDialogs::Error(&pPanel, "Rename profile",
				"A profile with this name exists already.");
			textField->SetText(profile->GetName());
			return {};
		}
		
		return projUProfileSetName::Ref::New(profile, name);
	}
};

class cTextDescription : public cBaseTextAreaListener{
public:
	typedef deTObjectReference<cTextDescription> Ref;
	cTextDescription(projPanelProfiles &panel) : cBaseTextAreaListener(panel){}
	
	virtual igdeUndo::Ref OnChanged(igdeTextArea *textArea, projProject*,
	projProfile *profile){
		if(textArea->GetText() == profile->GetDescription()){
			return {};
		}
		return projUProfileSetDescription::Ref::New(profile, textArea->GetText());
	}
};

class cTextScriptDirectory : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextScriptDirectory> Ref;
	cTextScriptDirectory(projPanelProfiles &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, projProject*, projProfile *profile) override{
		if(textField->GetText() == profile->GetScriptDirectory()){
			return {};
		}
		return projUProfileSetScriptDirectory::Ref::New(profile, textField->GetText());
	}
};

class cActionScriptDirectory : public cActionBase{
public:
	typedef deTObjectReference<cActionScriptDirectory> Ref;
	
	cActionScriptDirectory(projPanelProfiles &panel) : cActionBase(panel,
		"...", nullptr, "VFS directory where the game scripts are located"){}
	
	igdeUndo::Ref OnAction(projProject*, projProfile*) override{
		return {}; // TODO we need a VFS version of FXDirDialog since this is VFS path
	}
};

class cTextGameObject : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextGameObject> Ref;
	cTextGameObject(projPanelProfiles &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, projProject*, projProfile *profile) override{
		if(textField->GetText() == profile->GetGameObject()){
			return {};
		}
		return projUProfileSetGameObject::Ref::New(profile, textField->GetText());
	}
};

class cTextPathConfig : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextPathConfig> Ref;
	cTextPathConfig(projPanelProfiles &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, projProject*, projProfile *profile) override{
		if(textField->GetText() == profile->GetPathConfig()){
			return {};
		}
		return projUProfileSetPathConfig::Ref::New(profile, textField->GetText());
	}
};

class cActionPathConfig : public cActionBase{
public:
	typedef deTObjectReference<cActionPathConfig> Ref;
	
	cActionPathConfig(projPanelProfiles &panel) : cActionBase(panel,
		"...", nullptr, "VFS directory where the game stores configuration files"){}
	
	igdeUndo::Ref OnAction(projProject*, projProfile*) override{
		return {}; // TODO we need a VFS version of FXDirDialog since this is VFS path
	}
};

class cTextPathCapture : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextPathCapture> Ref;
	cTextPathCapture(projPanelProfiles &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, projProject*, projProfile *profile) override{
		if(textField->GetText() == profile->GetPathCapture()){
			return {};
		}
		return projUProfileSetPathCapture::Ref::New(profile, textField->GetText());
	}
};

class cActionPathCapture : public cActionBase{
public:
	typedef deTObjectReference<cActionPathCapture> Ref;
	
	cActionPathCapture(projPanelProfiles &panel) : cActionBase(panel,
		"...", nullptr, "VFS directory where the game stores captured files"){}
	
	igdeUndo::Ref OnAction(projProject*, projProfile*) override{
		return {}; // TODO we need a VFS version of FXDirDialog since this is VFS path
	}
};

class cActionGenerateIdentifier : public cActionBase{
public:
	typedef deTObjectReference<cActionGenerateIdentifier> Ref;
	
	cActionGenerateIdentifier(projPanelProfiles &panel) : cActionBase(panel,
		"Generate Identifier", nullptr, "Generate Identifier"){}
	
	igdeUndo::Ref OnAction(projProject*, projProfile *profile) override{
		if(igdeCommonDialogs::Question(&pPanel, igdeCommonDialogs::ebsYesNo, "Generate Identifier",
		"Generating new identifier can break the game. Do you really want to generate new identifier?")
		== igdeCommonDialogs::ebYes){
			return projUProfileSetIdentifier::Ref::New(profile, decUuid::Random());
		}
		return {};
	}
};

class cActionMenuIdentifier : public igdeActionContextMenu{
	projPanelProfiles &pPanel;
	
public:
	typedef deTObjectReference<cActionMenuIdentifier> Ref;
	
	cActionMenuIdentifier(projPanelProfiles &panel) : igdeActionContextMenu("",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallDown), "Identifier menu"),
	pPanel(panel){}
	
	virtual void AddContextMenuEntries(igdeMenuCascade &contextMenu){
		if(!pPanel.GetActiveProfile()){
			return;
		}
		
		igdeUIHelper &helper = pPanel.GetEnvironment().GetUIHelper();
		helper.MenuCommand(contextMenu, cActionGenerateIdentifier::Ref::New(pPanel));
	}
};

class cTextIdentifier : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextIdentifier> Ref;
	cTextIdentifier(projPanelProfiles &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, projProject*, projProfile *profile) override{
		try{
			const decUuid value(textField->GetText(), false);
			return value != profile->GetIdentifier()
				? projUProfileSetIdentifier::Ref::New(profile, value) : projUProfileSetIdentifier::Ref();
			
		}catch(const deException &){
			igdeCommonDialogs::Error(&pPanel, "Invalid Input",
				"Identifier has to be a valid UUID in the format 8-4-4-4-6 groups of hex-encoded values");
			textField->SetText(profile->GetIdentifier().ToHexString(false));
			return {};
		}
	}
};

class cTextAliasIdentifier : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextAliasIdentifier> Ref;
	cTextAliasIdentifier(projPanelProfiles &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, projProject*, projProfile *profile) override{
		if(textField->GetText() == profile->GetAliasIdentifier()){
			return {};
		}
		return projUProfileSetAliasIdentifier::Ref::New(profile, textField->GetText());
	}
};

class cTextTitle : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextTitle> Ref;
	cTextTitle(projPanelProfiles &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, projProject*, projProfile *profile) override{
		if(textField->GetText() == profile->GetTitle()){
			return {};
		}
		return projUProfileSetTitle::Ref::New(profile, textField->GetText());
	}
};

class cTextGameDescription : public cBaseTextAreaListener{
public:
	typedef deTObjectReference<cTextGameDescription> Ref;
	cTextGameDescription(projPanelProfiles &panel) : cBaseTextAreaListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextArea *textArea, projProject*, projProfile *profile) override{
		if(textArea->GetText() == profile->GetGameDescription()){
			return {};
		}
		return projUProfileSetGameDescription::Ref::New(profile, textArea->GetText());
	}
};

class cTextCreator : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextCreator> Ref;
	cTextCreator(projPanelProfiles &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, projProject*, projProfile *profile) override{
		if(textField->GetText() == profile->GetCreator()){
			return {};
		}
		return projUProfileSetCreator::Ref::New(profile, textField->GetText());
	}
};

class cTextWebsite : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextWebsite> Ref;
	cTextWebsite(projPanelProfiles &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, projProject*, projProfile *profile) override{
		if(textField->GetText() == profile->GetWebsite()){
			return {};
		}
		return projUProfileSetWebsite::Ref::New(profile, textField->GetText());
	}
};

class cEditWindowSize : public cBaseEditPointListener{
public:
	typedef deTObjectReference<cEditWindowSize> Ref;
	cEditWindowSize(projPanelProfiles &panel) : cBaseEditPointListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeEditPoint *editPoint, projProject*, projProfile *profile) override{
		if(editPoint->GetPoint() == profile->GetWindowSize()){
			return {};
		}
		return projUProfileSetWindowSize::Ref::New(profile, editPoint->GetPoint());
	}
};



class cActionAddIcon : public cActionBase{
	igdeEditPath &pEditPath;
	
public:
	typedef deTObjectReference<cActionAddIcon> Ref;
	
	cActionAddIcon(projPanelProfiles &panel, igdeEditPath &editPath) :
	cActionBase(panel, "", panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallPlus), "Add icon"),
	pEditPath(editPath){}
	
	igdeUndo::Ref OnAction(projProject*, projProfile *profile) override{
		if(pEditPath.GetPath().IsEmpty() || profile->GetIcons().Has(pEditPath.GetPath())){
			return {};
		}
		
		decStringSet icons(profile->GetIcons());
		icons.Add(pEditPath.GetPath());
		return projUProfileSetIcons::Ref::New(profile, icons);
	}
};

class cActionRemoveIcon : public cActionBase{
	igdeListBox &pListIcons;
	
public:
	typedef deTObjectReference<cActionRemoveIcon> Ref;
	
	cActionRemoveIcon(projPanelProfiles &panel, igdeListBox &listPatterns) :
	cActionBase(panel, "", panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallMinus),
		"Remove selected icon"),
	pListIcons(listPatterns){}
	
	igdeUndo::Ref OnAction(projProject*, projProfile *profile) override{
		const igdeListItem * const selection = pListIcons.GetSelectedItem();
		if(!selection || !profile->GetIcons().Has(selection->GetText())){
			return {};
		}
		
		decStringSet icons(profile->GetIcons());
		icons.Remove(selection->GetText());
		return projUProfileSetIcons::Ref::New(profile, icons);
	}
	
	void Update() override{
		const projProject * const project = pPanel.GetProject();
		SetEnabled(project && project->GetActiveProfile() && pListIcons.GetSelectedItem());
	}
};



class cActionAddExcludePattern : public cActionBase{
	igdeTextField &pTextPattern;
	
public:
	typedef deTObjectReference<cActionAddExcludePattern> Ref;
	
	cActionAddExcludePattern(projPanelProfiles &panel, igdeTextField &textPattern) :
	cActionBase(panel, "", panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallPlus),
		"Add file pattern to set"),
	pTextPattern(textPattern){}
	
	igdeUndo::Ref OnAction(projProject*, projProfile *profile) override{
		if(profile->GetExcludePatterns().Has(pTextPattern.GetText())){
			return {};
		}
		
		decStringSet patterns(profile->GetExcludePatterns());
		patterns.Add(pTextPattern.GetText());
		return projUProfileSetExcludePatterns::Ref::New(profile, patterns);
	}
};

class cActionRemoveExcludePattern : public cActionBase{
	igdeListBox &pListPatterns;
	
public:
	typedef deTObjectReference<cActionRemoveExcludePattern> Ref;
	
	cActionRemoveExcludePattern(projPanelProfiles &panel, igdeListBox &listPatterns) :
	cActionBase(panel, "", panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallMinus),
		"Remove selected file pattern from set"),
	pListPatterns(listPatterns){}
	
	igdeUndo::Ref OnAction(projProject*, projProfile *profile) override{
		const igdeListItem * const selection = pListPatterns.GetSelectedItem();
		if(!selection || !profile->GetExcludePatterns().Has(selection->GetText())){
			return {};
		}
		
		decStringSet patterns(profile->GetExcludePatterns());
		patterns.Remove(selection->GetText());
		return projUProfileSetExcludePatterns::Ref::New(profile, patterns);
	}
	
	void Update() override{
		const projProject * const project = pPanel.GetProject();
		SetEnabled(project && project->GetActiveProfile() && pListPatterns.GetSelectedItem());
	}
};



class cActionAddRequiredExtension : public cActionBase{
	igdeTextField &pTextExtension;
	
public:
	typedef deTObjectReference<cActionAddRequiredExtension> Ref;
	
	cActionAddRequiredExtension(projPanelProfiles &panel, igdeTextField &textExtension) :
	cActionBase(panel, "", panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallPlus),
		"Add resource file extension to set"),
	pTextExtension(textExtension){}
	
	igdeUndo::Ref OnAction(projProject*, projProfile *profile) override{
		if(profile->GetRequiredExtensions().Has(pTextExtension.GetText())){
			return {};
		}
		
		decStringSet patterns(profile->GetRequiredExtensions());
		patterns.Add(pTextExtension.GetText());
		return projUProfileSetRequiredExtensions::Ref::New(profile, patterns);
	}
};

class cActionRemoveRequiredExtension : public cActionBase{
	igdeListBox &pListPatterns;
	
public:
	typedef deTObjectReference<cActionRemoveRequiredExtension> Ref;
	
	cActionRemoveRequiredExtension(projPanelProfiles &panel, igdeListBox &listPatterns) :
	cActionBase(panel, "", panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallMinus),
		"Remove selected resource file extension from set"),
	pListPatterns(listPatterns){}
	
	igdeUndo::Ref OnAction(projProject*, projProfile *profile) override{
		const igdeListItem * const selection = pListPatterns.GetSelectedItem();
		if(!selection || !profile->GetRequiredExtensions().Has(selection->GetText())){
			return {};
		}
		
		decStringSet patterns(profile->GetRequiredExtensions());
		patterns.Remove(selection->GetText());
		return projUProfileSetRequiredExtensions::Ref::New(profile, patterns);
	}
	
	void Update() override{
		const projProject * const project = pPanel.GetProject();
		SetEnabled(project && project->GetActiveProfile() && pListPatterns.GetSelectedItem());
	}
};



class cTextDelgaPath : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextDelgaPath> Ref;
	cTextDelgaPath(projPanelProfiles &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, projProject*, projProfile *profile) override{
		if(textField->GetText() == profile->GetDelgaPath()){
			return {};
		}
		return projUProfileSetDelgaPath::Ref::New(profile, textField->GetText());
	}
};

class cActionDelgaPath : public cActionBase{
public:
	typedef deTObjectReference<cActionDelgaPath> Ref;
	
	cActionDelgaPath(projPanelProfiles &panel) : cActionBase(panel,
		"...", nullptr, "VFS directory where to place the build DELGA file"){}
	
	igdeUndo::Ref OnAction(projProject*, projProfile*) override{
		return {}; // TODO we need a VFS version of FXDirDialog since this is VFS path
	}
};



class cTextRunArguments : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextRunArguments> Ref;
	cTextRunArguments(projPanelProfiles &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, projProject*, projProfile *profile) override{
		if(textField->GetText() == profile->GetRunArguments()){
			return {};
		}
		return projUProfileSetRunArguments::Ref::New(profile, textField->GetText());
	}
};

}



// Class projPanelProfiles
////////////////////////////

// Constructor, destructor
////////////////////////////

projPanelProfiles::projPanelProfiles(projWindowMain &windowMain) :
igdeContainerSplitted(windowMain.GetEnvironment(), igdeContainerSplitted::espLeft,
	igdeApplication::app().DisplayScaled(250)),

pWindowMain(windowMain)
{
	igdeEnvironment &env = windowMain.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelper();
	
	pListener = projPanelProfilesListener::Ref::New(*this);
	
	
	
	// side panel
	igdeContainerScroll::Ref scroll;
	igdeContainer::Ref sidePanel;
	helper.SidePanel(scroll, sidePanel, false, 5);
	sidePanel->SetWidgetGuiThemeName("");
	AddChild(scroll, eaSide);
	
	helper.ListBox(sidePanel, 10, "Profiles", pListProfiles, cListProfile::Ref::New(*this));
	helper.Button(sidePanel, windowMain.GetActionProfileAdd());
	helper.Button(sidePanel, windowMain.GetActionProfileRemove());
	helper.Button(sidePanel, windowMain.GetActionProfileDuplicate());
	
	igdeContainer::Ref groupBox, formLine;
	helper.GroupBoxStaticFlow(sidePanel, groupBox, "Content:");
	helper.Button(groupBox, windowMain.GetActionShowContent());
	
	helper.GroupBoxStaticFlow(sidePanel, groupBox, "Test-Run:");
	helper.Button(groupBox, windowMain.GetActionProfileTestRun());
	
	helper.GroupBoxStaticFlow(sidePanel, groupBox, "Distribution:");
	helper.Button(groupBox, windowMain.GetActionProfileDistribute());
	helper.Button(groupBox, windowMain.GetActionShowDistribute());
	
	
	
	// profile panel
	helper.SidePanel(scroll, sidePanel, false, 5);
	sidePanel->SetWidgetGuiThemeName("");
	AddChild(scroll, eaCenter);
	
	igdeContainer::Ref frameLine;
	const char *description;
	
	groupBox = igdeContainerForm::Ref::New(env);
	sidePanel->AddChild(groupBox);
	helper.EditString(groupBox, "Name:", "Profile name.", pEditName, cTextName::Ref::New(*this));
	helper.EditString(groupBox, "Description:", "Profile description.",
		pEditDescription, 5, cTextDescription::Ref::New(*this));
	
	
	// game parameters
	helper.GroupBox(sidePanel, groupBox, "Game Parameters:");
	
	description = "VFS directory where the game scripts are located.";
	helper.FormLineStretchFirst(groupBox, "Script Directory:", description, frameLine);
	helper.EditString(frameLine, description, pEditScriptDirectory, cTextScriptDirectory::Ref::New(*this));
	pActionScriptDirectory = cActionScriptDirectory::Ref::New(*this);
	helper.Button(frameLine, pActionScriptDirectory);
	
	helper.EditString(groupBox, "Game Object:",
		"Game object the scripting module uses as entry point to run the game.",
		pEditGameObject, cTextGameObject::Ref::New(*this));
	
	description = "VFS directory where the game stores configuration files.";
	helper.FormLineStretchFirst(groupBox, "Config Path:", description, frameLine);
	helper.EditString(frameLine, description, pEditPathConfig, cTextPathConfig::Ref::New(*this));
	pActionPathConfig = cActionPathConfig::Ref::New(*this);
	helper.Button(frameLine, pActionPathConfig);
	
	description = "VFS directory where the game stores captured files.";
	helper.FormLineStretchFirst(groupBox, "Capture Path:", description, frameLine);
	helper.EditString(frameLine, description, pEditPathCapture, cTextPathCapture::Ref::New(*this));
	pActionPathCapture = cActionPathCapture::Ref::New(*this);
	helper.Button(frameLine, pActionPathCapture);
	
	pActionMenuIdentifier = cActionMenuIdentifier::Ref::New(*this);
	helper.FormLineStretchFirst(groupBox, "Identifier:",
		"Unique identifier of game used by Launchers. CHANGING THIS CAN BREAK YOUR GAME!", formLine);
	helper.EditString(formLine, "Unique identifier of game used by Launchers. CHANGING THIS CAN BREAK YOUR GAME!",
		pEditIdentifier, cTextIdentifier::Ref::New(*this));
	helper.Button(formLine, pBtnMenuIdentifier, pActionMenuIdentifier);
	pActionMenuIdentifier->SetWidget(pBtnMenuIdentifier);
	
	helper.EditString(groupBox, "Alias Identifier:", "Alias identifier of game used by Launchers to simplify running the game.",
		pEditAliasIdentifier, cTextAliasIdentifier::Ref::New(*this));
	
	helper.EditString(groupBox, "Title:", "Title to display for the game by Launchers.",
		pEditTitle, cTextTitle::Ref::New(*this));
	
	helper.EditString(groupBox, "Description:", "Description to display for the game by Launchers.",
		pEditGameDescription, 10, cTextGameDescription::Ref::New(*this));
	
	helper.EditString(groupBox, "Creator:", "Creator (or author) of the game.",
		pEditCreator, cTextCreator::Ref::New(*this));
	
	helper.EditString(groupBox, "Website:", "Website (or any other kind of internet presence) of project or creator (author).",
		pEditWebsite, cTextWebsite::Ref::New(*this));
	
	helper.EditPoint(groupBox, "Window Size:", "Size of run-time window or 0 to run fullscreen.",
		pEditWindowSize, cEditWindowSize::Ref::New(*this));
	
	
	// processing parameters
	groupBox = igdeGroupBox::Ref::New(env, "Processing Parameters:", false);
	sidePanel->AddChild(groupBox);
	
	igdeContainer::Ref subGroup, subGroup2;
	subGroup = igdeContainerBox::Ref::New(env, igdeContainerBox::eaX);
	groupBox->AddChild(subGroup);
	
	// icons
	helper.GroupBoxStaticFlow(subGroup, subGroup2, "Icons:");
	
	description = "Set of icons of different size representing the project.";
	helper.FormLineStretchFirst(subGroup2, "Path:", description, frameLine);
	helper.EditPath(frameLine, description, igdeEnvironment::efpltImage, pEditIconPath, {});
	pActionAddIcon = cActionAddIcon::Ref::New(*this, pEditIconPath);
	helper.Button(frameLine, pActionAddIcon);
	
	helper.ListBox(subGroup2, 4, description, pListIcons, {});
	pListIcons->SetDefaultSorter();
	
	pActionRemoveIcon = cActionRemoveIcon::Ref::New(*this, pListIcons);
	helper.Button(frameLine, pActionRemoveIcon);
	
	// exclude patterns
	helper.GroupBoxStaticFlow(subGroup, subGroup2, "Exclude Patterns:");
	
	description = "Set of file patterns to exclude from projribution process.";
	helper.FormLineStretchFirst(subGroup2, "Pattern:", description, frameLine);
	helper.EditString(frameLine, description, pEditExcludePattern, {});
	pActionAddExcludePattern = cActionAddExcludePattern::Ref::New(*this, pEditExcludePattern);
	helper.Button(frameLine, pActionAddExcludePattern);
	
	helper.ListBox(subGroup2, 4, description, pListExcludePatterns, {});
	pListExcludePatterns->SetDefaultSorter();
	
	pActionRemoveExcludePattern = cActionRemoveExcludePattern::Ref::New(*this, pListExcludePatterns);
	helper.Button(frameLine, pActionRemoveExcludePattern);
	
	// required extensions
	helper.GroupBoxStaticFlow(subGroup, subGroup2, "Required Extensions:");
	
	description = "Set of resource file extensions (.extension) required by the project";
	helper.FormLineStretchFirst(subGroup2, "Extension:", description, frameLine);
	helper.EditString(frameLine, description, pEditRequiredExtension, {});
	pActionAddRequiredExtension = cActionAddRequiredExtension::Ref::New(*this, pEditRequiredExtension);
	helper.Button(frameLine, pActionAddRequiredExtension);
	
	helper.ListBox(subGroup2, 4, description, pListRequiredExtensions, {});
	pListRequiredExtensions->SetDefaultSorter();
	
	pActionRemoveRequiredExtension = cActionRemoveRequiredExtension::Ref::New(*this, pListRequiredExtensions);
	helper.Button(frameLine, pActionRemoveRequiredExtension);
	
	
	// row
	subGroup = igdeContainerBox::Ref::New(env, igdeContainerBox::eaX);
	sidePanel->AddChild(subGroup);
	
	// delga parameters
	helper.GroupBox(subGroup, groupBox, "DELGA Parameters:");
	
	description = "VFS directory where to place the build DELGA file.";
	helper.FormLineStretchFirst(groupBox, "DELGA File:", description, frameLine);
	helper.EditString(frameLine, description, pEditDelgaPath, cTextDelgaPath::Ref::New(*this));
	pActionDelgaPath = cActionDelgaPath::Ref::New(*this);
	helper.Button(frameLine, pActionDelgaPath);
	
	
	// test running
	helper.GroupBox(subGroup, groupBox, "Test Run:");
	
	helper.EditString(groupBox, "Arguments:", "Arguments to use while test running",
		pEditRunArguments, cTextRunArguments::Ref::New(*this));
}

projPanelProfiles::~projPanelProfiles(){
	SetProject(nullptr);
}



// Management
///////////////

void projPanelProfiles::SetProject(projProject *project){
	if(project == pProject){
		return;
	}
	
	pEditIconPath->ClearPath();
	pEditExcludePattern->ClearText();
	pEditRequiredExtension->ClearText();
	
	if(pProject){
		pProject->RemoveListener(pListener);
	}
	
	pProject = project;
	
	if(project){
		project->AddListener(pListener);
	}
	
	UpdateProject();
	UpdateProfiles();
}

void projPanelProfiles::UpdateProject(){
	pListProfiles->SetEnabled(pProject);
}

projProfile *projPanelProfiles::GetActiveProfile() const{
	if(!pProject){
		return nullptr;
	}
	return pProject->GetActiveProfile();
}

void projPanelProfiles::SelectActiveProfile(){
	pListProfiles->SetSelection(pListProfiles->IndexOfItemWithData(GetActiveProfile()));
	if(pListProfiles->GetSelection() != -1){
		pListProfiles->MakeItemVisible(pListProfiles->GetSelection());
	}
	UpdateProfile();
}

void projPanelProfiles::UpdateProfiles(){
	pListProfiles->RemoveAllItems();
	
	if(pProject){
		const projProfileList &list = pProject->GetProfiles();
		const int count = list.GetCount();
		int i;
		
		for(i=0; i<count; i++){
			projProfile * const profile = list.GetAt(i);
			pListProfiles->AddItem(profile->GetName(), nullptr, profile);
		}
		
		pListProfiles->SortItems();
	}
	
	SelectActiveProfile();
}

void projPanelProfiles::UpdateProfile(){
	const projProfile * const profile = GetActiveProfile();
	
	pActionScriptDirectory->Update();
	pActionPathCapture->Update();
	pActionPathConfig->Update();
	pActionDelgaPath->Update();
	
	const bool enable = profile != nullptr;
	pEditName->SetEnabled(enable);
	pEditDescription->SetEnabled(enable);
	pEditScriptDirectory->SetEnabled(enable);
	pEditGameObject->SetEnabled(enable);
	pEditPathConfig->SetEnabled(enable);
	pEditPathCapture->SetEnabled(enable);
	pEditIdentifier->SetEnabled(enable);
	pEditAliasIdentifier->SetEnabled(enable);
	pEditTitle->SetEnabled(enable);
	pEditGameDescription->SetEnabled(enable);
	pEditWindowSize->SetEnabled(enable);
	pEditDelgaPath->SetEnabled(enable);
	pEditRunArguments->SetEnabled(enable);
	
	if(profile){
		pEditName->SetText(profile->GetName());
		pEditDescription->SetText(profile->GetDescription());
		pEditScriptDirectory->SetText(profile->GetScriptDirectory());
		pEditGameObject->SetText(profile->GetGameObject());
		pEditPathConfig->SetText(profile->GetPathConfig());
		pEditPathCapture->SetText(profile->GetPathCapture());
		pEditIdentifier->SetText(profile->GetIdentifier().ToHexString(false));
		pEditAliasIdentifier->SetText(profile->GetAliasIdentifier());
		pEditTitle->SetText(profile->GetTitle());
		pEditGameDescription->SetText(profile->GetGameDescription());
		pEditCreator->SetText(profile->GetCreator());
		pEditWebsite->SetText(profile->GetWebsite());
		pEditWindowSize->SetPoint(profile->GetWindowSize());
		pEditDelgaPath->SetText(profile->GetDelgaPath());
		pEditRunArguments->SetText(profile->GetRunArguments());
		
	}else{
		pEditName->ClearText();
		pEditDescription->ClearText();
		pEditScriptDirectory->ClearText();
		pEditGameObject->ClearText();
		pEditPathConfig->ClearText();
		pEditPathCapture->ClearText();
		pEditIdentifier->ClearText();
		pEditAliasIdentifier->ClearText();
		pEditTitle->ClearText();
		pEditGameDescription->ClearText();
		pEditCreator->ClearText();
		pEditWebsite->ClearText();
		pEditWindowSize->SetPoint(decPoint());
		pEditDelgaPath->ClearText();
		pEditRunArguments->ClearText();
	}
	
	UpdateIcons();
	UpdateExcludePatterns();
	UpdateRequiredExtensions();
}

void projPanelProfiles::UpdateIcons(){
	decString selection;
	if(pListIcons->GetSelectedItem()){
		selection = pListIcons->GetSelectedItem()->GetText();
	}
	
	pListIcons->RemoveAllItems();
	
	const projProfile * const profile = GetActiveProfile();
	if(profile){
		const decStringSet &icons = profile->GetIcons();
		const int count = icons.GetCount();
		int i;
		
		for(i=0; i<count; i++){
			pListIcons->AddItem(icons.GetAt(i));
		}
		
		pListIcons->SortItems();
	}
	
	if(!selection.IsEmpty()){
		pListIcons->SetSelection(pListIcons->IndexOfItem(selection));
	}
	
	if(pListIcons->GetSelection() == -1 && pListIcons->GetItems().IsNotEmpty()){
		pListIcons->SetSelection(0);
	}
	
	pEditIconPath->SetEnabled(profile != nullptr);
	pListIcons->SetEnabled(profile != nullptr);
	
	pActionAddIcon->Update();
	pActionRemoveIcon->Update();
}

void projPanelProfiles::UpdateExcludePatterns(){
	decString selection;
	if(pListExcludePatterns->GetSelectedItem()){
		selection = pListExcludePatterns->GetSelectedItem()->GetText();
	}
	
	pListExcludePatterns->RemoveAllItems();
	
	const projProfile * const profile = GetActiveProfile();
	if(profile){
		const decStringSet &patterns = profile->GetExcludePatterns();
		const int count = patterns.GetCount();
		int i;
		
		for(i=0; i<count; i++){
			pListExcludePatterns->AddItem(patterns.GetAt(i));
		}
		
		pListExcludePatterns->SortItems();
	}
	
	if(!selection.IsEmpty()){
		pListExcludePatterns->SetSelection(pListExcludePatterns->IndexOfItem(selection));
	}
	
	if(pListExcludePatterns->GetSelection() == -1 && pListExcludePatterns->GetItems().IsNotEmpty()){
		pListExcludePatterns->SetSelection(0);
	}
	
	pEditExcludePattern->SetEnabled(profile != nullptr);
	pListExcludePatterns->SetEnabled(profile != nullptr);
	
	pActionAddExcludePattern->Update();
	pActionRemoveExcludePattern->Update();
}

void projPanelProfiles::UpdateRequiredExtensions(){
	decString selection;
	if(pListRequiredExtensions->GetSelectedItem()){
		selection = pListRequiredExtensions->GetSelectedItem()->GetText();
	}
	
	pListRequiredExtensions->RemoveAllItems();
	
	const projProfile * const profile = GetActiveProfile();
	if(profile){
		const decStringSet &extensions = profile->GetRequiredExtensions();
		const int count = extensions.GetCount();
		int i;
		
		for(i=0; i<count; i++){
			pListRequiredExtensions->AddItem(extensions.GetAt(i));
		}
		
		pListRequiredExtensions->SortItems();
	}
	
	if(!selection.IsEmpty()){
		pListRequiredExtensions->SetSelection(pListRequiredExtensions->IndexOfItem(selection));
	}
	
	if(pListRequiredExtensions->GetSelection() == -1 && pListRequiredExtensions->GetItems().IsNotEmpty()){
		pListRequiredExtensions->SetSelection(0);
	}
	
	pEditRequiredExtension->SetEnabled(profile != nullptr);
	pListRequiredExtensions->SetEnabled(profile != nullptr);
	
	pActionAddRequiredExtension->Update();
	pActionRemoveRequiredExtension->Update();
}
