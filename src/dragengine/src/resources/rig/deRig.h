/* 
 * Drag[en]gine Game Engine
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

#ifndef _DERIG_H_
#define _DERIG_H_

#include "../deFileResource.h"
#include "../../common/math/decMath.h"
#include "../../common/shape/decShapeList.h"
#include "../../common/string/decStringList.h"

class deRigBone;
class deRigManager;
class deBasePhysicsRig;


/**
 * \brief Rig file resource.
 *
 * Rigs define the sceleton of a static or articulated physics bodies. Animations and
 * components are mapped to a rig for animation. Rigs are also used by the physics
 * system to provide collision detection and physical simulations. For the physics
 * system the root bone is mapped to the component collider. If this bone is missing
 * the simulation produces incorrect behavior.
 * 
 * \par Shapes
 * Shapes define the physical shape of resource and are only used by the physics system.
 * See decShapeList for information about the available shape types. Shapes can be
 * defined for bones as well as the rig as a whole. For static rigs shapes are only
 * placed on the rig itself. For articulated rigs shapes are only placed on bines. Shapes
 * on the entire rig are ignored for articulated rigs.
 * 
 * Shapes can be assigned a single shape property string. This allows to transport some
 * additional game specific information with individual shapes of a rig. A typical use
 * case is modifying the material sound for certain shapes. The user can choose the
 * content and format of the string. By default all shapes have an empty string assigned.
 * 
 * \par Model Collision
 * By default rig and bones use shapes to define the collision volumes. It is possible
 * to use the model of the component (if a deColliderComponent) if present. If model
 * collision is used all shapes for rigs and bones are ignored and the model hull is
 * used as collision volume. This allows for a fine grained and dynamic collision
 * volume but is slower than shape collision and can lead to strange results if model
 * faces intersect due to animation. Non-deformable models provide safer results and
 * and faster than deformable models. If you can always use shapes since they provide
 * the fastest and most stable behavior.
 * 
 */
class deRig : public deFileResource{
private:
	deRigBone **pBones;
	int pBoneCount;
	int pBoneSize;
	
	int pRootBone;
	decVector pCMP;
	bool pModelCollision;
	
	decShapeList pShapes;
	decStringList pShapeProperties;
	
	deBasePhysicsRig *pPeerPhysics;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create rig. */
	deRig( deRigManager *resourceManager, deVirtualFileSystem *vfs,
		const char *filename, TIME_SYSTEM modificationTime );
	
protected:
	/**
	 * \brief Clean up rig.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deRig();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Central mass point position. */
	inline const decVector &GetCentralMassPoint() const{ return pCMP; }
	
	/** \brief Set central mass point position. */
	void SetCentralMassPoint( const decVector &cmp );
	
	/** \brief Use component model for collision instead of shapes if present. */
	inline bool GetModelCollision() const{ return pModelCollision; }
	
	/** \brief Set if component model is used for collision instead of shapes if present. */
	void SetModelCollision( bool modelCollision );
	
	
	
	/** \brief Verify rig is valid. */
	bool Verify() const;
	
	/** \brief Prepare rig after loading. */
	void Prepare();
	/*@}*/
	
	
	
	/** \name Bones */
	/*@{*/
	/** \brief Number of bones. */
	inline int GetBoneCount() const{ return pBoneCount; }
	
	/**
	 * \brief Bone at index.
	 * \throws deeOutOfBoundary \em index is less than 0 or greater than or equal to GetBoneCount().
	 */
	deRigBone &GetBoneAt( int index ) const;
	
	/** \brief Index of named bone or -1 if absent. */
	int IndexOfBoneNamed( const char *name ) const;
	
	/** \brief Named bone is present. */
	bool HasBoneNamed( const char *name ) const;
	
	/**
	 * \brief Add bone.
	 * \throws deeInvalidParam Named bone is present.
	 */
	void AddBone( deRigBone *bone );
	
	/** \brief Remove all bones. */
	void RemoveAllBones();
	
	/** \brief Index of physics root bone or -1 if not set. */
	inline int GetRootBone() const{ return pRootBone; }
	
	/**
	 * \brief Set physics root bone or -1 if not set.
	 * \throws deeOutOfBoundary \em index is less than -1 or greater than or equal to GetBoneCount().
	 */
	void SetRootBone( int rootBone );
	/*@}*/
	
	
	
	/** \name Shapes */
	/*@{*/
	/** \brief Shapes. */
	inline const decShapeList &GetShapes() const{ return pShapes; }
	
	/**
	 * \brief Set shapes.
	 * 
	 * Resets shape properties to empty strings for all shapes.
	 */
	void SetShapes( const decShapeList &shapes );
	
	/** \brief Shape properties. */
	inline const decStringList &GetShapeProperties() const{ return pShapeProperties; }
	
	/**
	 * \brief Set shape properties.
	 * \throws deeInvalidParam Number of strings in \em properties does not match GetShapes().GetCount().
	 */
	void SetShapeProperties( const decStringList &properties );
	/*@}*/
	
	
	
	/** \name System Peers */
	/*@{*/
	/** \brief Physics system peer or NULL if not set. */
	inline deBasePhysicsRig *GetPeerPhysics() const{ return pPeerPhysics; }
	
	/** \brief Set physics system peer or NULL if not set. */
	void SetPeerPhysics( deBasePhysicsRig *peer );
	/*@}*/
};

#endif
