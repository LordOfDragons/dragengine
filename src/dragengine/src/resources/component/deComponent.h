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

#ifndef _DECOMPONENT_H_
#define _DECOMPONENT_H_

#include "../deResource.h"
#include "../model/deModel.h"
#include "../rig/deRig.h"
#include "../skin/deSkin.h"
#include "../skin/dynamic/deDynamicSkin.h"
#include "../occlusionmesh/deOcclusionMesh.h"
#include "../animator/deAnimator.h"
#include "../../parallel/deParallelTask.h"
#include "../../common/collection/decObjectOrderedSet.h"
#include "../../common/math/decMath.h"
#include "../../common/utils/decLayerMask.h"
#include "../../common/string/decString.h"
#include <dragengine/src/resources/decal/deDecal.h>


class deBaseAnimatorComponent;
class deBaseAudioComponent;
class deBaseGraphicComponent;
class deBasePhysicsComponent;
class deComponentBone;
class deComponentManager;
class deComponentTexture;
class deDecalList;
class deWorld;
class decShape;


/**
 * \brief Component Resource.
 * 
 * Combines various resources to represent an element visually, physically and audio wise
 * in the game world. Model textures are mapped to skin textures. Rig bones define the
 * available component bones. Model bones are mapped to Rig bones.
 */
class DE_DLL_EXPORT deComponent : public deResource{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deComponent> Ref;
	
	
public:
	/** \brief Movement hints. */
	enum eMovementHints{
		/** \brief Component remains static for the entire lifetime. */
		emhStationary,
		
		/** \brief Component remains mostly static jittering in a small area. */
		emhJittering,
		
		/** \brief Component moves around freely. */
		emhDynamic
	};
	
	
	
private:
	deModel::Ref pModel;
	deSkin::Ref pSkin;
	deRig::Ref pRig;
	deOcclusionMesh::Ref pOcclusionMesh;
	deModel::Ref pAudioModel;
	
	decDVector pPosition;
	decQuaternion pOrientation;
	decVector pScale;
	decDMatrix pMatrix;
	decDMatrix pInverseMatrix;
	bool pVisible;
	eMovementHints pHintMovement;
	bool pEnableGI;
	int pHintGIImportance;
	
	deAnimator::Ref pAnimator;
	
	decLayerMask pLayerMask;
	
	deComponentBone *pBones;
	int pBoneCount;
	
	float *pVertexPositionSetWeights;
	int pVertexPositionSetCount;
	
	bool pBonesDirty;
	bool pMatrixDirty;
	
	deComponentTexture *pTextures;
	int pTextureCount;
	
	deDynamicSkin::Ref pDynamicSkin;
	
	deDecal *pDecalRoot;
	deDecal::Ref pDecalTail;
	int pDecalCount;
	
	deParallelTask::Ref pAnimatorTask;
	
	deBaseGraphicComponent *pPeerGraphic;
	deBasePhysicsComponent *pPeerPhysics;
	deBaseAudioComponent *pPeerAudio;
	deBaseAnimatorComponent *pPeerAnimator;
	
	deWorld *pParentWorld;
	deComponent *pLLWorldPrev;
	deComponent *pLLWorldNext;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new component. */
	deComponent(deComponentManager *manager, deModel *model, deSkin *skin);
	
protected:
	/**
	 * \brief Clean up component.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	~deComponent() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set position. */
	void SetPosition(const decDVector &pos);
	
	/** \brief Orientation. */
	inline const decQuaternion &GetOrientation() const{ return pOrientation; }
	
	/** \brief Set orientation. */
	void SetOrientation(const decQuaternion &orientation);
	
	/** \brief Scaling. */
	inline const decVector &GetScaling() const{ return pScale; }
	
	/** \brief Set scaling. */
	void SetScaling(const decVector &scale);
	
	/** \brief Model or NULL if not assigned. */
	inline deModel *GetModel() const{ return pModel; }
	
	/**
	 * \brief Set model or NULL if not set.
	 * 
	 * Textures are reset. To keep textures use setModelKeepTextures.
	 */
	void SetModel(deModel *model);
	
	/**
	 * \brief Set model or NULL if not set while keeping textures.
	 * 
	 * Restores current state of textures after switching model if possible.
	 * Use this method instead of setModel if you set textures on the component
	 * and the new model is similar enough to match. Textures not existing anymore
	 * are not restored.
	 */
	void SetModelKeepTextures(deModel *model);
	
	/** \brief Skin or NULL if not assigned. */
	inline deSkin *GetSkin() const{ return pSkin; }
	
	/** \brief Set skin or NULL if not assigned. */
	void SetSkin(deSkin *skin);
	
	/** \brief Rig or NULL if not assigned. */
	inline deRig *GetRig() const{ return pRig; }
	
	/** \brief Set rig or NULL if not assigned. */
	void SetRig(deRig *rig);
	
	/** \brief Set model and skin at the same time. Either one can be NULL. */
	void SetModelAndSkin(deModel *model, deSkin *skin);
	
	/** \brief Occlusion mesh or NULL if not assigned. */
	inline deOcclusionMesh *GetOcclusionMesh() const{ return pOcclusionMesh; }
	
	/** \brief Set occlusion mesh or NULL if not assigned. */
	void SetOcclusionMesh(deOcclusionMesh *occmesh);
	
	/** \brief Audio model or NULL if not used. */
	inline deModel *GetAudioModel() const{ return pAudioModel; }
	
	/**
	 * \brief Set audio model or NULL if not set.
	 * 
	 * Shares textures with assigned model.
	 */
	void SetAudioModel(deModel *model);
	
	/** \brief Component is visible. */
	inline bool GetVisible() const{ return pVisible; }
	
	/** \brief Set if component is visible. */
	void SetVisible(bool visible);
	
	/** \brief Movement hint. */
	inline eMovementHints GetHintMovement() const{ return pHintMovement; }
	
	/** \brief Set movement hint. */
	void SetHintMovement(eMovementHints hint);
	
	/** \brief Enable GI in graphic module if supported. */
	bool GetEnableGI() const{ return pEnableGI; }
	
	/** \brief Set if enabled for GI in graphic module if supported. */
	void SetEnableGI(bool enable);
	
	/**
	 * \brief GI important hint.
	 * 
	 * Value is in the range from 0 (very unimportant) to 4 (very important). This hint
	 * can be used by the graphic module to improve performance by excluding components
	 * with a GI important below a user chosen threashold. The default important is 4.
	 */
	inline int GetHintGIImportance() const{ return pHintGIImportance; }
	
	/**
	 * \brief Set GI important hint.
	 * 
	 * Value is in the range from 0 (very unimportant) to 4 (very important). This hint
	 * can be used by the graphic module to improve performance by excluding components
	 * with a GI important below a user chosen threshold.
	 */
	void SetHintGIImportance(int importance);
	
	/** \brief Component matrix. */
	const decDMatrix &GetMatrix();
	
	/** \brief Inverse component matrix. */
	const decDMatrix &GetInverseMatrix();
	
	/** \brief Count of bones. */
	inline int GetBoneCount() const{ return pBoneCount; }
	
	/** \brief Bone at index. */
	deComponentBone &GetBoneAt(int index) const;
	
	/** \brief Updates the given bone. */
	void UpdateBoneAt(int index);
	
	/**
	 * \brief Mark bones dirty.
	 * 
	 * Mesh and extends are marked dirty too notifying peers if required.
	 */
	void InvalidateBones();
	
	/**
	 * \brief Mark bones no more dirty.
	 * 
	 * No other action is carried out except setting the dirty flag for bones. Use this
	 * function if you updated all bones on your own (typically physics modules during
	 * simulation). The mesh and extends dirty flag keep their values. You have to call
	 * InvalidateMesh function after validating bones to ensure they are updated correctly
	 * during the next update step.
	 */
	void ValidateBones();
	
	/** \brief Vertex position set count. */
	inline int GetVertexPositionSetCount() const{ return pVertexPositionSetCount; }
	
	/** \brief Vertex position set weight. */
	float GetVertexPositionSetWeightAt(int index) const;
	
	/** \brief Set vertex position set weight. */
	void SetVertexPositionSetWeightAt(int index, float weight);
	
	/** \brief Mark mesh dirty. */
	void InvalidateMesh();
	
	/** \brief Mark extends dirty. */
	void InvalidateExtends();
	
	/** \brief Copy bone states to matching bones states of another component. */
	void CopyBonesToComponent(deComponent& component);
	
	/** \brief Dynamic skin or null if not used. */
	inline deDynamicSkin *GetDynamicSkin() const{ return pDynamicSkin; }
	
	/** \brief Set dynamic skin or null if not used. */
	void SetDynamicSkin(deDynamicSkin *dynamicSkin);
	
	/** \brief Render only animator or NULL to use none. */
	inline deAnimator *GetAnimator() const{ return pAnimator; }
	
	/** \brief Set render only animator or NULL to use none. */
	void SetAnimator(deAnimator *animator);
	
	/** \brief Layer mask. */
	inline const decLayerMask &GetLayerMask() const{ return pLayerMask; }
	
	/** \brief Set layer mask. */
	void SetLayerMask(const decLayerMask &layerMask);
	
	/** \brief Prepare matrices. */
	void PrepareMatrix();
	
	/** \brief Prepare bone matrices. */
	void PrepareBones();
	
	/*@}*/
	
	
	
	/** \name Animator Tasks */
	/*@{*/
	/**
	 * \brief Animator task or NULL if none is pending affecting this component.
	 * 
	 * Animator tasks can be created by Animator Modules to calculate bone states
	 * in parallel. Multiple animator tasks can affect a component but only the
	 * last one is stored in the component itself. If a task exists and PrepareBones()
	 * is called (or a function using it) the animator task is waited on to finish
	 * before continueing. Only Animator Modules use this function.
	 */
	inline deParallelTask *GetAnimatorTask() const{ return pAnimatorTask; }
	
	/**
	 * \brief Set animator task or NULL if none is pending affecting this component.
	 * 
	 * Animator tasks can be created by Animator Modules to calculate bone states
	 * in parallel. Multiple animator tasks can affect a component but only the
	 * last one is stored in the component itself. If a task exists and PrepareBones()
	 * is called (or a function using it) the animator task is waited on to finish
	 * before continueing. Only Animator Modules use this function.
	 */
	void SetAnimatorTask(deParallelTask *task);
	
	/**
	 * \brief Ensure animator task has finished if present.
	 * 
	 * For use by modules requiring to access data outside using Prepare*() calls.
	 */
	void WaitAnimatorTaskFinished();
	/*@}*/
	
	
	
	/** \name Textures */
	/*@{*/
	/** \brief Number of textures. */
	inline int GetTextureCount() const{ return pTextureCount; }
	
	/** \brief Texture with the given index. */
	deComponentTexture &GetTextureAt(int index) const;
	
	/** \brief Retrieve index of the texture of the face closest to a position or -1 if not found. */
	int IndexOfTextureClosestTo(const decVector &position, float radius) const;
	
	/** \brief Notifies the peers that a texture changed. */
	void NotifyTextureChanged(int index) const;
	/*@}*/
	
	
	
	/** \name Decals Management */
	/*@{*/
	/** \brief Number of decals. */
	inline int GetDecalCount() const{ return pDecalCount; }
	
	/** \brief Root decal or NULL if there are none. */
	inline deDecal *GetRootDecal() const{ return pDecalRoot; }
	
	/**
	 * \brief Add decal.
	 * \throws deeInvalidParam \em decal is NULL.
	 * \throws deeInvalidParam \em decal has a parent world.
	 */
	void AddDecal(deDecal *decal);
	
	/**
	 * \brief Remove decal.
	 * \throws deeInvalidParam \em decal is NULL.
	 * \throws deeInvalidParam Parent world of \em decal is not this world.
	 */
	void RemoveDecal(deDecal *decal);
	
	/** \brief Remove all decals. */
	void RemoveAllDecals();
	/*@}*/
	
	
	
	/** \name Collision Detection */
	/*@{*/
	/**
	 * \brief Collect decals in contact with point.
	 * 
	 * The point is best taken from an earlier collision test and is in world
	 * coordinates. The point is considered to be in the same sector as the component.
	 * 
	 * \param[in] point Point on the surface to test.
	 * \param[out] list List of decals to fill in. Not emptied before filling.
	 */
	void FindDecalsAt(const decVector &point, deDecalList &list) const;
	
	/**
	 * \brief Collect decals in contact with shape.
	 * 
	 * The shape is considered to be in the same sector as the component.
	 * 
	 * \param[in] shape Shape to test.
	 * \param[out] list List of decals to fill in. Not emptied before filling.
	 */
	void FindDecalsTouching(decShape *shape, deDecalList &list) const;
	/*@}*/
	
	
	
	/** \name System Peers */
	/*@{*/
	/** \brief Graphic System Peer. */
	inline deBaseGraphicComponent *GetPeerGraphic() const{ return pPeerGraphic; }
	
	/** \brief Set graphic System Peer. */
	void SetPeerGraphic(deBaseGraphicComponent *peer);
	
	/** \brief Physics System Peer. */
	inline deBasePhysicsComponent *GetPeerPhysics() const{ return pPeerPhysics; }
	
	/** \brief Set physics System Peer. */
	void SetPeerPhysics(deBasePhysicsComponent *peer);
	
	/** \brief Audio System Peer. */
	inline deBaseAudioComponent *GetPeerAudio() const{ return pPeerAudio; }
	
	/** \brief Set audio System Peer. */
	void SetPeerAudio(deBaseAudioComponent *peer);
	
	/** \brief Animator peer or NULL if not set. */
	inline deBaseAnimatorComponent *GetPeerAnimator() const{ return pPeerAnimator; }
	
	/** \brief Set animator peer or NULL if not set. */
	void SetPeerAnimator(deBaseAnimatorComponent *peer);
	/*@}*/
	
	
	
	/** \name Linked List */
	/*@{*/
	/** \brief Parent world or NULL. */
	inline deWorld *GetParentWorld() const{ return pParentWorld; }
	
	/** \brief Set parent world or NULL. */
	void SetParentWorld(deWorld *world);
	
	/** \brief Previous component in the parent world linked list. */
	inline deComponent *GetLLWorldPrev() const{ return pLLWorldPrev; }
	
	/** \brief Set next component in the parent world linked list. */
	void SetLLWorldPrev(deComponent *component);
	
	/** \brief Next component in the parent world linked list. */
	inline deComponent *GetLLWorldNext() const{ return pLLWorldNext; }
	
	/** \brief Set next component in the parent world linked list. */
	void SetLLWorldNext(deComponent *component);
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pUpdateBoneAt(int bone);
	void pChangeModel(deModel *model);
	void pChangeRig(deRig *rig);
};

#endif
