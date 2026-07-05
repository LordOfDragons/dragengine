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

#ifndef _AEANIMATOR_H_
#define _AEANIMATOR_H_

#include "aeCamera.h"
#include "attachment/aeAttachment.h"
#include "controller/aeController.h"
#include "link/aeLink.h"
#include "rule/aeRule.h"

#include <deigde/editableentity/igdeEditableEntity.h>
#include <deigde/gui/wrapper/igdeWObject.h>
#include <deigde/gui/wrapper/igdeWSky.h>
#include <deigde/meta/igdeMetaContext.h>
#include <deigde/meta/property/igdeMetaPropertyBoolean.h>
#include <deigde/meta/property/igdeMetaPropertyContext.h>
#include <deigde/meta/property/igdeMetaPropertyFloat.h>
#include <deigde/meta/property/igdeMetaPropertyInteger.h>
#include <deigde/meta/property/igdeMetaPropertyList.h>
#include <deigde/meta/property/igdeMetaPropertyObject.h>
#include <deigde/meta/property/igdeMetaPropertyPath.h>
#include <deigde/meta/property/igdeMetaPropertySelection.h>
#include <deigde/meta/property/igdeMetaPropertySet.h>
#include <deigde/meta/property/igdeMetaPropertySliderBoard.h>
#include <deigde/meta/property/igdeMetaPropertyString.h>
#include <deigde/meta/property/igdeMetaPropertyStringSet.h>
#include <deigde/meta/property/igdeMetaPropertyTreeList.h>
#include <deigde/meta/property/igdeMetaPropertyVector.h>
#include <deigde/utils/igdeUniqueNameGenerator.h>

#include <dragengine/deObject.h>
#include <dragengine/deTWeakObjectReference.h>
#include <dragengine/deTUniqueReference.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decStringSet.h>
#include <dragengine/common/collection/decTOrderedSet.h>
#include <dragengine/common/collection/decTUniqueList.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/light/deLight.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/collider/deColliderComponent.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/deAnimatorInstance.h>


// predefinitions
class igdeEnvironment;

class aeAnimatorLocomotion;
class aeWakeboard;
class aeUndoSystem;
class aeSubAnimator;
class aeWindowMain;

class igdeGameDefinition;
class igdeWCoordSysArrows;

class deEngine;
class deColliderVolume;
class deAnimatorRule;
class deLogger;



/**
 * Animator.
 *
 * Working object for an animator.
 */
class aeAnimator : public igdeEditableEntity{
public:
	using Ref = deTObjectReference<aeAnimator>;
	using WeakRef = deTWeakObjectReference<aeAnimator>;
	using AttachmentSet = decTObjectSet<aeAttachment>;
	
public:
	/** Collision Layers. */
	enum eCollisionLayers{
		/** Non-AI Elements blocked by walls. */
		eclTerrain,
		
		/** Non-AI Elements ignoring walls. */
		eclElements,
		
		/** Proxies. */
		eclProxies,
		
		/** AI Elements ignoring walls. */
		eclAI,
		
		/** Everything that actors can stand upon. */
		eclGround,
		
		/** Gizmos. */
		eclGizmo
	};
	
	using MetaContext = igdeMetaContextType<aeAnimator>;
	static MetaContext::Ref CreateMetaContext(aeWindowMain &windowMain, aeAnimator *animator);
	static MetaContext::Ref CreateMetaContextController(aeWindowMain &windowMain, aeAnimator *animator);
	static MetaContext::Ref CreateMetaContextLink(aeWindowMain &windowMain, aeAnimator *animator);
	static MetaContext::Ref CreateMetaContextRule(aeWindowMain &windowMain, aeAnimator *animator);
	static MetaContext::Ref CreateMetaContextPlayground(aeWindowMain &windowMain, aeAnimator *animator);
	static MetaContext::Ref CreateMetaContextAttachment(aeWindowMain &windowMain, aeAnimator *animator);
	static MetaContext::Ref CreateMetaContextView(aeWindowMain &windowMain, aeAnimator *animator);
	
	template<typename T>
	using MetaProperty = igdeMetaPropertyMCT<T, MetaContext>;
	
	template<typename T>
	using MetaPropertyNoCapture = igdeMetaPropertyMCTNoCapture<T, MetaContext>;
	
private:
	aeWindowMain &pWindowMain;
	MetaContext::Ref pMetaContext;
	MetaContext::Ref pMetaContextController;
	MetaContext::Ref pMetaContextLink;
	MetaContext::Ref pMetaContextRule;
	MetaContext::Ref pMetaContextPlayground;
	MetaContext::Ref pMetaContextAttachment;
	MetaContext::Ref pMetaContextView;
	
	deWorld::Ref pEngWorld;
	
	igdeWSky::Ref pSky;
	igdeWObject::Ref pEnvObject;
	
	deLight::Ref pEngLight;
	deComponent::Ref pEngComponent;
	deAnimator::Ref pEngAnimator;
	deAnimatorInstance::Ref pEngAnimatorInstance;
	deColliderComponent::Ref pEngCollider;
	deRig::Ref pEngRig;
	
	deDebugDrawer::Ref pDDBones;
	decTUniqueList<igdeWCoordSysArrows> pDDSBones;
	float pDDSBoneSize;
	
	aeCamera::Ref pCamera;
	
	aeAnimatorLocomotion *pLocomotion;
	aeWakeboard *pWakeboard;
	deTUniqueReference<aeSubAnimator> pSubAnimator;
	deTUniqueReference<aeSubAnimator> pTestingSubAnimator;
	
	
public:
	igdeMetaPropertyStringSetStorage::Storage mpHiddenBoneNames, mpHiddenVpsNames, mpHiddenMoveNames;
	
	igdeMetaPropertyPathStorage::Storage mpRigPath, mpAnimationPath;
	igdeMetaPropertyStringSetStorage::Storage mpAffectedBones, mpAffectedVps;
	
	igdeMetaPropertyListStorage<aeController>::Storage mpControllers;
	igdeMetaPropertyContextStorage::Storage mpController;
	
	igdeMetaPropertyListStorage<aeLink>::Storage mpLinks;
	igdeMetaPropertyContextStorage::Storage mpLink;
	
	igdeMetaPropertyTreeListStorage<aeRule>::Storage mpRuleTree;
	igdeMetaPropertyListStorage<aeRule>::Storage mpRules;
	igdeMetaPropertyContextStorage::Storage mpRule;
	
	igdeMetaPropertyObjectType<aeController>::ObjectTypeList mpAllowedListControllers;
	
	igdeMetaPropertySliderBoardStorage<aeController::MetaContext>::Storage mpPlaygroundControllers;
	
	igdeMetaPropertyPathStorage::Storage mpDisplayModelPath, mpDisplaySkinPath, mpDisplayRigPath;
	
	igdeMetaPropertyPathStorage::Storage mpBaseAnimatorPath;
	igdeMetaPropertyBooleanStorage::Storage mpResetState;
	
	igdeMetaPropertyFloatStorage::Storage mpPlaySpeed, mpTimeStep;
	igdeMetaPropertyBooleanStorage::Storage mpPaused;
	
	igdeMetaPropertyContextStorage::Storage mpSky, mpEnvironmentObject, mpCamera;
	
	igdeMetaPropertySetStorage<aeAttachment>::Storage mpAttachments;
	igdeMetaPropertyContextStorage::Storage mpAttachment;
	
	igdeUniqueNameGenerator uniqueNameController, uniqueNameLink, uniqueNameRule, uniqueNameAttachment;
	
	decString pathAttachmentConfig;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new actor animator. */
	aeAnimator(aeWindowMain &windowMain);
	
	/** Cleans up the actor animator. */
protected:
	~aeAnimator() override;
public:
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Main window. */
	inline aeWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** Meta context. */
	inline const MetaContext::Ref &GetMetaContext() const{ return pMetaContext; }
	inline const MetaContext::Ref &GetMetaContextController() const{ return pMetaContextController; }
	inline const MetaContext::Ref &GetMetaContextLink() const{ return pMetaContextLink; }
	inline const MetaContext::Ref &GetMetaContextRule() const{ return pMetaContextRule; }
	inline const MetaContext::Ref &GetMetaContextPlayground() const{ return pMetaContextPlayground; }
	inline const MetaContext::Ref &GetMetaContextAttachment() const{ return pMetaContextAttachment; }
	inline const MetaContext::Ref &GetMetaContextView() const{ return pMetaContextView; }
	
	/** Dispose of all resources. */
	void Dispose();
	
	/** Determines if bones are shown. */
	bool GetShowBones() const;
	
	/** Sets if bones are shown. */
	void SetShowBones(bool showBones);
	
	/** Retrieves the base debug drawer bone size. */
	inline float GetDDBoneSize() const{ return pDDSBoneSize; }
	
	/** Set the base debug drawer bone scale. */
	void SetDDBoneSize(float size);
	
	/** Retrieves the engine debug drawer for bones. */
	inline const deDebugDrawer::Ref &GetDDBones() const{ return pDDBones; }
	
	/** Retrieves the sky wrapper. */
	inline const igdeWSky::Ref &GetSky() const{ return pSky; }
	
	/** Retrieves the environment wrapper object. */
	inline const igdeWObject::Ref &GetEnvObject() const{ return pEnvObject; }
	
	/** Retrieves the locomotion. */
	inline aeAnimatorLocomotion &GetLocomotion() const{ return *pLocomotion; }
	
	/** Retrieves the wakeboard. */
	inline aeWakeboard &GetWakeboard() const{ return *pWakeboard; }
	
	/** Retrieves the sub animator. */
	inline const deTUniqueReference<aeSubAnimator> &GetSubAnimator() const{ return pSubAnimator; }
	
	/** Retrieves the testing sub animator. */
	inline const deTUniqueReference<aeSubAnimator> &GetTestingSubAnimator() const{ return pTestingSubAnimator; }
	
	/** Force physics module to update attachments. */
	void AttachmentsForceUpdate();
	
	/** Reset simulation. */
	void ResetSimulation();
	/*@}*/
	
	/** \name Engine Specific */
	/*@{*/
	/** Retrieves the engine world. */
	inline const deWorld::Ref &GetEngineWorld() const{ return pEngWorld; }
	
	/** Retrieves the engine animator. */
	inline const deAnimator::Ref &GetEngineAnimator() const{ return pEngAnimator; }
	
	/** Retrieves the engine animator instance. */
	inline const deAnimatorInstance::Ref &GetEngineAnimatorInstance() const{ return pEngAnimatorInstance; }
	
	/** Retrieves the engine component. */
	inline const deComponent::Ref &GetEngineComponent() const{ return pEngComponent; }
	
	/** Retrieves the engine light. */
	inline const deLight::Ref &GetEngineLight() const{ return pEngLight; }
	
	/** Retrieves the engine collider. */
	inline const deColliderComponent::Ref &GetEngineCollider() const{ return pEngCollider; }
	
	/** Updates the world. */
	void UpdateWorld(float elapsed);
	
	/** Retrieves the camera. */
	inline const aeCamera::Ref &GetCamera() const{ return pCamera; }
	
	/** Engine rig if present. */
	inline const deRig::Ref &GetEngineRig() const{ return pEngRig; }
	
	
	/** Reset all controllers for use with the locomotion system. */
	void ResetControllers();
	
	/** Reset all controller with the given locomotion attribute. */
	void ResetControllersWith(int locomotionAttribute);
	
	/** Inverse all controller with the given locomotion attribute. */
	void InverseControllersWith(int locomotionAttribute);
	
	/** Increment all controller with the given locomotion attribute. */
	void IncrementControllersWith(int locomotionAttribute, float incrementBy);
	
	
	/** Number of targets using a given link. */
	int CountLinkUsage(aeLink *link) const;
	
	
	/** Rebuild rules. */
	void RebuildRules();
	/*@}*/
	
private:
	void pCleanUp();
	
	void pCreateWorld();
	void pCreateCamera();
	void pCreateCollider();
	void pUpdateComponent();
	void pUpdateAnimator();
	void pUpdateLinks();
	void pAnimCompChanged();
	void pUpdateEngineControllers();
	void pUpdateDDSBones();
	void pUpdateRuleIndices();
	void pUpdatePlaygroundControllers();
	
	void pUpdateHiddenBoneNames();
	void pUpdateHiddenVertexPositionSetNames();
	void pUpdateHiddenMoveNames();
};

#endif
