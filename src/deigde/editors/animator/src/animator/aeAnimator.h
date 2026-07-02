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
#include "aeAnimatorNotifier.h"
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
	using AttachmentSet = decTCollectionQueryByName<decTObjectSet<aeAttachment>, aeAttachment>;
	using NotifierSet = decTObjectOrderedSet<aeAnimatorNotifier>;
	
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
	
	NotifierSet pNotifiers;
	
	
	igdeMetaPropertyStringSetStorage::Storage pMPHiddenBoneNames, pMPHiddenVpsNames, pMPHiddenMoveNames;
	
	igdeMetaPropertyPathStorage::Storage pMPRigPath, pMPAnimationPath;
	igdeMetaPropertyStringSetStorage::Storage pMPAffectedBones, pMPAffectedVps;
	
	igdeMetaPropertyListStorage<aeController, aeController::List>::Storage pMPControllers;
	igdeMetaPropertyContextStorage::Storage pMPController;
	
	igdeMetaPropertyListStorage<aeLink, aeLink::List>::Storage pMPLinks;
	igdeMetaPropertyContextStorage::Storage pMPLink;
	
	igdeMetaPropertyTreeListStorage<aeRule>::Storage pMPRuleTree;
	igdeMetaPropertyListStorage<aeRule, aeRule::List>::Storage pMPRules;
	igdeMetaPropertyContextStorage::Storage pMPRule;
	
	igdeMetaPropertyObjectType<aeController>::ObjectTypeList pMPAllowedListControllers;
	
	igdeMetaPropertySliderBoardStorage<aeController::MetaContext>::Storage pMPPlaygroundControllers;
	
	igdeMetaPropertyPathStorage::Storage pMPDisplayModelPath, pMPDisplaySkinPath, pMPDisplayRigPath;
	
	igdeMetaPropertyPathStorage::Storage pMPBaseAnimatorPath;
	igdeMetaPropertyBooleanStorage::Storage pMPResetState;
	
	igdeMetaPropertyFloatStorage::Storage pMPPlaySpeed, pMPTimeStep;
	igdeMetaPropertyBooleanStorage::Storage pMPPaused;
	
	igdeMetaPropertyContextStorage::Storage pMPSky, pMPEnvironmentObject, pMPCamera;
	
	igdeMetaPropertySetStorage<aeAttachment, AttachmentSet>::Storage pMPAttachments;
	igdeMetaPropertyContextStorage::Storage pMPAttachment;
	
	
public:
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
	
	inline igdeMetaPropertyStringSetStorage::Storage &GetMPHiddenBoneNames(){ return pMPHiddenBoneNames; }
	inline igdeMetaPropertyStringSetStorage::Storage &GetMPHiddenVPSNames(){ return pMPHiddenVpsNames; }
	inline igdeMetaPropertyStringSetStorage::Storage &GetMPHiddenMoveNames(){ return pMPHiddenMoveNames; }
	
	inline igdeMetaPropertyPathStorage::Storage &GetMPRigPath(){ return pMPRigPath; }
	inline igdeMetaPropertyPathStorage::Storage &GetMPAnimationPath(){ return pMPAnimationPath; }
	inline igdeMetaPropertyStringSetStorage::Storage &GetMPAffectedBones(){ return pMPAffectedBones; }
	inline igdeMetaPropertyStringSetStorage::Storage &GetMPAffectedVertexPositionSets(){ return pMPAffectedVps; }
	
	inline igdeMetaPropertyListStorage<aeController, aeController::List>::Storage &GetMPControllers(){ return pMPControllers; }
	inline igdeMetaPropertyContextStorage::Storage &GetMPController(){ return pMPController; }
	
	inline igdeMetaPropertyListStorage<aeLink, aeLink::List>::Storage &GetMPLinks(){ return pMPLinks; }
	inline igdeMetaPropertyContextStorage::Storage &GetMPLink(){ return pMPLink; }
	
	inline igdeMetaPropertyTreeListStorage<aeRule>::Storage &GetMPRuleTree(){ return pMPRuleTree; }
	inline igdeMetaPropertyListStorage<aeRule, aeRule::List>::Storage &GetMPRules(){ return pMPRules; }
	inline igdeMetaPropertyContextStorage::Storage &GetMPRule(){ return pMPRule; }
	
	inline igdeMetaPropertyObjectType<aeController>::ObjectTypeList &GetMPAllowedListControllers(){ return pMPAllowedListControllers; }
	
	inline igdeMetaPropertySliderBoardStorage<aeController::MetaContext>::Storage &GetMPPlayground(){ return pMPPlaygroundControllers; }
	
	inline igdeMetaPropertyPathStorage::Storage &GetMPDisplayModelPath(){ return pMPDisplayModelPath; }
	inline igdeMetaPropertyPathStorage::Storage &GetMPDisplaySkinPath(){ return pMPDisplaySkinPath; }
	inline igdeMetaPropertyPathStorage::Storage &GetMPDisplayRigPath(){ return pMPDisplayRigPath; }
	
	inline igdeMetaPropertyPathStorage::Storage &GetMPBaseAnimatorPath(){ return pMPBaseAnimatorPath; }
	inline igdeMetaPropertyBooleanStorage::Storage &GetMPResetState(){ return pMPResetState; }
	
	inline igdeMetaPropertyFloatStorage::Storage &GetMPPlaySpeed(){ return pMPPlaySpeed; }
	inline igdeMetaPropertyFloatStorage::Storage &GetMPTimeStep(){ return pMPTimeStep; }
	inline igdeMetaPropertyBooleanStorage::Storage &GetMPPaused(){ return pMPPaused; }
	
	inline igdeMetaPropertyContextStorage::Storage &GetMPSky(){ return pMPSky; }
	inline igdeMetaPropertyContextStorage::Storage &GetMPEnvironmentObject(){ return pMPEnvironmentObject; }
	inline igdeMetaPropertyContextStorage::Storage &GetMPCamera(){ return pMPCamera; }
	
	inline igdeMetaPropertySetStorage<aeAttachment, AttachmentSet>::Storage &GetMPAttachments(){ return pMPAttachments; }
	inline igdeMetaPropertyContextStorage::Storage &GetMPAttachment(){ return pMPAttachment; }
	
	
	/** Dispose of all resources. */
	void Dispose();
	
	/** Resets the actor animator. */
	void Reset();
	
	/** Retrieves the display model path. */
	inline const decString &GetDisplayModelPath() const{ return pMPDisplayModelPath; }
	/** Sets the display model path. */
	void SetDisplayModelPath(const char *path);
	/** Retrieves the display skin path. */
	inline const decString &GetDisplaySkinPath() const{ return pMPDisplaySkinPath; }
	/** Sets the display skin path. */
	void SetDisplaySkinPath(const char *path);
	/** Retrieves the display rig path. */
	inline const decString &GetDisplayRigPath() const{ return pMPDisplayRigPath; }
	/** Sets the display rig path. */
	void SetDisplayRigPath(const char *path);
	
	/** Retrieves the rig path. */
	inline const decString &GetRigPath() const{ return pMPRigPath; }
	/** Sets the rig path. */
	void SetRigPath(const char *path);
	/** Retrieves the rig path. */
	inline const decString &GetAnimationPath() const{ return pMPAnimationPath; }
	/** Sets the rig path. */
	void SetAnimationPath(const char *path);
	
	/** Determines if bones are shown. */
	bool GetShowBones() const;
	/** Sets if bones are shown. */
	void SetShowBones(bool showBones);
	/** Retrieves the base debug drawer bone size. */
	inline float GetDDBoneSize() const{ return pDDSBoneSize; }
	/** Set the base debug drawer bone scale. */
	void SetDDBoneSize(float size);
	
	/** Determines if the animation is paused. */
	inline bool GetPaused() const{ return pMPPaused; }
	/** Sets if the animation is paused. */
	void SetPaused(bool paused);
	/** Retrieves the play speed. */
	inline float GetPlaySpeed() const{ return pMPPlaySpeed; }
	/** Sets the play speed. */
	void SetPlaySpeed(float playSpeed);
	/** Retrieves the time step. */
	inline float GetTimeStep() const{ return pMPTimeStep; }
	/** Sets the time step. */
	void SetTimeStep(float timeStep);
	
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
	
	/** Determines if the state is reset before applying the animator. */
	inline bool GetResetState() const{ return pMPResetState; }
	/** Sets if the state is reset before applying the animator. */
	void SetResetState(bool resetState);
	
	/** Force physics module to update attachments. */
	void AttachmentsForceUpdate();
	/** Reset simulation. */
	void ResetSimulation();
	
	
	
	/** Last file dialog attachment configuration path. */
	inline const decString &GetPathAttachmentConfig() const{ return pathAttachmentConfig; }
	
	/** Set last file dialog attachment configuration path. */
	void SetPathAttachmentConfig(const char *path);
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
	/*@}*/
	
	
	
	/** \name Controllers */
	/*@{*/
	/** Controllers. */
	inline const aeController::List &GetControllers() const{ return pMPControllers; }
	
	/** Add controller. */
	void AddController(aeController *controller);
	
	/** Insert new controller. */
	void InsertControllerAt(aeController *controller, int index);
	
	/** Move controller to a new position. */
	void MoveControllerTo(aeController *controller, int index);
	
	/** Remove given controller. */
	void RemoveController(aeController *controller);
	
	/** Remove all controllers. */
	void RemoveAllControllers();
	
	/** Active controller or nullptr. */
	inline const aeController::Ref &GetActiveController() const{ return pMPControllers.GetActive(); }
	
	/** Set active controller or nullptr. */
	void SetActiveController(aeController *controller);
	
	/** Reset all controllers for use with the locomotion system. */
	void ResetControllers();
	
	/** Reset all controller with the given locomotion attribute. */
	void ResetControllersWith(int locomotionAttribute);
	
	/** Inverse all controller with the given locomotion attribute. */
	void InverseControllersWith(int locomotionAttribute);
	
	/** Increment all controller with the given locomotion attribute. */
	void IncrementControllersWith(int locomotionAttribute, float incrementBy);
	/*@}*/
	
	
	
	/** \name Links */
	/*@{*/
	/** Links. */
	inline const aeLink::List &GetLinks() const{ return pMPLinks; }
	
	/** Add link. */
	void AddLink(aeLink *link);
	
	/** Remove link. */
	void RemoveLink(aeLink *link);
	
	/** Remove all links. */
	void RemoveAllLinks();
	
	/** Active link or nullptr. */
	inline const aeLink::Ref &GetActiveLink() const{ return pMPLinks.GetActive(); }
	
	/** Set active link or nullptr. */
	void SetActiveLink(aeLink *link);
	
	/** Number of targets using a given link. */
	int CountLinkUsage(aeLink *link) const;
	/*@}*/
	
	
	
	/** \name Rules */
	/*@{*/
	/** Rules. */
	inline const aeRule::List &GetRules() const{ return pMPRules; }
	
	/** Add rule. */
	void AddRule(aeRule *rule);
	
	/** Insert rule. */
	void InsertRuleAt(aeRule *rule, int index);
	
	/** Move rule to new position. */
	void MoveRuleTo(aeRule *rule, int index);
	
	/** Remove rule. */
	void RemoveRule(aeRule *rule);
	
	/** Remove all rules. */
	void RemoveAllRules();
	
	/** Active rule or nullptr. */
	inline const aeRule::Ref &GetActiveRule() const{ return pMPRules.GetActive(); }
	
	/** Set active rule or nullptr. */
	void SetActiveRule(aeRule *rule);
	
	/** Rebuild rules. */
	void RebuildRules();
	/*@}*/
	
	
	
	/** \name Bone Management */
	/*@{*/
	/** List of bones. */
	inline const decStringSet &GetListBones() const{ return pMPAffectedBones; }
	
	/** Set list of bones. */
	void SetListBones(const decStringSet &bones);
	
	/** Adds a bone. */
	void AddBone(const char *bone);
	
	/** Removes the given bone. */
	void RemoveBone(const char *bone);
	
	/** Removes all bones. */
	void RemoveAllBones();
	/*@}*/
	
	
	
	/** \name Vertex position set Management */
	/*@{*/
	/** List of vertex position sets. */
	inline const decStringSet &GetListVertexPositionSets() const{ return pMPAffectedVps; }
	
	/** Set list of vertex position sets. */
	void SetListVertexPositionSets(const decStringSet &sets);
	
	/** Adds a vertex position set. */
	void AddVertexPositionSet(const char *vertexPositionSet);
	
	/** Removes the given vertex position set. */
	void RemoveVertexPositionSet(const char *vertexPositionSet);
	
	/** Removes all vertex position sets. */
	void RemoveAllVertexPositionSets();
	/*@}*/
	
	
	
	/** \name Attachments */
	/*@{*/
	/** Attachments. */
	inline const AttachmentSet &GetAttachments() const{ return pMPAttachments; }
	
	/** Visitor to find attachment by name. */
	aeAttachment *GetAttachmentNamed(const char *name) const;
	
	/** Add attachment. */
	void AddAttachment(aeAttachment *attachment);
	
	/** Remove attachment. */
	void RemoveAttachment(aeAttachment *attachment);
	
	/** Removes all attachments. */
	void RemoveAllAttachments();
	
	/** Active attachment or nullptr. */
	inline const aeAttachment::Ref &GetActiveAttachment() const{ return pMPAttachments.GetActive(); }
	
	/** Set active attachment or nullptr. */
	void SetActiveAttachment(aeAttachment *attachment);
	
	/** Attach all attachments. */
	void AttachAttachments();
	
	/** Detach all attachments. */
	void DetachAttachments();
	
	/** Reset physics states of all attachments. */
	void AttachmentsResetPhysics();
	/*@}*/
	
	
	/** \name Notifiers */
	/*@{*/
	/** Notifiers. */
	inline const NotifierSet &GetNotifiers() const{ return pNotifiers; }
	
	/** Add notifier. */
	void AddNotifier(aeAnimatorNotifier *notifier);
	
	/** Remove notifier. */
	void RemoveNotifier(aeAnimatorNotifier *notifier);
	
	/** Removes all notifiers. */
	void RemoveAllNotifiers();
	
	/** Notify base path changed. */
	void NotifyBasePathChanged();
	
	/** Notifies all listeners that the changed or saved state changed. */
	void NotifyStateChanged() override;
	/** Notifies all listeners that the undo system changed. */
	void NotifyUndoChanged() override;
	/** Notifies all that the animator changed. */
	void NotifyAnimatorChanged();
	/** Notifies all that the view changed. */
	void NotifyViewChanged();
	/** Notifies all that the model changed. */
	void NotifyModelChanged();
	/** Notifies all that the sky changed. */
	void NotifySkyChanged();
	/** Notifies all that the environment object changed. */
	void NotifyEnvObjectChanged();
	/** Notifies all that the rig changed. */
	void NotifyRigChanged();
	/** Notifies all that the animation changed. */
	void NotifyAnimationChanged();
	/** Notifies all that the playback changed. */
	void NotifyPlaybackChanged();
	/** Notifies all that the locomotion changed. */
	void NotifyLocomotionChanged();
	
	/** Notifies all that the active controller has changed. */
	void NotifyActiveControllerChanged();
	/** Notifies all that a controller has changed. */
	void NotifyControllerChanged(aeController *controller);
	/** Notify all controller name changed. */
	void NotifyControllerNameChanged(aeController *controller);
	/** Notifies all that a controller value has changed. */
	void NotifyControllerValueChanged(aeController *controller);
	/** Notifies all that the order or count of controllers changed. */
	void NotifyControllerStructureChanged();
	
	/** Notifies all that the active link has changed. */
	void NotifyActiveLinkChanged();
	/** Notifies all that a link has changed. */
	void NotifyLinkChanged(aeLink *link);
	/** Notifies all that a link has changed. */
	void NotifyLinkNameChanged(aeLink *link);
	/** Notifies all that the order or count of links changed. */
	void NotifyLinkStructureChanged();
	
	/** Notifies all that the active rule has changed. */
	void NotifyActiveRuleChanged();
	/** Notifies all that a rule has changed. */
	void NotifyRuleChanged(aeRule *rule);
	/** Notifies all that a rule has changed name. */
	void NotifyRuleNameChanged(aeRule *rule);
	/** Notifies all that the order or count of rules changed. */
	void NotifyRuleStructureChanged();
	
	/** Notifies all that the active attachment has changed. */
	void NotifyActiveAttachmentChanged();
	/** Notifies all that a attachment has changed. */
	void NotifyAttachmentChanged(aeAttachment *attachment);
	/** Notifies all that the count of attachments changed. */
	void NotifyAttachmentStructureChanged();
	
	/** Notifies all that the camera changed. */
	void NotifyCameraChanged();
	/** Notifies all that the camera view changed. */
	void NotifyCameraViewChanged();
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
