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

#ifndef _CEWPTOPIC_H_
#define _CEWPTOPIC_H_

#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeTreeList.h>
#include <deigde/gui/igdeSwitcher.h>
#include <deigde/gui/layout/igdeContainerFlow.h>

class ceWPAComment;
class ceWPACoordSystemRemove;
class ceWPACoordSystemAdd;
class ceWPAActorRemove;
class ceWPAActorAdd;
class ceWPCTrigger;
class ceWPATrigger;
class ceWindowProperties;
class ceWPTopicListener;
class ceConversation;
class ceConversationFile;
class ceConversationTopic;
class ceConversationAction;
class ceConversationActionList;
class ceConversationCondition;
class ceConversationConditionList;
class ceWPACameraShot;
class ceWPAMusic;
class ceWPAActorSpeak;
class ceWPAIfElse;
class ceWPAPlayerChoice;
class ceWPAStopConversation;
class ceWPAStopTopic;
class ceWPASnippet;
class ceWPASetVariable;
class ceWPASetAParam;
class ceWPAActorCmd;
class ceWPAGameCommand;
class ceWPAWait;
class ceWPCLogic;
class ceWPCHasActor;
class ceWPCActorInConversation;
class ceWPCVariable;
class ceWPCAParam;
class ceWPCActorCommand;
class ceWPCGameCommand;

class ceWPTTreeModel;
class ceWPTTreeItemModel;
class ceWPTTreeItem;



/**
 * Topic property window.
 */
class ceWPTopic : public igdeContainerFlow{
private:
	ceWindowProperties &pWindowProperties;
	ceWPTopicListener *pListener;
	ceConversation *pConversation;
	
	igdeComboBoxFilter::Ref pCBFile;
	igdeButton::Ref pBtnFile;
	
	igdeComboBoxFilter::Ref pCBTopic;
	igdeButton::Ref pBtnTopic;
	
	igdeTreeList::Ref pTreeActions;
	ceWPTTreeModel *pModelTreeActions;
	
	igdeSwitcher::Ref pSwitcher;
	
	ceWPACameraShot *pPanelACameraShot;
	ceWPAMusic *pPanelAMusic;
	ceWPAActorSpeak *pPanelAActorSpeak;
	ceWPAIfElse *pPanelAIfElse;
	ceWPAPlayerChoice *pPanelAPlayerChoice;
	ceWPAStopConversation *pPanelAStopConversation;
	ceWPAStopTopic *pPanelAStopTopic;
	ceWPASnippet *pPanelASnippet;
	ceWPASetVariable *pPanelASetVariable;
	ceWPASetAParam *pPanelASetAParam;
	ceWPAActorCmd *pPanelAActorCmd;
	ceWPAGameCommand *pPanelAGameCommand;
	ceWPAWait *pPanelAWait;
	ceWPATrigger *pPanelATrigger;
	ceWPAActorAdd *pPanelAActorAdd;
	ceWPAActorRemove *pPanelAActorRemove;
	ceWPACoordSystemAdd *pPanelACoordSystemAdd;
	ceWPACoordSystemRemove *pPanelACoordSystemRemove;
	ceWPAComment *pPanelAComment;
	
	ceWPCLogic *pPanelCLogic;
	ceWPCHasActor *pPanelCHasActor;
	ceWPCActorInConversation *pPanelCActorInConversation;
	ceWPCVariable *pPanelCVariable;
	ceWPCAParam *pPanelCAParam;
	ceWPCActorCommand *pPanelCActorCommand;
	ceWPCGameCommand *pPanelCGameCommand;
	ceWPCTrigger *pPanelCTrigger;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create panel. */
	ceWPTopic(ceWindowProperties &windowProperties);
	
protected:
	/** Clean up panel. */
	virtual ~ceWPTopic();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Main window. */
	inline ceWindowProperties &GetWindowProperties() const{ return pWindowProperties; }
	
	/** Conversation. */
	inline ceConversation *GetConversation() const{ return pConversation; }
	
	/** Set conversation. */
	void SetConversation(ceConversation *conversation);
	
	/** Action tree model. */
	inline ceWPTTreeModel *GetActionTreeModel() const{ return pModelTreeActions; }
	
	/** Active action tree item model. */
	ceWPTTreeItemModel *GetActionTreeItem();
	
	/** Active file. */
	ceConversationFile *GetFile() const;
	
	/** Update list of files. */
	void UpdateFileList();
	
	/** Select active file. */
	void SelectActiveFile();
	
	/** Update active file. */
	void UpdateFile();
	
	/** Active topic. */
	ceConversationTopic *GetTopic() const;
	
	/** Update list of topics. */
	void UpdateTopicList();
	
	/** Select active topic. */
	void SelectActiveTopic();
	
	/** Update active topic. */
	void UpdateTopic();
	
	/** Active action from tree. */
	ceConversationAction *GetTreeAction() const;
	
	/** Active condition from tree. */
	ceConversationCondition *GetTreeCondition() const;
	
	/** Set topic active element from tree selection. */
	void SyncTopicActive();
	
	/** Selected active panel. */
	void SelectActivePanel();
	
	/** Update active. */
	void UpdateActive();
	
	
	/** Update gesture lists. */
	void UpdateGestureLists();
	
	/** Update face pose lists. */
	void UpdateFacePoseLists();
	
	/** Update camera shot lists. */
	void UpdateCameraShotLists();
	
	/** Update actor lists. */
	void UpdateActorLists();
	
	/** Update target lists. */
	void UpdateTargetLists();
	
	/** Update conversation coordinate system lists. */
	void UpdateConvoCoordSysLists();
	
	/** Conversation path changed. */
	void OnConversationPathChanged();
	
	
	
	/** Locate action from active topic in tree and make it visible. */
	void LocateAction(ceConversationAction *action);
	
	/** Start playing starting with action selected in tree. */
	void PlayActionFromHere();
	/*@}*/
};

#endif
