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

#ifndef _GDEWINDOWMAIN_H_
#define _GDEWINDOWMAIN_H_

#include <deigde/clipboard/igdeClipboard.h>
#include <deigde/gui/igdeEditorWindow.h>
#include <deigde/gui/igdeToolBarReference.h>
#include <deigde/gui/event/igdeActionReference.h>
#include <deigde/gui/event/igdeActionUndoReference.h>
#include <deigde/gui/event/igdeActionRedoReference.h>
#include <deigde/gui/resources/igdeIconReference.h>

class gdeMACategory;
class gdeMAParticleEmitter;
class gdeMASkin;
class gdeMASky;
class gdeMAObjectClass;
class gdeMAOCBillboard;
class gdeMAOCCamera;
class gdeMAOCComponent;
class gdeMAOCEnvMapProbe;
class gdeMAOCLight;
class gdeMAOCNavBlocker;
class gdeMAOCNavSpace;
class gdeMAOCParticleEmitter;
class gdeMAOCSnapPoint;
class gdeMAOCSpeaker;

class gdeConfiguration;
class gdeGameDefinition;
class gdeIGDEModule;
class gdeLoadSaveSystem;
class gdeViewActiveObject;
class gdeWindowMainListener;
class gdeWindowProperties;

class igdeStepableTask;
class decStringList;



/**
 * \brief Main editor window.
 */
class gdeWindowMain : public igdeEditorWindow{
private:
	gdeWindowMainListener *pListener;
	
	igdeIconReference pIconShowEnvMapProbes;
	igdeIconReference pIconShowNavBlockers;
	
	igdeActionReference pActionGDNew;
	igdeActionReference pActionGDOpen;
	igdeActionReference pActionGDOpenProject;
	igdeActionReference pActionGDSave;
	igdeActionReference pActionGDSaveAs;
	
	igdeActionUndoReference pActionEditUndo;
	igdeActionRedoReference pActionEditRedo;
	igdeActionReference pActionEditCut;
	igdeActionReference pActionEditCopy;
	igdeActionReference pActionEditPaste;
	igdeActionReference pActionEditFind;
	
	igdeActionReference pActionViewShowEnvMapProbes;
	igdeActionReference pActionViewShowNavBlockers;
	
	igdeActionReference pActionCategoryAdd;
	igdeActionReference pActionCategoryRemove;
	igdeActionReference pActionCategoryObjectClassAdd;
	igdeActionReference pActionCategoryParticleEmitterAdd;
	igdeActionReference pActionCategorySkinAdd;
	igdeActionReference pActionCategorySkyAdd;
	
	igdeActionReference pActionObjectClassAdd;
	igdeActionReference pActionObjectClassRemove;
	igdeActionReference pActionObjectClassCopy;
	igdeActionReference pActionObjectClassCut;
	igdeActionReference pActionObjectClassPaste;
	igdeActionReference pActionObjectClassExportXmlEClass;
	igdeActionReference pActionObjectClassDuplicate;
	igdeActionReference pActionObjectClassSubclass;
	
	igdeActionReference pActionOCBillboardAdd;
	igdeActionReference pActionOCBillboardRemove;
	igdeActionReference pActionOCBillboardCopy;
	igdeActionReference pActionOCBillboardCut;
	igdeActionReference pActionOCBillboardPaste;
	
	igdeActionReference pActionOCCameraAdd;
	igdeActionReference pActionOCCameraRemove;
	igdeActionReference pActionOCCameraCopy;
	igdeActionReference pActionOCCameraCut;
	igdeActionReference pActionOCCameraPaste;
	
	igdeActionReference pActionOCComponentAdd;
	igdeActionReference pActionOCComponentRemove;
	igdeActionReference pActionOCComponentCopy;
	igdeActionReference pActionOCComponentCut;
	igdeActionReference pActionOCComponentPaste;
	
	igdeActionReference pActionOCEnvMapProbeAdd;
	igdeActionReference pActionOCEnvMapProbeRemove;
	igdeActionReference pActionOCEnvMapProbeCopy;
	igdeActionReference pActionOCEnvMapProbeCut;
	igdeActionReference pActionOCEnvMapProbePaste;
	
	igdeActionReference pActionOCLightAdd;
	igdeActionReference pActionOCLightRemove;
	igdeActionReference pActionOCLightCopy;
	igdeActionReference pActionOCLightCut;
	igdeActionReference pActionOCLightPaste;
	
	igdeActionReference pActionOCNavBlockerAdd;
	igdeActionReference pActionOCNavBlockerRemove;
	igdeActionReference pActionOCNavBlockerCopy;
	igdeActionReference pActionOCNavBlockerCut;
	igdeActionReference pActionOCNavBlockerPaste;
	
	igdeActionReference pActionOCNavSpaceAdd;
	igdeActionReference pActionOCNavSpaceRemove;
	igdeActionReference pActionOCNavSpaceCopy;
	igdeActionReference pActionOCNavSpaceCut;
	igdeActionReference pActionOCNavSpacePaste;
	
	igdeActionReference pActionOCParticleEmitterAdd;
	igdeActionReference pActionOCParticleEmitterRemove;
	igdeActionReference pActionOCParticleEmitterCopy;
	igdeActionReference pActionOCParticleEmitterCut;
	igdeActionReference pActionOCParticleEmitterPaste;
	
	igdeActionReference pActionOCForceFieldAdd;
	igdeActionReference pActionOCForceFieldRemove;
	igdeActionReference pActionOCForceFieldCopy;
	igdeActionReference pActionOCForceFieldCut;
	igdeActionReference pActionOCForceFieldPaste;
	
	igdeActionReference pActionOCSnapPointAdd;
	igdeActionReference pActionOCSnapPointRemove;
	igdeActionReference pActionOCSnapPointCopy;
	igdeActionReference pActionOCSnapPointCut;
	igdeActionReference pActionOCSnapPointPaste;
	
	igdeActionReference pActionOCSpeakerAdd;
	igdeActionReference pActionOCSpeakerRemove;
	igdeActionReference pActionOCSpeakerCopy;
	igdeActionReference pActionOCSpeakerCut;
	igdeActionReference pActionOCSpeakerPaste;
	
	igdeActionReference pActionParticleEmitterAdd;
	igdeActionReference pActionParticleEmitterRemove;
	
	igdeActionReference pActionSkinAdd;
	igdeActionReference pActionSkinRemove;
	
	igdeActionReference pActionSkyAdd;
	igdeActionReference pActionSkyRemove;
	
	igdeToolBarReference pTBFile;
	igdeToolBarReference pTBEdit;
	
	gdeConfiguration *pConfiguration;
	igdeClipboard pClipboard;
	gdeLoadSaveSystem *pLoadSaveSystem;
	
	gdeViewActiveObject *pViewActiveObject;
	gdeWindowProperties *pWindowProperties;
	
	gdeGameDefinition *pActiveGameDefinition;
	
	decString pLastPathGameDef;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create main window. */
	gdeWindowMain( igdeEditorModule &module );
	
protected:
	/** \brief Clean up main window. */
	virtual ~gdeWindowMain();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Icons. */
	inline igdeIcon* GetIconShowEnvMapProbes() const{ return pIconShowEnvMapProbes; }
	inline igdeIcon* GetIconShowNavBlockers() const{ return pIconShowNavBlockers; }
	
	/** \brief Actions. */
	inline igdeAction *GetActionGDNew() const{ return pActionGDNew; }
	inline igdeAction *GetActionGDOpen() const{ return pActionGDOpen; }
	inline igdeAction *GetActionGDOpenProject() const{ return pActionGDOpenProject; }
	inline igdeAction *GetActionGDSave() const{ return pActionGDSave; }
	inline igdeAction *GetActionGDSaveAs() const{ return pActionGDSaveAs; }
	
	inline igdeActionUndo *GetActionEditUndo() const{ return pActionEditUndo; }
	inline igdeActionRedo *GetActionEditRedo() const{ return pActionEditRedo; }
	inline igdeAction *GetActionEditCut() const{ return pActionEditCut; }
	inline igdeAction *GetActionEditCopy() const{ return pActionEditCopy; }
	inline igdeAction *GetActionEditPaste() const{ return pActionEditPaste; }
	inline igdeAction *GetActionEditFind() const{ return pActionEditFind; }
	
	inline igdeAction *GetActionViewShowEnvMapProbes() const{ return pActionViewShowEnvMapProbes; }
	inline igdeAction *GetActionViewShowNavBlockers() const{ return pActionViewShowNavBlockers; }
	
	inline igdeAction *GetActionCategoryAdd() const{ return pActionCategoryAdd; }
	inline igdeAction *GetActionCategoryRemove() const{ return pActionCategoryRemove; }
	inline igdeAction *GetActionCategoryObjectClassAdd() const{ return pActionCategoryObjectClassAdd; }
	inline igdeAction *GetActionCategoryParticleEmitterAdd() const{ return pActionCategoryParticleEmitterAdd; }
	inline igdeAction *GetActionCategorySkinAdd() const{ return pActionCategorySkinAdd; }
	inline igdeAction *GetActionCategorySkyAdd() const{ return pActionCategorySkyAdd; }
	
	inline igdeAction *GetActionObjectClassAdd() const{ return pActionObjectClassAdd; }
	inline igdeAction *GetActionObjectClassRemove() const{ return pActionObjectClassRemove; }
	inline igdeAction *GetActionObjectClassCopy() const{ return pActionObjectClassCopy; }
	inline igdeAction *GetActionObjectClassCut() const{ return pActionObjectClassCut; }
	inline igdeAction *GetActionObjectClassPaste() const{ return pActionObjectClassPaste; }
	inline igdeAction *GetActionObjectClassExportXmlEClass() const{ return pActionObjectClassExportXmlEClass; }
	inline igdeAction *GetActionObjectClassDuplicate() const{ return pActionObjectClassDuplicate; }
	inline igdeAction *GetActionObjectClassSubclass() const{ return pActionObjectClassSubclass; }
	
	inline igdeAction *GetActionOCBillboardAdd() const{ return pActionOCBillboardAdd; }
	inline igdeAction *GetActionOCBillboardRemove() const{ return pActionOCBillboardRemove; }
	inline igdeAction *GetActionOCBillboardCopy() const{ return pActionOCBillboardCopy; }
	inline igdeAction *GetActionOCBillboardCut() const{ return pActionOCBillboardCut; }
	inline igdeAction *GetActionOCBillboardPaste() const{ return pActionOCBillboardPaste; }
	
	inline igdeAction *GetActionOCCameraAdd() const{ return pActionOCCameraAdd; }
	inline igdeAction *GetActionOCCameraRemove() const{ return pActionOCCameraRemove; }
	inline igdeAction *GetActionOCCameraCopy() const{ return pActionOCCameraCopy; }
	inline igdeAction *GetActionOCCameraCut() const{ return pActionOCCameraCut; }
	inline igdeAction *GetActionOCCameraPaste() const{ return pActionOCCameraPaste; }
	
	inline igdeAction *GetActionOCComponentAdd() const{ return pActionOCComponentAdd; }
	inline igdeAction *GetActionOCComponentRemove() const{ return pActionOCComponentRemove; }
	inline igdeAction *GetActionOCComponentCopy() const{ return pActionOCComponentCopy; }
	inline igdeAction *GetActionOCComponentCut() const{ return pActionOCComponentCut; }
	inline igdeAction *GetActionOCComponentPaste() const{ return pActionOCComponentPaste; }
	
	inline igdeAction *GetActionOCEnvMapProbeAdd() const{ return pActionOCEnvMapProbeAdd; }
	inline igdeAction *GetActionOCEnvMapProbeRemove() const{ return pActionOCEnvMapProbeRemove; }
	inline igdeAction *GetActionOCEnvMapProbeCopy() const{ return pActionOCEnvMapProbeCopy; }
	inline igdeAction *GetActionOCEnvMapProbeCut() const{ return pActionOCEnvMapProbeCut; }
	inline igdeAction *GetActionOCEnvMapProbePaste() const{ return pActionOCEnvMapProbePaste; }
	
	inline igdeAction *GetActionOCLightAdd() const{ return pActionOCLightAdd; }
	inline igdeAction *GetActionOCLightRemove() const{ return pActionOCLightRemove; }
	inline igdeAction *GetActionOCLightCopy() const{ return pActionOCLightCopy; }
	inline igdeAction *GetActionOCLightCut() const{ return pActionOCLightCut; }
	inline igdeAction *GetActionOCLightPaste() const{ return pActionOCLightPaste; }
	
	inline igdeAction *GetActionOCNavBlockerAdd() const{ return pActionOCNavBlockerAdd; }
	inline igdeAction *GetActionOCNavBlockerRemove() const{ return pActionOCNavBlockerRemove; }
	inline igdeAction *GetActionOCNavBlockerCopy() const{ return pActionOCNavBlockerCopy; }
	inline igdeAction *GetActionOCNavBlockerCut() const{ return pActionOCNavBlockerCut; }
	inline igdeAction *GetActionOCNavBlockerPaste() const{ return pActionOCNavBlockerPaste; }
	
	inline igdeAction *GetActionOCNavSpaceAdd() const{ return pActionOCNavSpaceAdd; }
	inline igdeAction *GetActionOCNavSpaceRemove() const{ return pActionOCNavSpaceRemove; }
	inline igdeAction *GetActionOCNavSpaceCopy() const{ return pActionOCNavSpaceCopy; }
	inline igdeAction *GetActionOCNavSpaceCut() const{ return pActionOCNavSpaceCut; }
	inline igdeAction *GetActionOCNavSpacePaste() const{ return pActionOCNavSpacePaste; }
	
	inline igdeAction *GetActionOCParticleEmitterAdd() const{ return pActionOCParticleEmitterAdd; }
	inline igdeAction *GetActionOCParticleEmitterRemove() const{ return pActionOCParticleEmitterRemove; }
	inline igdeAction *GetActionOCParticleEmitterCopy() const{ return pActionOCParticleEmitterCopy; }
	inline igdeAction *GetActionOCParticleEmitterCut() const{ return pActionOCParticleEmitterCut; }
	inline igdeAction *GetActionOCParticleEmitterPaste() const{ return pActionOCParticleEmitterPaste; }
	
	inline igdeAction *GetActionOCForceFieldAdd() const{ return pActionOCForceFieldAdd; }
	inline igdeAction *GetActionOCForceFieldRemove() const{ return pActionOCForceFieldRemove; }
	inline igdeAction *GetActionOCForceFieldCopy() const{ return pActionOCForceFieldCopy; }
	inline igdeAction *GetActionOCForceFieldCut() const{ return pActionOCForceFieldCut; }
	inline igdeAction *GetActionOCForceFieldPaste() const{ return pActionOCForceFieldPaste; }
	
	inline igdeAction *GetActionOCSnapPointAdd() const{ return pActionOCSnapPointAdd; }
	inline igdeAction *GetActionOCSnapPointRemove() const{ return pActionOCSnapPointRemove; }
	inline igdeAction *GetActionOCSnapPointCopy() const{ return pActionOCSnapPointCopy; }
	inline igdeAction *GetActionOCSnapPointCut() const{ return pActionOCSnapPointCut; }
	inline igdeAction *GetActionOCSnapPointPaste() const{ return pActionOCSnapPointPaste; }
	
	inline igdeAction *GetActionOCSpeakerAdd() const{ return pActionOCSpeakerAdd; }
	inline igdeAction *GetActionOCSpeakerRemove() const{ return pActionOCSpeakerRemove; }
	inline igdeAction *GetActionOCSpeakerCopy() const{ return pActionOCSpeakerCopy; }
	inline igdeAction *GetActionOCSpeakerCut() const{ return pActionOCSpeakerCut; }
	inline igdeAction *GetActionOCSpeakerPaste() const{ return pActionOCSpeakerPaste; }
	
	inline igdeAction *GetActionParticleEmitterAdd() const{ return pActionParticleEmitterAdd; }
	inline igdeAction *GetActionParticleEmitterRemove() const{ return pActionParticleEmitterRemove; }
	
	inline igdeAction *GetActionSkinAdd() const{ return pActionSkinAdd; }
	inline igdeAction *GetActionSkinRemove() const{ return pActionSkinRemove; }
	
	inline igdeAction *GetActionSkyAdd() const{ return pActionSkyAdd; }
	inline igdeAction *GetActionSkyRemove() const{ return pActionSkyRemove; }
	
	
	
	/** \brief Ask user if it is okay to quit the application. */
	bool QuitRequest();
	
	/** \brief Reset views. */
	void ResetViews();
	
	/** \brief Configuration. */
	inline gdeConfiguration &GetConfiguration() const{ return *pConfiguration; }
	
	/** \brief Clipboard. */
	inline igdeClipboard &GetClipboard(){ return pClipboard; }
	
	/** \brief Load save system. */
	inline gdeLoadSaveSystem &GetLoadSaveSystem() const{ return *pLoadSaveSystem; }
	
	/** \brief View active object. */
	inline gdeViewActiveObject &GetViewActiveObject() const{ return *pViewActiveObject; }
	
	/** \brief Window properties. */
	inline gdeWindowProperties &GetWindowProperties() const{ return *pWindowProperties; }
	
	
	
	/** \brief Active game definition. */
	inline gdeGameDefinition *GetActiveGameDefinition() const{ return pActiveGameDefinition; }
	
	/** \brief Set active game definition. */
	void SetActiveGameDefinition( gdeGameDefinition *gameDefinition );
	
	/** \brief Create new game definition. */
	void CreateNewGameDefinition();
	
	/** \brief Load game definition project. */
	void LoadGameProject( bool silentErrors );
	
	/** \brief Save game definition to file. */
	void SaveGameDefinition( const char *filename );
	
	
	
	/** \brief Last path game definition. */
	inline const decString &GetLastPathGameDef() const{ return pLastPathGameDef; }
	
	/** \brief Set last path game definition. */
	void SetLastPathGameDef( const char *path );
	
	
	
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
	void pCreateMenuGameDef( igdeMenuCascade &menu );
	void pCreateMenuEdit( igdeMenuCascade &menu );
	void pCreateMenuView( igdeMenuCascade &menu );
};

#endif
