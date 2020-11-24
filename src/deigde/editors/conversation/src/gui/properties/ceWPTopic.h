/* 
 * Drag[en]gine IGDE Conversation Editor
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _CEWPTOPIC_H_
#define _CEWPTOPIC_H_

#include <deigde/gui/igdeComboBoxFilterReference.h>
#include <deigde/gui/igdeButtonReference.h>
#include <deigde/gui/igdeTreeListReference.h>
#include <deigde/gui/igdeSwitcherReference.h>
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
 * \brief Topic property window.
 */
class ceWPTopic : public igdeContainerFlow{
private:
	ceWindowProperties &pWindowProperties;
	ceWPTopicListener *pListener;
	ceConversation *pConversation;
	
	igdeComboBoxFilterReference pCBFile;
	igdeButtonReference pBtnFile;
	
	igdeComboBoxFilterReference pCBTopic;
	igdeButtonReference pBtnTopic;
	
	igdeTreeListReference pTreeActions;
	ceWPTTreeModel *pModelTreeActions;
	
	igdeSwitcherReference pSwitcher;
	
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
	/** \brief Create panel. */
	ceWPTopic( ceWindowProperties &windowProperties );
	
protected:
	/** \brief Clean up panel. */
	virtual ~ceWPTopic();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Main window. */
	inline ceWindowProperties &GetWindowProperties() const{ return pWindowProperties; }
	
	/** \brief Conversation. */
	inline ceConversation *GetConversation() const{ return pConversation; }
	
	/** \brief Set conversation. */
	void SetConversation( ceConversation *conversation );
	
	/** \brief Action tree model. */
	inline ceWPTTreeModel *GetActionTreeModel() const{ return pModelTreeActions; }
	
	/** \brief Active action tree item model. */
	ceWPTTreeItemModel *GetActionTreeItem();
	
	/** \brief Active file. */
	ceConversationFile *GetFile() const;
	
	/** \brief Update list of files. */
	void UpdateFileList();
	
	/** \brief Select active file. */
	void SelectActiveFile();
	
	/** \brief Update active file. */
	void UpdateFile();
	
	/** \brief Active topic. */
	ceConversationTopic *GetTopic() const;
	
	/** \brief Update list of topics. */
	void UpdateTopicList();
	
	/** \brief Select active topic. */
	void SelectActiveTopic();
	
	/** \brief Update active topic. */
	void UpdateTopic();
	
	/** \brief Active action from tree. */
	ceConversationAction *GetTreeAction() const;
	
	/** \brief Active condition from tree. */
	ceConversationCondition *GetTreeCondition() const;
	
	/** \brief Set topic active element from tree selection. */
	void SyncTopicActive();
	
	/** \brief Selected active panel. */
	void SelectActivePanel();
	
	/** \brief Update active. */
	void UpdateActive();
	
	
	
	/** \brief Update gesture lists. */
	void UpdateGestureLists();
	
	/** \brief Update face pose lists. */
	void UpdateFacePoseLists();
	
	/** \brief Update camera shot lists. */
	void UpdateCameraShotLists();
	
	/** \brief Update actor lists. */
	void UpdateActorLists();
	
	/** \brief Update target lists. */
	void UpdateTargetLists();
	
	/** \brief Update conversation coordinate system lists. */
	void UpdateConvoCoordSysLists();
	
	
	
	/** \brief Locate action from active topic in tree and make it visible. */
	void LocateAction( ceConversationAction *action );
	
	/** \brief Start playing starting with action selected in tree. */
	void PlayActionFromHere();
	/*@}*/
};

#endif
