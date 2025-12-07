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

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decIntList.h>
#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decStringList.h>
#include <dragengine/common/utils/decLayerMask.h>

class deoalAComponentBone;
class deoalAComponentTexture;
class deoalAModel;
class deoalASkin;
class deoalAudioThread;
class deoalAWorld;
class deoalComponentOctree;
class deoalModelFace;
class deoalModelOctree;
class deoalModelRTBVH;
class deoalWorldOctree;

class deComponent;



/**
 * \brief Component resource peer.
 */
class deoalAComponent : public deObject{
private:
	deoalAudioThread &pAudioThread;
	
	decObjectList pTextures;
	
	deoalAWorld *pParentWorld;
	deoalWorldOctree *pOctreeNode;
	
	decIntList pModelTextureMappings;
	decIntList pTextureSkinMappings;
	decIntList pModelRigMappings;
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
	
	deoalAComponentBone *pBones;
	int pBoneCount;
	decMatrix *pWeightMatrices;
	
	deoalModelFace *pFaces;
	int pFaceCount;
// 	deoalModelOctree *pOctree;
	deoalModelRTBVH *pBVH;
	decVector pDynamicMinExtend;
	decVector pDynamicMaxExtend;
	
	bool pWorldMarkedRemove;
	deoalAComponent *pLLWorldPrev;
	deoalAComponent *pLLWorldNext;
	
	
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deoalAComponent> Ref;


	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create component peer. */
	deoalAComponent(deoalAudioThread &audioThread);
	
protected:
	/** \brief Clean up component peer. */
	virtual ~deoalAComponent();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Module. */
	inline deoalAudioThread &GetAudioThread() const{ return pAudioThread; }
	
	/** \brief Parent world or NULL. */
	inline deoalAWorld *GetParentWorld() const{ return pParentWorld; }
	
	/** \brief Set parent world or NULL. */
	void SetParentWorld(deoalAWorld *world);
	
	
	
	/** \brief World octree node or NULL. */
	inline deoalWorldOctree *GetOctreeNode() const{ return pOctreeNode; }
	
	/** \brief Set world octree node or NULL. */
	void SetOctreeNode(deoalWorldOctree *node);
	
	/** \brief Update octree node. */
	void UpdateOctreeNode();
	
	/** \brief Quick dispose. */
	void PrepareQuickDispose();
	
	
	
	/** \brief Model. */
	inline const deoalAModel::Ref &GetModel() const{ return pModel; }
	
	/** \brief Set model. */
	void SetModel(deoalAModel *model, const decStringList *textureNames);
	
	/** \brief Skin. */
	inline const deoalASkin::Ref &GetSkin() const{ return pSkin; }
	
	/** \brief Set skin. */
	void SetSkin(deoalASkin *skin);
	
	/** \brief Texture names. */
	inline const decStringList *GetTextureNames() const{ return pTextureNames; }
	
	/** \brief Model to texture mappings. */
	inline const decIntList &GetModelTextureMappings() const{ return pModelTextureMappings; }
	
	/** \brief Texture to skin mappings. */
	inline const decIntList &GetTextureSkinMappings() const{ return pTextureSkinMappings; }
	
	/** \brief Model to rig mappings. */
	inline const decIntList &GetModelRigMappings() const{ return pModelRigMappings; }
	
	
	
	/** \brief Number of textures. */
	int GetTextureCount() const;
	
	/** \brief Texture at index. */
	deoalAComponentTexture &GetTextureAt(int index) const;
	
	/**
	 * \brief Texture at index mapped from model.
	 * 
	 * Short version for calling GetTextureAt(GetModelTextureMappings().GetAt(index)).
	 */
	deoalAComponentTexture &GetModelTextureAt(int index) const;
	
	/** \brief Add texture. */
	void AddTexture(deoalAComponentTexture *texture);
	
	/** \brief Remove all textures. */
	void RemoveAllTextures();
	
	
	
	/** \brief Position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	
	/** \brief Orientation. */
	inline const decQuaternion &GetOrientation() const{ return pOrientation; }
	
	/** \brief Scaling. */
	inline const decVector &GetScaling() const{ return pScaling; }
	
	/** \brief Set geometry. */
	void SetGeometry(const decDVector &position, const decQuaternion &orientation,
		const decVector &scaling);
	
	
	
	/** \brief Layer mask. */
	inline const decLayerMask &GetLayerMask() const{ return pLayerMask; }
	
	/** \brief Set layer mask. */
	void SetLayerMask(const decLayerMask &layerMask);
	
	
	
	/** \brief Minimum extend. */
	inline const decDVector &GetMinExtend() const{ return pMinExtend; }
	
	/** \brief Maximum extend. */
	inline const decDVector &GetMaxExtend() const{ return pMaxExtend; }
	
	/** \brief Box center. */
	inline const decDVector &GetBoxCenter() const{ return pBoxCenter; }
	
	/** \brief Box half extends. */
	inline const decDVector &GetBoxHalfExtends() const{ return pBoxHalfExtends; }
	
	/** \brief Ray tracing sphere center. */
// 	inline const decDVector &GetRTSphereCenter() const{ return pRTSphereCenter; }
	
	/** \brief Ray tracing sphere radius squared. */
// 	inline double GetRTSphereRadiusSquared() const{ return pRTSphereRadiusSquared; }
	
	/** \brief Matrix. */
	inline const decDMatrix &GetMatrix() const{ return pMatrix; }
	
	/** \brief Inverse matrix. */
	inline const decDMatrix &GetInverseMatrix() const{ return pInvMatrix; }
	
	
	
	/** \brief One or more texture is present and affects sound. */
	inline bool GetAffectsSound() const{ return pAffectsSound; }
	
	/** \brief Update affects sound. */
	void UpdateAffectsSound();
	
	/** \brief Component hjas scaling other than 1. */
	inline bool GetHasScaling() const{ return pHasScaling; }
	
	
	
	/** \brief Initialize bones. */
	void InitBones(const deComponent &component);
	
	/** \brief Update bone geometry. */
	void UpdateBoneGeometry(const deComponent &component);
	
	/** \brief Dynamic octree or \em NULL to use static model octree. */
// 	inline deoalModelOctree *GetOctree() const{ return pOctree; }
	
	/** \brief Dynamic BVH or \em NULL to use static model octree. */
	inline deoalModelRTBVH *GetBVH() const{ return pBVH; }
	
	/**
	 * \brief Prepare octree.
	 * 
	 * Prepares static model octree or dynamic octree.
	 */
	void PrepareOctree();
	
	/** \brief Number of faces. */
	inline int GetFaceCount() const{ return pFaceCount; }
	
	/** \brief Face at index. */
	const deoalModelFace &GetFaceAt(int index) const;
	/*@}*/
	
	
	
	/** \name Render world usage */
	/*@{*/
	/**
	 * \brief Marked for removal.
	 * \details For use by deoglRWorld only. Non-thread safe.
	 */
	inline bool GetWorldMarkedRemove() const{ return pWorldMarkedRemove; }
	
	/**
	 * \brief Set marked for removal.
	 * \details For use by deoglRWorld only. Non-thread safe.
	 */
	void SetWorldMarkedRemove(bool marked);
		
	/** \brief Linked list world previous. */
	inline deoalAComponent *GetLLWorldPrev() const{ return pLLWorldPrev; }
	
	/** \brief Set linked list world previous. */
	void SetLLWorldPrev(deoalAComponent *component);
	
	/** \brief Linked list world next. */
	inline deoalAComponent *GetLLWorldNext() const{ return pLLWorldNext; }
	
	/** \brief Set linked list world next. */
	void SetLLWorldNext(deoalAComponent *component);
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
