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

#ifndef _DEARBONESTATE_H_
#define _DEARBONESTATE_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/animator/rule/deAnimatorRule.h>

class dearComponentBoneState;
class dearAnimationState;
class deComponentBone;
class deRigBone;



/**
 * Bone state class.
 * Stores the state of a bone driven by an animator. Contains also
 * the mappings of the bone to an animation if present.
 */
class dearBoneState{
private:
	deRigBone *pRigBone;
	const char *pRigBoneName;
	int pIndex;
	int pRigIndex;
	dearBoneState *pParentState;
	dearBoneState **pChildStates;
	int pChildStateCount, pChildStateSize;
	decVector pPosition;
	decQuaternion pOrientation;
	decVector pScale;
	decMatrix pRigLocalMatrix;
	decMatrix pInvRigLocalMatrix;
	decMatrix pLocalMatrix;
	decMatrix pInvLocalMatrix;
	decMatrix pGlobalMatrix;
	decMatrix pInvGlobalMatrix;
	bool pProtect;
	bool pDirty;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new bone state object. */
	dearBoneState();
	/** Cleans up the bone state. */
	~dearBoneState();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the rig bone or NULL if not linked. */
	inline deRigBone *GetRigBone() const{ return pRigBone; }
	/** Sets the rig bone or NULL if not linked. */
	void SetRigBone( deRigBone *bone );
	/** Retrieves the rig bone name. */
	inline const char *GetRigBoneName() const{ return pRigBoneName; }
	/** Sets the rig bone name. */
	void SetRigBoneName( const char *name );
	/** Retrieves the index of this bone state. */
	inline int GetIndex() const{ return pIndex; }
	/** Sets the index of this bone state. */
	void SetIndex( int index );
	/** Retrieves the rig index. */
	inline int GetRigIndex() const{ return pRigIndex; }
	/** Sets the rig index. */
	void SetRigIndex( int index );
	/** Retrieves the parent bone state or NULL if not existing. */
	inline dearBoneState *GetParentState() const{ return pParentState; }
	/** Sets the parent bone state or NULL if not existing. */
	void SetParentState( dearBoneState *boneState );
	/** Retrieves the position. */
	inline const decVector &GetPosition() const{ return pPosition; }
	/** Sets the position. */
	void SetPosition( const decVector &position );
	/** Retrieves the orientation. */
	inline const decQuaternion &GetOrientation() const{ return pOrientation; }
	/** Sets the orientation. */
	void SetOrientation( const decQuaternion &orientation );
	/** Retrieves the scale. */
	inline const decVector &GetScale() const{ return pScale; }
	/** Sets the scale. */
	void SetScale( const decVector &size );
	/** Sets position and orientation. */
	void SetPosOrient( const decVector &position, const decQuaternion &orientation );
	
	/** Retrieves the rig local matrix. */
	inline const decMatrix &GetRigLocalMatrix() const{ return pRigLocalMatrix; }
	/** Retrieves the inverse rig local matrix. */
	inline const decMatrix &GetInverseRigLocalMatrix() const{ return pInvRigLocalMatrix; }
	/** Sets the rig local matrix. */
	void SetRigLocalMatrix( const decMatrix &matrix );
	
	/** Retrieves the local matrix. */
	inline const decMatrix &GetLocalMatrix() const{ return pLocalMatrix; }
	/** Sets the local matrix. */
	void SetLocalMatrix( const decMatrix &matrix );
	/** Retrieves the inverse local matrix. */
	inline const decMatrix &GetInverseLocalMatrix() const{ return pInvLocalMatrix; }
	/** Sets the inverse local matrix. */
	void SetInverseLocalMatrix( const decMatrix &matrix );
	
	/** Retrieves the global matrix. */
	inline const decMatrix &GetGlobalMatrix() const{ return pGlobalMatrix; }
	/** Sets the global matrix. */
	void SetGlobalMatrix( const decMatrix &matrix );
	/** Retrieves the inverse global matrix. */
	inline const decMatrix &GetInverseGlobalMatrix() const{ return pInvGlobalMatrix; }
	/** Sets the inverse global matrix. */
	void SetInverseGlobalMatrix( const decMatrix &matrix );
	
	/** Determines if the state is protected. */
	inline bool GetProtected() const{ return pProtect; }
	/** Sets if the state is protected. */
	void SetProtected( bool prot );
	
	/** Determines if the state is dirty. */
	inline bool GetDirty() const{ return pDirty; }
	/** Sets if the state is dirty. */
	void SetDirty( bool dirty );
	/** Sets the matrices from the current state. */
	void UpdateMatrices();
	/**
	 * Sets the matrices from the current state. The global matrix is not touched except
	 * the inverse is calculated and stored.
	 */
	void UpdateMatricesKeepGlobal();
	/** Updates the local parameters from the global matrix. */
	void UpdateFromGlobalMatrix();
	/** Calculate local matric from global matrix. */
	decMatrix CalcLocalFromGlobal( const decMatrix &globalMatrix ) const;
	/** Resets the matrices to identity. */
	void ResetMatrices();
	
	/** Copy another state to this state. */
	void SetFrom( const dearBoneState &state );
	
	/** Copy component bone state to this state. */
	void SetFrom( const deComponentBone &bone );
	
	/** Copy component bone state to this state. */
	void SetFrom( const dearComponentBoneState &state );
	
	/** Update state by blending it with an empty state. */
	void BlendWithDefault( deAnimatorRule::eBlendModes blendMode, float blendFactor,
		bool enablePosition, bool enableOrientation, bool enableScale );
	
	/** Update state by blending it with an incoming state. */
	void BlendWith( const dearBoneState &state, deAnimatorRule::eBlendModes blendMode,
		float blendFactor, bool enablePosition, bool enableOrientation, bool enableScale );
	
	/** Update state by blending it with an incoming state. */
	void BlendWith( const dearAnimationState &state, deAnimatorRule::eBlendModes blendMode,
		float blendFactor, bool enablePosition, bool enableOrientation, bool enableScale );
	
	/** Update state by blending it with an incoming state. */
	void BlendWith( const deComponentBone &bone, deAnimatorRule::eBlendModes blendMode,
		float blendFactor, bool enablePosition, bool enableOrientation, bool enableScale );
	
	/** Update state by blending it with an incoming state. */
	void BlendWith( const dearComponentBoneState &state, deAnimatorRule::eBlendModes blendMode,
		float blendFactor, bool enablePosition, bool enableOrientation, bool enableScale );
	
	/** Update state by blending it with an incoming state. */
	void BlendWith( const decVector &position, const decQuaternion &orientation,
		deAnimatorRule::eBlendModes blendMode, float blendFactor,
		bool enablePosition, bool enableOrientation );
	
	/** Update state by blending it with an incoming state. */
	void BlendWith( const decVector &position, const decQuaternion &orientation,
		const decVector &scale, deAnimatorRule::eBlendModes blendMode, float blendFactor,
		bool enablePosition, bool enableOrientation, bool enableScale );
	/*@}*/
	
	/** \name Child states */
	/*@{*/
	/** Retrieves the number of child states. */
	inline int GetChildStateCount() const{ return pChildStateCount; }
	/** Retrieves the child state at the given index. */
	dearBoneState *GetChildStateAt( int index ) const;
	/** Adds a child state. */
	void AddChildState( dearBoneState *boneState );
	/** Removes all child states. */
	void RemoveAllChildStates();
	/*@}*/
};

#endif
