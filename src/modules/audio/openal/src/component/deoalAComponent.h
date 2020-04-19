/* 
 * Drag[en]gine OpenAL Audio Module
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
	deoalAModel *pModel;
	deoalASkin *pSkin;
	
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
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create component peer. */
	deoalAComponent( deoalAudioThread &audioThread );
	
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
	void SetParentWorld( deoalAWorld *world );
	
	
	
	/** \brief World octree node or NULL. */
	inline deoalWorldOctree *GetOctreeNode() const{ return pOctreeNode; }
	
	/** \brief Set world octree node or NULL. */
	void SetOctreeNode( deoalWorldOctree *node );
	
	/** \brief Update octree node. */
	void UpdateOctreeNode();
	
	/** \brief Quick dispose. */
	void PrepareQuickDispose();
	
	
	
	/** \brief Model. */
	inline deoalAModel *GetModel() const{ return pModel; }
	
	/** \brief Set model. */
	void SetModel( deoalAModel *model, const decStringList *textureNames );
	
	/** \brief Skin. */
	inline deoalASkin *GetSkin() const{ return pSkin; }
	
	/** \brief Set skin. */
	void SetSkin( deoalASkin *skin );
	
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
	deoalAComponentTexture &GetTextureAt( int index ) const;
	
	/**
	 * \brief Texture at index mapped from model.
	 * 
	 * Short version for calling GetTextureAt(GetModelTextureMappings().GetAt(index)).
	 */
	deoalAComponentTexture &GetModelTextureAt( int index ) const;
	
	/** \brief Add texture. */
	void AddTexture( deoalAComponentTexture *texture );
	
	/** \brief Remove all textures. */
	void RemoveAllTextures();
	
	
	
	/** \brief Position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	
	/** \brief Orientation. */
	inline const decQuaternion &GetOrientation() const{ return pOrientation; }
	
	/** \brief Scaling. */
	inline const decVector &GetScaling() const{ return pScaling; }
	
	/** \brief Set geometry. */
	void SetGeometry( const decDVector &position, const decQuaternion &orientation,
		const decVector &scaling );
	
	
	
	/** \brief Layer mask. */
	inline const decLayerMask &GetLayerMask() const{ return pLayerMask; }
	
	/** \brief Set layer mask. */
	void SetLayerMask( const decLayerMask &layerMask );
	
	
	
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
	void InitBones( const deComponent &component );
	
	/** \brief Update bone geometry. */
	void UpdateBoneGeometry( const deComponent &component );
	
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
	const deoalModelFace &GetFaceAt( int index ) const;
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
	void SetWorldMarkedRemove( bool marked );
		
	/** \brief Linked list world previous. */
	inline deoalAComponent *GetLLWorldPrev() const{ return pLLWorldPrev; }
	
	/** \brief Set linked list world previous. */
	void SetLLWorldPrev( deoalAComponent *component );
	
	/** \brief Linked list world next. */
	inline deoalAComponent *GetLLWorldNext() const{ return pLLWorldNext; }
	
	/** \brief Set linked list world next. */
	void SetLLWorldNext( deoalAComponent *component );
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
	
	bool pIgnoreBySize( const decVector &size ) const;
};

#endif
