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

#ifndef _DEOALACOMPONENT_H_
#define _DEOALACOMPONENT_H_

#include "../model/deoalAModel.h"
#include "../skin/deoalASkin.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decTList.h>
#include <dragengine/common/collection/decTLinkedList.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decStringList.h>
#include <dragengine/common/utils/decLayerMask.h>

class deoalAComponentBone;
class deoalAComponentTexture;
class deoalAudioThread;
class deoalAWorld;
class deoalComponentOctree;
class deoalModelFace;
class deoalModelOctree;
class deoalModelRTBVH;
class deoalWorldOctree;

class deComponent;



/**
 * Component resource peer.
 */
class deoalAComponent : public deObject{
private:
	deoalAudioThread &pAudioThread;
	
	decTObjectList<deoalAComponentTexture> pTextures;
	
	deoalAWorld *pParentWorld;
	deoalWorldOctree *pOctreeNode;
	
	decTList<int> pModelTextureMappings;
	decTList<int> pTextureSkinMappings;
	decTList<int> pModelRigMappings;
	const decStringList *pTextureNames;
	deoalAModel::Ref pModel;
	deoalASkin::Ref pSkin;
	
	decDVector pPosition;
	decQuaternion pOrientation;
	decVector pScaling;
	
	decLayerMask pLayerMask;
	
	decDVector pMinExtend;
	decDVector pMaxExtend;
	decDVector pBoxCenter;
	decDVector pBoxHalfExtends;
// 	decDVector pRTSphereCenter;
// 	double pRTSphereRadiusSquared;
	
	decDMatrix pMatrix;
	decDMatrix pInvMatrix;
	
	bool pAffectsSound;
	bool pHasScaling;
	
	bool pDirtyMatrices;
	bool pDirtyWeightMatrices;
	bool pDirtyExtends;
// 	bool pDirtyRTSphere;
	bool pDirtyTextureUseSkin;
	
	decTList<deoalAComponentBone> pBones;
	decTList<decMatrix> pWeightMatrices;
	decTList<deoalModelFace> pFaces;
// 	deoalModelOctree *pOctree;
	deoalModelRTBVH *pBVH;
	decVector pDynamicMinExtend;
	decVector pDynamicMaxExtend;
	
	bool pWorldMarkedRemove;
	decTObjectLinkedList<deoalAComponent>::Element pLLWorld;
	
	
	
public:
	/** Type holding strong reference. */
	using Ref = deTObjectReference<deoalAComponent>;


	/** \name Constructors and Destructors */
	/*@{*/
	/** Create component peer. */
	deoalAComponent(deoalAudioThread &audioThread);
	
protected:
	/** Clean up component peer. */
	~deoalAComponent() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Module. */
	inline deoalAudioThread &GetAudioThread() const{ return pAudioThread; }
	
	/** Parent world or NULL. */
	inline deoalAWorld *GetParentWorld() const{ return pParentWorld; }
	
	/** Set parent world or NULL. */
	void SetParentWorld(deoalAWorld *world);
	
	
	
	/** World octree node or NULL. */
	inline deoalWorldOctree *GetOctreeNode() const{ return pOctreeNode; }
	
	/** Set world octree node or NULL. */
	void SetOctreeNode(deoalWorldOctree *node);
	
	/** Update octree node. */
	void UpdateOctreeNode();
	
	/** Quick dispose. */
	void PrepareQuickDispose();
	
	
	
	/** Model. */
	inline const deoalAModel::Ref &GetModel() const{ return pModel; }
	
	/** Set model. */
	void SetModel(deoalAModel *model, const decStringList *textureNames);
	
	/** Skin. */
	inline const deoalASkin::Ref &GetSkin() const{ return pSkin; }
	
	/** Set skin. */
	void SetSkin(deoalASkin *skin);
	
	/** Texture names. */
	inline const decStringList *GetTextureNames() const{ return pTextureNames; }
	
	/** Model to texture mappings. */
	inline const decTList<int> &GetModelTextureMappings() const{ return pModelTextureMappings; }
	
	/** Texture to skin mappings. */
	inline const decTList<int> &GetTextureSkinMappings() const{ return pTextureSkinMappings; }
	
	/** Model to rig mappings. */
	inline const decTList<int> &GetModelRigMappings() const{ return pModelRigMappings; }
	
	
	
	/** Number of textures. */
	int GetTextureCount() const;
	
	/** Texture at index. */
	deoalAComponentTexture &GetTextureAt(int index) const;
	
	/**
	 * Texture at index mapped from model.
	 * 
	 * Short version for calling GetTextureAt(GetModelTextureMappings().GetAt(index)).
	 */
	deoalAComponentTexture &GetModelTextureAt(int index) const;
	
	/** Add texture. */
	void AddTexture(deoalAComponentTexture *texture);
	
	/** Remove all textures. */
	void RemoveAllTextures();
	
	
	
	/** Position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	
	/** Orientation. */
	inline const decQuaternion &GetOrientation() const{ return pOrientation; }
	
	/** Scaling. */
	inline const decVector &GetScaling() const{ return pScaling; }
	
	/** Set geometry. */
	void SetGeometry(const decDVector &position, const decQuaternion &orientation,
		const decVector &scaling);
	
	
	
	/** Layer mask. */
	inline const decLayerMask &GetLayerMask() const{ return pLayerMask; }
	
	/** Set layer mask. */
	void SetLayerMask(const decLayerMask &layerMask);
	
	
	
	/** Minimum extend. */
	inline const decDVector &GetMinExtend() const{ return pMinExtend; }
	
	/** Maximum extend. */
	inline const decDVector &GetMaxExtend() const{ return pMaxExtend; }
	
	/** Box center. */
	inline const decDVector &GetBoxCenter() const{ return pBoxCenter; }
	
	/** Box half extends. */
	inline const decDVector &GetBoxHalfExtends() const{ return pBoxHalfExtends; }
	
	/** Ray tracing sphere center. */
// 	inline const decDVector &GetRTSphereCenter() const{ return pRTSphereCenter; }
	
	/** Ray tracing sphere radius squared. */
// 	inline double GetRTSphereRadiusSquared() const{ return pRTSphereRadiusSquared; }
	
	/** Matrix. */
	inline const decDMatrix &GetMatrix() const{ return pMatrix; }
	
	/** Inverse matrix. */
	inline const decDMatrix &GetInverseMatrix() const{ return pInvMatrix; }
	
	
	
	/** One or more texture is present and affects sound. */
	inline bool GetAffectsSound() const{ return pAffectsSound; }
	
	/** Update affects sound. */
	void UpdateAffectsSound();
	
	/** Component hjas scaling other than 1. */
	inline bool GetHasScaling() const{ return pHasScaling; }
	
	
	
	/** Initialize bones. */
	void InitBones(const deComponent &component);
	
	/** Update bone geometry. */
	void UpdateBoneGeometry(const deComponent &component);
	
	/** Dynamic octree or \em NULL to use static model octree. */
// 	inline deoalModelOctree *GetOctree() const{ return pOctree; }
	
	/** Dynamic BVH or \em NULL to use static model octree. */
	inline deoalModelRTBVH *GetBVH() const{ return pBVH; }
	
	/**
	 * Prepare octree.
	 * 
	 * Prepares static model octree or dynamic octree.
	 */
	void PrepareOctree();
	
	/** Faces. */
	inline const decTList<deoalModelFace> &GetFaces() const{ return pFaces; }
	/*@}*/
	
	
	
	/** \name Render world usage */
	/*@{*/
	/**
	 * Marked for removal.
	 * \details For use by deoglRWorld only. Non-thread safe.
	 */
	inline bool GetWorldMarkedRemove() const{ return pWorldMarkedRemove; }
	
	/**
	 * Set marked for removal.
	 * \details For use by deoglRWorld only. Non-thread safe.
	 */
	void SetWorldMarkedRemove(bool marked);
		
	/** Linked list world element. */
	inline decTObjectLinkedList<deoalAComponent>::Element &GetLLWorld(){ return pLLWorld; }
	inline const decTObjectLinkedList<deoalAComponent>::Element &GetLLWorld() const{ return pLLWorld; }
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pUpdateModelTextureMappings();
	void pResizeModelTextureMappings();
	void pUpdateTextureSkinMappings();
	void pResizeTextureSkinMappings();
	void pUpdateModelRigMappings();
	void pResizeModelRigMappings();
	
	void pPrepareMatrices();
	void pPrepareExtends();
// 	void pPrepareRTSphere();
	
	void pPrepareBoneMatrices();
	void pPrepareWeightMatrices();
	void pDropWeightMatrices();
	
	void pUpdateFaces();
	void pDropFaces();
	void pBuildOctree();
	void pDropOctree();
	
	bool pIgnoreBySize(const decVector &size) const;
};

#endif
