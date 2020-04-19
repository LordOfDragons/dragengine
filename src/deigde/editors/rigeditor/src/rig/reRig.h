/* 
 * Drag[en]gine IGDE Rig Editor
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

#ifndef _RERIG_H_
#define _RERIG_H_

#include "../collisions/reCLRigSimulation.h"

#include <deigde/editableentity/igdeEditableEntity.h>

#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/collider/deColliderConstraint.h>

class igdeWObject;
class reRigConstraintDof;
class igdeGameDefinition;
class igdeEnvironment;
class igdeWSky;
class reCamera;
class reRigPush;
class reRigBone;
class reRigShape;
class reRigConstraint;
class reSelectionBones;
class reSelectionPushes;
class reSelectionShapes;
class reSelectionConstraints;
class reRigNotifier;
class reRigTexture;

class deEngine;
class deWorld;
class deLight;
class deRig;
class deColliderComponent;
class deComponent;
class deAnimator;
class deAnimatorInstance;
class deAnimatorRuleAnimation;
class deAnimatorRuleStateManipulator;
class deCollider;
class deCollisionInfo;
class deColliderVolume;
class reCLRigSimulation;
class deLogger;



/**
 * @brief Rig.
 * Working object for a rig.
 */
class reRig : public igdeEditableEntity{
public:
	/** Element modes. */
	enum eElementModes{
		/** Bone mode. */
		eemBone,
		/** Shape mode. */
		eemShape,
		/** Constraint mode. */
		eemConstraint,
		/** Push mode. */
		eemPush,
	};
	
	/** Work modes. */
	enum eWorkModes{
		/** Select elements mode. */
		ewmSelect,
		/** Move elements mode. */
		ewmMove,
		/** Scale elements mode. */
		ewmScale,
		/** Rotate elements mode. */
		ewmRotate,
		/** Place 3D cursor mode. */
		ewm3DCursor,
	};
	
	/** Layer masks for colliders. */
	enum eColliderLayerMasks{
		/** Bones. */
		eclmBones,
		/** Shapes. */
		eclmShapes,
		/** Constraints. */
		eclmConstraints,
		/** Pushes. */
		eclmPushes,
		/** Simulation. */
		eclmSimulation,
	};
	
private:
	deWorld *pEngWorld;
	deLight *pEngLight;
	
	deComponent *pEngComponent;
	deAnimator *pEngAnimator;
	deAnimatorInstance *pEngAnimatorInstance;
	deColliderComponent *pEngSimCollider;
	deAnimatorRuleAnimation *pEngAnimatorAnim;
	deAnimatorRuleStateManipulator *pEngAnimatorRestPose;
	
	reRigTexture **pComponentTextures;
	int pComponentTextureCount;
	
	igdeWSky *pSky;
	igdeWObject *pEnvObject;
	
	deRig *pEngRig;
	bool pPoseChanged;
	
	decString pModelPath;
	decString pSkinPath;
	decString pAnimationPath;
	
	decVector pCMP;
	bool pModelCollision;
	
	decString pMoveName;
	float pMoveTime;
	float pPlayTime;
	bool pUseRestPose;
	bool pPlaybackMove;
	
	reCamera *pCamera;
	
	reRigBone **pBones;
	int pBoneCount;
	int pBoneSize;
	reRigBone *pRootBone;
	
	reRigShape **pShapes;
	int pShapeCount;
	int pShapeSize;
	
	reRigConstraint **pConstraints;
	int pConstraintCount;
	int pConstraintSize;
	
	reRigPush **pPushes;
	int pPushCount;
	int pPushSize;
	
	reRigNotifier **pNotifiers;
	int pNotifierCount;
	int pNotifierSize;
	
	bool pLockAxisX;
	bool pLockAxisY;
	bool pLockAxisZ;
	bool pUseLocal;
	
	bool pShowBones;
	bool pShowRigShapes;
	bool pShowAllBoneShapes;
	bool pShowRigConstraints;
	bool pShowAllBoneConstraints;
	bool pShowRigPushes;
	bool pShowShapeXRay;
	
	eElementModes pElementMode;
	eWorkModes pWorkMode;
	bool pSimulationRunning;
	reCLRigSimulation pCLSimulation;
	decVector pGravity;
	decVector pLocalGravity;
	bool pDynamic;
	float pSlowmotion;
	decDMatrix pPoseMatrix;
	
	reSelectionBones *pSelectionBones;
	reSelectionShapes *pSelectionShapes;
	reSelectionPushes *pSelectionPushes;
	reSelectionConstraints *pSelectionConstraints;
	
	bool pDirtyRig;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new rig. */
	reRig( igdeEnvironment *environment );
	/** Cleans up the rig. */
	virtual ~reRig();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Dispose of all resources. */
	void Dispose();
	
	/** Resets the rig. */
	void Reset();
	
	/** Invalidate rig. */
	void Invalidate();
	/** Rebuilds the rig. */
	void Rebuild();
	
	/** Updates the bone matrices. */
	void UpdateBoneMatrices();
	/** Updates the pose matrix from the rest pose. */
	void SetPoseFromRest();
	/** Retrieves the model path. */
	inline const decString &GetModelPath() const{ return pModelPath; }
	/** Sets the model path. */
	void SetModelPath( const char *path );
	/** Retrieves the skin path. */
	inline const decString &GetSkinPath() const{ return pSkinPath; }
	/** Sets the skin path. */
	void SetSkinPath( const char *path );
	/** Retrieves the rig path. */
	inline const decString &GetAnimationPath() const{ return pAnimationPath; }
	/** Sets the rig path. */
	void SetAnimationPath( const char *path );
	
	/** \brief Central mass point position. */
	inline const decVector &GetCentralMassPoint() const{ return pCMP; }
	
	/** \brief Set central mass point position. */
	void SetCentralMassPoint( const decVector &cmp );
	
	/** \brief Use component model for collision instead of shapes if present. */
	inline bool GetModelCollision() const{ return pModelCollision; }
	
	/** \brief Set if component model is used for collision instead of shapes if present. */
	void SetModelCollision( bool modelCollision );
	
	/** Retrieves the name of the move to display. */
	inline const decString &GetMoveName() const{ return pMoveName; }
	/** Sets the move name to display. */
	void SetMoveName( const char *moveName );
	/** Retrieves the move time to display. */
	inline float GetMoveTime() const{ return pMoveTime; }
	/** Sets the move time to display. */
	void SetMoveTime( float moveTime );
	/** Retrieves the play time. */
	inline float GetPlayTime() const{ return pPlayTime; }
	/** Determines if the rest pose is used or the animation pose. */
	inline bool GetUseRestPose() const{ return pUseRestPose; }
	/** Sets if the rest pose is used. */
	void SetUseRestPose( bool useRestPose );
	/** Determines if the animation move is played back. */
	inline bool GetPlaybackMove() const{ return pPlaybackMove; }
	/** Sets if the animation move is played back. */
	void SetPlaybackMove( bool playbackMove );
	
	/** Determines if rig bones are shown. */
	inline bool GetShowBones() const{ return pShowBones; }
	/** Sets if rig bones are shown. */
	void SetShowBones( bool showBones );
	
	/** Determines if rig shapes are shown. */
	inline bool GetShowRigShapes() const{ return pShowRigShapes; }
	/** Sets if rig shapes are shown. */
	void SetShowRigShapes( bool showShapes );
	/** Determines if all bone shapes are shown. */
	inline bool GetShowAllBoneShapes() const{ return pShowAllBoneShapes; }
	/** Sets if all bone shapes are shown. */
	void SetShowAllBoneShapes( bool showShapes );
	/** Determines if shapes are rendered using x-ray. */
	inline bool GetShapeXRay() const{ return pShowShapeXRay; }
	/** Sets of shapes are rendered using x-ray. */
	void SetShapeXRay( bool xray );
	
	/** Determines if rig constraints are shown. */
	inline bool GetShowRigConstraints() const{ return pShowRigConstraints; }
	/** Sets if rig constraints are shown. */
	void SetShowRigConstraints( bool showConstraints );
	/** Determines if all bone constraints are shown. */
	inline bool GetShowAllBoneConstraints() const{ return pShowAllBoneConstraints; }
	/** Sets if all bone constraints are shown. */
	void SetShowAllBoneConstraints( bool showConstraints );
	
	/** Determines if rig push are shown. */
	inline bool GetShowRigPushes() const{ return pShowRigPushes; }
	/** Sets if rig push are shown. */
	void SetShowRigPushes( bool showPushes );
	
	/** Retrieves the element mode indicating which type of element to edit. */
	inline eElementModes GetElementMode() const{ return pElementMode; }
	/** Sets the element mode indicating which type of element to edit. */
	void SetElementMode( eElementModes mode );
	/** Retrieves the work mode. */
	inline eWorkModes GetWorkMode() const{ return pWorkMode; }
	/** Sets the work mode. */
	void SetWorkMode( eWorkModes mode );
	
	/** Determines if the simulation is running. */
	inline bool GetSimulationRunning() const{ return pSimulationRunning; }
	/** Sets if the simulation is running. */
	void SetSimulationRunning( bool simulationRunning );
	/** Retrieves the gravity. */
	inline const decVector &GetGravity() const{ return pGravity; }
	/** Sets the gravity. */
	void SetGravity( const decVector &gravity );
	/** Retrieves the local gravity. */
	inline const decVector &GetLocalGravity() const{ return pLocalGravity; }
	/** Sets the local gravity. */
	void SetLocalGravity( const decVector &gravity );
	/** \brief Retrieves the slowmotion factor. */
	inline float GetSlowmotion() const{ return pSlowmotion; }
	/** \brief Sets the slowmotion factor. */
	void SetSlowmotion( float slowmotion );
	/** Determines if dynamic mode has to be used. */
	inline bool GetDynamic() const{ return pDynamic; }
	/** Sets if dynamic mode has to be used. */
	void SetDynamic( bool dynamic );
	
	/** Retrieves the current pose matrix. */
	inline const decDMatrix &GetPoseMatrix() const{ return pPoseMatrix; }
	/** Updates from the simulation. */
	void UpdateFromSimulation();
	
	/** Retrieves the bone selection. */
	inline reSelectionBones *GetSelectionBones() const{ return pSelectionBones; }
	/** Retrieves the shape selection. */
	inline reSelectionShapes *GetSelectionShapes() const{ return pSelectionShapes; }
	/** Retrieves the push selection. */
	inline reSelectionPushes *GetSelectionPushes() const{ return pSelectionPushes; }
	/** Retrieves the constraint selection. */
	inline reSelectionConstraints *GetSelectionConstraints() const{ return pSelectionConstraints; }
	/*@}*/
	
	/** @name Engine Specific */
	/*@{*/
	/** Retrieves the engine world. */
	inline deWorld *GetEngineWorld() const{ return pEngWorld; }
	/** Retrieves the engine animator. */
	inline deAnimator *GetEngineAnimator() const{ return pEngAnimator; }
	/** Retrieves the engine component. */
	inline deComponent *GetEngineComponent() const{ return pEngComponent; }
	/** Retrieves the simulation collider. */
	inline deColliderComponent *GetEngineSimulationCollider() const{ return pEngSimCollider; }
	/** Retrieves the engine light. */
	inline deLight *GetEngineLight() const{ return pEngLight; }
	/** Retrieves the engine rig. */
	inline deRig *GetEngineRig() const{ return pEngRig; }
	/** Retrieves the camera. */
	inline reCamera *GetCamera() const{ return pCamera; }
	/** Updates the world. */
	void UpdateWorld( float elapsed );
	
	/** Retrieves the number of component textures. */
	inline int GetComponentTextureCount() const{ return pComponentTextureCount; }
	/** Retrieves a component texture. */
	reRigTexture *GetComponentTextureAt( int index ) const;
	/** Updates component textures. */
	void UpdateComponentTextures();
	
	/** Retrieves the light position. */
	decVector GetLightPosition() const;
	/** Sets the light position. */
	void SetLightPosition( const decVector &position );
	/** Retrieves the ambient light. */
	const decColor &GetAmbientLight() const;
	/** Sets the ambient light. */
	void SetAmbientLight( const decColor &color );
	
	/** Initialize delegates for engine objects if required. */
	void InitDelegates();
	
	/** Retrieves the sky wrapper. */
	inline igdeWSky *GetSky() const{ return pSky; }
	/** Retrieves the environment wrapper object. */
	inline igdeWObject *GetEnvObject() const{ return pEnvObject; }
	/*@}*/
	
	/** @name Editing */
	/*@{*/
	/** Determines if the x axis is locked during editing. */
	inline bool GetLockAxisX() const{ return pLockAxisX; }
	/** Sets if the x axis is locked during editing. */
	void SetLockAxisX( bool lock );
	/** Determines if the y axis is locked during editing. */
	inline bool GetLockAxisY() const{ return pLockAxisY; }
	/** Sets if the y axis is locked during editing. */
	void SetLockAxisY( bool lock );
	/** Determines if the z axis is locked during editing. */
	inline bool GetLockAxisZ() const{ return pLockAxisZ; }
	/** Sets if the z axis is locked during editing. */
	void SetLockAxisZ( bool lock );
	/** Determines if the object local coordinates are used during editing. */
	inline bool GetUseLocal() const{ return pUseLocal; }
	/** Sets if the object local coordinates are used during editing. */
	void SetUseLocal( bool useLocal );
	/*@}*/
	
	/** @name Bones */
	/*@{*/
	/** Retrieves the number of bones. */
	inline int GetBoneCount() const{ return pBoneCount; }
	/** Retrieves the bone at the given index. */
	reRigBone *GetBoneAt( int index ) const;
	/** Retrieves the bone with the given name or NULL if not found. */
	reRigBone *GetBoneNamed( const char *name ) const;
	/** Retrieves the bone with the given collider or NULL if not found. */
	reRigBone *GetBoneWith( deColliderVolume *collider ) const;
	/** Retrieves the bone with the given order or NULL if not found. */
	reRigBone *GetBoneWithOrder( int order ) const;
	/** Retrieves the index of the bone or -1 if not found. */
	int IndexOfBone( reRigBone *bone ) const;
	/** Determines if the bone exists. */
	bool HasBone( reRigBone *bone ) const;
	/** Adds a new bone. */
	void AddBone( reRigBone *bone );
	/** Removes the given bone. */
	void RemoveBone( reRigBone *bone );
	/** Removes all bones. */
	void RemoveAllBones();
	
	/** Reorders the bones to be usable with an engine rig. */
	void ReorderBones();
	/** Sets the visited state of all bones. */
	void SetAllBonesVisited( bool visited );
	
	/** Retrieves the root bone or NULL if not set. */
	inline reRigBone *GetRootBone() const{ return pRootBone; }
	/** Sets the root bone or NULL if not set. */
	void SetRootBone( reRigBone *rootBone );
	/*@}*/
	
	/** @name Shapes */
	/*@{*/
	/** Retrieves the number of shapes. */
	inline int GetShapeCount() const{ return pShapeCount; }
	/** Retrieves the shape at the given index. */
	reRigShape *GetShapeAt( int index ) const;
	/** Retrieves the shape with the given collider or NULL if not found. */
	reRigShape *GetShapeWith( deColliderVolume *collider ) const;
	/** Retrieves the index of the shape or -1 if not found. */
	int IndexOfShape( reRigShape *shape ) const;
	/** Determines if the shape exists. */
	bool HasShape( reRigShape *shape ) const;
	/** Adds a new shape. */
	void AddShape( reRigShape *shape );
	/** Removes the given shape. */
	void RemoveShape( reRigShape *shape );
	/** Removes all shapes. */
	void RemoveAllShapes();
	/*@}*/
	
	/** @name Constraints */
	/*@{*/
	/** Retrieves the number of constraints. */
	inline int GetConstraintCount() const{ return pConstraintCount; }
	/** Retrieves the constraint at the given index. */
	reRigConstraint *GetConstraintAt( int index ) const;
	/** Retrieves the constraint with the given collider or NULL if not found. */
	reRigConstraint *GetConstraintWith( deColliderVolume *collider ) const;
	/** Retrieves the index of the constraint or -1 if not found. */
	int IndexOfConstraint( reRigConstraint *constraint ) const;
	/** Determines if the constraint exists. */
	bool HasConstraint( reRigConstraint *constraint ) const;
	/** Adds a new constraint. */
	void AddConstraint( reRigConstraint *constraint );
	/** Removes the given constraint. */
	void RemoveConstraint( reRigConstraint *constraint );
	/** Removes all constraints. */
	void RemoveAllConstraints();
	/*@}*/
	
	/** @name Pushes */
	/*@{*/
	/** Retrieves the number of pushs. */
	inline int GetPushCount() const{ return pPushCount; }
	/** Retrieves the push at the given index. */
	reRigPush *GetPushAt( int index ) const;
	/** Retrieves the push with the given collider or NULL if not found. */
	reRigPush *GetPushWith( deColliderVolume *collider ) const;
	/** Retrieves the index of the push or -1 if not found. */
	int IndexOfPush( reRigPush *push ) const;
	/** Determines if the push exists. */
	bool HasPush( reRigPush *push ) const;
	/** Adds a new push. */
	void AddPush( reRigPush *push );
	/** Removes the given push. */
	void RemovePush( reRigPush *push );
	/** Removes all pushes. */
	void RemoveAllPushes();
	/*@}*/
	
	/** @name Notifiers */
	/*@{*/
	/** Retrieves the number of notifiers. */
	inline int GetNotifierCount() const{ return pNotifierCount; }
	/** Retrieves the notifier at the given index. */
	reRigNotifier *GetNotifierAt( int index ) const;
	/** Retrieves the index of the notifier or -1 if not found. */
	int IndexOfNotifier( reRigNotifier *notifier ) const;
	/** Determines if the notifier exists. */
	bool HasNotifier( reRigNotifier *notifier ) const;
	/** Adds a new notifier. */
	void AddNotifier( reRigNotifier *notifier );
	/** Removes the given notifier. */
	void RemoveNotifier( reRigNotifier *notifier );
	/** Removes all notifiers. */
	void RemoveAllNotifiers();
	
	/** Notifies all that the element or work mode changed. */
	void NotifyModeChanged();
	/** Notifies all that the changed or saved state changed. */
	virtual void NotifyStateChanged();
	/** Notifies all that the undos changed. */
	virtual void NotifyUndoChanged();
	
	/** Notifies all that the bone structure changed. */
	void NotifyAllStructureChanged();
	/** Notifies all that a rig parameter changed. */
	void NotifyRigChanged();
	/** Notifies all that the resource changed. */
	void NotifyResourceChanged();
	/** Notifies all that the sky changed. */
	void NotifySkyChanged();
	/** Notifies all that the environment object changed. */
	void NotifyEnvObjectChanged();
	/** Notifies all that the view changed. */
	void NotifyViewChanged();
	
	/** Notifies all that the camera changed. */
	void NotifyCameraChanged();
	/** Notifies all that the camera view changed. */
	void NotifyCameraViewChanged();
	
	/** Notifies all that the number of bones changed. */
	void NotifyBoneCountChanged();
	/** Notifies all that a bone changed. */
	void NotifyBoneChanged( reRigBone *bone );
	/** Notifies all that a bone changed selection state. */
	void NotifyBoneSelectedChanged( reRigBone *bone );
	/** Notifies all that all bones have been deselected. */
	void NotifyAllBonesDeselected();
	/** Notifies all that the active bone changed. */
	void NotifyActiveBoneChanged();
	/** Notifies all that a bone changed. */
	void NotifyAllBoneChanged( reRigBone *bone );
	
	/** Notifies all that the number of shapes changed. */
	void NotifyShapeCountChanged();
	/** Notifies all that a shape changed. */
	void NotifyShapeChanged( reRigShape *shape );
	/** Notifies all that a shape changed selection state. */
	void NotifyShapeSelectedChanged( reRigShape *shape );
	/** Notifies all that all shapes have been deselected. */
	void NotifyAllShapesDeselected();
	/** Notifies all that the active shape changed. */
	void NotifyActiveShapeChanged();
	/** Notifies all that a shape changed. */
	void NotifyAllShapeChanged( reRigShape *shape );
	
	/** Notifies all that the number of constraints changed. */
	void NotifyConstraintCountChanged();
	/** Notifies all that a constraint changed. */
	void NotifyConstraintChanged( reRigConstraint *constraint );
	/** Notifies all that a constraint changed selection state. */
	void NotifyConstraintSelectedChanged( reRigConstraint *constraint );
	/** Notifies all that all constraints have been deselected. */
	void NotifyAllConstraintsDeselected();
	/** Notifies all that the active constraint changed. */
	void NotifyActiveConstraintChanged();
	/** Notifies all that a constraint changed. */
	void NotifyAllConstraintChanged( reRigConstraint *constraint );
	/** Notifies all that a constraint degree of freedom changed. */
	void NotifyAllConstraintDofChanged( reRigConstraint *constraint, deColliderConstraint::eDegreesOfFreedom dof );
	
	/** Notifies all that the number of pushes changed. */
	void NotifyPushCountChanged();
	/** Notifies all that a push changed. */
	void NotifyPushChanged( reRigPush *push );
	/** Notifies all that a push changed selection state. */
	void NotifyPushSelectedChanged( reRigPush *push );
	/** Notifies all that all pushes have been deselected. */
	void NotifyAllPushesDeselected();
	/** Notifies all that the active push changed. */
	void NotifyActivePushChanged();
	/*@}*/
	
private:
	void pCleanUp();
	
	void pCreateWorld();
	void pCreateCamera();
	void pUpdateComponent();
	void pUpdateAnimator();
	void pUpdateAnimatorMove();
	void pUpdateAnimatorTime();
	
	void pBoneShowStateChanged();
	void pShapeShowStateChanged();
	void pConstraintShowStateChanged();
	void pPushShowStateChanged();
};

// end of include only once
#endif
