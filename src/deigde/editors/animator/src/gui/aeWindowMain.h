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

#ifndef _AEWINDOWMAIN_H_
#define _AEWINDOWMAIN_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <deigde/clipboard/igdeClipboard.h>
#include <deigde/gui/igdeEditorWindow.h>
#include <deigde/gui/igdeToolBar.h>
#include <deigde/gui/igdeTabBook.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionUndo.h>
#include <deigde/gui/event/igdeActionRedo.h>
#include <deigde/gui/resources/igdeIcon.h>

#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>


class aeRule;
class decStringList;
class aeConfiguration;
class aeView3D;
class aeAnimator;
class aeWindowProperties;
class deCmdLineArgs;
class aeLoadSaveSystem;
class aeWindowMainListener;
class aeIGDEModule;
class igdeGameDefinition;
class igdeEnvironment;
class igdeEngineController;
class igdeMenuDefinition;
class deException;
class deEngine;
class deLogger;
class igdeStepableTask;



/**
 * Main window.
 */
class aeWindowMain : public igdeEditorWindow{
private:
	aeWindowMainListener::Ref pListener;
	
	igdeIcon::Ref pIconRuleAnimation;
	igdeIcon::Ref pIconRuleAnimationDifference;
	igdeIcon::Ref pIconRuleAnimationSelect;
	igdeIcon::Ref pIconRuleBoneTransformator;
	igdeIcon::Ref pIconRuleForeignState;
	igdeIcon::Ref pIconRuleGroup;
	igdeIcon::Ref pIconRuleInverseKinematic;
	igdeIcon::Ref pIconRuleLimit;
	igdeIcon::Ref pIconRuleStateManipulator;
	igdeIcon::Ref pIconRuleStateSnapshot;
	igdeIcon::Ref pIconRuleSubAnimator;
	igdeIcon::Ref pIconRuleTrackTo;
	igdeIcon::Ref pIconRuleMirror;
	
	igdeAction::Ref pActionFileNew;
	igdeAction::Ref pActionFileOpen;
	igdeAction::Ref pActionFileSave;
	igdeAction::Ref pActionFileSaveAs;
	
	igdeActionUndo::Ref pActionEditUndo;
	igdeActionRedo::Ref pActionEditRedo;
	
	igdeAction::Ref pActionEditCut;
	igdeAction::Ref pActionEditCopy;
	igdeAction::Ref pActionEditPaste;
	igdeAction::Ref pActionEditLocoEnabled;
	igdeAction::Ref pActionEditWBTracking;
	igdeAction::Ref pActionEditShowBones;
	igdeAction::Ref pActionEditDDBoneSize;
	
	igdeAction::Ref pActionControllerAdd;
	igdeAction::Ref pActionControllerDuplicate;
	igdeAction::Ref pActionControllerRemove;
	igdeAction::Ref pActionControllerUp;
	igdeAction::Ref pActionControllerDown;
	
	igdeAction::Ref pActionLinkAdd;
	igdeAction::Ref pActionLinkDuplicate;
	igdeAction::Ref pActionLinkRemove;
	igdeAction::Ref pActionLinkRemoveUnused;
	
	igdeAction::Ref pActionRuleAddAnim;
	igdeAction::Ref pActionRuleAddAnimDiff;
	igdeAction::Ref pActionRuleAddAnimSelect;
	igdeAction::Ref pActionRuleAddBoneRot;
	igdeAction::Ref pActionRuleAddInvKin;
	igdeAction::Ref pActionRuleAddStateManip;
	igdeAction::Ref pActionRuleAddStateSnap;
	igdeAction::Ref pActionRuleAddForeignState;
	igdeAction::Ref pActionRuleAddGroup;
	igdeAction::Ref pActionRuleAddSubAnimator;
	igdeAction::Ref pActionRuleAddTrackTo;
	igdeAction::Ref pActionRuleAddLimit;
	igdeAction::Ref pActionRuleAddMirror;
	
	igdeAction::Ref pActionRuleAddIntoGroupAnim;
	igdeAction::Ref pActionRuleAddIntoGroupAnimDiff;
	igdeAction::Ref pActionRuleAddIntoGroupAnimSelect;
	igdeAction::Ref pActionRuleAddIntoGroupBoneRot;
	igdeAction::Ref pActionRuleAddIntoGroupInvKin;
	igdeAction::Ref pActionRuleAddIntoGroupStateManip;
	igdeAction::Ref pActionRuleAddIntoGroupStateSnap;
	igdeAction::Ref pActionRuleAddIntoGroupForeignState;
	igdeAction::Ref pActionRuleAddIntoGroupGroup;
	igdeAction::Ref pActionRuleAddIntoGroupSubAnimator;
	igdeAction::Ref pActionRuleAddIntoGroupTrackTo;
	igdeAction::Ref pActionRuleAddIntoGroupLimit;
	igdeAction::Ref pActionRuleAddIntoGroupMirror;
	
	igdeAction::Ref pActionRuleInsertAnim;
	igdeAction::Ref pActionRuleInsertAnimDiff;
	igdeAction::Ref pActionRuleInsertAnimSelect;
	igdeAction::Ref pActionRuleInsertBoneRot;
	igdeAction::Ref pActionRuleInsertInvKin;
	igdeAction::Ref pActionRuleInsertStateManip;
	igdeAction::Ref pActionRuleInsertStateSnap;
	igdeAction::Ref pActionRuleInsertForeignState;
	igdeAction::Ref pActionRuleInsertGroup;
	igdeAction::Ref pActionRuleInsertSubAnimator;
	igdeAction::Ref pActionRuleInsertTrackTo;
	igdeAction::Ref pActionRuleInsertLimit;
	igdeAction::Ref pActionRuleInsertMirror;
	
	igdeAction::Ref pActionRuleRemove;
	igdeAction::Ref pActionRuleUp;
	igdeAction::Ref pActionRuleDown;
	
	igdeToolBar::Ref pTBFile;
	igdeToolBar::Ref pTBEdit;
	
	aeConfiguration *pConfiguration;
	igdeClipboard pClipboard;
	aeLoadSaveSystem *pLoadSaveSystem;
	
	aeView3D::Ref pView3D;
	aeWindowProperties::Ref pWindowProperties;
	
	aeAnimator::Ref pAnimator;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create main window. */
	aeWindowMain(aeIGDEModule &module);
	
protected:
	/** Clean up main window. */
	virtual ~aeWindowMain();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Ask user if it is okay to quit the application. */
	bool QuitRequest();
	
	/** Reset views. */
	void ResetViews();
	
	/** Configuration. */
	inline aeConfiguration &GetConfiguration() const{ return *pConfiguration; }
	
	/** Clipboard. */
	inline igdeClipboard &GetClipboard(){ return pClipboard; }
	inline const igdeClipboard &GetClipboard() const{ return pClipboard; }
	
	/** 3D View. */
	inline aeView3D &GetView3D() const{ return *pView3D; }
	
	/** Load save system. */
	inline aeLoadSaveSystem &GetLoadSaveSystem() const{ return *pLoadSaveSystem; }
	
	/** Animator. */
	inline const aeAnimator::Ref &GetAnimator() const{ return pAnimator; }
	
	/** Set animator. */
	void SetAnimator(aeAnimator *animator);
	
	/** Create animator. */
	void CreateNewAnimator();
	
	/** Save animator. */
	void SaveAnimator(const char *filename);
	
	/** Create a new rule of a given type using the matching undo action. */
	void CreateRule(deAnimatorRuleVisitorIdentify::eRuleTypes type, bool insert, bool intoGroup);
	
	/** Sets the visibility of the progress bar in the status bar. */
	void SetProgressVisible(bool visible);
	
	/** Sets the progress bar progress. */
	void SetProgress(float progress);
	
	/** Sets the progress text. */
	void SetProgressText(const char *text);
	
	
	
	/** Icons. */
	inline igdeIcon *GetIconRuleAnimation() const{ return pIconRuleAnimation; }
	inline igdeIcon *GetIconRuleAnimationDifference() const{ return pIconRuleAnimationDifference; }
	inline igdeIcon *GetIconRuleAnimationSelect() const{ return pIconRuleAnimationSelect; }
	inline igdeIcon *GetIconRuleBoneTransformator() const{ return pIconRuleBoneTransformator; }
	inline igdeIcon *GetIconRuleForeignState() const{ return pIconRuleForeignState; }
	inline igdeIcon *GetIconRuleGroup() const{ return pIconRuleGroup; }
	inline igdeIcon *GetIconRuleInverseKinematic() const{ return pIconRuleInverseKinematic; }
	inline igdeIcon *GetIconRuleLimit() const{ return pIconRuleLimit; }
	inline igdeIcon *GetIconRuleStateManipulator() const{ return pIconRuleStateManipulator; }
	inline igdeIcon *GetIconRuleStateSnapshot() const{ return pIconRuleStateSnapshot; }
	inline igdeIcon *GetIconRuleSubAnimator() const{ return pIconRuleSubAnimator; }
	inline igdeIcon *GetIconRuleTrackTo() const{ return pIconRuleTrackTo; }
	inline igdeIcon *GetIconRuleMirror() const{ return pIconRuleMirror; }
	igdeIcon *GetRuleIcon(deAnimatorRuleVisitorIdentify::eRuleTypes type) const;
	
	
	
	/** Actions. */
	inline igdeAction *GetActionControllerAdd() const{ return pActionControllerAdd; }
	inline igdeAction *GetActionControllerDuplicate() const{ return pActionControllerDuplicate; }
	inline igdeAction *GetActionControllerRemove() const{ return pActionControllerRemove; }
	inline igdeAction *GetActionControllerUp() const{ return pActionControllerUp; }
	inline igdeAction *GetActionControllerDown() const{ return pActionControllerDown; }
	
	inline igdeAction *GetActionLinkAdd() const{ return pActionLinkAdd; }
	inline igdeAction *GetActionLinkDuplicate() const{ return pActionLinkDuplicate; }
	inline igdeAction *GetActionLinkRemove() const{ return pActionLinkRemove; }
	inline igdeAction *GetActionLinkRemoveUnused() const{ return pActionLinkRemoveUnused; }
	
	inline igdeAction *GetActionRuleAddAnim() const{ return pActionRuleAddAnim; }
	inline igdeAction *GetActionRuleAddAnimDiff() const{ return pActionRuleAddAnimDiff; }
	inline igdeAction *GetActionRuleAddAnimSelect() const{ return pActionRuleAddAnimSelect; }
	inline igdeAction *GetActionRuleAddBoneRot() const{ return pActionRuleAddBoneRot; }
	inline igdeAction *GetActionRuleAddInvKin() const{ return pActionRuleAddInvKin; }
	inline igdeAction *GetActionRuleAddStateManip() const{ return pActionRuleAddStateManip; }
	inline igdeAction *GetActionRuleAddStateSnap() const{ return pActionRuleAddStateSnap; }
	inline igdeAction *GetActionRuleAddForeignState() const{ return pActionRuleAddForeignState; }
	inline igdeAction *GetActionRuleAddGroup() const{ return pActionRuleAddGroup; }
	inline igdeAction *GetActionRuleAddSubAnimator() const{ return pActionRuleAddSubAnimator; }
	inline igdeAction *GetActionRuleAddTrackTo() const{ return pActionRuleAddTrackTo; }
	inline igdeAction *GetActionRuleAddLimit() const{ return pActionRuleAddLimit; }
	inline igdeAction *GetActionRuleAddMirror() const{ return pActionRuleAddMirror; }
	
	inline igdeAction *GetActionRuleAddIntoGroupAnim() const{ return pActionRuleAddIntoGroupAnim; }
	inline igdeAction *GetActionRuleAddIntoGroupAnimDiff() const{ return pActionRuleAddIntoGroupAnimDiff; }
	inline igdeAction *GetActionRuleAddIntoGroupAnimSelect() const{ return pActionRuleAddIntoGroupAnimSelect; }
	inline igdeAction *GetActionRuleAddIntoGroupBoneRot() const{ return pActionRuleAddIntoGroupBoneRot; }
	inline igdeAction *GetActionRuleAddIntoGroupInvKin() const{ return pActionRuleAddIntoGroupInvKin; }
	inline igdeAction *GetActionRuleAddIntoGroupStateManip() const{ return pActionRuleAddIntoGroupStateManip; }
	inline igdeAction *GetActionRuleAddIntoGroupStateSnap() const{ return pActionRuleAddIntoGroupStateSnap; }
	inline igdeAction *GetActionRuleAddIntoGroupForeignState() const{ return pActionRuleAddIntoGroupForeignState; }
	inline igdeAction *GetActionRuleAddIntoGroupGroup() const{ return pActionRuleAddIntoGroupGroup; }
	inline igdeAction *GetActionRuleAddIntoGroupSubAnimator() const{ return pActionRuleAddIntoGroupSubAnimator; }
	inline igdeAction *GetActionRuleAddIntoGroupTrackTo() const{ return pActionRuleAddIntoGroupTrackTo; }
	inline igdeAction *GetActionRuleAddIntoGroupLimit() const{ return pActionRuleAddIntoGroupLimit; }
	inline igdeAction *GetActionRuleAddIntoGroupMirror() const{ return pActionRuleAddIntoGroupMirror; }
	
	inline igdeAction *GetActionRuleInsertAnim() const{ return pActionRuleInsertAnim; }
	inline igdeAction *GetActionRuleInsertAnimDiff() const{ return pActionRuleInsertAnimDiff; }
	inline igdeAction *GetActionRuleInsertAnimSelect() const{ return pActionRuleInsertAnimSelect; }
	inline igdeAction *GetActionRuleInsertBoneRot() const{ return pActionRuleInsertBoneRot; }
	inline igdeAction *GetActionRuleInsertInvKin() const{ return pActionRuleInsertInvKin; }
	inline igdeAction *GetActionRuleInsertStateManip() const{ return pActionRuleInsertStateManip; }
	inline igdeAction *GetActionRuleInsertStateSnap() const{ return pActionRuleInsertStateSnap; }
	inline igdeAction *GetActionRuleInsertForeignState() const{ return pActionRuleInsertForeignState; }
	inline igdeAction *GetActionRuleInsertGroup() const{ return pActionRuleInsertGroup; }
	inline igdeAction *GetActionRuleInsertSubAnimator() const{ return pActionRuleInsertSubAnimator; }
	inline igdeAction *GetActionRuleInsertTrackTo() const{ return pActionRuleInsertTrackTo; }
	inline igdeAction *GetActionRuleInsertLimit() const{ return pActionRuleInsertLimit; }
	inline igdeAction *GetActionRuleInsertMirror() const{ return pActionRuleInsertMirror; }
	
	inline igdeAction *GetActionRuleRemove() const{ return pActionRuleRemove; }
	inline igdeAction *GetActionRuleUp() const{ return pActionRuleUp; }
	inline igdeAction *GetActionRuleDown() const{ return pActionRuleDown; }
	
	
	
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
	void pCreateMenuFile(igdeMenuCascade &menu);
	void pCreateMenuEdit(igdeMenuCascade &menu);
	void pCreateMenuController(igdeMenuCascade &menu);
	void pCreateMenuLink(igdeMenuCascade &menu);
	void pCreateMenuRule(igdeMenuCascade &menu);
};

#endif
