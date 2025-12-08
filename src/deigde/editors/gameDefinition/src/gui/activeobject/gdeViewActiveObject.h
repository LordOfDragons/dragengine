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

#ifndef _GDEVIEWACTIVEOBJECT_H_
#define _GDEVIEWACTIVEOBJECT_H_

#include <deigde/gui/igdeViewRenderWindow.h>
#include <deigde/gui/event/igdeMouseCameraListener.h>

#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/particle/deParticleEmitterInstance.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/sky/deSkyInstance.h>

class gdeWindowMain;
class gdeViewActiveObjectListener;

class gdeGameDefinition;
class gdeObjectClass;
class gdeOCBillboard;
class gdeOCComponent;
class gdeOCComponentTexture;
class gdeOCCamera;
class gdeOCLight;
class gdeOCEnvMapProbe;
class gdeOCNavigationSpace;
class gdeOCNavigationBlocker;
class gdeOCParticleEmitter;
class gdeOCForceField;
class gdeOCSnapPoint;
class gdeOCSpeaker;
class gdeOCWorld;

class igdeCamera;
class igdeWDebugDrawerShape;

class deCollider;



/**
 * \brief Active object view.
 * 
 * \todo Add DebugDrawerShape to show ObjectClass bounding box if an object class is selected.
 */
class gdeViewActiveObject : public igdeViewRenderWindow{
private:
	gdeWindowMain &pWindowMain;
	gdeViewActiveObjectListener *pListener;
	
	gdeGameDefinition::Ref pGameDefinition;
	
	gdeObjectClass::Ref pObjectClass;
	
	deComponent::Ref pPreviewComponent;
	deModel::Ref pPreviewModelBox;
	deSkin::Ref pPreviewSkin;
	deSkyInstance::Ref pPreviewSky;
	deParticleEmitterInstance::Ref pPreviewParticleEmitter;
	
	decObjectList pOCBillboards;
	decObjectList pOCCameras;
	decObjectList pOCComponents;
	decObjectList pOCParticleEmitters;
	decObjectList pOCForceFields;
	decObjectList pOCLights;
	decObjectList pOCEnvMapProbes;
	decObjectList pOCNavSpaces;
	decObjectList pOCNavBlockers;
	decObjectList pOCSnapPoints;
	decObjectList pOCSpeakers;
	decObjectList pOCWorlds;
	
	deDebugDrawer::Ref pDebugDrawer;
	
	bool pShowEnvMapProbes;
	bool pShowNavBlockers;
	
	igdeMouseCameraListener::Ref pCameraInteraction;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create view. */
	gdeViewActiveObject(gdeWindowMain &windowMain);
	
protected:
	/** \brief Clean up view. */
	virtual ~gdeViewActiveObject();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Reset view. */
	void ResetView();
	
	
	
	/** \brief Window main. */
	inline gdeWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** \brief Monitored game definition. */
	inline gdeGameDefinition *GetGameDefinition() const{ return pGameDefinition; }
	
	/** \brief Set game definition to monitor. */
	void SetGameDefinition(gdeGameDefinition::Ref gameDefinition);
	
	/** \brief Debug drawer. */
	inline deDebugDrawer *GetDebugDrawer() const{ return pDebugDrawer; }
	
	
	
	/** \brief Game like frame update. */
	virtual void OnFrameUpdate(float elapsed);
	
	/** \brief Widget size changed. */
	virtual void OnResize();
	
	/** \brief Create canvas. */
	virtual void CreateCanvas();
	
	
	
	/** \brief Selected object class or \em NULL. */
	inline gdeObjectClass *GetObjectClass() const{ return pObjectClass; }
	
	/** \brief Set camera position to show active object properly. */
	void ResetCameraPosition();
	
	/** \brief Clear resources. */
	void ClearResources();
	
	/** \brief Init selected object. */
	void InitSelectedObject();
	
	/** \brief Selected sub object changed. */
	void SelectedSubObjectChanged();
	
	
	
	/** \brief Attach component collider or \em NULL. */
	deCollider *GetAttachComponentCollider() const;
	
	
	
	/** \brief Rebuild object class billboard. */
	void RebuildOCBillboards();
	
	/** \brief Rebuild object class billboard. */
	void RebuildOCBillboard(gdeOCBillboard *ocbillboard);
	
	/** \brief Rebuild object class cameras. */
	void RebuildOCCameras();
	
	/** \brief Rebuild object class camera. */
	void RebuildOCCamera(gdeOCCamera *occamera);
	
	/** \brief Rebuild object class components. */
	void RebuildOCComponents();
	
	/** \brief Rebuild object class component. */
	void RebuildOCComponent(gdeOCComponent *occomponent);
	
	/** \brief Update object class component texture. */
	void UpdateOCComponentTexture(gdeOCComponent *occomponent, gdeOCComponentTexture *texture);
	
	/** \brief Rebuild object class lights. */
	void RebuildOCLights();
	
	/** \brief Rebuild object class light. */
	void RebuildOCLight(gdeOCLight *oclight);
	
	/** \brief Rebuild object class environment map probes. */
	void RebuildOCEnvMapProbes();
	
	/** \brief Rebuild object class environment map probe. */
	void RebuildOCEnvMapProbe(gdeOCEnvMapProbe *ocenvMapProbe);
	
	/** \brief Rebuild object class navigation spaces. */
	void RebuildOCNavSpaces();
	
	/** \brief Rebuild object class navigation spaces. */
	void RebuildOCNavSpace(gdeOCNavigationSpace *ocnavspace);
	
	/** \brief Rebuild object class navigation blockers. */
	void RebuildOCNavBlockers();
	
	/** \brief Rebuild object class navigation blockers. */
	void RebuildOCNavBlocker(gdeOCNavigationBlocker *ocnavblocker);
	
	/** \brief Rebuild object class particle emitters. */
	void RebuildOCParticleEmitters();
	
	/** \brief Rebuild object class particle emitter. */
	void RebuildOCParticleEmitter(gdeOCParticleEmitter *ocemitter);
	
	/** \brief Rebuild object class force fields. */
	void RebuildOCForceFields();
	
	/** \brief Rebuild object class force field. */
	void RebuildOCForceField(gdeOCForceField *ocfield);
	
	/** \brief Rebuild object class snap points. */
	void RebuildOCSnapPoints();
	
	/** \brief Rebuild object class snap points. */
	void RebuildOCSnapPoint(gdeOCSnapPoint *ocsnapPoint);
	
	/** \brief Rebuild object class speakers. */
	void RebuildOCSpeakers();
	
	/** \brief Rebuild object class speaker. */
	void RebuildOCSpeaker(gdeOCSpeaker *ocspeaker);
	
	/** \brief Rebuild object class worlds. */
	void RebuildOCWorlds();
	
	/** \brief Rebuild object class worlds. */
	void RebuildOCWorld(gdeOCWorld *ocworld);
	
	
	
	/** \brief Show all environment map probes. */
	inline bool GetShowEnvMapProbes() const{ return pShowEnvMapProbes; }
	
	/** \brief Set show all environment map probes. */
	void SetShowEnvMapProbes(bool show);
	
	/** \brief Show all navigation blockers. */
	inline bool GetShowNavBlockers() const{ return pShowNavBlockers; }
	
	/** \brief Set show all navigation blockers. */
	void SetShowNavBlockers(bool show);
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pUpdateCameraFovRatio();
	
	void pInitObjectClass();
	void pInitSkin();
	void pInitSky();
	void pInitParticleEmitter();
	
	void pInitObjectClassOCs(const gdeObjectClass &objectClass, const decString &propertyPrefix, int filter);
	void pInitOCBillboards(const gdeObjectClass &objectClass, const decString &propertyPrefix);
	void pInitOCCameras(const gdeObjectClass &objectClass, const decString &propertyPrefix);
	void pInitOCComponents(const gdeObjectClass &objectClass, const decString &propertyPrefix);
	void pInitOCLights(const gdeObjectClass &objectClass, const decString &propertyPrefix);
	void pInitOCParticleEmitters(const gdeObjectClass &objectClass, const decString &propertyPrefix);
	void pInitOCForceFields(const gdeObjectClass &objectClass, const decString &propertyPrefix);
	void pInitOCEnvMapProbes(const gdeObjectClass &objectClass, const decString &propertyPrefix);
	void pInitOCNavigationSpaces(const gdeObjectClass &objectClass, const decString &propertyPrefix);
	void pInitOCNavigationBlockers(const gdeObjectClass &objectClass, const decString &propertyPrefix);
	void pInitOCSnapPoints(const gdeObjectClass &objectClass, const decString &propertyPrefix);
	void pInitOCSpeakers(const gdeObjectClass &objectClass, const decString &propertyPrefix);
	void pInitOCWorlds(const gdeObjectClass &objectClass, const decString &propertyPrefix);
	void pAddComponentShadowIgnore();
	void pRebuildOCBillboards(const gdeObjectClass &objectClass, const decString &propertyPrefix);
	void pRebuildOCCameras(const gdeObjectClass &objectClass, const decString &propertyPrefix);
	void pRebuildOCComponents(const gdeObjectClass &objectClass, const decString &propertyPrefix);
	void pRebuildOCLights(const gdeObjectClass &objectClass, const decString &propertyPrefix);
	void pRebuildOCParticleEmitters(const gdeObjectClass &objectClass, const decString &propertyPrefix);
	void pRebuildOCForceFields(const gdeObjectClass &objectClass, const decString &propertyPrefix);
	void pRebuildOCEnvMapProbes(const gdeObjectClass &objectClass, const decString &propertyPrefix);
	void pRebuildOCNavigationSpaces(const gdeObjectClass &objectClass, const decString &propertyPrefix);
	void pRebuildOCNavigationBlockers(const gdeObjectClass &objectClass, const decString &propertyPrefix);
	void pRebuildOCSnapPoints(const gdeObjectClass &objectClass, const decString &propertyPrefix);
	void pRebuildOCSpeakers(const gdeObjectClass &objectClass, const decString &propertyPrefix);
	void pRebuildOCWorlds(const gdeObjectClass &objectClass, const decString &propertyPrefix);
	
	void pOCReattachAllObjects();
	
	void pCenterOnObjectClass(igdeCamera &camera);
	void pCenterOnSkin(igdeCamera &camera);
	void pCenterOnSky(igdeCamera &camera);
	void pCenterOnParticleEmitter(igdeCamera &camera);
	void pCenterOnBox(igdeCamera &camera, const decVector &minExtend, const decVector &maxExtend);
};

#endif
