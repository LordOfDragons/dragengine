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

#ifndef _DERIGBONE_H_
#define _DERIGBONE_H_

#include "../../common/math/decMath.h"
#include "../../common/string/decString.h"
#include "../../common/string/decStringList.h"
#include "../../common/shape/decShapeList.h"

class deRigConstraint;


/**
 * \brief Rig bone.
 * 
 * Rig bones define a bone in an articulated rig.
 * 
 * \par Shapes
 * Shapes define the physical shape of the bone and are only used by the physics system.
 * See decShapeList for information about the available shape types.
 * 
 * Shapes can be assigned a single shape property string. This allows to transport some
 * additional game specific information with individual shapes of a rig. A typical use
 * case is modifying the material sound for certain shapes. The user can choose the
 * content and format of the string. By default all shapes have an empty string assigned.
 *
 */
class DE_DLL_EXPORT deRigBone{
private:
	decString pName;
	int pParent; // -1=no-parent, otherwise id=parent
	decVector pPos; // position
	decVector pRot; // rotation
	decMatrix pMatrix;
	decMatrix pInvMatrix;
	decVector pCMP;
	float pMass;
	decShapeList pShapes;
	decStringList pShapeProperties;
	bool pDynamic;
	
	decVector pIKLimitsLower;
	decVector pIKLimitsUpper;
	decVector pIKResistance;
	bool pIKLocked[ 3 ];
	
	deRigConstraint **pConstraints;
	int pConstraintCount;
	int pConstraintSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create named rig bone.
	 * \throws deeInvalidParam \em name is empty string.
	 */
	deRigBone( const char *name );
	
	/** \brief Clean up rig bone. */
	~deRigBone();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Bone name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Index of the parent bone or -1 if top level bone. */
	inline int GetParent() const{ return pParent; }
	
	/**
	 * \brief Set index of the parent bone or -1 if top level bone.
	 * \throws deeInvalidParam \em bone is less than -1.
	 */
	void SetParent( int bone );
	
	/** \brief Position relative to parent bone or rig. */
	inline decVector GetPosition() const{ return pPos; }
	
	/** \brief Set position relative to parent bone or rig. */
	void SetPosition( const decVector &position );
	
	/** \brief Rotation relative to parent bone or rig. */
	inline decVector GetRotation() const{ return pRot; }
	
	/** \brief Set rotation relative to parent bone or rig. */
	void SetRotation( const decVector &rotation );
	
	/** \brief Central mass point position. */
	inline const decVector &GetCentralMassPoint() const{ return pCMP; }
	
	/** \brief Set central mass point position. */
	void SetCentralMassPoint( const decVector &cmp );
	
	/** \brief Matrix transforming from local bone space to model space in reference pose. */
	inline decMatrix GetMatrix() const{ return pMatrix; }
	
	/** \brief Matrix transforming from model space to local bone space in reference pose. */
	inline decMatrix GetInverseMatrix() const{ return pInvMatrix; }
	
	/** \brief Set matrix transforming from local bone space to model space in reference pose. */
	void SetMatrices( const decMatrix &matrix );
	
	/** \brief Bone is dynamic. */
	inline bool GetDynamic() const{ return pDynamic; }
	
	/** \brief Set if bone is dynamic. */
	void SetDynamic( bool dynamic );
	
	/** \brief Mass in kg. */
	inline float GetMass() const{ return pMass; }
	
	/** \brief Set mass in kg clamped to 0 or larger. */
	void SetMass( float mass );
	/*@}*/
	
	
	
	/** \name Inverse Kinematics */
	/*@{*/
	/** \brief Lower ik limits. */
	inline const decVector &GetIKLimitsLower() const{ return pIKLimitsLower; }
	
	/** \brief Upper ik limits. */
	inline const decVector &GetIKLimitsUpper() const{ return pIKLimitsUpper; }
	
	/** \brief Set ik limits. */
	void SetIKLimits( const decVector &lower, const decVector &upper );
	
	/** \brief IK resistance. */
	inline const decVector &GetIKResistance() const{ return pIKResistance; }
	
	/** \brief Set ik resistance. */
	void SetIKResistance( const decVector &resistance );
	
	/** \brief X rotation is locked under IK. */
	inline bool GetIKLockedX() const{ return pIKLocked[ 0 ]; }
	
	/** \brief Set if X rotation is locked under IK. */
	void SetIKLockedX( bool locked );
	
	/** \brief Y rotation is locked under IK. */
	inline bool GetIKLockedY() const{ return pIKLocked[ 1 ]; }
	
	/** \brief Set if y rotation is locked under IK. */
	void SetIKLockedY( bool locked );
	
	/** \brief Z rotation is locked under IK. */
	inline bool GetIKLockedZ() const{ return pIKLocked[ 2 ]; }
	
	/** \brief Set z rotation is locked under IK. */
	void SetIKLockedZ( bool locked );
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
	
	
	
	/** \name Constraints */
	/*@{*/
	/** \brief Number of constraints. */
	inline int GetConstraintCount() const{ return pConstraintCount; }
	
	/**
	 * \brief Constraint at index.
	 * \throws deeOutOfBoundary \em index is less than 0 or greater than or equal to GetConstraintCount().
	 */
	deRigConstraint &GetConstraintAt( int index ) const;
	
	/** \brief Add constraint. */
	void AddConstraint( deRigConstraint *constraint );
	/*@}*/
	
	
	
private:
	bool pHasConstraint( deRigConstraint *constraint ) const;
};

#endif
