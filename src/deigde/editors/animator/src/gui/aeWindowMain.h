/* 
 * Drag[en]gine IGDE Animator Editor
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

#ifndef _AEWINDOWMAIN_H_
#define _AEWINDOWMAIN_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <deigde/clipboard/igdeClipboard.h>
#include <deigde/gui/igdeEditorWindow.h>
#include <deigde/gui/igdeToolBarReference.h>
#include <deigde/gui/igdeTabBookReference.h>
#include <deigde/gui/event/igdeActionReference.h>
#include <deigde/gui/event/igdeActionUndoReference.h>
#include <deigde/gui/event/igdeActionRedoReference.h>
#include <deigde/gui/resources/igdeIconReference.h>

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
 * \brief Main window.
 */
class aeWindowMain : public igdeEditorWindow{
private:
	aeWindowMainListener *pListener;
	
	igdeIconReference pIconRuleAnimation;
	igdeIconReference pIconRuleAnimationDifference;
	igdeIconReference pIconRuleAnimationSelect;
	igdeIconReference pIconRuleBoneTransformator;
	igdeIconReference pIconRuleForeignState;
	igdeIconReference pIconRuleGroup;
	igdeIconReference pIconRuleInverseKinematic;
	igdeIconReference pIconRuleLimit;
	igdeIconReference pIconRuleStateManipulator;
	igdeIconReference pIconRuleStateSnapshot;
	igdeIconReference pIconRuleSubAnimator;
	igdeIconReference pIconRuleTrackTo;
	
	igdeActionReference pActionFileNew;
	igdeActionReference pActionFileOpen;
	igdeActionReference pActionFileSave;
	igdeActionReference pActionFileSaveAs;
	
	igdeActionUndoReference pActionEditUndo;
	igdeActionRedoReference pActionEditRedo;
	
	igdeActionReference pActionEditCut;
	igdeActionReference pActionEditCopy;
	igdeActionReference pActionEditPaste;
	igdeActionReference pActionEditLocoEnabled;
	igdeActionReference pActionEditWBTracking;
	igdeActionReference pActionEditShowBones;
	igdeActionReference pActionEditDDBoneSize;
	
	igdeActionReference pActionControllerAdd;
	igdeActionReference pActionControllerRemove;
	igdeActionReference pActionControllerUp;
	igdeActionReference pActionControllerDown;
	
	igdeActionReference pActionLinkAdd;
	igdeActionReference pActionLinkDuplicate;
	igdeActionReference pActionLinkRemove;
	igdeActionReference pActionLinkRemoveUnused;
	
	igdeActionReference pActionRuleAddAnim;
	igdeActionReference pActionRuleAddAnimDiff;
	igdeActionReference pActionRuleAddAnimSelect;
	igdeActionReference pActionRuleAddBoneRot;
	igdeActionReference pActionRuleAddInvKin;
	igdeActionReference pActionRuleAddStateManip;
	igdeActionReference pActionRuleAddStateSnap;
	igdeActionReference pActionRuleAddForeignState;
	igdeActionReference pActionRuleAddGroup;
	igdeActionReference pActionRuleAddSubAnimator;
	igdeActionReference pActionRuleAddTrackTo;
	igdeActionReference pActionRuleAddLimit;
	
	igdeActionReference pActionRuleAddIntoGroupAnim;
	igdeActionReference pActionRuleAddIntoGroupAnimDiff;
	igdeActionReference pActionRuleAddIntoGroupAnimSelect;
	igdeActionReference pActionRuleAddIntoGroupBoneRot;
	igdeActionReference pActionRuleAddIntoGroupInvKin;
	igdeActionReference pActionRuleAddIntoGroupStateManip;
	igdeActionReference pActionRuleAddIntoGroupStateSnap;
	igdeActionReference pActionRuleAddIntoGroupForeignState;
	igdeActionReference pActionRuleAddIntoGroupGroup;
	igdeActionReference pActionRuleAddIntoGroupSubAnimator;
	igdeActionReference pActionRuleAddIntoGroupTrackTo;
	igdeActionReference pActionRuleAddIntoGroupLimit;
	
	igdeActionReference pActionRuleInsertAnim;
	igdeActionReference pActionRuleInsertAnimDiff;
	igdeActionReference pActionRuleInsertAnimSelect;
	igdeActionReference pActionRuleInsertBoneRot;
	igdeActionReference pActionRuleInsertInvKin;
	igdeActionReference pActionRuleInsertStateManip;
	igdeActionReference pActionRuleInsertStateSnap;
	igdeActionReference pActionRuleInsertForeignState;
	igdeActionReference pActionRuleInsertGroup;
	igdeActionReference pActionRuleInsertSubAnimator;
	igdeActionReference pActionRuleInsertTrackTo;
	igdeActionReference pActionRuleInsertLimit;
	
	igdeActionReference pActionRuleRemove;
	igdeActionReference pActionRuleUp;
	igdeActionReference pActionRuleDown;
	
	igdeToolBarReference pTBFile;
	igdeToolBarReference pTBEdit;
	
	aeConfiguration *pConfiguration;
	igdeClipboard pClipboard;
	aeLoadSaveSystem *pLoadSaveSystem;
	
	aeView3D *pView3D;
	aeWindowProperties *pWindowProperties;
	
	aeAnimator *pAnimator;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create main window. */
	aeWindowMain( aeIGDEModule &module );
	
protected:
	/** \brief Clean up main window. */
	virtual ~aeWindowMain();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Ask user if it is okay to quit the application. */
	bool QuitRequest();
	
	/** \brief Reset views. */
	void ResetViews();
	
	/** \brief Configuration. */
	inline aeConfiguration &GetConfiguration() const{ return *pConfiguration; }
	
	/** \brief Clipboard. */
	inline igdeClipboard &GetClipboard(){ return pClipboard; }
	inline const igdeClipboard &GetClipboard() const{ return pClipboard; }
	
	/** \brief 3D View. */
	inline aeView3D &GetView3D() const{ return *pView3D; }
	
	/** \brief Load save system. */
	inline aeLoadSaveSystem &GetLoadSaveSystem() const{ return *pLoadSaveSystem; }
	
	/** \brief Animator. */
	inline aeAnimator *GetAnimator() const{ return pAnimator; }
	
	/** \brief Set animator. */
	void SetAnimator( aeAnimator *animator );
	
	/** \brief Create animator. */
	void CreateNewAnimator();
	
	/** \brief Save animator. */
	void SaveAnimator( const char *filename );
	
	/** \brief Create a new rule of a given type using the matching undo action. */
	void CreateRule( deAnimatorRuleVisitorIdentify::eRuleTypes type, bool insert, bool intoGroup );
	
	/** \brief Sets the visibility of the progress bar in the status bar. */
	void SetProgressVisible( bool visible );
	
	/** \brief Sets the progress bar progress. */
	void SetProgress( float progress );
	
	/** \brief Sets the progress text. */
	void SetProgressText( const char *text );
	
	
	
	/** \brief Icons. */
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
	igdeIcon *GetRuleIcon( deAnimatorRuleVisitorIdentify::eRuleTypes type ) const;
	
	
	
	/** \brief Actions. */
	inline igdeAction *GetActionControllerAdd() const{ return pActionControllerAdd; }
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
	
	inline igdeAction *GetActionRuleRemove() const{ return pActionRuleRemove; }
	inline igdeAction *GetActionRuleUp() const{ return pActionRuleUp; }
	inline igdeAction *GetActionRuleDown() const{ return pActionRuleDown; }
	
	
	
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
	void pCreateMenuController( igdeMenuCascade &menu );
	void pCreateMenuLink( igdeMenuCascade &menu );
	void pCreateMenuRule( igdeMenuCascade &menu );
};

#endif
