/* 
 * Drag[en]gine Bullet Physics Module
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

#ifndef _DEBPCOMPONENT_H_
#define _DEBPCOMPONENT_H_

#include <dragengine/common/collection/decIntList.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/systems/modules/physics/deBasePhysicsComponent.h>

class dePhysicsBullet;
class deComponent;
class debpWorld;
class debpColliderComponent;
class debpModel;



/**
 * \brief Bullet component peer.
 */
class debpComponent : public deBasePhysicsComponent{
public:
	struct sBone{
		bool dirty;
		decMatrix weightMatrix;
	};
	
	
	
private:
	dePhysicsBullet &pBullet;
	
	deComponent *pComponent;
	decVector pMinExtend;
	decVector pMaxExtend;
	int pIndex;
	bool pDirtyModel;
	bool pDirtyMesh;
	bool pDirtyExtends;
	bool pDirtyBoneWeights;
	bool pDirtyWeights;
	bool pEnabled;
	
	sBone *pBones;
	int pBoneCount;
	debpModel *pModel;
	
	decIntList pModelRigMappings;
	bool pDirtyModelRigMappings;
	decVector *pVertices;
	decMatrix *pWeights;
	int pVertexCount;
	int pWeightsCount;
	
	debpColliderComponent *pLinkedCollider;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create peer. */
	debpComponent( dePhysicsBullet &bullet, deComponent *comp );
	
	/** \brief Clean up peer. */
	~debpComponent();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Bullet module. */
	inline dePhysicsBullet &GetBullet() const{ return pBullet; }
	
	/** \brief Component resource. */
	inline deComponent *GetComponent() const{ return pComponent; }
	
	/** \brief Component is enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	/** \brief Set if component is enabled. */
	void SetEnabled( bool enabled );
	
	/** \brief Minimum extends. */
	inline const decVector &GetMinimumExtend() const{ return pMinExtend; }
	
	/** \brief Maximum extends. */
	inline const decVector &GetMaximumExtend() const{ return pMaxExtend; }
	
	inline int GetIndex() const{ return pIndex; }
	void SetIndex( int index );
	
	/** \brief Model or NULL. */
	inline debpModel *GetModel() const{ return pModel; }
	
	/** \brief Prepare mesh. */
	void PrepareMesh();
	
	/** \brief Prepare extends. */
	void PrepareExtends();
	
	/** \brief Prepare bone weights. */
	void PrepareBoneWeights();
	
	/** \brief Prepare weights. */
	void PrepareWeights();
	
	/** \brief Weights at index. */
	const decMatrix &GetWeights( int index ) const;
	
	/** \brief Vertex at index. */
	const decVector &GetVertex( int index ) const;
	
	/** Retrieves the linked collider or NULL if not linked. */
	inline debpColliderComponent *GetLinkedCollider() const{ return pLinkedCollider; }
	
	/** Sets the linked collider or NULL if not linked. */
	void SetLinkedCollider( debpColliderComponent *collider );
	
	
	
	/** \brief Bone. */
	sBone &GetBoneAt( int index ) const;
	
	/** \brief Prepare bone. */
	void PrepareBone( int index );
	
	/** \brief Set dirty flag of all bones. */
	void SetAllBoneDirty();
	
	/** \brief Clear dirty flag of all bones. */
	void ClearAllBoneDirty();
	
	/** \brief Model to rig mapping for bone. */
	int GetModelRigMapping( int bone ) const;
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Position changed. */
	virtual void PositionChanged();
	
	/** \brief Scaling changed. */
	virtual void ScalingChanged();
	
	/** \brief Orientation changed. */
	virtual void OrientationChanged();
	
	/** \brief Model object changed. */
	virtual void ModelChanged();
	
	/** \brief Rig object changed. */
	virtual void RigChanged();
	
	/** \brief Visitility changed. */
	virtual void VisibilityChanged();
	
	/** \brief Extends changed. */
	virtual void ExtendsChanged();
	
	/**
	 * \brief Mesh vertices have been invalidated.
	 * 
	 * Called if Model changed or bones have been invalidated.
	 */
	virtual void MeshDirty();
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
	virtual void FindDecalsAt( const decVector &point, deDecalList &list );
	
	/**
	 * \brief Collect decals in contact with shape.
	 * 
	 * The shape is considered to be in the same sector as the component.
	 * 
	 * \param[in] shape Shape to test.
	 * \param[out] list List of decals to fill in. Not emptied before filling.
	 */
	virtual void FindDecalsTouching( decShape *shape, deDecalList &list );
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pRebuildBoneArrays();
	void pChangeModel();
	void pUpdateModelRigMappings();
	void pPrepareBone( int index );
};

#endif
