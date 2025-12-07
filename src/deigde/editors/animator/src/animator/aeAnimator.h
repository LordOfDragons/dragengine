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

#include "controller/aeControllerList.h"
#include "link/aeLinkList.h"
#include "rule/aeRuleList.h"

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decStringSet.h>
#include <dragengine/resources/rig/deRig.h>

#include <deigde/editableentity/igdeEditableEntity.h>
#include <deigde/gui/wrapper/igdeWObject.h>

// predefinitions
class igdeEnvironment;

class aeAnimatorLocomotion;
class aeWakeboard;
class aeAttachment;
class aeRule;
class aeAnimatorNotifier;
class aeUndoSystem;
class aeCamera;
class aeSubAnimator;
class aeWindowMain;

class igdeGameDefinition;
class igdeWSky;
class igdeWCoordSysArrows;

class deEngine;
class deWorld;
class deLight;
class deComponent;
class deColliderVolume;
class deColliderComponent;
class deDebugDrawer;
class deAnimator;
class deAnimatorInstance;
class deAnimatorRule;
class deLogger;



/**
 * Animator.
 *
 * Working object for an animator.
 */
class aeAnimator : public igdeEditableEntity{
public:
	typedef deTObjectReference<aeAnimator> Ref;
	
	
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
	
private:
	aeWindowMain &pWindowMain;
	deWorld *pEngWorld;
	
	igdeWSky *pSky;
	igdeWObject::Ref pEnvObject;
	
	deLight *pEngLight;
	deComponent *pEngComponent;
	deAnimator *pEngAnimator;
	deAnimatorInstance *pEngAnimatorInstance;
	deColliderComponent *pEngCollider;
	deRig::Ref pEngRig;
	
	deDebugDrawer *pDDBones;
	igdeWCoordSysArrows *pDDSBones;
	int pDDSBoneCount;
	float pDDSBoneSize;
	
	decString pDisplayModelPath;
	decString pDisplaySkinPath;
	decString pDisplayRigPath;
	
	decString pRigPath;
	decString pAnimationPath;
	
	aeCamera *pCamera;
	
	aeControllerList pControllers;
	aeController *pActiveController;
	
	aeLinkList pLinks;
	aeLink *pActiveLink;
	
	aeRuleList pRules;
	aeRule *pActiveRule;
	
	decStringSet pListBones;
	decStringSet pListVertexPositionSets;
	
	aeAttachment **pAttachments;
	int pAttachmentCount;
	int pAttachmentSize;
	aeAttachment *pActiveAttachment;
	
	bool pPaused;
	float pPlaySpeed;
	float pTimeStep;
	
	aeAnimatorLocomotion *pLocomotion;
	aeWakeboard *pWakeboard;
	aeSubAnimator *pSubAnimator;
	aeSubAnimator *pTestingSubAnimator;
	bool pResetState;
	
	decString pPathAttConfig;
	
	aeAnimatorNotifier **pNotifiers;
	int pNotifierCount;
	int pNotifierSize;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new actor animator. */
	aeAnimator(aeWindowMain &windowMain);
	
	/** Cleans up the actor animator. */
	virtual ~aeAnimator();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Main window. */
	aeWindowMain &GetWindowMain() const{return pWindowMain;}
	
	/** Dispose of all resources. */
	void Dispose();
	
	/** Resets the actor animator. */
	void Reset();
	
	/** Retrieves the display model path. */
	inline const decString &GetDisplayModelPath() const{return pDisplayModelPath;}
	/** Sets the display model path. */
	void SetDisplayModelPath(const char *path);
	/** Retrieves the display skin path. */
	inline const decString &GetDisplaySkinPath() const{return pDisplaySkinPath;}
	/** Sets the display skin path. */
	void SetDisplaySkinPath(const char *path);
	/** Retrieves the display rig path. */
	inline const decString &GetDisplayRigPath() const{return pDisplayRigPath;}
	/** Sets the display rig path. */
	void SetDisplayRigPath(const char *path);
	
	/** Retrieves the rig path. */
	inline const decString &GetRigPath() const{return pRigPath;}
	/** Sets the rig path. */
	void SetRigPath(const char *path);
	/** Retrieves the rig path. */
	inline const decString &GetAnimationPath() const{return pAnimationPath;}
	/** Sets the rig path. */
	void SetAnimationPath(const char *path);
	
	/** Determines if bones are shown. */
	bool GetShowBones() const;
	/** Sets if bones are shown. */
	void SetShowBones(bool showBones);
	/** Retrieves the base debug drawer bone size. */
	inline float GetDDBoneSize() const{return pDDSBoneSize;}
	/** Set the base debug drawer bone scale. */
	void SetDDBoneSize(float size);
	
	/** Determines if the animation is paused. */
	inline bool GetPaused() const{return pPaused;}
	/** Sets if the animation is paused. */
	void SetPaused(bool paused);
	/** Retrieves the play speed. */
	inline float GetPlaySpeed() const{return pPlaySpeed;}
	/** Sets the play speed. */
	void SetPlaySpeed(float playSpeed);
	/** Retrieves the time step. */
	inline float GetTimeStep() const{return pTimeStep;}
	/** Sets the time step. */
	void SetTimeStep(float timeStep);
	
	/** Retrieves the engine debug drawer for bones. */
	inline deDebugDrawer *GetDDBones() const{return pDDBones;}
	
	/** Retrieves the sky wrapper. */
	inline igdeWSky *GetSky() const{return pSky;}
	/** Retrieves the environment wrapper object. */
	inline const igdeWObject::Ref &GetEnvObject() const{return pEnvObject;}
	
	/** Retrieves the locomotion. */
	inline aeAnimatorLocomotion &GetLocomotion() const{return *pLocomotion;}
	/** Retrieves the wakeboard. */
	inline aeWakeboard &GetWakeboard() const{return *pWakeboard;}
	/** Retrieves the sub animator. */
	inline aeSubAnimator *GetSubAnimator() const{return pSubAnimator;}
	/** Retrieves the testing sub animator. */
	inline aeSubAnimator *GetTestingSubAnimator() const{return pTestingSubAnimator;}
	
	/** Determines if the state is reset before applying the animator. */
	inline bool GetResetState() const{return pResetState;}
	/** Sets if the state is reset before applying the animator. */
	void SetResetState(bool resetState);
	
	/** Force physics module to update attachments. */
	void AttachmentsForceUpdate();
	/** Reset simulation. */
	void ResetSimulation();
	
	
	
	/** Last file dialog attachment configuration path. */
	inline const decString &GetPathAttachmentConfig() const{return pPathAttConfig;}
	
	/** Set last file dialog attachment configuration path. */
	void SetPathAttachmentConfig(const char *path);
	/*@}*/
	
	/** \name Engine Specific */
	/*@{*/
	/** Retrieves the engine world. */
	inline deWorld *GetEngineWorld() const{return pEngWorld;}
	/** Retrieves the engine animator. */
	inline deAnimator *GetEngineAnimator() const{return pEngAnimator;}
	/** Retrieves the engine animator instance. */
	inline deAnimatorInstance *GetEngineAnimatorInstance() const{return pEngAnimatorInstance;}
	/** Retrieves the engine component. */
	inline deComponent *GetEngineComponent() const{return pEngComponent;}
	/** Retrieves the engine light. */
	inline deLight *GetEngineLight() const{return pEngLight;}
	/** Retrieves the engine collider. */
	inline deColliderComponent *GetEngineCollider() const{return pEngCollider;}
	/** Updates the world. */
	void UpdateWorld(float elapsed);
	/** Retrieves the camera. */
	inline aeCamera *GetCamera() const{return pCamera;}
	
	/** Engine rig if present. */
	inline deRig *GetEngineRig() const{return pEngRig;}
	/*@}*/
	
	
	
	/** \name Controllers */
	/*@{*/
	/** Controllers. */
	inline const aeControllerList &GetControllers() const{return pControllers;}
	
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
	
	/** Active controller or NULL. */
	inline aeController *GetActiveController() const{return pActiveController;}
	
	/** Set active controller or NULL. */
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
	inline const aeLinkList &GetLinks() const{return pLinks;}
	
	/** Add link. */
	void AddLink(aeLink *link);
	
	/** Remove link. */
	void RemoveLink(aeLink *link);
	
	/** Remove all links. */
	void RemoveAllLinks();
	
	/** Active link or NULL. */
	inline aeLink *GetActiveLink() const{return pActiveLink;}
	
	/** Set active link or NULL. */
	void SetActiveLink(aeLink *link);
	
	/** Number of targets using a given link. */
	int CountLinkUsage(aeLink *link) const;
	/*@}*/
	
	
	
	/** \name Rules */
	/*@{*/
	/** Rules. */
	inline const aeRuleList &GetRules() const{return pRules;}
	
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
	
	/** Active rule or NULL. */
	inline aeRule *GetActiveRule() const{return pActiveRule;}
	
	/** Set active rule or NULL. */
	void SetActiveRule(aeRule *rule);
	
	/** Rebuild rules. */
	void RebuildRules();
	/*@}*/
	
	
	
	/** \name Bone Management */
	/*@{*/
	/** List of bones. */
	inline const decStringSet &GetListBones() const{return pListBones;}
	
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
	inline const decStringSet &GetListVertexPositionSets() const{return pListVertexPositionSets;}
	
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
	/** Retrieves the number of attachments. */
	inline int GetAttachmentCount() const{return pAttachmentCount;}
	/** Retrieves the attachment at the given index. */
	aeAttachment *GetAttachmentAt(int index) const;
	/** Retrieves the attachment with the given name or NULL if not found. */
	aeAttachment *GetAttachmentNamed(const char *name) const;
	/** Retrieves the index of the attachment or -1 if not found. */
	int IndexOfAttachment(aeAttachment *attachment) const;
	/** Determines if the attachment exists. */
	bool HasAttachment(aeAttachment *attachment) const;
	/** Determines if the attachment exists. */
	bool HasAttachmentNamed(const char *name) const;
	/** Adds a new attachment. */
	void AddAttachment(aeAttachment *attachment);
	/** Removes the given attachment. */
	void RemoveAttachment(aeAttachment *attachment);
	/** Removes all attachments. */
	void RemoveAllAttachments();
	/** Retrieves the active attachment or NULL. */
	inline aeAttachment *GetActiveAttachment() const{return pActiveAttachment;}
	/** Sets the active attachment or NULL. */
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
	/** Retrieves the number of notifiers. */
	inline int GetNotifierCount() const{return pNotifierCount;}
	/** Retrieves the notifier at the given index. */
	aeAnimatorNotifier *GetNotifierAt(int index) const;
	/** Retrieves the index of the notifier or -1 if not found. */
	int IndexOfNotifier(aeAnimatorNotifier *notifier) const;
	/** Determines if the notifier exists. */
	bool HasNotifier(aeAnimatorNotifier *notifier) const;
	/** Adds a new notifier. */
	void AddNotifier(aeAnimatorNotifier *notifier);
	/** Removes the given notifier. */
	void RemoveNotifier(aeAnimatorNotifier *notifier);
	/** Removes all notifiers. */
	void RemoveAllNotifiers();
	
	/** Notifies all listeners that the changed or saved state changed. */
	virtual void NotifyStateChanged();
	/** Notifies all listeners that the undo system changed. */
	virtual void NotifyUndoChanged();
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
};

#endif
