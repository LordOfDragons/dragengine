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

#ifndef _CEWINDOWMAIN_H_
#define _CEWINDOWMAIN_H_

#include <deigde/clipboard/igdeClipboard.h>
#include <deigde/gui/igdeEditorWindow.h>
#include <deigde/gui/igdeToolBarReference.h>
#include <deigde/gui/igdeTabBookReference.h>
#include <deigde/gui/event/igdeActionReference.h>
#include <deigde/gui/event/igdeActionUndoReference.h>
#include <deigde/gui/event/igdeActionRedoReference.h>
#include <deigde/gui/resources/igdeIconReference.h>

class decStringList;
class ceWindowMainListener;
class ceViewConversation;
class ceConfiguration;
class ceConversation;
class ceWindowProperties;
class ceWindowDopeSheet;
class ceLoadSaveSystem;
class ceIGDEModule;
class igdeStepableTask;



/**
 * \brief Main Editor Window.
 */
class ceWindowMain : public igdeEditorWindow{
private:
	ceWindowMainListener *pListener;
	
	igdeIconReference pIconActionCameraShot;
	igdeIconReference pIconActionMusic;
	igdeIconReference pIconActionActorSpeak;
	igdeIconReference pIconActionIfElse;
	igdeIconReference pIconActionIfElseCaseIf;
	igdeIconReference pIconActionIfElseCaseElse;
	igdeIconReference pIconActionPlayerChoice;
	igdeIconReference pIconActionOption;
	igdeIconReference pIconActionSnippet;
	igdeIconReference pIconActionStop;
	igdeIconReference pIconActionActorCommand;
	igdeIconReference pIconActionCommand;
	igdeIconReference pIconActionVariable;
	igdeIconReference pIconActionWait;
	igdeIconReference pIconActionTrigger;
	igdeIconReference pIconActionActorAdd;
	igdeIconReference pIconActionActorRemove;
	igdeIconReference pIconActionCoordSysAdd;
	igdeIconReference pIconActionCoordSysRemove;
	igdeIconReference pIconActionComment;
	
	igdeIconReference pIconConditionLogic;
	igdeIconReference pIconConditionActorCommand;
	igdeIconReference pIconConditionCommand;
	igdeIconReference pIconConditionVariable;
	igdeIconReference pIconConditionHasActor;
	igdeIconReference pIconConditionActorInConversation;
	igdeIconReference pIconConditionTrigger;
	
	igdeIconReference pIconPlayAction;
	igdeIconReference pIconPlayFromHere;
	igdeIconReference pIconPlayPause;
	igdeIconReference pIconPlaySelectCurAction;
	
	igdeActionReference pActionFileNew;
	igdeActionReference pActionFileOpen;
	igdeActionReference pActionFileSave;
	igdeActionReference pActionFileSaveAs;
	
	igdeActionUndoReference pActionEditUndo;
	igdeActionRedoReference pActionEditRedo;
	
	igdeActionReference pActionEditCut;
	igdeActionReference pActionEditCopy;
	igdeActionReference pActionEditPaste;
	
	igdeActionReference pActionViewCTSLoad;
	igdeActionReference pActionViewCTSSave;
	igdeActionReference pActionViewCTALoad;
	igdeActionReference pActionViewCTASave;
	igdeActionReference pActionViewCTGSLoad;
	igdeActionReference pActionViewCTGSSave;
	igdeActionReference pActionViewShowRuleOfThirdsAid;
	
	igdeToolBarReference pTBFile;
	igdeToolBarReference pTBEdit;
	
	ceConfiguration *pConfiguration;
	igdeClipboard pClipboard;
	ceLoadSaveSystem *pLoadSaveSystem;
	
	ceViewConversation *pViewConversation;
	ceWindowProperties *pWindowProperties;
	ceWindowDopeSheet *pWindowDopeSheet;
	
	ceConversation *pConversation;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create main window. */
	ceWindowMain( ceIGDEModule &module );
	
protected:
	/** \brief Clean up main window. */
	virtual ~ceWindowMain();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Ask user if it is okay to quit the application. */
	bool QuitRequest();
	
	/** \brief Reset views. */
	void ResetViews();
	
	/** \brief Configuration. */
	inline ceConfiguration &GetConfiguration() const{ return *pConfiguration; }
	
	/** \brief Clipboard. */
	inline igdeClipboard &GetClipboard(){ return pClipboard; }
	inline const igdeClipboard &GetClipboard() const{ return pClipboard; }
	
	/** \brief Load save system. */
	inline ceLoadSaveSystem &GetLoadSaveSystem() const{ return *pLoadSaveSystem; }
	
	/** \brief Conversation. */
	inline ceConversation *GetConversation() const{ return pConversation; }
	
	/** \brief Set conversation. */
	void SetConversation( ceConversation *conversation );
	
	/** \brief Create new conversation. */
	void CreateNewConversation();
	
	/** \brief Save conversation to file. */
	void SaveConversation( const char *filename );
	
	/** \brief Properties window. */
	inline ceWindowProperties &GetWindowProperties() const{ return *pWindowProperties; }
	
	
	
	/** \brief Icons. */
	inline igdeIcon *GetIconActionCameraShot() const{ return pIconActionCameraShot; }
	inline igdeIcon *GetIconActionMusic() const{ return pIconActionMusic; }
	inline igdeIcon *GetIconActionActorSpeak() const{ return pIconActionActorSpeak; }
	inline igdeIcon *GetIconActionIfElse() const{ return pIconActionIfElse; }
	inline igdeIcon *GetIconActionIfElseCaseIf() const{ return pIconActionIfElseCaseIf; }
	inline igdeIcon *GetIconActionIfElseCaseElse() const{ return pIconActionIfElseCaseElse; }
	inline igdeIcon *GetIconActionPlayerChoice() const{ return pIconActionPlayerChoice; }
	inline igdeIcon *GetIconActionOption() const{ return pIconActionOption; }
	inline igdeIcon *GetIconActionSnippet() const{ return pIconActionSnippet; }
	inline igdeIcon *GetIconActionStop() const{ return pIconActionStop; }
	inline igdeIcon *GetIconActionActorCommand() const{ return pIconActionActorCommand; }
	inline igdeIcon *GetIconActionCommand() const{ return pIconActionCommand; }
	inline igdeIcon *GetIconActionVariable() const{ return pIconActionVariable; }
	inline igdeIcon *GetIconActionWait() const{ return pIconActionWait; }
	inline igdeIcon *GetIconActionTrigger() const{ return pIconActionTrigger; }
	inline igdeIcon *GetIconActionActorAdd() const{ return pIconActionActorAdd; }
	inline igdeIcon *GetIconActionActorRemove() const{ return pIconActionActorRemove; }
	inline igdeIcon *GetIconActionCoordSysAdd() const{ return pIconActionCoordSysAdd; }
	inline igdeIcon *GetIconActionCoordSysRemove() const{ return pIconActionCoordSysRemove; }
	inline igdeIcon *GetIconActionComment() const{ return pIconActionComment; }
	
	inline igdeIcon *GetIconConditionLogic() const{ return pIconConditionLogic; }
	inline igdeIcon *GetIconConditionActorCommand() const{ return pIconConditionActorCommand; }
	inline igdeIcon *GetIconConditionCommand() const{ return pIconConditionCommand; }
	inline igdeIcon *GetIconConditionVariable() const{ return pIconConditionVariable; }
	inline igdeIcon *GetIconConditionHasActor() const{ return pIconConditionHasActor; }
	inline igdeIcon *GetIconConditionActorInConversation() const{ return pIconConditionActorInConversation; }
	inline igdeIcon *GetIconConditionTrigger() const{ return pIconConditionTrigger; }
	
	inline igdeIcon *GetIconPlayAction() const{ return pIconPlayAction; }
	inline igdeIcon *GetIconPlayFromHere() const{ return pIconPlayFromHere; }
	inline igdeIcon *GetIconPlayPause() const{ return pIconPlayPause; }
	inline igdeIcon *GetIconPlaySelectCurAction() const{ return pIconPlaySelectCurAction; }
	
	
	
	/** \brief Actions. */
	
	
	
	/** \brief Game engine is about to be started. */
	virtual void OnBeforeEngineStart();
	
	/** \brief Game engine has been started. */
	virtual void OnAfterEngineStart();
	
	/** \brief Game engine is about to be stopped. */
	virtual void OnBeforeEngineStop();
	
	/** \brief Game engine has been stopped. */
	virtual void OnAfterEngineStop();
	
	/** \brief Module has been activated. */
	virtual void OnActivate();
	
	/** \brief Module has been deactivated. */
	virtual void OnDeactivate();
	
	/** \brief Game like frame update. */
	virtual void OnFrameUpdate( float elapsed );
	
	/**
	 * \brief Retrieves a list of changed documents.
	 * 
	 * This list is requested by the IGDE if a game project is closed due to creating or
	 * loading a new one or because the application is about to be closed. Editors modules
	 * have to add to the list the filenames of open documents that have changed and need
	 * saving. The filename is later used in calls to \ref SaveDocument to save the file
	 * if requested by the user. All other files are discarded.
	 */
	virtual void GetChangedDocuments( decStringList &list );
	
	/**
	 * \brief Requests a document to be loaded.
	 */
	virtual void LoadDocument( const char *filename );
	
	/**
	 * \brief Requests a document to be saved.
	 * 
	 * The document has to be saved if changed. If not changed this call can be ignored.
	 * This call is usually made after a previous call to \ref GetUnsavedDocuments.
	 * 
	 * \returns True if the saving has been successful or false otherwise.
	 */
	virtual bool SaveDocument( const char *filename );
	
	/**
	 * \brief Recent files changed.
	 */
	virtual void RecentFilesChanged();
	
	/**
	 * \brief The game project has changed.
	 * 
	 * Notification send to the editor modules after a new game project has been set.
	 * The editor module has to discard all open documents and all references held of
	 * objects related to the previous game project. The old game project is kept
	 * alive until all editor modules have processed the OnGameProjectChanged().
	 */
	virtual void OnGameProjectChanged();
	
	/**
	 * \brief Project game definition changed.
	 * 
	 * Called after an editor changed the game definition. The old game definition used so
	 * far is replaced by a new game definition. The module has to update everything
	 * using the old game definition. This process can be potentially lengthy. For this
	 * reason the module has to return a steppable task to do the processing. If the module
	 * does not need any update \em NULL can be returned. The caller delets the task once
	 * finished processing.
	 * 
	 * The default implementation returns \em NULL.
	 */
	virtual igdeStepableTask *OnGameDefinitionChanged();
	/*@}*/
	
	
	
private:
	void pLoadIcons();
	void pCreateActions();
	void pCreateToolBarFile();
	void pCreateToolBarEdit();
	void pCreateMenu();
	void pCreateMenuFile( igdeMenuCascade &menu );
	void pCreateMenuEdit( igdeMenuCascade &menu );
	void pCreateMenuView( igdeMenuCascade &menu );
};

#endif
