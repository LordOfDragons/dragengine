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

#ifndef _CEWINDOWMAIN_H_
#define _CEWINDOWMAIN_H_

#include "ceWindowMainListener.h"
#include "ceViewConversation.h"
#include "properties/ceWindowProperties.h"
#include "dopesheet/ceWindowDopeSheet.h"
#include "../configuration/ceConfiguration.h"
#include "../conversation/ceConversation.h"
#include "../loadsave/ceLoadSaveSystem.h"

#include <deigde/clipboard/igdeClipboard.h>
#include <deigde/gui/igdeEditorWindow.h>
#include <deigde/gui/igdeToolBar.h>
#include <deigde/gui/igdeTabBook.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionUndo.h>
#include <deigde/gui/event/igdeActionRedo.h>
#include <deigde/gui/resources/igdeIcon.h>

#include <dragengine/common/string/decStringList.h>

class decStringSet;
class ceIGDEModule;
class igdeStepableTask;



/**
 * Main Editor Window.
 */
class ceWindowMain : public igdeEditorWindow{
public:
	typedef deTObjectReference<ceWindowMain> Ref;
	
	
private:
	class cRecentFilesCTS : public igdeRecentFiles{
		ceWindowMain &pWindowMain;
		
	public:
		cRecentFilesCTS(ceWindowMain &windowMain);
		void OpenFile(const char *filename) override;
		void FilesChanged() override;
	};
	
	class cRecentFilesCTA : public igdeRecentFiles{
		ceWindowMain &pWindowMain;
		
	public:
		cRecentFilesCTA(ceWindowMain &windowMain);
		void OpenFile(const char *filename) override;
		void FilesChanged() override;
	};
	
	class cRecentFilesCTGS : public igdeRecentFiles{
		ceWindowMain &pWindowMain;
		
	public:
		cRecentFilesCTGS(ceWindowMain &windowMain);
		void OpenFile(const char *filename) override;
		void FilesChanged() override;
	};
	
	class cRecentFilesLangPack : public igdeRecentFiles{
		ceWindowMain &pWindowMain;
		
	public:
		cRecentFilesLangPack(ceWindowMain &windowMain);
		void OpenFile(const char *filename) override;
		void FilesChanged() override;
	};
	
	ceWindowMainListener::Ref pListener;
	
	igdeIcon::Ref pIconActionCameraShot;
	igdeIcon::Ref pIconActionMusic;
	igdeIcon::Ref pIconActionActorSpeak;
	igdeIcon::Ref pIconActionIfElse;
	igdeIcon::Ref pIconActionIfElseCaseIf;
	igdeIcon::Ref pIconActionIfElseCaseElse;
	igdeIcon::Ref pIconActionPlayerChoice;
	igdeIcon::Ref pIconActionOption;
	igdeIcon::Ref pIconActionSnippet;
	igdeIcon::Ref pIconActionStop;
	igdeIcon::Ref pIconActionActorCommand;
	igdeIcon::Ref pIconActionCommand;
	igdeIcon::Ref pIconActionVariable;
	igdeIcon::Ref pIconActionWait;
	igdeIcon::Ref pIconActionTrigger;
	igdeIcon::Ref pIconActionActorAdd;
	igdeIcon::Ref pIconActionActorRemove;
	igdeIcon::Ref pIconActionCoordSysAdd;
	igdeIcon::Ref pIconActionCoordSysRemove;
	igdeIcon::Ref pIconActionComment;
	
	igdeIcon::Ref pIconConditionLogic;
	igdeIcon::Ref pIconConditionActorCommand;
	igdeIcon::Ref pIconConditionCommand;
	igdeIcon::Ref pIconConditionVariable;
	igdeIcon::Ref pIconConditionHasActor;
	igdeIcon::Ref pIconConditionActorInConversation;
	igdeIcon::Ref pIconConditionTrigger;
	
	igdeIcon::Ref pIconPlayAction;
	igdeIcon::Ref pIconPlayFromHere;
	igdeIcon::Ref pIconPlayPause;
	igdeIcon::Ref pIconPlaySelectCurAction;
	
	igdeAction::Ref pActionFileNew;
	igdeAction::Ref pActionFileOpen;
	igdeAction::Ref pActionFileSave;
	igdeAction::Ref pActionFileSaveAs;
	
	igdeActionUndo::Ref pActionEditUndo;
	igdeActionRedo::Ref pActionEditRedo;
	
	igdeAction::Ref pActionEditCut;
	igdeAction::Ref pActionEditCopy;
	igdeAction::Ref pActionEditPaste;
	
	igdeAction::Ref pActionViewCTSLoad;
	igdeAction::Ref pActionViewCTSSave;
	igdeAction::Ref pActionViewCTALoad;
	igdeAction::Ref pActionViewCTASave;
	igdeAction::Ref pActionViewCTGSLoad;
	igdeAction::Ref pActionViewCTGSSave;
	igdeAction::Ref pActionViewShowRuleOfThirdsAid;
	igdeAction::Ref pActionViewAttachLangPack;
	igdeAction::Ref pActionViewDetachLangPack;
	igdeAction::Ref pActionViewMissingWords;
	
	igdeToolBar::Ref pTBFile;
	igdeToolBar::Ref pTBEdit;
	
	ceConfiguration::Ref pConfiguration;
	igdeClipboard pClipboard;
	ceLoadSaveSystem::Ref pLoadSaveSystem;
	
	ceViewConversation::Ref pViewConversation;
	ceWindowProperties::Ref pWindowProperties;
	ceWindowDopeSheet::Ref pWindowDopeSheet;
	
	ceConversation::Ref pConversation;
	
	cRecentFilesCTS pRecentFilesCTS;
	cRecentFilesCTA pRecentFilesCTA;
	cRecentFilesCTGS pRecentFilesCTGS;
	cRecentFilesLangPack pRecentFilesLangPack;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create main window. */
	ceWindowMain(ceIGDEModule &module);
	
protected:
	/** Clean up main window. */
	virtual ~ceWindowMain();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Ask user if it is okay to quit the application. */
	bool QuitRequest();
	
	/** Reset views. */
	void ResetViews();
	
	/** Configuration. */
	inline const ceConfiguration::Ref &GetConfiguration() const{ return pConfiguration; }
	
	/** Clipboard. */
	inline igdeClipboard &GetClipboard(){ return pClipboard; }
	inline const igdeClipboard &GetClipboard() const{ return pClipboard; }
	
	/** Load save system. */
	inline const ceLoadSaveSystem::Ref &GetLoadSaveSystem() const{ return pLoadSaveSystem; }
	
	/** Conversation. */
	inline const ceConversation::Ref &GetConversation() const{ return pConversation; }
	
	/** Set conversation. */
	void SetConversation(ceConversation *conversation);
	
	/** Create new conversation. */
	void CreateNewConversation();
	
	/** Save conversation to file. */
	void SaveConversation(const char *filename);
	
	/** Properties window. */
	inline ceWindowProperties &GetWindowProperties() const{ return *pWindowProperties; }
	
	/** Show found missing words dialog. */
	void ShowFoundMissingWordsDialog(decStringSet &missingWords);
	
	/** Recent files. */
	inline cRecentFilesCTS &GetRecentFilesCTS(){ return pRecentFilesCTS; }
	inline cRecentFilesCTA &GetRecentFilesCTA(){ return pRecentFilesCTA; }
	inline cRecentFilesCTGS &GetRecentFilesCTGS(){ return pRecentFilesCTGS; }
	inline cRecentFilesLangPack &GetRecentFilesLangPack(){ return pRecentFilesLangPack; }
	
	/** Open conversation test actor. */
	void LoadCTA(const char *filename);
	
	/** Attach language pack. */
	void AttachLangPack(const char *filename);
	
	
	/** Icons. */
	inline const igdeIcon::Ref &GetIconActionCameraShot() const{ return pIconActionCameraShot; }
	inline const igdeIcon::Ref &GetIconActionMusic() const{ return pIconActionMusic; }
	inline const igdeIcon::Ref &GetIconActionActorSpeak() const{ return pIconActionActorSpeak; }
	inline const igdeIcon::Ref &GetIconActionIfElse() const{ return pIconActionIfElse; }
	inline const igdeIcon::Ref &GetIconActionIfElseCaseIf() const{ return pIconActionIfElseCaseIf; }
	inline const igdeIcon::Ref &GetIconActionIfElseCaseElse() const{ return pIconActionIfElseCaseElse; }
	inline const igdeIcon::Ref &GetIconActionPlayerChoice() const{ return pIconActionPlayerChoice; }
	inline const igdeIcon::Ref &GetIconActionOption() const{ return pIconActionOption; }
	inline const igdeIcon::Ref &GetIconActionSnippet() const{ return pIconActionSnippet; }
	inline const igdeIcon::Ref &GetIconActionStop() const{ return pIconActionStop; }
	inline const igdeIcon::Ref &GetIconActionActorCommand() const{ return pIconActionActorCommand; }
	inline const igdeIcon::Ref &GetIconActionCommand() const{ return pIconActionCommand; }
	inline const igdeIcon::Ref &GetIconActionVariable() const{ return pIconActionVariable; }
	inline const igdeIcon::Ref &GetIconActionWait() const{ return pIconActionWait; }
	inline const igdeIcon::Ref &GetIconActionTrigger() const{ return pIconActionTrigger; }
	inline const igdeIcon::Ref &GetIconActionActorAdd() const{ return pIconActionActorAdd; }
	inline const igdeIcon::Ref &GetIconActionActorRemove() const{ return pIconActionActorRemove; }
	inline const igdeIcon::Ref &GetIconActionCoordSysAdd() const{ return pIconActionCoordSysAdd; }
	inline const igdeIcon::Ref &GetIconActionCoordSysRemove() const{ return pIconActionCoordSysRemove; }
	inline const igdeIcon::Ref &GetIconActionComment() const{ return pIconActionComment; }
	
	inline const igdeIcon::Ref &GetIconConditionLogic() const{ return pIconConditionLogic; }
	inline const igdeIcon::Ref &GetIconConditionActorCommand() const{ return pIconConditionActorCommand; }
	inline const igdeIcon::Ref &GetIconConditionCommand() const{ return pIconConditionCommand; }
	inline const igdeIcon::Ref &GetIconConditionVariable() const{ return pIconConditionVariable; }
	inline const igdeIcon::Ref &GetIconConditionHasActor() const{ return pIconConditionHasActor; }
	inline const igdeIcon::Ref &GetIconConditionActorInConversation() const{ return pIconConditionActorInConversation; }
	inline const igdeIcon::Ref &GetIconConditionTrigger() const{ return pIconConditionTrigger; }
	
	inline const igdeIcon::Ref &GetIconPlayAction() const{ return pIconPlayAction; }
	inline const igdeIcon::Ref &GetIconPlayFromHere() const{ return pIconPlayFromHere; }
	inline const igdeIcon::Ref &GetIconPlayPause() const{ return pIconPlayPause; }
	inline const igdeIcon::Ref &GetIconPlaySelectCurAction() const{ return pIconPlaySelectCurAction; }
	
	
	
	/** Actions. */
	
	
	
	/** Game engine is about to be started. */
	virtual void OnBeforeEngineStart();
	
	/** Game engine has been started. */
	virtual void OnAfterEngineStart();
	
	/** Game engine is about to be stopped. */
	virtual void OnBeforeEngineStop();
	
	/** Game engine has been stopped. */
	virtual void OnAfterEngineStop();
	
	/** Module has been activated. */
	virtual void OnActivate();
	
	/** Module has been deactivated. */
	virtual void OnDeactivate();
	
	/** Game like frame update. */
	virtual void OnFrameUpdate(float elapsed);
	
	/**
	 * Retrieves a list of changed documents.
	 * 
	 * This list is requested by the IGDE if a game project is closed due to creating or
	 * loading a new one or because the application is about to be closed. Editors modules
	 * have to add to the list the filenames of open documents that have changed and need
	 * saving. The filename is later used in calls to \ref SaveDocument to save the file
	 * if requested by the user. All other files are discarded.
	 */
	virtual void GetChangedDocuments(decStringList &list);
	
	/**
	 * Requests a document to be loaded.
	 */
	virtual void LoadDocument(const char *filename);
	
	/**
	 * Requests a document to be saved.
	 * 
	 * The document has to be saved if changed. If not changed this call can be ignored.
	 * This call is usually made after a previous call to \ref GetUnsavedDocuments.
	 * 
	 * \returns True if the saving has been successful or false otherwise.
	 */
	virtual bool SaveDocument(const char *filename);
	
	/**
	 * Recent files changed.
	 */
	virtual void RecentFilesChanged();
	
	/**
	 * The game project has changed.
	 * 
	 * Notification send to the editor modules after a new game project has been set.
	 * The editor module has to discard all open documents and all references held of
	 * objects related to the previous game project. The old game project is kept
	 * alive until all editor modules have processed the OnGameProjectChanged().
	 */
	virtual void OnGameProjectChanged();
	
	/**
	 * Project game definition changed.
	 * 
	 * Called after an editor changed the game definition. The old game definition used so
	 * far is replaced by a new game definition. The module has to update everything
	 * using the old game definition. This process can be potentially lengthy. For this
	 * reason the module has to return a steppable task to do the processing. If the module
	 * does not need any update \em nullptr can be returned. The caller delets the task once
	 * finished processing.
	 * 
	 * The default implementation returns \em nullptr.
	 */
	virtual igdeStepableTask *OnGameDefinitionChanged();
	/*@}*/
	
	
	
private:
	void pLoadIcons();
	void pCreateActions();
	void pCreateToolBarFile();
	void pCreateToolBarEdit();
	void pCreateMenu();
	void pCreateMenuFile(igdeMenuCascade &menu);
	void pCreateMenuEdit(igdeMenuCascade &menu);
	void pCreateMenuView(igdeMenuCascade &menu);
};

#endif
