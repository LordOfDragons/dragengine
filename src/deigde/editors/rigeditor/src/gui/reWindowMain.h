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

#ifndef _REWINDOWMAIN_H_
#define _REWINDOWMAIN_H_

#include <deigde/gui/igdeEditorWindow.h>
#include <deigde/gui/igdeToolBar.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionUndo.h>
#include <deigde/gui/event/igdeActionRedo.h>
#include <deigde/gui/resources/igdeIcon.h>

#include <dragengine/common/math/decMath.h>

#include "reWindowMainListener.h"
class reConfiguration;
#include "reView3D.h"
#include "../rig/reRig.h"
class reClipboard;
#include "properties/reWindowProperties.h"
class reEngineController;
class reLoadSaveSystem;
class reIGDEModule;

class decStringList;
class deCmdLineArgs;
class deEngine;
class deLogger;
class deException;

class igdeGameDefinition;
class igdeEngineController;
class igdeStepableTask;

class igdeEnvironment;



/**
 * \brief Main application window.
 */
class reWindowMain : public igdeEditorWindow{
private:
	reWindowMainListener::Ref pListener;
	
	igdeIcon::Ref pIconEditBone;
	igdeIcon::Ref pIconEditShape;
	igdeIcon::Ref pIconEditConstraint;
	igdeIcon::Ref pIconEditPush;
	igdeIcon::Ref pIconEditSelect;
	igdeIcon::Ref pIconEditMove;
	igdeIcon::Ref pIconEditScale;
	igdeIcon::Ref pIconEditRotate;
// 	igdeIcon::Ref pIconEdit3DCursor;
	igdeIcon::Ref pIconEditLockAxisX;
	igdeIcon::Ref pIconEditLockAxisY;
	igdeIcon::Ref pIconEditLockAxisZ;
	igdeIcon::Ref pIconEditLockLocal;
	
	igdeAction::Ref pActionFileNew;
	igdeAction::Ref pActionFileOpen;
	igdeAction::Ref pActionFileSave;
	igdeAction::Ref pActionFileSaveAs;
	
	igdeActionUndo::Ref pActionEditUndo;
	igdeActionRedo::Ref pActionEditRedo;
	igdeAction::Ref pActionEditCut;
	igdeAction::Ref pActionEditCopy;
	igdeAction::Ref pActionEditPaste;
	igdeAction::Ref pActionEditBoneMode;
	igdeAction::Ref pActionEditShapeMode;
	igdeAction::Ref pActionEditConstraintMode;
	igdeAction::Ref pActionEditPushMode;
	igdeAction::Ref pActionEditSelectMode;
	igdeAction::Ref pActionEditMoveMode;
	igdeAction::Ref pActionEditScaleMode;
	igdeAction::Ref pActionEditRotateMode;
// 	igdeAction::Ref pActionEdit3DCursorMode;
	igdeAction::Ref pActionEditSelectAll;
	igdeAction::Ref pActionEditSelectAllWithShapes;
	igdeAction::Ref pActionEditSelectNone;
	igdeAction::Ref pActionEditDelete;
	igdeAction::Ref pActionEditLockAxisX;
	igdeAction::Ref pActionEditLockAxisY;
	igdeAction::Ref pActionEditLockAxisZ;
	igdeAction::Ref pActionEditLockLocal;
	
	igdeAction::Ref pActionRigAddSphere;
	igdeAction::Ref pActionRigAddBox;
	igdeAction::Ref pActionRigAddCylinder;
	igdeAction::Ref pActionRigAddCapsule;
	igdeAction::Ref pActionRigAddConstraint;
	igdeAction::Ref pActionRigAddPush;
	igdeAction::Ref pActionRigShowShapes;
	igdeAction::Ref pActionRigShowConstraints;
	igdeAction::Ref pActionRigShowPushes;
	
	igdeAction::Ref pActionBoneAdd;
	igdeAction::Ref pActionBoneAddSphere;
	igdeAction::Ref pActionBoneAddBox;
	igdeAction::Ref pActionBoneAddCylinder;
	igdeAction::Ref pActionBoneAddCapsule;
	igdeAction::Ref pActionBoneAddConstraint;
	igdeAction::Ref pActionBoneShowBones;
	igdeAction::Ref pActionBoneShowAllShapes;
	igdeAction::Ref pActionBoneShowAllConstraints;
	igdeAction::Ref pActionBoneMirror;
	igdeAction::Ref pActionBoneImport;
	igdeAction::Ref pActionBoneScaleMass;
	igdeAction::Ref pActionBoneMassFromVolume;
	
	igdeAction::Ref pActionViewShapeXRay;
	
	igdeAction::Ref pActionSimulationRun;
	
	igdeToolBar::Ref pTBFile;
	igdeToolBar::Ref pTBEdit;
	
	reConfiguration *pConfiguration;
	reClipboard *pClipboard;
	reLoadSaveSystem *pLoadSaveSystem;
	
	reView3D::Ref pView3D;
	reWindowProperties::Ref pWindowProperties;
	
	reRig::Ref pRig;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create window. */
	reWindowMain(reIGDEModule &module);
	
protected:
	/** \brief Clean up window. */
	virtual ~reWindowMain();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Ask user if it is okay to quit the application. */
	bool QuitRequest();
	
	/** \brief Reset views. */
	void ResetViews();
	
	/** \brief Configuration. */
	inline reConfiguration &GetConfiguration() const{ return *pConfiguration; }
	
	/** \brief Clipboard. */
	inline reClipboard &GetClipboard() const{ return *pClipboard; }
	
	/** \brief Load save system. */
	inline reLoadSaveSystem &GetLoadSaveSystem() const{ return *pLoadSaveSystem; }
	
	/** \brief 3D View. */
	inline reView3D &GetView3D() const{ return *pView3D; }
	
	
	
	/** \brief Rig. */
	inline const reRig::Ref &GetRig() const{ return pRig; }
	
	/** \brief Set rig. */
	void SetRig(reRig *rig);
	
	/** \brief Create new rig. */
	void CreateNewRig();
	
	/** \brief Save rig under the given file. */
	void SaveRig(const char *filename);
	
	
	
	/** \brief Icons. */
	inline const igdeIcon::Ref &GetIconEditBone() const{ return pIconEditBone; }
	inline const igdeIcon::Ref &GetIconEditShape() const{ return pIconEditShape; }
	inline const igdeIcon::Ref &GetIconEditConstraint() const{ return pIconEditConstraint; }
	inline const igdeIcon::Ref &GetIconEditPush() const{ return pIconEditPush; }
	inline const igdeIcon::Ref &GetIconEditSelect() const{ return pIconEditSelect; }
	inline const igdeIcon::Ref &GetIconEditMove() const{ return pIconEditMove; }
	inline const igdeIcon::Ref &GetIconEditScale() const{ return pIconEditScale; }
	inline const igdeIcon::Ref &GetIconEditRotate() const{ return pIconEditRotate; }
// 	inline const igdeIcon::Ref &GetIconEdit3DCursor() const{ return pIconEdit3DCursor; }
	inline const igdeIcon::Ref &GetIconEditLockAxisX() const{ return pIconEditLockAxisX; }
	inline const igdeIcon::Ref &GetIconEditLockAxisY() const{ return pIconEditLockAxisY; }
	inline const igdeIcon::Ref &GetIconEditLockAxisZ() const{ return pIconEditLockAxisZ; }
	inline const igdeIcon::Ref &GetIconEditLockLocal() const{ return pIconEditLockLocal; }
	
	
	
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
	virtual void OnFrameUpdate(float elapsed);
	
	/**
	 * \brief Retrieves a list of changed documents.
	 * 
	 * This list is requested by the IGDE if a game project is closed due to creating or
	 * loading a new one or because the application is about to be closed. Editors modules
	 * have to add to the list the filenames of open documents that have changed and need
	 * saving. The filename is later used in calls to \ref SaveDocument to save the file
	 * if requested by the user. All other files are discarded.
	 */
	virtual void GetChangedDocuments(decStringList &list);
	
	/**
	 * \brief Requests a document to be loaded.
	 */
	virtual void LoadDocument(const char *filename);
	
	/**
	 * \brief Requests a document to be saved.
	 * 
	 * The document has to be saved if changed. If not changed this call can be ignored.
	 * This call is usually made after a previous call to \ref GetUnsavedDocuments.
	 * 
	 * \returns True if the saving has been successful or false otherwise.
	 */
	virtual bool SaveDocument(const char *filename);
	
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
	void pCreateMenuFile(igdeMenuCascade &menu);
	void pCreateMenuEdit(igdeMenuCascade &menu);
	void pCreateMenuRig(igdeMenuCascade &menu);
	void pCreateMenuBone(igdeMenuCascade &menu);
	void pCreateMenuView(igdeMenuCascade &menu);
	void pCreateMenuSimulation(igdeMenuCascade &menu);
};

#endif
