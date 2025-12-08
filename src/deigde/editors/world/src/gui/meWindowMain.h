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

#ifndef _MEWINDOWMAIN_H_
#define _MEWINDOWMAIN_H_

#include <deigde/clipboard/igdeClipboard.h>
#include <deigde/gui/igdeEditorWindow.h>
#include <deigde/gui/igdeToolBar.h>
#include <deigde/gui/igdeTabBook.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionUndo.h>
#include <deigde/gui/event/igdeActionRedo.h>
#include <deigde/gui/resources/igdeIcon.h>

class meConfiguration;
class meIGDEModule;
class meLoadSaveSystem;
class meSaveSupport;
class meView3D;
class meView3D;
class meWindowChangelog;
class meWindowMainListener;
class meWindowProperties;
class meWindowVegetation;
class meWorld;

class igdeEngineController;
class igdeGameDefinition;
class igdeStepableTask;
class igdeEnvironment;
class igdeMenuDefinition;

class decStringList;
class deEngine;
class deException;
class deLogger;



/**
 * Main Application Window.
 */
class meWindowMain : public igdeEditorWindow{
private:
	meWindowMainListener::Ref pListener;
	bool pActiveModule;
	
	igdeIcon::Ref pIconEditObject;
	igdeIcon::Ref pIconEditDecal;
	igdeIcon::Ref pIconEditNavSpace;
	igdeIcon::Ref pIconEditObjectShape;
	igdeIcon::Ref pIconEditSelect;
	igdeIcon::Ref pIconEditMove;
	igdeIcon::Ref pIconEditScale;
	igdeIcon::Ref pIconEditRotate;
	igdeIcon::Ref pIconEditRotateRandom;
	igdeIcon::Ref pIconEdit3DCursor;
	igdeIcon::Ref pIconEditHeightPaint;
	igdeIcon::Ref pIconEditMaskPaint;
	igdeIcon::Ref pIconEditVisibilityPaint;
	igdeIcon::Ref pIconEditLockAxisX;
	igdeIcon::Ref pIconEditLockAxisY;
	igdeIcon::Ref pIconEditLockAxisZ;
	igdeIcon::Ref pIconEditLockAxisFlip;
	igdeIcon::Ref pIconEditUseLocal;
	igdeIcon::Ref pIconEditSnap;
	igdeIcon::Ref pIconViewFullBrightOn;
	igdeIcon::Ref pIconViewFullBrightOff;
	igdeIcon::Ref pIconViewMuteSoundOn;
	igdeIcon::Ref pIconViewMuteSoundOff;
	igdeIcon::Ref pIconObjectLightToggle;
	
	igdeAction::Ref pActionFileNew;
	igdeAction::Ref pActionFileOpen;
	igdeAction::Ref pActionFileSave;
	igdeAction::Ref pActionFileSaveAs;
	
	igdeActionUndo::Ref pActionEditUndo;
	igdeActionRedo::Ref pActionEditRedo;
	
	igdeAction::Ref pActionEditCut;
	igdeAction::Ref pActionEditCopy;
	igdeAction::Ref pActionEditPaste;
	igdeAction::Ref pActionEditDuplicate;
	igdeAction::Ref pActionEditDelete;
	igdeAction::Ref pActionEditEModeObject;
	igdeAction::Ref pActionEditEModeDecal;
	igdeAction::Ref pActionEditEModeNavSpace;
	igdeAction::Ref pActionEditEModeObjectShape;
	igdeAction::Ref pActionEditSelectMode;
	igdeAction::Ref pActionEdit3DCursorMode;
	igdeAction::Ref pActionEditMoveMode;
	igdeAction::Ref pActionEditScaleMode;
	igdeAction::Ref pActionEditRotateMode;
	igdeAction::Ref pActionEditAddMode;
	igdeAction::Ref pActionEditHeightPaintMode;
	igdeAction::Ref pActionEditMaskPaintMode;
	igdeAction::Ref pActionEditVisPaintMode;
	igdeAction::Ref pActionEditModeNavSpaceEdit;
	igdeAction::Ref pActionEditLockAxisX, pActionEditLockAxisY, pActionEditLockAxisZ;
	igdeAction::Ref pActionEditUseLocal, pActionEditLockAxisFlip;
	igdeAction::Ref pActionEditSnapSnapPoints;
	igdeAction::Ref pActionEditRPCenterActive;
	igdeAction::Ref pActionEditRPCenterSelected;
	igdeAction::Ref pActionEditRPCenterIndividual;
	igdeAction::Ref pActionEditUse3DCursor;
	igdeAction::Ref pActionEditSelectNone;
	
	igdeAction::Ref pActionObjectLightToggle;
	igdeAction::Ref pActionObjectRotateL45;
	igdeAction::Ref pActionObjectRotateL90;
	igdeAction::Ref pActionObjectRotateR45;
	igdeAction::Ref pActionObjectRotateR90;
	igdeAction::Ref pActionObjectRotate180;
	igdeAction::Ref pActionObjectRotateRandom;
	igdeAction::Ref pActionObjectDropToGround;
	igdeAction::Ref pActionObjectSnapToGrid;
	igdeAction::Ref pActionObjectCopyPositionX;
	igdeAction::Ref pActionObjectCopyPositionY;
	igdeAction::Ref pActionObjectCopyPositionZ;
	igdeAction::Ref pActionObjectCopyPositionXZ;
	igdeAction::Ref pActionObjectCopyPositionXYZ;
	igdeAction::Ref pActionObjectCopyRotationX;
	igdeAction::Ref pActionObjectCopyRotationY;
	igdeAction::Ref pActionObjectCopyRotationZ;
	igdeAction::Ref pActionObjectCopyRotationXYZ;
	igdeAction::Ref pActionObjectCopyScaleX;
	igdeAction::Ref pActionObjectCopyScaleY;
	igdeAction::Ref pActionObjectCopyScaleZ;
	igdeAction::Ref pActionObjectCopyScaleXYZ;
	igdeAction::Ref pActionObjectAttachTo;
	igdeAction::Ref pActionObjectDetach;
	igdeAction::Ref pActionObjectSelectAttached;
	igdeAction::Ref pActionObjectReassignIDs;
	igdeAction::Ref pActionObjectSubclassAsEclass;
	
	igdeAction::Ref pActionObjectShapeAddSphere;
	igdeAction::Ref pActionObjectShapeAddBox;
	igdeAction::Ref pActionObjectShapeAddCylinder;
	igdeAction::Ref pActionObjectShapeAddCapsule;
	igdeAction::Ref pActionObjectShapeDelete;
	
	igdeAction::Ref pActionDecalDelete;
	igdeAction::Ref pActionDecalRaiseTop;
	igdeAction::Ref pActionDecalRaiseOne;
	igdeAction::Ref pActionDecalLowerOne;
	igdeAction::Ref pActionDecalLowerBottom;
	
	igdeAction::Ref pActionFullBright;
	igdeAction::Ref pActionMuteSound;
	igdeAction::Ref pActionShowOcclusionMeshes;
	igdeAction::Ref pActionShowOcclusionMeshesSelected;
	igdeAction::Ref pActionShowNavigationSpaces;
	igdeAction::Ref pActionShowNavigationSpacesSelected;
	igdeAction::Ref pActionShowShapes;
	igdeAction::Ref pActionShowShapesSelected;
	igdeAction::Ref pActionNavTestLoad;
	igdeAction::Ref pActionNavTestSave;
	
	igdeToolBar::Ref pTBFile;
	igdeToolBar::Ref pTBEdit;
	igdeToolBar::Ref pTBObject;
	igdeToolBar::Ref pTBDecal;
	
	meConfiguration *pConfiguration;
	igdeClipboard pClipboard;
	meLoadSaveSystem *pLoadSaveSystem;
	meSaveSupport *pSaveSupport;
	
	meWindowProperties::Ref pWindowProperties;
	igdeTabBook::Ref pTabContent;
	meView3D::Ref pView3D;
	meWindowVegetation::Ref pViewVegetation;
	meWindowChangelog::Ref pViewChangelog;
	
	meWorld::Ref pWorld;
	bool pUse3DCursor;
	decVector p3DCursor;
	
	decString pLoadFilename;
	meWorld::Ref pLoadWorld;
	igdeStepableTask *pLoadTask;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create main window. */
	meWindowMain(meIGDEModule &module);
	
protected:
	/** Clean up main window. */
	virtual ~meWindowMain();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Ask user if it is okay to quit the application. */
	bool QuitRequest();
	
	/** Reset views. */
	void ResetViews();
	
	/** Configuration. */
	inline meConfiguration &GetConfiguration() const{ return *pConfiguration; }
	
	/** Clipboard. */
	inline igdeClipboard &GetClipboard(){ return pClipboard; }
	inline const igdeClipboard &GetClipboard() const { return pClipboard; }
	
	/** Load/save system. */
	inline meLoadSaveSystem &GetLoadSaveSystem() const{ return *pLoadSaveSystem; }
	
	/** Save support. */
	inline meSaveSupport &GetSaveSupport() const{ return *pSaveSupport; }
	
	
	
	/** Properties window. */
	inline meWindowProperties *GetWindowProperties() const{ return pWindowProperties; }
	
	/** View 3D. */
	inline meView3D *GetView3D() const{ return pView3D; }
	
	
	
	/** World. */
	inline meWorld *GetWorld() const{ return pWorld; }
	
	/** Set world. */
	void SetWorld(meWorld::Ref world);
	
	/** Create world. */
	void CreateNewWorld();
	
	/** Load game world. */
	void LoadWorld(const char *filename);
	
	/** Notify configuration enable GI changed. */
	void ConfigEnableGIChanged();
	
	/** Notify configuration enable auralization changed. */
	void ConfigEnableAuralizationChanged();
	
	
	
	inline bool GetUse3DCursor() const{ return pUse3DCursor; }
	void SetUse3DCursor(bool useIt);
	inline decVector Get3DCursor() const{ return p3DCursor; }
	inline void Set3DCursor(const decVector &pos){p3DCursor = pos;}
	
	void RotateActiveObjectBy(const decVector &rotation);
	
	/** Sets the visibility of the progress bar in the status bar. */
	void SetProgressVisible(bool visible);
	/** Sets the progress bar progress. */
	void SetProgress(float progress);
	/** Sets the progress text. */
	void SetProgressText(const char *text);
	
	
	
	/** Icons. */
	inline igdeIcon *GetIconEditObject() const{ return pIconEditObject; }
	inline igdeIcon *GetIconEditDecal() const{ return pIconEditDecal; }
	inline igdeIcon *GetIconEditNavSpace() const{ return pIconEditNavSpace; }
	inline igdeIcon *GetIconEditObjectShape() const{ return pIconEditObjectShape; }
	inline igdeIcon *GetIconEditSelect() const{ return pIconEditSelect; }
	inline igdeIcon *GetIconEditMove() const{ return pIconEditMove; }
	inline igdeIcon *GetIconEditScale() const{ return pIconEditScale; }
	inline igdeIcon *GetIconEditRotate() const{ return pIconEditRotate; }
	inline igdeIcon *GetIconEditRotateRandom() const{ return pIconEditRotateRandom; }
	inline igdeIcon *GetIconEdit3DCursor() const{ return pIconEdit3DCursor; }
	inline igdeIcon *GetIconEditHeightPaint() const{ return pIconEditHeightPaint; }
	inline igdeIcon *GetIconEditMaskPaint() const{ return pIconEditMaskPaint; }
	inline igdeIcon *GetIconEditVisibilityPaint() const{ return pIconEditVisibilityPaint; }
	inline igdeIcon *GetIconEditLockAxisX() const{ return pIconEditLockAxisX; }
	inline igdeIcon *GetIconEditLockAxisY() const{ return pIconEditLockAxisY; }
	inline igdeIcon *GetIconEditLockAxisZ() const{ return pIconEditLockAxisZ; }
	inline const igdeIcon::Ref &GetIconEditLockAxisFlip() const{ return pIconEditLockAxisFlip; }
	inline igdeIcon *GetIconEditUseLocal() const{ return pIconEditUseLocal; }
	inline igdeIcon *GetIconEditSnap() const{ return pIconEditSnap; }
	inline igdeIcon *GetIconViewFullBrightOn() const{ return pIconViewFullBrightOn; }
	inline igdeIcon *GetIconViewFullBrightOff() const{ return pIconViewFullBrightOff; }
	inline igdeIcon *GetIconViewMuteSoundOn() const{ return pIconViewMuteSoundOn; }
	inline igdeIcon *GetIconViewMuteSoundOff() const{ return pIconViewMuteSoundOff; }
	inline igdeIcon *GetIconObjectLightToggle() const{ return pIconObjectLightToggle; }
	
	
	
	/** Actions. */
	inline igdeAction *GetActionObjectSubclassAsEclass() const{ return pActionObjectSubclassAsEclass; }
	inline igdeAction *GetActionObjectRotateL45() const{ return pActionObjectRotateL45; }
	inline igdeAction *GetActionObjectRotateL90() const{ return pActionObjectRotateL90; }
	inline igdeAction *GetActionObjectRotateR45() const{ return pActionObjectRotateR45; }
	inline igdeAction *GetActionObjectRotateR90() const{ return pActionObjectRotateR90; }
	inline igdeAction *GetActionObjectRotate180() const{ return pActionObjectRotate180; }
	inline igdeAction *GetActionObjectRotateRandom() const{ return pActionObjectRotateRandom; }
	inline igdeAction *GetActionObjectDropToGround() const{ return pActionObjectDropToGround; }
	inline igdeAction *GetActionObjectSnapToGrid() const{ return pActionObjectSnapToGrid; }
	inline igdeAction *GetActionObjectCopyPositionX() const{ return pActionObjectCopyPositionX; }
	inline igdeAction *GetActionObjectCopyPositionY() const{ return pActionObjectCopyPositionY; }
	inline igdeAction *GetActionObjectCopyPositionZ() const{ return pActionObjectCopyPositionZ; }
	inline igdeAction *GetActionObjectCopyPositionXZ() const{ return pActionObjectCopyPositionXZ; }
	inline igdeAction *GetActionObjectCopyPositionXYZ() const{ return pActionObjectCopyPositionXYZ; }
	inline igdeAction *GetActionObjectCopyRotationX() const{ return pActionObjectCopyRotationX; }
	inline igdeAction *GetActionObjectCopyRotationY() const{ return pActionObjectCopyRotationY; }
	inline igdeAction *GetActionObjectCopyRotationZ() const{ return pActionObjectCopyRotationZ; }
	inline igdeAction *GetActionObjectCopyRotationXYZ() const{ return pActionObjectCopyRotationXYZ; }
	inline igdeAction *GetActionObjectCopyScaleX() const{ return pActionObjectCopyScaleX; }
	inline igdeAction *GetActionObjectCopyScaleY() const{ return pActionObjectCopyScaleY; }
	inline igdeAction *GetActionObjectCopyScaleZ() const{ return pActionObjectCopyScaleZ; }
	inline igdeAction *GetActionObjectCopyScaleXYZ() const{ return pActionObjectCopyScaleXYZ; }
	
	
	
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
	void pCreateToolBarObject();
	void pCreateToolBarDecal();
	void pCreateMenu();
	void pCreateMenuFile(igdeMenuCascade &menu);
	void pCreateMenuEdit(igdeMenuCascade &menu);
	void pCreateMenuObject(igdeMenuCascade &menu);
	void pCreateMenuDecal(igdeMenuCascade &menu);
	void pCreateMenuView(igdeMenuCascade &menu);
	
	void pUpdateLoading();
};

#endif
