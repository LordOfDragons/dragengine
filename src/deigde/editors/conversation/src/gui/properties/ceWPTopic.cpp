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
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "ceWPTopic.h"
#include "ceWPTopicListener.h"
#include "ceWindowProperties.h"
#include "actions/ceWPACameraShot.h"
#include "actions/ceWPAMusic.h"
#include "actions/ceWPAActorSpeak.h"
#include "actions/ceWPAIfElse.h"
#include "actions/ceWPAPlayerChoice.h"
#include "actions/ceWPAStopConversation.h"
#include "actions/ceWPAStopTopic.h"
#include "actions/ceWPASnippet.h"
#include "actions/ceWPASetVariable.h"
#include "actions/ceWPASetAParam.h"
#include "actions/ceWPAActorCmd.h"
#include "actions/ceWPAGameCommand.h"
#include "actions/ceWPAWait.h"
#include "actions/ceWPATrigger.h"
#include "actions/ceWPAActorAdd.h"
#include "actions/ceWPAActorRemove.h"
#include "actions/ceWPACoordSystemAdd.h"
#include "actions/ceWPACoordSystemRemove.h"
#include "actions/ceWPAComment.h"
#include "conditions/ceWPCLogic.h"
#include "conditions/ceWPCHasActor.h"
#include "conditions/ceWPCActorInConversation.h"
#include "conditions/ceWPCVariable.h"
#include "conditions/ceWPCAParam.h"
#include "conditions/ceWPCActorCommand.h"
#include "conditions/ceWPCGameCommand.h"
#include "conditions/ceWPCTrigger.h"
#include "model/ceWPTTreeModel.h"
#include "model/ceWPTTreeItem.h"
#include "model/ceWPTTreeItemModel.h"
#include "model/action/ceWPTTIMAction.h"
#include "model/condition/ceWPTTIMCondition.h"
#include "../ceWindowMain.h"
#include "../../clipboard/ceClipboardDataTopic.h"
#include "../../clipboard/ceClipboardDataFile.h"
#include "../../conversation/ceConversation.h"
#include "../../conversation/action/ceConversationAction.h"
#include "../../conversation/action/ceCAPlayerChoiceOption.h"
#include "../../conversation/action/ceCACameraShot.h"
#include "../../conversation/action/ceCAMusic.h"
#include "../../conversation/action/ceCAIfElse.h"
#include "../../conversation/action/ceCAActorSpeak.h"
#include "../../conversation/action/ceCAPlayerChoice.h"
#include "../../conversation/action/ceCAStopConversation.h"
#include "../../conversation/action/ceCAStopTopic.h"
#include "../../conversation/action/ceCASnippet.h"
#include "../../conversation/action/ceCASetVariable.h"
#include "../../conversation/action/ceCASetActorParameter.h"
#include "../../conversation/action/ceCAActorCommand.h"
#include "../../conversation/action/ceCAGameCommand.h"
#include "../../conversation/action/ceCAWait.h"
#include "../../conversation/action/ceCATrigger.h"
#include "../../conversation/action/ceCAActorAdd.h"
#include "../../conversation/action/ceCAActorRemove.h"
#include "../../conversation/action/ceCACoordSystemAdd.h"
#include "../../conversation/action/ceCACoordSystemRemove.h"
#include "../../conversation/action/ceCAComment.h"
#include "../../conversation/action/ceCAIfElseCase.h"
#include "../../conversation/condition/ceCConditionLogic.h"
#include "../../conversation/condition/ceCConditionHasActor.h"
#include "../../conversation/condition/ceCConditionActorInConversation.h"
#include "../../conversation/condition/ceCConditionVariable.h"
#include "../../conversation/condition/ceCConditionActorParameter.h"
#include "../../conversation/condition/ceCConditionActorCommand.h"
#include "../../conversation/condition/ceCConditionGameCommand.h"
#include "../../conversation/condition/ceCConditionTrigger.h"
#include "../../conversation/file/ceConversationFile.h"
#include "../../conversation/topic/ceConversationTopic.h"
#include "../../conversation/playback/cePlayback.h"
#include "../../conversation/playback/cePlaybackActionStackEntry.h"
#include "../../conversation/playback/cePlaybackProcessAction.h"
#include "../../conversation/actor/ceConversationActor.h"
#include "../../conversation/actor/speechAnimation/ceSpeechAnimation.h"
#include "../../conversation/strip/ceStrip.h"
#include "../../configuration/ceConfiguration.h"
#include "../../undosys/file/ceUCFileAdd.h"
#include "../../undosys/file/ceUCFileRemove.h"
#include "../../undosys/file/ceUCFileSetID.h"
#include "../../undosys/file/ceUCFilePaste.h"
#include "../../undosys/file/ceUCFileDuplicate.h"
#include "../../undosys/topic/ceUCTopicAdd.h"
#include "../../undosys/topic/ceUCTopicRemove.h"
#include "../../undosys/topic/ceUCTopicSetID.h"
#include "../../undosys/topic/ceUCTopicPaste.h"
#include "../../undosys/topic/ceUCTopicDuplicate.h"

#include <deigde/clipboard/igdeClipboard.h>
#include <deigde/clipboard/igdeClipboardData.h>
#include <deigde/engine/igdeEngineController.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeTreeList.h>
#include <deigde/gui/igdeSwitcher.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerBorder.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionContextMenu.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeTreeListListener.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Definitions
////////////////

enum ePanels{
	epEmpty,
	
	epACameraShot,
	epAMusic,
	epAActorSpeak,
	epAIfElse,
	epAPlayerChoice,
	epAStopConversation,
	epAStopTopic,
	epASnippet,
	epASetVariable,
	epASetAParam,
	epAActorCmd,
	epAGameCommand,
	epAWait,
	epATrigger,
	epAActorAdd,
	epAActorRemove,
	epACoordSystemAdd,
	epACoordSystemRemove,
	epAComment,
	
	epCLogic,
	epCHasActor,
	epCActorInConversation,
	epCVariable,
	epCAParam,
	epCActorCommand,
	epCGameCommand,
	epCTrigger
};



// Actions
////////////

namespace {

class cComboFile : public igdeComboBoxListener{
	ceWPTopic &pPanel;
	
public:
	typedef deTObjectReference<cComboFile> Ref;
	cComboFile(ceWPTopic &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		ceConversation * const conversation = pPanel.GetConversation();
		if(conversation){
			conversation->SetActiveFile(comboBox->GetSelectedItem()
				? (ceConversationFile*)comboBox->GetSelectedItem()->GetData() : nullptr);
		}
	}
};

class cActionGroupAdd : public igdeAction{
public:
	typedef deTObjectReference<cActionGroupAdd> Ref;
	
private:
	ceWPTopic &pPanel;
	
public:
	cActionGroupAdd(ceWPTopic &panel) : igdeAction("Add...",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "Add topic group"),
		pPanel(panel){}
	
	void OnAction() override{
		ceConversation * const conversation = pPanel.GetConversation();
		if(!conversation){
			return;
		}
		
		decString name("Group");
		if(!igdeCommonDialogs::GetString(&pPanel, "Add Topic Group", "Name:", name)){
			return;
		}
		
		if(conversation->GetFiles().HasMatching([&](const ceConversationFile &f){ return f.GetID() == name; })){
			igdeCommonDialogs::Error(&pPanel, "Add Topic Group", "Topic group exists");
			return;
		}
		
		igdeUndo::Ref undo;
		const ceConversationFile::Ref group(ceConversationFile::Ref::New(name));
		undo = ceUCFileAdd::Ref::New(conversation, group);
		conversation->GetUndoSystem()->Add(undo);
	}
	
	void Update() override{
		SetEnabled(pPanel.GetConversation());
	}
};

class cActionGroupRemove : public igdeAction{
public:
	typedef deTObjectReference<cActionGroupRemove> Ref;
	
private:
	ceWPTopic &pPanel;
	
public:
	cActionGroupRemove(ceWPTopic &panel) : igdeAction("Remove",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove topic group"),
		pPanel(panel){}
	
	void OnAction() override{
		if(pPanel.GetFile()){
			pPanel.GetConversation()->GetUndoSystem()->Add(
				ceUCFileRemove::Ref::New(pPanel.GetFile()));
		}
	}
	
	void Update() override{
		SetEnabled(pPanel.GetFile());
	}
};

class cActionGroupRename : public igdeAction{
public:
	typedef deTObjectReference<cActionGroupRename> Ref;
	
private:
	ceWPTopic &pPanel;
	
public:
	cActionGroupRename(ceWPTopic &panel) : igdeAction("Rename...",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "Rename topic group"),
		pPanel(panel){}
	
	void OnAction() override{
		ceConversation * const conversation = pPanel.GetConversation();
		ceConversationFile * const file = pPanel.GetFile();
		if(!file || !conversation){
			return;
		}
		
		decString name(file->GetID());
		if(!igdeCommonDialogs::GetString(&pPanel, "Rename Topic Group", "Name:", name) || name == file->GetID()){
			return;
		}
		
		if(conversation->GetFiles().HasMatching([&](const ceConversationFile &f){ return f.GetID() == name; })){
			igdeCommonDialogs::Error(&pPanel, "Rename Topic Group", "Topic group exists");
			return;
		}
		
		conversation->GetUndoSystem()->Add(ceUCFileSetID::Ref::New(*conversation, file, name));
	}
	
	void Update() override{
		SetEnabled(pPanel.GetFile());
	}
};

class cActionGroupCopy : public igdeAction{
public:
	typedef deTObjectReference<cActionGroupCopy> Ref;
	
private:
	ceWPTopic &pPanel;
	
public:
	cActionGroupCopy(ceWPTopic &panel) : igdeAction("Copy",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy), "Copy topic group"),
		pPanel(panel){}
	
	void OnAction() override{
		ceConversationFile * const file = pPanel.GetFile();
		if(!file){
			return;
		}
		
		igdeClipboardData::Ref clip;
		ceConversationFile::List list;
		list.Add(file);
		clip = ceClipboardDataFile::Ref::New(list);
		pPanel.GetWindowProperties().GetWindowMain().GetClipboard().Set(clip);
	}
	
	void Update() override{
		SetEnabled(pPanel.GetFile());
	}
};

class cActionGroupPaste : public igdeAction{
public:
	typedef deTObjectReference<cActionGroupPaste> Ref;
	
private:
	ceWPTopic &pPanel;
	
public:
	cActionGroupPaste(ceWPTopic &panel) : igdeAction("Paste",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPaste), "Paste topic group"),
		pPanel(panel){}
	
	void OnAction() override{
		ceConversation * const conversation = pPanel.GetConversation();
		ceClipboardDataFile * const clip = (ceClipboardDataFile*)pPanel.GetWindowProperties().GetWindowMain()
			.GetClipboard().GetWithTypeName(ceClipboardDataFile::TYPE_NAME);
		if(!conversation || !clip){
			return;
		}
		
		conversation->GetUndoSystem()->Add(
			ceUCFilePaste::Ref::New(conversation, clip->GetFiles()));
	}
	
	void Update() override{
		SetEnabled(pPanel.GetConversation() && pPanel.GetWindowProperties().GetWindowMain().GetClipboard()
			.HasWithTypeName(ceClipboardDataFile::TYPE_NAME));
	}
};

class cActionGroupDuplicate : public igdeAction{
public:
	typedef deTObjectReference<cActionGroupDuplicate> Ref;
	
private:
	ceWPTopic &pPanel;
	
public:
	cActionGroupDuplicate(ceWPTopic &panel) : igdeAction("Duplicate...",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "Duplicate topic group"),
		pPanel(panel){}
	
	void OnAction() override{
		ceConversation * const conversation = pPanel.GetConversation();
		ceConversationFile * const file = pPanel.GetFile();
		if(!file || !conversation){
			return;
		}
		
		decString name(file->GetID());
		if(!igdeCommonDialogs::GetString(&pPanel, "Duplicate Topic Group", "Name:", name)){
			return;
		}
		
		if(conversation->GetFiles().HasMatching([&](const ceConversationFile &f){ return f.GetID() == name; })){
			igdeCommonDialogs::Error(&pPanel, "Duplicate Topic Group", "Topic group exists");
			return;
		}
		
		conversation->GetUndoSystem()->Add(ceUCFileDuplicate::Ref::New(
			conversation, *file, name));
	}
	
	void Update() override{
		SetEnabled(pPanel.GetFile());
	}
};

class cActionGroupMissingWords : public igdeAction{
public:
	typedef deTObjectReference<cActionGroupMissingWords> Ref;
	
private:
	ceWPTopic &pPanel;
	
public:
	cActionGroupMissingWords(ceWPTopic &panel) : igdeAction("Missing Words...",
		nullptr, "Show missing words"), pPanel(panel){}
	
	void OnAction() override{
		ceConversationFile * const group = pPanel.GetFile();
		if(!group){
			return;
		}
		
		decStringSet missingWords;
		group->GetTopics().Visit([&](const ceConversationTopic &t){
			t.FindMissingWords(missingWords);
		});
		pPanel.GetWindowProperties().GetWindowMain().ShowFoundMissingWordsDialog(missingWords);
	}
	
	void Update() override{
		SetEnabled(pPanel.GetFile());
	}
};

class cActionFileMenu : public igdeActionContextMenu{
	ceWPTopic &pPanel;
	
public:
	typedef deTObjectReference<cActionFileMenu> Ref;
	cActionFileMenu(ceWPTopic &panel) : igdeActionContextMenu("",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallDown), "Show File Menu"),
		pPanel(panel){}
	
	virtual void AddContextMenuEntries(igdeMenuCascade &contextMenu){
		igdeUIHelper &helper = contextMenu.GetEnvironment().GetUIHelper();
		
		helper.MenuCommand(contextMenu, cActionGroupAdd::Ref::New(pPanel));
		helper.MenuCommand(contextMenu, cActionGroupRemove::Ref::New(pPanel));
		helper.MenuCommand(contextMenu, cActionGroupRename::Ref::New(pPanel));
		
		helper.MenuSeparator(contextMenu);
		helper.MenuCommand(contextMenu, cActionGroupMissingWords::Ref::New(pPanel));
		
		helper.MenuSeparator(contextMenu);
		helper.MenuCommand(contextMenu, cActionGroupCopy::Ref::New(pPanel));
		helper.MenuCommand(contextMenu, cActionGroupPaste::Ref::New(pPanel));
		helper.MenuCommand(contextMenu, cActionGroupDuplicate::Ref::New(pPanel));
	}
	
	void Update() override{
		SetEnabled(pPanel.GetConversation());
	}
};



class cComboTopic : public igdeComboBoxListener{
	ceWPTopic &pPanel;
	
public:
	typedef deTObjectReference<cComboTopic> Ref;
	cComboTopic(ceWPTopic &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		ceConversationFile * const file = pPanel.GetFile();
		if(file){
			file->SetActiveTopic(comboBox->GetSelectedItem()
				? (ceConversationTopic*)comboBox->GetSelectedItem()->GetData() : nullptr);
		}
	}
};

class cActionTopicAdd : public igdeAction{
public:
	typedef deTObjectReference<cActionTopicAdd> Ref;
	
private:
	ceWPTopic &pPanel;
	
public:
	cActionTopicAdd(ceWPTopic &panel) : igdeAction("Add...",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "Add topic"),
		pPanel(panel){}
	
	void OnAction() override{
		ceConversationFile * const file = pPanel.GetFile();
		if(!file){
			return;
		}
		
		decString name("Topic");
		if(!igdeCommonDialogs::GetString(&pPanel, "Add Topic", "Name:", name)){
			return;
		}
		
		if(file->GetTopics().HasMatching([&](const ceConversationTopic &t){ return t.GetID() == name; })){
			igdeCommonDialogs::Error(&pPanel, "Add Topic", "Topic exists");
			return;
		}
		
		igdeUndo::Ref undo;
		const ceConversationTopic::Ref topic(ceConversationTopic::Ref::New(name));
		undo = ceUCTopicAdd::Ref::New(file, topic);
		pPanel.GetConversation()->GetUndoSystem()->Add(undo);
	}
	
	void Update() override{
		SetEnabled(pPanel.GetFile());
	}
};

class cActionTopicRemove : public igdeAction{
public:
	typedef deTObjectReference<cActionTopicRemove> Ref;
	
private:
	ceWPTopic &pPanel;
	
public:
	cActionTopicRemove(ceWPTopic &panel) : igdeAction("Remove",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove topic"),
		pPanel(panel){}
	
	void OnAction() override{
		if(pPanel.GetTopic()){
			pPanel.GetConversation()->GetUndoSystem()->Add(
				ceUCTopicRemove::Ref::New(pPanel.GetTopic()));
		}
	}
	
	void Update() override{
		SetEnabled(pPanel.GetTopic());
	}
};

class cActionTopicRename : public igdeAction{
public:
	typedef deTObjectReference<cActionTopicRename> Ref;
	
private:
	ceWPTopic &pPanel;
	
public:
	cActionTopicRename(ceWPTopic &panel) : igdeAction("Rename...",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "Rename topic"),
		pPanel(panel){}
	
	void OnAction() override{
		ceConversationFile * const file = pPanel.GetFile();
		ceConversationTopic * const topic = pPanel.GetTopic();
		ceConversation * const conversation = pPanel.GetConversation();
		if(!topic || !file || !conversation){
			return;
		}
		
		decString name(topic->GetID());
		if(!igdeCommonDialogs::GetString(&pPanel, "Rename Topic", "Name:", name) || name == topic->GetID()){
			return;
		}
		
		if(file->GetTopics().HasMatching([&](const ceConversationTopic &t){ return t.GetID() == name; })){
			igdeCommonDialogs::Error(&pPanel, "Rename Topic", "Topic exists");
			return;
		}
		
		conversation->GetUndoSystem()->Add(ceUCTopicSetID::Ref::New(*conversation, topic, name));
	}
	
	void Update() override{
		SetEnabled(pPanel.GetTopic());
	}
};

class cActionTopicMissingWords : public igdeAction{
public:
	typedef deTObjectReference<cActionTopicMissingWords> Ref;
	
private:
	ceWPTopic &pPanel;
	
public:
	cActionTopicMissingWords(ceWPTopic &panel) : igdeAction("Missing Words...",
		nullptr, "Show missing words"), pPanel(panel){}
	
	void OnAction() override{
		ceConversationTopic * const topic = pPanel.GetTopic();
		if(!topic){
			return;
		}
		
		decStringSet missingWords;
		topic->FindMissingWords(missingWords);
		pPanel.GetWindowProperties().GetWindowMain().ShowFoundMissingWordsDialog(missingWords);
	}
	
	void Update() override{
		SetEnabled(pPanel.GetTopic());
	}
};

class cActionTopicCopy : public igdeAction{
public:
	typedef deTObjectReference<cActionTopicCopy> Ref;
	
private:
	ceWPTopic &pPanel;
	
public:
	cActionTopicCopy(ceWPTopic &panel) : igdeAction("Copy",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy), "Copy topic"),
		pPanel(panel){}
	
	void OnAction() override{
		ceConversationTopic * const topic = pPanel.GetTopic();
		if(!topic){
			return;
		}
		
		igdeClipboardData::Ref clip;
		ceConversationTopic::List list;
		list.Add(topic);
		clip = ceClipboardDataTopic::Ref::New(list);
		pPanel.GetWindowProperties().GetWindowMain().GetClipboard().Set(clip);
	}
	
	void Update() override{
		SetEnabled(pPanel.GetTopic());
	}
};

class cActionTopicPaste : public igdeAction{
public:
	typedef deTObjectReference<cActionTopicPaste> Ref;
	
private:
	ceWPTopic &pPanel;
	
public:
	cActionTopicPaste(ceWPTopic &panel) : igdeAction("Paste",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPaste), "Paste topic"),
		pPanel(panel){}
	
	void OnAction() override{
		ceConversationFile * const file = pPanel.GetFile();
		ceClipboardDataTopic * const clip = (ceClipboardDataTopic*)pPanel.GetWindowProperties().GetWindowMain()
			.GetClipboard().GetWithTypeName(ceClipboardDataTopic::TYPE_NAME);
		if(!file || !clip){
			return;
		}
		
		pPanel.GetConversation()->GetUndoSystem()->Add(
			ceUCTopicPaste::Ref::New(file, clip->GetTopics()));
	}
	
	void Update() override{
		SetEnabled(pPanel.GetFile() && pPanel.GetWindowProperties().GetWindowMain().GetClipboard()
			.HasWithTypeName(ceClipboardDataTopic::TYPE_NAME));
	}
};

class cActionTopicDuplicate : public igdeAction{
public:
	typedef deTObjectReference<cActionTopicDuplicate> Ref;
	
private:
	ceWPTopic &pPanel;
	
public:
	cActionTopicDuplicate(ceWPTopic &panel) : igdeAction("Duplicate...",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "Duplicate topic"),
		pPanel(panel){}
	
	void OnAction() override{
		ceConversationFile * const file = pPanel.GetFile();
		ceConversationTopic * const topic = pPanel.GetTopic();
		if(!file || !topic){
			return;
		}
		
		decString name(topic->GetID());
		if(!igdeCommonDialogs::GetString(&pPanel, "Duplicate Topic", "Name:", name)){
			return;
		}
		
		if(file->GetTopics().HasMatching([&](const ceConversationTopic &t){ return t.GetID() == name; })){
			igdeCommonDialogs::Error(&pPanel, "Duplicate Topic", "Topic exists");
			return;
		}
		
		pPanel.GetConversation()->GetUndoSystem()->Add(
			ceUCTopicDuplicate::Ref::New(file, *topic, name));
	}
	
	void Update() override{
		SetEnabled(pPanel.GetTopic());
	}
};

class cActionTopicMenu : public igdeActionContextMenu{
	ceWPTopic &pPanel;
	
public:
	typedef deTObjectReference<cActionTopicMenu> Ref;
	cActionTopicMenu(ceWPTopic &panel) : igdeActionContextMenu("",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallDown), "Show Topic Menu"),
		pPanel(panel){}
	
	virtual void AddContextMenuEntries(igdeMenuCascade &contextMenu){
		igdeUIHelper &helper = contextMenu.GetEnvironment().GetUIHelper();
		
		helper.MenuCommand(contextMenu, cActionTopicAdd::Ref::New(pPanel));
		helper.MenuCommand(contextMenu, cActionTopicRemove::Ref::New(pPanel));
		helper.MenuCommand(contextMenu, cActionTopicRename::Ref::New(pPanel));
		
		helper.MenuSeparator(contextMenu);
		helper.MenuCommand(contextMenu, cActionTopicMissingWords::Ref::New(pPanel));
		
		helper.MenuSeparator(contextMenu);
		helper.MenuCommand(contextMenu, cActionTopicCopy::Ref::New(pPanel));
		helper.MenuCommand(contextMenu, cActionTopicPaste::Ref::New(pPanel));
		helper.MenuCommand(contextMenu, cActionTopicDuplicate::Ref::New(pPanel));
	}
	
	void Update() override{
		SetEnabled(pPanel.GetFile());
	}
};



class cTreeActionsListener : public igdeTreeListListener{
	ceWPTopic &pPanel;
	
public:
	typedef deTObjectReference<cTreeActionsListener> Ref;
	cTreeActionsListener(ceWPTopic &panel) : pPanel(panel){}
	
	virtual void OnSelectionChanged(igdeTreeList *treeList){
		ceConversationTopic * const topic = pPanel.GetTopic();
		if(!topic || pPanel.GetActionTreeModel()->GetPreventUpdate()){
			return;
		}
		
		ceWPTTreeItem * const item = (ceWPTTreeItem*)treeList->GetSelection().Pointer();
		if(item){
			item->OnSelected();
		}
		pPanel.SyncTopicActive();
	}
	
	virtual void OnItemExpanded(igdeTreeList*, igdeTreeItem *item){
		((ceWPTTreeItem*)item)->OnExpanded();
	}
	
	virtual void OnItemCollapsed(igdeTreeList*, igdeTreeItem *item){
		((ceWPTTreeItem*)item)->OnCollapsed();
	}
	
	virtual void AddContextMenuEntries(igdeTreeList *treeList, igdeMenuCascade &menu){
		if(treeList->GetSelection()){
			((ceWPTTreeItem&)*treeList->GetSelection()).OnContextMenu(menu);
			
		}else{
			pPanel.GetActionTreeModel()->OnContextMenu(menu);
		}
	}
};

}


// Class ceWPTopic
////////////////////

// Constructor, destructor
////////////////////////////

ceWPTopic::ceWPTopic(ceWindowProperties &windowProperties) :
igdeContainerFlow(windowProperties.GetEnvironment(), igdeContainerFlow::eaY, igdeContainerFlow::esLast),
pWindowProperties(windowProperties),

pPanelACameraShot(nullptr),
pPanelAMusic(nullptr),
pPanelAActorSpeak(nullptr),
pPanelAIfElse(nullptr),
pPanelAPlayerChoice(nullptr),
pPanelAStopConversation(nullptr),
pPanelAStopTopic(nullptr),
pPanelASnippet(nullptr),
pPanelASetVariable(nullptr),
pPanelASetAParam(nullptr),
pPanelAActorCmd(nullptr),
pPanelAGameCommand(nullptr),
pPanelAWait(nullptr),
pPanelATrigger(nullptr),
pPanelAActorAdd(nullptr),
pPanelAActorRemove(nullptr),
pPanelACoordSystemAdd(nullptr),
pPanelACoordSystemRemove(nullptr),
pPanelAComment(nullptr),

pPanelCLogic(nullptr),
pPanelCHasActor(nullptr),
pPanelCActorInConversation(nullptr),
pPanelCVariable(nullptr),
pPanelCAParam(nullptr),
pPanelCActorCommand(nullptr),
pPanelCGameCommand(nullptr),
pPanelCTrigger(nullptr)
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref groupBox, formLine;
	
	pListener = ceWPTopicListener::Ref::New(*this);
	
	
	// conversation
	helper.GroupBox(*this, groupBox, "Conversation:");
	
	helper.FormLineStretchFirst(groupBox, "File:", "File to edit", formLine);
	helper.ComboBoxFilter(formLine, "File to edit", pCBFile, cComboFile::Ref::New(*this));
	pCBFile->SetDefaultSorter();
	const cActionFileMenu::Ref actionFileMenu(cActionFileMenu::Ref::New(*this));
	helper.Button(formLine, pBtnFile, actionFileMenu);
	actionFileMenu->SetWidget(pBtnFile);
	
	helper.FormLineStretchFirst(groupBox, "Topic:", "Topic to edit", formLine);
	helper.ComboBoxFilter(formLine, "Topic to edit", pCBTopic, cComboTopic::Ref::New(*this));
	pCBTopic->SetDefaultSorter();
	const cActionTopicMenu::Ref actionTopicMenu(cActionTopicMenu::Ref::New(*this));
	helper.Button(formLine, pBtnTopic, actionTopicMenu);
	actionTopicMenu->SetWidget(pBtnTopic);
	
	
	// actions
	igdeContainerBorder::Ref groupActions;
	
	helper.GroupBoxStaticBorder(*this, groupActions, "Actions:", true);
	helper.TreeList(10, "Topic Actions", pTreeActions, cTreeActionsListener::Ref::New(*this));
	groupActions->AddChild(pTreeActions, igdeContainerBorder::eaCenter);
	
	pSwitcher = igdeSwitcher::Ref::New(env);
	groupActions->AddChild(pSwitcher, igdeContainerBorder::eaBottom);
	
	helper.Label(pSwitcher, ""); // empty panel
	
	pPanelACameraShot = ceWPACameraShot::Ref::New(*this);
	pSwitcher->AddChild(pPanelACameraShot);
	pPanelAMusic = ceWPAMusic::Ref::New(*this);
	pSwitcher->AddChild(pPanelAMusic);
	pPanelAActorSpeak = ceWPAActorSpeak::Ref::New(*this);
	pSwitcher->AddChild(pPanelAActorSpeak);
	pPanelAIfElse = ceWPAIfElse::Ref::New(*this);
	pSwitcher->AddChild(pPanelAIfElse);
	pPanelAPlayerChoice = ceWPAPlayerChoice::Ref::New(*this);
	pSwitcher->AddChild(pPanelAPlayerChoice);
	pPanelAStopConversation = ceWPAStopConversation::Ref::New(*this);
	pSwitcher->AddChild(pPanelAStopConversation);
	pPanelAStopTopic = ceWPAStopTopic::Ref::New(*this);
	pSwitcher->AddChild(pPanelAStopTopic);
	pPanelASnippet = ceWPASnippet::Ref::New(*this);
	pSwitcher->AddChild(pPanelASnippet);
	pPanelASetVariable = ceWPASetVariable::Ref::New(*this);
	pSwitcher->AddChild(pPanelASetVariable);
	pPanelASetAParam = ceWPASetAParam::Ref::New(*this);
	pSwitcher->AddChild(pPanelASetAParam);
	pPanelAActorCmd = ceWPAActorCmd::Ref::New(*this);
	pSwitcher->AddChild(pPanelAActorCmd);
	pPanelAGameCommand = ceWPAGameCommand::Ref::New(*this);
	pSwitcher->AddChild(pPanelAGameCommand);
	pPanelAWait = ceWPAWait::Ref::New(*this);
	pSwitcher->AddChild(pPanelAWait);
	pPanelATrigger = ceWPATrigger::Ref::New(*this);
	pSwitcher->AddChild(pPanelATrigger);
	pPanelAActorAdd = ceWPAActorAdd::Ref::New(*this);
	pSwitcher->AddChild(pPanelAActorAdd);
	pPanelAActorRemove = ceWPAActorRemove::Ref::New(*this);
	pSwitcher->AddChild(pPanelAActorRemove);
	pPanelACoordSystemAdd = ceWPACoordSystemAdd::Ref::New(*this);
	pSwitcher->AddChild(pPanelACoordSystemAdd);
	pPanelACoordSystemRemove = ceWPACoordSystemRemove::Ref::New(*this);
	pSwitcher->AddChild(pPanelACoordSystemRemove);
	pPanelAComment = ceWPAComment::Ref::New(*this);
	pSwitcher->AddChild(pPanelAComment);
	
	pPanelCLogic = ceWPCLogic::Ref::New(*this);
	pSwitcher->AddChild(pPanelCLogic);
	pPanelCHasActor = ceWPCHasActor::Ref::New(*this);
	pSwitcher->AddChild(pPanelCHasActor);
	pPanelCActorInConversation = ceWPCActorInConversation::Ref::New(*this);
	pSwitcher->AddChild(pPanelCActorInConversation);
	pPanelCVariable = ceWPCVariable::Ref::New(*this);
	pSwitcher->AddChild(pPanelCVariable);
	pPanelCAParam = ceWPCAParam::Ref::New(*this);
	pSwitcher->AddChild(pPanelCAParam);
	pPanelCActorCommand = ceWPCActorCommand::Ref::New(*this);
	pSwitcher->AddChild(pPanelCActorCommand);
	pPanelCGameCommand = ceWPCGameCommand::Ref::New(*this);
	pSwitcher->AddChild(pPanelCGameCommand);
	pPanelCTrigger = ceWPCTrigger::Ref::New(*this);
	pSwitcher->AddChild(pPanelCTrigger);
	
	pSwitcher->SetCurrent(epEmpty);
}

ceWPTopic::~ceWPTopic(){
	SetConversation(nullptr);
}



// Management
///////////////

void ceWPTopic::SetConversation(ceConversation *conversation){
	if(conversation == pConversation){
		return;
	}
	
	if(pModelTreeActions){
		pModelTreeActions->SetTreeList(nullptr);
		pModelTreeActions = nullptr;
	}
	
	if(pConversation){
		pConversation->RemoveListener(pListener);
	}
	
	pConversation = conversation;
	
	if(conversation){
		conversation->AddListener(pListener);
		pModelTreeActions = ceWPTTreeModel::Ref::New(pWindowProperties.GetWindowMain(), conversation, *pListener);
		pModelTreeActions->SetTreeList(pTreeActions);
		
	}else{
		pCBFile->ClearText();
		pCBTopic->ClearText();
	}
	
	const bool enabled = conversation;
	pCBFile->SetEnabled(enabled);
	pCBTopic->SetEnabled(enabled);
	pTreeActions->SetEnabled(enabled);
	
	pBtnFile->GetAction()->Update();
	pBtnTopic->GetAction()->Update();
	
	UpdateFileList();
	UpdateGestureLists();
	UpdateFacePoseLists();
	UpdateCameraShotLists();
	UpdateTargetLists();
	OnConversationPathChanged();
}



ceConversationFile *ceWPTopic::GetFile() const{
	return pConversation ? pConversation->GetActiveFile() : nullptr;
}

ceWPTTreeItemModel *ceWPTopic::GetActionTreeItem(){
	ceWPTTreeItem * const item = (ceWPTTreeItem*)pTreeActions->GetSelection().Pointer();
	return item ? item->GetModel() : nullptr;
}

void ceWPTopic::UpdateFileList(){
	ceConversationFile * const file = GetFile();
	
	pCBFile->RemoveAllItems();
	
	if(pConversation){
		pConversation->GetFiles().Visit([&](ceConversationFile *f){
			pCBFile->AddItem(f->GetID(), nullptr, f);
		});
		
		pCBFile->SortItems();
		pCBFile->StoreFilterItems();
	}
	
	SelectActiveFile();
	
	pPanelASnippet->UpdateFileList();
	
	if(file){
		pConversation->SetActiveFile(file);
	}
}

void ceWPTopic::SelectActiveFile(){
	pCBFile->SetSelectionWithData(GetFile());
	
	pCBTopic->SetEnabled(pCBFile->GetSelectedItem());
	pBtnTopic->GetAction()->Update();
	if(!pCBTopic->GetEnabled()){
		pCBTopic->ClearText();
	}
	
	UpdateTopicList();
	UpdateFile();
}

void ceWPTopic::UpdateFile(){
}



ceConversationTopic *ceWPTopic::GetTopic() const{
	ceConversationFile * const file = GetFile();
	return file ? file->GetActiveTopic() : nullptr;
}

void ceWPTopic::UpdateTopicList(){
	ceConversationTopic * const topic = GetTopic();
	ceConversationFile * const file = GetFile();
	
	pCBTopic->RemoveAllItems();
	
	if(file){
		file->GetTopics().Visit([&](ceConversationTopic *t){
			pCBTopic->AddItem(t->GetID(), nullptr, t);
		});
		pCBTopic->SortItems();
		pCBTopic->StoreFilterItems();
	}
	
	SelectActiveTopic();
	
	pPanelASnippet->UpdateTopicList();
	
	if(file && topic){
		file->SetActiveTopic(topic);
	}
}

void ceWPTopic::SelectActiveTopic(){
	pCBTopic->SetSelectionWithData(GetTopic());
	
	UpdateTopic();
}

void ceWPTopic::UpdateTopic(){
}



ceConversationAction *ceWPTopic::GetTreeAction() const{
	ceConversationTopic * const topic = GetTopic();
	if(!topic){
		return nullptr;
	}
	
	const ceWPTTreeItem * const item = (const ceWPTTreeItem*)pTreeActions->GetSelection().Pointer();
	return item && item->GetModel() ? item->GetModel()->GetOwnerAction() : nullptr;
}

ceConversationCondition *ceWPTopic::GetTreeCondition() const{
	ceConversationTopic * const topic = GetTopic();
	if(!topic){
		return nullptr;
	}
	
	const ceWPTTreeItem * const item = (ceWPTTreeItem*)pTreeActions->GetSelection().Pointer();
	return item && item->GetModel() ? item->GetModel()->GetOwnerCondition() : nullptr;
}

void ceWPTopic::SyncTopicActive(){
	if(pModelTreeActions){
		ceConversationTopic * const topic = GetTopic();
		if(topic){
			const ceWPTTreeModel::PreventUpdateGuard preventUpdate(pModelTreeActions);
			topic->SetActive(GetTreeAction(), GetTreeCondition());
		}
	}
	
	SelectActivePanel();
}

void ceWPTopic::SelectActivePanel(){
	const ceConversationCondition * const condition = GetTreeCondition();
	const ceConversationAction * const action = GetTreeAction();
	
	if(condition){
		switch(condition->GetType()){
		case ceConversationCondition::ectLogic:
			pSwitcher->SetCurrent(epCLogic);
			break;
			
		case ceConversationCondition::ectHasActor:
			pSwitcher->SetCurrent(epCHasActor);
			break;
			
		case ceConversationCondition::ectActorInConversation:
			pSwitcher->SetCurrent(epCActorInConversation);
			break;
			
		case ceConversationCondition::ectVariable:
			pSwitcher->SetCurrent(epCVariable);
			break;
			
		case ceConversationCondition::ectActorParameter:
			pSwitcher->SetCurrent(epCAParam);
			break;
			
		case ceConversationCondition::ectActorCommand:
			pSwitcher->SetCurrent(epCActorCommand);
			break;
			
		case ceConversationCondition::ectGameCommand:
			pSwitcher->SetCurrent(epCGameCommand);
			break;
			
		case ceConversationCondition::ectTrigger:
			pSwitcher->SetCurrent(epCTrigger);
			break;
		}
		
	}else if(action){
		switch(action->GetType()){
		case ceConversationAction::eatCameraShot:
			pSwitcher->SetCurrent(epACameraShot);
			break;
			
		case ceConversationAction::eatMusic:
			pSwitcher->SetCurrent(epAMusic);
			break;
			
		case ceConversationAction::eatActorSpeak:
			pSwitcher->SetCurrent(epAActorSpeak);
			break;
			
		case ceConversationAction::eatIfElse:
			pSwitcher->SetCurrent(epAIfElse);
			break;
			
		case ceConversationAction::eatPlayerChoice:
			pSwitcher->SetCurrent(epAPlayerChoice);
			break;
			
		case ceConversationAction::eatStopConversation:
			pSwitcher->SetCurrent(epAStopConversation);
			break;
			
		case ceConversationAction::eatStopTopic:
			pSwitcher->SetCurrent(epAStopTopic);
			break;
			
		case ceConversationAction::eatSnippet:
			pSwitcher->SetCurrent(epASnippet);
			break;
			
		case ceConversationAction::eatSetVariable:
			pSwitcher->SetCurrent(epASetVariable);
			break;
			
		case ceConversationAction::eatSetActorParameter:
			pSwitcher->SetCurrent(epASetAParam);
			break;
			
		case ceConversationAction::eatActorCommand:
			pSwitcher->SetCurrent(epAActorCmd);
			break;
			
		case ceConversationAction::eatGameCommand:
			pSwitcher->SetCurrent(epAGameCommand);
			break;
			
		case ceConversationAction::eatWait:
			pSwitcher->SetCurrent(epAWait);
			break;
			
		case ceConversationAction::eatTrigger:
			pSwitcher->SetCurrent(epATrigger);
			break;
			
		case ceConversationAction::eatActorAdd:
			pSwitcher->SetCurrent(epAActorAdd);
			break;
			
		case ceConversationAction::eatActorRemove:
			pSwitcher->SetCurrent(epAActorRemove);
			break;
			
		case ceConversationAction::eatCoordSystemAdd:
			pSwitcher->SetCurrent(epACoordSystemAdd);
			break;
			
		case ceConversationAction::eatCoordSystemRemove:
			pSwitcher->SetCurrent(epACoordSystemRemove);
			break;
			
		case ceConversationAction::eatComment:
			pSwitcher->SetCurrent(epAComment);
			break;
		}
		
	}else{
		pSwitcher->SetCurrent(epEmpty);
	}
	
	UpdateActive();
}

void ceWPTopic::UpdateActive(){
	switch((ePanels)pSwitcher->GetCurrent()){
	case epACameraShot:
		pPanelACameraShot->UpdateAction();
		break;
		
	case epAMusic:
		pPanelAMusic->UpdateAction();
		break;
		
	case epAActorSpeak:
		pPanelAActorSpeak->UpdateAction();
		break;
		
	case epAIfElse:
		pPanelAIfElse->UpdateAction();
		break;
		
	case epAPlayerChoice:
		pPanelAPlayerChoice->UpdateAction();
		break;
		
	case epAStopConversation:
		pPanelAStopConversation->UpdateAction();
		break;
		
	case epAStopTopic:
		pPanelAStopTopic->UpdateAction();
		break;
		
	case epASnippet:
		pPanelASnippet->UpdateAction();
		break;
		
	case epASetVariable:
		pPanelASetVariable->UpdateAction();
		break;
		
	case epASetAParam:
		pPanelASetAParam->UpdateAction();
		break;
		
	case epAActorCmd:
		pPanelAActorCmd->UpdateAction();
		break;
		
	case epAGameCommand:
		pPanelAGameCommand->UpdateAction();
		break;
		
	case epAWait:
		pPanelAWait->UpdateAction();
		break;
		
	case epATrigger:
		pPanelATrigger->UpdateAction();
		break;
		
	case epAActorAdd:
		pPanelAActorAdd->UpdateAction();
		break;
		
	case epAActorRemove:
		pPanelAActorRemove->UpdateAction();
		break;
		
	case epACoordSystemAdd:
		pPanelACoordSystemAdd->UpdateAction();
		break;
		
	case epACoordSystemRemove:
		pPanelACoordSystemRemove->UpdateAction();
		break;
		
	case epAComment:
		pPanelAComment->UpdateAction();
		break;
		
	case epCLogic:
		pPanelCLogic->UpdateCondition();
		break;
		
	case epCHasActor:
		pPanelCHasActor->UpdateCondition();
		break;
		
	case epCActorInConversation:
		pPanelCActorInConversation->UpdateCondition();
		break;
		
	case epCVariable:
		pPanelCVariable->UpdateCondition();
		break;
		
	case epCAParam:
		pPanelCAParam->UpdateCondition();
		break;
		
	case epCActorCommand:
		pPanelCActorCommand->UpdateCondition();
		break;
		
	case epCGameCommand:
		pPanelCGameCommand->UpdateCondition();
		break;
		
	case epCTrigger:
		pPanelCTrigger->UpdateCondition();
		break;
		
	case epEmpty:
		break;
	}
}



void ceWPTopic::UpdateGestureLists(){
}

void ceWPTopic::UpdateFacePoseLists(){
}

void ceWPTopic::UpdateCameraShotLists(){
	pPanelACameraShot->UpdateCameraShotList();
}

void ceWPTopic::UpdateActorLists(){
	pPanelACameraShot->UpdateActorIDLists();
	pPanelAMusic->UpdateActorIDLists();
	pPanelAActorSpeak->UpdateActorIDLists();
	pPanelAIfElse->UpdateActorIDLists();
	pPanelAPlayerChoice->UpdateActorIDLists();
	pPanelAStopConversation->UpdateActorIDLists();
	pPanelAStopTopic->UpdateActorIDLists();
	pPanelASnippet->UpdateActorIDLists();
	pPanelASetVariable->UpdateActorIDLists();
	pPanelASetAParam->UpdateActorIDLists();
	pPanelAActorCmd->UpdateActorIDLists();
	pPanelAGameCommand->UpdateActorIDLists();
	pPanelAWait->UpdateActorIDLists();
	pPanelATrigger->UpdateActorIDLists();
	pPanelAActorAdd->UpdateActorIDLists();
	pPanelAActorRemove->UpdateActorIDLists();
	pPanelACoordSystemAdd->UpdateActorIDLists();
	pPanelACoordSystemRemove->UpdateActorIDLists();
	pPanelAComment->UpdateActorIDLists();
	
	pPanelCHasActor->UpdateActorList();
	pPanelCActorInConversation->UpdateActorList();
	pPanelCAParam->UpdateActorIDList();
	pPanelCActorCommand->UpdateActorIDList();
}

void ceWPTopic::UpdateTargetLists(){
	pPanelACameraShot->UpdateTargetList();
}

void ceWPTopic::UpdateConvoCoordSysLists(){
	pPanelACoordSystemAdd->UpdateConvoCoordSysIDLists();
	pPanelACoordSystemRemove->UpdateConvoCoordSysIDLists();
}

void ceWPTopic::OnConversationPathChanged(){
	pPanelAActorSpeak->OnConversationPathChanged();
}



void ceWPTopic::LocateAction(ceConversationAction *action){
	if(!action){
		DETHROW(deeInvalidParam);
	}
	
	if(!pModelTreeActions){
		return;
	}
	
	ceWPTTIMAction *item = pModelTreeActions->DeepFindAction(action);
	if(!item){
		return;
	}
	
	item->SetAsCurrentItem();
	SyncTopicActive();
}

void ceWPTopic::PlayActionFromHere(){
	if(!pConversation){
		return;
	}
	
	ceConversationTopic * const topic = GetTopic();
	if(!topic){
		return;
	}
	
	ceWPTTreeItem * const item = (ceWPTTreeItem*)pTreeActions->GetSelection().Pointer();
	if(!item || !item->GetModel()){
		return;
	}
	
	if(!pConversation->GetPlayback()->GetMainActionStack()->GetTop().GetParentList()){
		return; // this should never happen
	}
	
	cePlayback &playback = *pConversation->GetPlayback();
	playback.SetRunning(false);
	playback.SetPaused(false);
	playback.SetTopic(topic);
	playback.Rewind();
	item->GetModel()->BuildPlaybackFromHere();
	
	playback.SetRunning(true);
}
