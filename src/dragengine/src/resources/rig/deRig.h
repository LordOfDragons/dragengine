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
class DE_DLL_EXPORT deRig : public deFileResource{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deRig> Ref;
	
	
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
	deRig(deRigManager *resourceManager, deVirtualFileSystem *vfs,
		const char *filename, TIME_SYSTEM modificationTime);
	
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
	void SetCentralMassPoint(const decVector &cmp);
	
	/** \brief Use component model for collision instead of shapes if present. */
	inline bool GetModelCollision() const{ return pModelCollision; }
	
	/** \brief Set if component model is used for collision instead of shapes if present. */
	void SetModelCollision(bool modelCollision);
	
	
	
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
	deRigBone &GetBoneAt(int index) const;
	
	/** \brief Index of named bone or -1 if absent. */
	int IndexOfBoneNamed(const char *name) const;
	
	/** \brief Named bone is present. */
	bool HasBoneNamed(const char *name) const;
	
	/**
	 * \brief Add bone.
	 * \throws deeInvalidParam Named bone is present.
	 */
	void AddBone(deRigBone *bone);
	
	/** \brief Remove all bones. */
	void RemoveAllBones();
	
	/** \brief Index of physics root bone or -1 if not set. */
	inline int GetRootBone() const{ return pRootBone; }
	
	/**
	 * \brief Set physics root bone or -1 if not set.
	 * \throws deeOutOfBoundary \em index is less than -1 or greater than or equal to GetBoneCount().
	 */
	void SetRootBone(int rootBone);
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
	void SetShapes(const decShapeList &shapes);
	
	/** \brief Shape properties. */
	inline const decStringList &GetShapeProperties() const{ return pShapeProperties; }
	
	/**
	 * \brief Set shape properties.
	 * \throws deeInvalidParam Number of strings in \em properties does not match GetShapes().GetCount().
	 */
	void SetShapeProperties(const decStringList &properties);
	/*@}*/
	
	
	
	/** \name System Peers */
	/*@{*/
	/** \brief Physics system peer or NULL if not set. */
	inline deBasePhysicsRig *GetPeerPhysics() const{ return pPeerPhysics; }
	
	/** \brief Set physics system peer or NULL if not set. */
	void SetPeerPhysics(deBasePhysicsRig *peer);
	/*@}*/
};

#endif
