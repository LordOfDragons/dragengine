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
#include <deigde/gui/igdeToolBarReference.h>
#include <deigde/gui/event/igdeActionReference.h>
#include <deigde/gui/event/igdeActionUndoReference.h>
#include <deigde/gui/event/igdeActionRedoReference.h>
#include <deigde/gui/resources/igdeIconReference.h>

#include <dragengine/common/math/decMath.h>

class reWindowMainListener;
class reConfiguration;
class reView3D;
class reRig;
class reClipboard;
class reWindowProperties;
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
	reWindowMainListener *pListener;
	
	igdeIconReference pIconEditBone;
	igdeIconReference pIconEditShape;
	igdeIconReference pIconEditConstraint;
	igdeIconReference pIconEditPush;
	igdeIconReference pIconEditSelect;
	igdeIconReference pIconEditMove;
	igdeIconReference pIconEditScale;
	igdeIconReference pIconEditRotate;
// 	igdeIconReference pIconEdit3DCursor;
	igdeIconReference pIconEditLockAxisX;
	igdeIconReference pIconEditLockAxisY;
	igdeIconReference pIconEditLockAxisZ;
	igdeIconReference pIconEditLockLocal;
	
	igdeActionReference pActionFileNew;
	igdeActionReference pActionFileOpen;
	igdeActionReference pActionFileSave;
	igdeActionReference pActionFileSaveAs;
	
	igdeActionUndoReference pActionEditUndo;
	igdeActionRedoReference pActionEditRedo;
	igdeActionReference pActionEditCut;
	igdeActionReference pActionEditCopy;
	igdeActionReference pActionEditPaste;
	igdeActionReference pActionEditBoneMode;
	igdeActionReference pActionEditShapeMode;
	igdeActionReference pActionEditConstraintMode;
	igdeActionReference pActionEditPushMode;
	igdeActionReference pActionEditSelectMode;
	igdeActionReference pActionEditMoveMode;
	igdeActionReference pActionEditScaleMode;
	igdeActionReference pActionEditRotateMode;
// 	igdeActionReference pActionEdit3DCursorMode;
	igdeActionReference pActionEditSelectAll;
	igdeActionReference pActionEditSelectAllWithShapes;
	igdeActionReference pActionEditSelectNone;
	igdeActionReference pActionEditDelete;
	igdeActionReference pActionEditLockAxisX;
	igdeActionReference pActionEditLockAxisY;
	igdeActionReference pActionEditLockAxisZ;
	igdeActionReference pActionEditLockLocal;
	
	igdeActionReference pActionRigAddSphere;
	igdeActionReference pActionRigAddBox;
	igdeActionReference pActionRigAddCylinder;
	igdeActionReference pActionRigAddCapsule;
	igdeActionReference pActionRigAddConstraint;
	igdeActionReference pActionRigAddPush;
	igdeActionReference pActionRigShowShapes;
	igdeActionReference pActionRigShowConstraints;
	igdeActionReference pActionRigShowPushes;
	
	igdeActionReference pActionBoneAdd;
	igdeActionReference pActionBoneAddSphere;
	igdeActionReference pActionBoneAddBox;
	igdeActionReference pActionBoneAddCylinder;
	igdeActionReference pActionBoneAddCapsule;
	igdeActionReference pActionBoneAddConstraint;
	igdeActionReference pActionBoneShowBones;
	igdeActionReference pActionBoneShowAllShapes;
	igdeActionReference pActionBoneShowAllConstraints;
	igdeActionReference pActionBoneMirror;
	igdeActionReference pActionBoneImport;
	igdeActionReference pActionBoneScaleMass;
	igdeActionReference pActionBoneMassFromVolume;
	
	igdeActionReference pActionViewShapeXRay;
	
	igdeActionReference pActionSimulationRun;
	
	igdeToolBarReference pTBFile;
	igdeToolBarReference pTBEdit;
	
	reConfiguration *pConfiguration;
	reClipboard *pClipboard;
	reLoadSaveSystem *pLoadSaveSystem;
	
	reView3D *pView3D;
	reWindowProperties *pWindowProperties;
	
	reRig *pRig;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create window. */
	reWindowMain( reIGDEModule &module );
	
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
	inline reRig *GetRig() const{ return pRig; }
	
	/** \brief Set rig. */
	void SetRig( reRig *rig );
	
	/** \brief Create new rig. */
	void CreateNewRig();
	
	/** \brief Save rig under the given file. */
	void SaveRig( const char *filename );
	
	
	
	/** \brief Icons. */
	inline igdeIcon *GetIconEditBone() const{ return pIconEditBone; }
	inline igdeIcon *GetIconEditShape() const{ return pIconEditShape; }
	inline igdeIcon *GetIconEditConstraint() const{ return pIconEditConstraint; }
	inline igdeIcon *GetIconEditPush() const{ return pIconEditPush; }
	inline igdeIcon *GetIconEditSelect() const{ return pIconEditSelect; }
	inline igdeIcon *GetIconEditMove() const{ return pIconEditMove; }
	inline igdeIcon *GetIconEditScale() const{ return pIconEditScale; }
	inline igdeIcon *GetIconEditRotate() const{ return pIconEditRotate; }
// 	inline igdeIcon *GetIconEdit3DCursor() const{ return pIconEdit3DCursor; }
	inline igdeIcon *GetIconEditLockAxisX() const{ return pIconEditLockAxisX; }
	inline igdeIcon *GetIconEditLockAxisY() const{ return pIconEditLockAxisY; }
	inline igdeIcon *GetIconEditLockAxisZ() const{ return pIconEditLockAxisZ; }
	inline igdeIcon *GetIconEditLockLocal() const{ return pIconEditLockLocal; }
	
	
	
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
	void pCreateMenuRig( igdeMenuCascade &menu );
	void pCreateMenuBone( igdeMenuCascade &menu );
	void pCreateMenuView( igdeMenuCascade &menu );
	void pCreateMenuSimulation( igdeMenuCascade &menu );
};

#endif
