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

#include "../gamedef/gdeGameDefinition.h"
#include "activeobject/gdeViewActiveObject.h"
#include "gdeWindowMainListener.h"
#include "properties/gdeWindowProperties.h"

#include <deigde/clipboard/igdeClipboard.h>
#include <deigde/gui/igdeEditorWindow.h>
#include <deigde/gui/igdeToolBar.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionUndo.h>
#include <deigde/gui/event/igdeActionRedo.h>
#include <deigde/gui/resources/igdeIcon.h>

#include <dragengine/common/string/decStringList.h>

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
class gdeMAOCWorld;

class gdeConfiguration;
class gdeIGDEModule;
class gdeLoadSaveSystem;

class igdeStepableTask;



/**
 * \brief Main editor window.
 */
class gdeWindowMain : public igdeEditorWindow{
public:
	typedef deTObjectReference<gdeWindowMain> Ref;
	
	
private:
	gdeWindowMainListener::Ref pListener;
	
	igdeIcon::Ref pIconShowEnvMapProbes;
	igdeIcon::Ref pIconShowNavBlockers;
	
	igdeAction::Ref pActionGDNew;
	igdeAction::Ref pActionGDOpen;
	igdeAction::Ref pActionGDOpenProject;
	igdeAction::Ref pActionGDSave;
	igdeAction::Ref pActionGDSaveAs;
	
	igdeActionUndo::Ref pActionEditUndo;
	igdeActionRedo::Ref pActionEditRedo;
	igdeAction::Ref pActionEditCut;
	igdeAction::Ref pActionEditCopy;
	igdeAction::Ref pActionEditPaste;
	igdeAction::Ref pActionEditFind;
	
	igdeAction::Ref pActionViewShowEnvMapProbes;
	igdeAction::Ref pActionViewShowNavBlockers;
	
	igdeAction::Ref pActionCategoryAdd;
	igdeAction::Ref pActionCategoryRemove;
	igdeAction::Ref pActionCategoryObjectClassAdd;
	igdeAction::Ref pActionCategoryParticleEmitterAdd;
	igdeAction::Ref pActionCategorySkinAdd;
	igdeAction::Ref pActionCategorySkyAdd;
	
	igdeAction::Ref pActionObjectClassAdd;
	igdeAction::Ref pActionObjectClassRemove;
	igdeAction::Ref pActionObjectClassCopy;
	igdeAction::Ref pActionObjectClassCut;
	igdeAction::Ref pActionObjectClassPaste;
	igdeAction::Ref pActionObjectClassExportXmlEClass;
	igdeAction::Ref pActionObjectClassDuplicate;
	igdeAction::Ref pActionObjectClassSubclass;
	
	igdeAction::Ref pActionOCBillboardAdd;
	igdeAction::Ref pActionOCBillboardRemove;
	igdeAction::Ref pActionOCBillboardCopy;
	igdeAction::Ref pActionOCBillboardCut;
	igdeAction::Ref pActionOCBillboardPaste;
	
	igdeAction::Ref pActionOCCameraAdd;
	igdeAction::Ref pActionOCCameraRemove;
	igdeAction::Ref pActionOCCameraCopy;
	igdeAction::Ref pActionOCCameraCut;
	igdeAction::Ref pActionOCCameraPaste;
	
	igdeAction::Ref pActionOCComponentAdd;
	igdeAction::Ref pActionOCComponentRemove;
	igdeAction::Ref pActionOCComponentCopy;
	igdeAction::Ref pActionOCComponentCut;
	igdeAction::Ref pActionOCComponentPaste;
	
	igdeAction::Ref pActionOCEnvMapProbeAdd;
	igdeAction::Ref pActionOCEnvMapProbeRemove;
	igdeAction::Ref pActionOCEnvMapProbeCopy;
	igdeAction::Ref pActionOCEnvMapProbeCut;
	igdeAction::Ref pActionOCEnvMapProbePaste;
	
	igdeAction::Ref pActionOCLightAdd;
	igdeAction::Ref pActionOCLightRemove;
	igdeAction::Ref pActionOCLightCopy;
	igdeAction::Ref pActionOCLightCut;
	igdeAction::Ref pActionOCLightPaste;
	
	igdeAction::Ref pActionOCNavBlockerAdd;
	igdeAction::Ref pActionOCNavBlockerRemove;
	igdeAction::Ref pActionOCNavBlockerCopy;
	igdeAction::Ref pActionOCNavBlockerCut;
	igdeAction::Ref pActionOCNavBlockerPaste;
	
	igdeAction::Ref pActionOCNavSpaceAdd;
	igdeAction::Ref pActionOCNavSpaceRemove;
	igdeAction::Ref pActionOCNavSpaceCopy;
	igdeAction::Ref pActionOCNavSpaceCut;
	igdeAction::Ref pActionOCNavSpacePaste;
	
	igdeAction::Ref pActionOCParticleEmitterAdd;
	igdeAction::Ref pActionOCParticleEmitterRemove;
	igdeAction::Ref pActionOCParticleEmitterCopy;
	igdeAction::Ref pActionOCParticleEmitterCut;
	igdeAction::Ref pActionOCParticleEmitterPaste;
	
	igdeAction::Ref pActionOCForceFieldAdd;
	igdeAction::Ref pActionOCForceFieldRemove;
	igdeAction::Ref pActionOCForceFieldCopy;
	igdeAction::Ref pActionOCForceFieldCut;
	igdeAction::Ref pActionOCForceFieldPaste;
	
	igdeAction::Ref pActionOCSnapPointAdd;
	igdeAction::Ref pActionOCSnapPointRemove;
	igdeAction::Ref pActionOCSnapPointCopy;
	igdeAction::Ref pActionOCSnapPointCut;
	igdeAction::Ref pActionOCSnapPointPaste;
	
	igdeAction::Ref pActionOCSpeakerAdd;
	igdeAction::Ref pActionOCSpeakerRemove;
	igdeAction::Ref pActionOCSpeakerCopy;
	igdeAction::Ref pActionOCSpeakerCut;
	igdeAction::Ref pActionOCSpeakerPaste;
	
	igdeAction::Ref pActionOCWorldAdd;
	igdeAction::Ref pActionOCWorldRemove;
	igdeAction::Ref pActionOCWorldCopy;
	igdeAction::Ref pActionOCWorldCut;
	igdeAction::Ref pActionOCWorldPaste;
	
	igdeAction::Ref pActionParticleEmitterAdd;
	igdeAction::Ref pActionParticleEmitterRemove;
	
	igdeAction::Ref pActionSkinAdd;
	igdeAction::Ref pActionSkinRemove;
	
	igdeAction::Ref pActionSkyAdd;
	igdeAction::Ref pActionSkyRemove;
	
	igdeToolBar::Ref pTBFile;
	igdeToolBar::Ref pTBEdit;
	
	gdeConfiguration *pConfiguration;
	igdeClipboard pClipboard;
	gdeLoadSaveSystem *pLoadSaveSystem;
	
	gdeViewActiveObject::Ref pViewActiveObject;
	gdeWindowProperties::Ref pWindowProperties;
	
	gdeGameDefinition::Ref pActiveGameDefinition;
	
	decString pLastPathGameDef;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create main window. */
	gdeWindowMain(igdeEditorModule &module);
	
protected:
	/** \brief Clean up main window. */
	virtual ~gdeWindowMain();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Icons. */
	inline const igdeIcon::Ref &GetIconShowEnvMapProbes() const{ return pIconShowEnvMapProbes; }
	inline const igdeIcon::Ref &GetIconShowNavBlockers() const{ return pIconShowNavBlockers; }
	
	/** \brief Actions. */
	inline const igdeAction::Ref &GetActionGDNew() const{ return pActionGDNew; }
	inline const igdeAction::Ref &GetActionGDOpen() const{ return pActionGDOpen; }
	inline const igdeAction::Ref &GetActionGDOpenProject() const{ return pActionGDOpenProject; }
	inline const igdeAction::Ref &GetActionGDSave() const{ return pActionGDSave; }
	inline const igdeAction::Ref &GetActionGDSaveAs() const{ return pActionGDSaveAs; }
	
	inline const igdeActionUndo::Ref &GetActionEditUndo() const{ return pActionEditUndo; }
	inline const igdeActionRedo::Ref &GetActionEditRedo() const{ return pActionEditRedo; }
	inline const igdeAction::Ref &GetActionEditCut() const{ return pActionEditCut; }
	inline const igdeAction::Ref &GetActionEditCopy() const{ return pActionEditCopy; }
	inline const igdeAction::Ref &GetActionEditPaste() const{ return pActionEditPaste; }
	inline const igdeAction::Ref &GetActionEditFind() const{ return pActionEditFind; }
	
	inline const igdeAction::Ref &GetActionViewShowEnvMapProbes() const{ return pActionViewShowEnvMapProbes; }
	inline const igdeAction::Ref &GetActionViewShowNavBlockers() const{ return pActionViewShowNavBlockers; }
	
	inline const igdeAction::Ref &GetActionCategoryAdd() const{ return pActionCategoryAdd; }
	inline const igdeAction::Ref &GetActionCategoryRemove() const{ return pActionCategoryRemove; }
	inline const igdeAction::Ref &GetActionCategoryObjectClassAdd() const{ return pActionCategoryObjectClassAdd; }
	inline const igdeAction::Ref &GetActionCategoryParticleEmitterAdd() const{ return pActionCategoryParticleEmitterAdd; }
	inline const igdeAction::Ref &GetActionCategorySkinAdd() const{ return pActionCategorySkinAdd; }
	inline const igdeAction::Ref &GetActionCategorySkyAdd() const{ return pActionCategorySkyAdd; }
	
	inline const igdeAction::Ref &GetActionObjectClassAdd() const{ return pActionObjectClassAdd; }
	inline const igdeAction::Ref &GetActionObjectClassRemove() const{ return pActionObjectClassRemove; }
	inline const igdeAction::Ref &GetActionObjectClassCopy() const{ return pActionObjectClassCopy; }
	inline const igdeAction::Ref &GetActionObjectClassCut() const{ return pActionObjectClassCut; }
	inline const igdeAction::Ref &GetActionObjectClassPaste() const{ return pActionObjectClassPaste; }
	inline const igdeAction::Ref &GetActionObjectClassExportXmlEClass() const{ return pActionObjectClassExportXmlEClass; }
	inline const igdeAction::Ref &GetActionObjectClassDuplicate() const{ return pActionObjectClassDuplicate; }
	inline const igdeAction::Ref &GetActionObjectClassSubclass() const{ return pActionObjectClassSubclass; }
	
	inline const igdeAction::Ref &GetActionOCBillboardAdd() const{ return pActionOCBillboardAdd; }
	inline const igdeAction::Ref &GetActionOCBillboardRemove() const{ return pActionOCBillboardRemove; }
	inline const igdeAction::Ref &GetActionOCBillboardCopy() const{ return pActionOCBillboardCopy; }
	inline const igdeAction::Ref &GetActionOCBillboardCut() const{ return pActionOCBillboardCut; }
	inline const igdeAction::Ref &GetActionOCBillboardPaste() const{ return pActionOCBillboardPaste; }
	
	inline const igdeAction::Ref &GetActionOCCameraAdd() const{ return pActionOCCameraAdd; }
	inline const igdeAction::Ref &GetActionOCCameraRemove() const{ return pActionOCCameraRemove; }
	inline const igdeAction::Ref &GetActionOCCameraCopy() const{ return pActionOCCameraCopy; }
	inline const igdeAction::Ref &GetActionOCCameraCut() const{ return pActionOCCameraCut; }
	inline const igdeAction::Ref &GetActionOCCameraPaste() const{ return pActionOCCameraPaste; }
	
	inline const igdeAction::Ref &GetActionOCComponentAdd() const{ return pActionOCComponentAdd; }
	inline const igdeAction::Ref &GetActionOCComponentRemove() const{ return pActionOCComponentRemove; }
	inline const igdeAction::Ref &GetActionOCComponentCopy() const{ return pActionOCComponentCopy; }
	inline const igdeAction::Ref &GetActionOCComponentCut() const{ return pActionOCComponentCut; }
	inline const igdeAction::Ref &GetActionOCComponentPaste() const{ return pActionOCComponentPaste; }
	
	inline const igdeAction::Ref &GetActionOCEnvMapProbeAdd() const{ return pActionOCEnvMapProbeAdd; }
	inline const igdeAction::Ref &GetActionOCEnvMapProbeRemove() const{ return pActionOCEnvMapProbeRemove; }
	inline const igdeAction::Ref &GetActionOCEnvMapProbeCopy() const{ return pActionOCEnvMapProbeCopy; }
	inline const igdeAction::Ref &GetActionOCEnvMapProbeCut() const{ return pActionOCEnvMapProbeCut; }
	inline const igdeAction::Ref &GetActionOCEnvMapProbePaste() const{ return pActionOCEnvMapProbePaste; }
	
	inline const igdeAction::Ref &GetActionOCLightAdd() const{ return pActionOCLightAdd; }
	inline const igdeAction::Ref &GetActionOCLightRemove() const{ return pActionOCLightRemove; }
	inline const igdeAction::Ref &GetActionOCLightCopy() const{ return pActionOCLightCopy; }
	inline const igdeAction::Ref &GetActionOCLightCut() const{ return pActionOCLightCut; }
	inline const igdeAction::Ref &GetActionOCLightPaste() const{ return pActionOCLightPaste; }
	
	inline const igdeAction::Ref &GetActionOCNavBlockerAdd() const{ return pActionOCNavBlockerAdd; }
	inline const igdeAction::Ref &GetActionOCNavBlockerRemove() const{ return pActionOCNavBlockerRemove; }
	inline const igdeAction::Ref &GetActionOCNavBlockerCopy() const{ return pActionOCNavBlockerCopy; }
	inline const igdeAction::Ref &GetActionOCNavBlockerCut() const{ return pActionOCNavBlockerCut; }
	inline const igdeAction::Ref &GetActionOCNavBlockerPaste() const{ return pActionOCNavBlockerPaste; }
	
	inline const igdeAction::Ref &GetActionOCNavSpaceAdd() const{ return pActionOCNavSpaceAdd; }
	inline const igdeAction::Ref &GetActionOCNavSpaceRemove() const{ return pActionOCNavSpaceRemove; }
	inline const igdeAction::Ref &GetActionOCNavSpaceCopy() const{ return pActionOCNavSpaceCopy; }
	inline const igdeAction::Ref &GetActionOCNavSpaceCut() const{ return pActionOCNavSpaceCut; }
	inline const igdeAction::Ref &GetActionOCNavSpacePaste() const{ return pActionOCNavSpacePaste; }
	
	inline const igdeAction::Ref &GetActionOCParticleEmitterAdd() const{ return pActionOCParticleEmitterAdd; }
	inline const igdeAction::Ref &GetActionOCParticleEmitterRemove() const{ return pActionOCParticleEmitterRemove; }
	inline const igdeAction::Ref &GetActionOCParticleEmitterCopy() const{ return pActionOCParticleEmitterCopy; }
	inline const igdeAction::Ref &GetActionOCParticleEmitterCut() const{ return pActionOCParticleEmitterCut; }
	inline const igdeAction::Ref &GetActionOCParticleEmitterPaste() const{ return pActionOCParticleEmitterPaste; }
	
	inline const igdeAction::Ref &GetActionOCForceFieldAdd() const{ return pActionOCForceFieldAdd; }
	inline const igdeAction::Ref &GetActionOCForceFieldRemove() const{ return pActionOCForceFieldRemove; }
	inline const igdeAction::Ref &GetActionOCForceFieldCopy() const{ return pActionOCForceFieldCopy; }
	inline const igdeAction::Ref &GetActionOCForceFieldCut() const{ return pActionOCForceFieldCut; }
	inline const igdeAction::Ref &GetActionOCForceFieldPaste() const{ return pActionOCForceFieldPaste; }
	
	inline const igdeAction::Ref &GetActionOCSnapPointAdd() const{ return pActionOCSnapPointAdd; }
	inline const igdeAction::Ref &GetActionOCSnapPointRemove() const{ return pActionOCSnapPointRemove; }
	inline const igdeAction::Ref &GetActionOCSnapPointCopy() const{ return pActionOCSnapPointCopy; }
	inline const igdeAction::Ref &GetActionOCSnapPointCut() const{ return pActionOCSnapPointCut; }
	inline const igdeAction::Ref &GetActionOCSnapPointPaste() const{ return pActionOCSnapPointPaste; }
	
	inline const igdeAction::Ref &GetActionOCSpeakerAdd() const{ return pActionOCSpeakerAdd; }
	inline const igdeAction::Ref &GetActionOCSpeakerRemove() const{ return pActionOCSpeakerRemove; }
	inline const igdeAction::Ref &GetActionOCSpeakerCopy() const{ return pActionOCSpeakerCopy; }
	inline const igdeAction::Ref &GetActionOCSpeakerCut() const{ return pActionOCSpeakerCut; }
	inline const igdeAction::Ref &GetActionOCSpeakerPaste() const{ return pActionOCSpeakerPaste; }
	
	inline const igdeAction::Ref &GetActionOCWorldAdd() const{ return pActionOCWorldAdd; }
	inline const igdeAction::Ref &GetActionOCWorldRemove() const{ return pActionOCWorldRemove; }
	inline const igdeAction::Ref &GetActionOCWorldCopy() const{ return pActionOCWorldCopy; }
	inline const igdeAction::Ref &GetActionOCWorldCut() const{ return pActionOCWorldCut; }
	inline const igdeAction::Ref &GetActionOCWorldPaste() const{ return pActionOCWorldPaste; }
	
	inline const igdeAction::Ref &GetActionParticleEmitterAdd() const{ return pActionParticleEmitterAdd; }
	inline const igdeAction::Ref &GetActionParticleEmitterRemove() const{ return pActionParticleEmitterRemove; }
	
	inline const igdeAction::Ref &GetActionSkinAdd() const{ return pActionSkinAdd; }
	inline const igdeAction::Ref &GetActionSkinRemove() const{ return pActionSkinRemove; }
	
	inline const igdeAction::Ref &GetActionSkyAdd() const{ return pActionSkyAdd; }
	inline const igdeAction::Ref &GetActionSkyRemove() const{ return pActionSkyRemove; }
	
	
	
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
	inline gdeViewActiveObject &GetViewActiveObject() const{ return pViewActiveObject; }
	
	/** \brief Window properties. */
	inline gdeWindowProperties &GetWindowProperties() const{ return pWindowProperties; }
	
	
	
	/** \brief Active game definition. */
	inline const gdeGameDefinition::Ref &GetActiveGameDefinition() const{ return pActiveGameDefinition; }
	
	/** \brief Set active game definition. */
	void SetActiveGameDefinition(gdeGameDefinition *gameDefinition);
	
	/** \brief Create new game definition. */
	void CreateNewGameDefinition();
	
	/** \brief Load game definition project. */
	void LoadGameProject(bool silentErrors);
	
	/** \brief Save game definition to file. */
	void SaveGameDefinition(const char *filename);
	
	
	
	/** \brief Last path game definition. */
	inline const decString &GetLastPathGameDef() const{ return pLastPathGameDef; }
	
	/** \brief Set last path game definition. */
	void SetLastPathGameDef(const char *path);
	
	
	
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
	 * does not need any update \em nullptr can be returned. The caller delets the task once
	 * finished processing.
	 * 
	 * The default implementation returns \em nullptr.
	 */
	virtual igdeStepableTask::Ref OnGameDefinitionChanged();
	/*@}*/
	
	
	
private:
	void pLoadIcons();
	void pCreateActions();
	void pCreateToolBarFile();
	void pCreateToolBarEdit();
	void pCreateMenu();
	void pCreateMenuGameDef(igdeMenuCascade &menu);
	void pCreateMenuEdit(igdeMenuCascade &menu);
	void pCreateMenuView(igdeMenuCascade &menu);
};

#endif
