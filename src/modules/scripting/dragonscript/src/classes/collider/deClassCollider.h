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

#ifndef _DECLASSCOLLIDER_H_
#define _DECLASSCOLLIDER_H_

#include <libdscript/libdscript.h>

#include <dragengine/common/math/decMath.h>

class deResource;
class deCollider;
class deCollisionInfo;
class deScriptingDragonScript;
class deColliderConstraint;



/**
 * \brief Collider script class.
 */
class deClassCollider : public dsClass{
private:
	deScriptingDragonScript &pDS;
	dsClass *pClsCollisionResponse;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new script class. */
	deClassCollider( deScriptingDragonScript &ds );
	
	/** \brief Clean up the script class. */
	virtual ~deClassCollider();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Script module owning the script class. */
	inline deScriptingDragonScript &GetDS(){ return pDS; }
	inline const deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Create script class members. */
	virtual void CreateClassMembers( dsEngine *engine );
	
	
	
	inline dsClass *GetClassCollisionResponse() const{ return pClsCollisionResponse; }
	
	/**
	 * \brief Retrieve collider from a script object.
	 * \details Returns NULL if \em myself is NULL.
	 */
	deCollider *GetCollider( dsRealObject *myself ) const;
	
	/**
	 * \brief Push collider onto the stack.
	 * \details Pushes a \em null object to the stack if \em collider is NULL.
	 */
	void PushCollider( dsRunTime *rt, deCollider *collider );
	
	/** \brief Assigns collider or \em NULL. */
	void AssignCollider( dsRealObject *myself, deCollider *collider );
	/*@}*/
	
	
	
	/** \name Class member use only. */
	/*@{*/
	/**
	 * \brief Retrieve the engine resource object for a script object.
	 * \details
	 * Return NULL if the value is not an object, is \em null or does not match a
	 * resource script class.
	 */
	deResource *GetResource( dsValue &myself ) const;
	
	/**
	 * \brief Push engine resource onto stack as script object.
	 * \details
	 * Push \em null object if the engine resource is null or does not have a script class.
	 */
	void PushResource( dsRunTime &rt, deResource *resource );
	
	/**
	 * \brief Retrieve the coordinate frame matrix for a resource.
	 * \throws EInvalidParam deColliderAttachment::CanAttachResource()
	 *                       returns false for the resource.
	 */
	static decDMatrix GetResourceMatrix( const deResource &resource );
	
	/**
	 * \brief Retrieve the scale of a resource.
	 * \throws EInvalidParam deColliderAttachment::CanAttachResource()
	 *                       returns false for the resource.
	 */
	static decVector GetResourceScale( const deResource &resource );
	
	/** \brief Attach resource using deColliderAttachment::eatStatic mode. */
	void AttachStatic( deCollider &collider, deResource *resource ) const;
	
	/** \brief Attach resource using deColliderAttachment::eatStatic mode. */
	void AttachStatic( deCollider &collider, deResource *resource, const decVector &position,
		const decQuaternion &orientation, const decVector &scale ) const;
	
	/** \brief Attach resource using deColliderAttachment::eatRig mode. */
	void AttachRig( deCollider &collider, deResource *resource ) const;
	
	/** \brief Attach resource using deColliderAttachment::eatBone mode. */
	void AttachBone( deCollider &collider, deResource *resource, const char *targetBone ) const;
	
	/** \brief Attach resource using deColliderAttachment::eatBone mode. */
	void AttachBone( deCollider &collider, deResource *resource, const char *targetBone,
	const decVector &position, const decQuaternion &orientation, const decVector &scale ) const;
	
	/** \brief Attach resource using deColliderAttachment::eatWeight mode. */
	void AttachWeight( deCollider &collider, deResource *resource, const deCollisionInfo &colInfo ) const;
	
	/** \brief Attach resource using deColliderAttachment::eatRelativeMovement mode. */
	void AttachRelativeMovement( deCollider &collider, deResource *resource ) const;
	
	/** \brief Index of matching constraint (same target collider and bone). */
	deColliderConstraint *FindConstraint( deCollider &collider, const deColliderConstraint &constraint ) const;
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsCol;
		
		dsClass *clsVoid;
		dsClass *clsInt;
		dsClass *clsFlt;
		dsClass *clsStr;
		
		dsClass *clsQuat;
		dsClass *clsCCon;
		dsClass *clsVec;
		dsClass *clsObj;
		dsClass *clsBool;
		dsClass *clsCLL;
		dsClass *clsCI;
		dsClass *clsDVec;
		dsClass *clsCF;
		dsClass *clsDMat;
		dsClass *clsCBL;
		dsClass *clsCCT;
		
		dsClass *clsCollisionResponse;
		dsClass *clsWorld;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetPosition );
	DEF_NATFUNC( nfSetPosition );
	DEF_NATFUNC( nfGetOrientation );
	DEF_NATFUNC( nfSetOrientation );
	DEF_NATFUNC( nfSetGeometry );
	DEF_NATFUNC( nfSetGeometry2 );
	DEF_NATFUNC( nfGetScale );
	DEF_NATFUNC( nfSetScale );
	DEF_NATFUNC( nfGetView );
	DEF_NATFUNC( nfGetUp );
	DEF_NATFUNC( nfGetRight );
	DEF_NATFUNC( nfGetMatrix );
	DEF_NATFUNC( nfGetInverseMatrix );
	DEF_NATFUNC( nfGetLinearVelocity );
	DEF_NATFUNC( nfSetLinearVelocity );
	DEF_NATFUNC( nfGetAngularVelocity );
	DEF_NATFUNC( nfSetAngularVelocity );
	DEF_NATFUNC( nfGetMass );
	DEF_NATFUNC( nfSetMass );
	DEF_NATFUNC( nfGetGravity );
	DEF_NATFUNC( nfSetGravity );
	DEF_NATFUNC( nfGetEnabled );
	DEF_NATFUNC( nfSetEnabled );
	DEF_NATFUNC( nfGetResponseType );
	DEF_NATFUNC( nfSetResponseType );
	DEF_NATFUNC( nfGetUseLocalGravity );
	DEF_NATFUNC( nfSetUseLocalGravity );
	DEF_NATFUNC(nfGetParentWorld);
	
	DEF_NATFUNC( nfGetOwner );
	DEF_NATFUNC( nfSetOwner );
	
	DEF_NATFUNC( nfGetForceFieldDirect );
	DEF_NATFUNC( nfSetForceFieldDirect );
	DEF_NATFUNC( nfGetForceFieldSurface );
	DEF_NATFUNC( nfSetForceFieldSurface );
	DEF_NATFUNC( nfGetForceFieldMass );
	DEF_NATFUNC( nfSetForceFieldMass );
	DEF_NATFUNC( nfGetForceFieldSpeed );
	DEF_NATFUNC( nfSetForceFieldSpeed );
	
	DEF_NATFUNC( nfGetCollisionFilter );
	DEF_NATFUNC( nfSetCollisionFilter );
	
	DEF_NATFUNC( nfGetAttachmentCount );
	DEF_NATFUNC( nfGetAttachedResource );
	DEF_NATFUNC( nfAttachStatic );
	DEF_NATFUNC( nfAttachStatic2 );
	DEF_NATFUNC( nfAttachRelativeMovement );
	DEF_NATFUNC( nfDetach );
	DEF_NATFUNC( nfDetachAll );
	DEF_NATFUNC( nfAttachmentsForceUpdate );
	
	DEF_NATFUNC( nfGetConstraintCount );
	DEF_NATFUNC( nfAddConstraint );
	DEF_NATFUNC( nfHasConstraint );
	DEF_NATFUNC( nfIndexOfConstraint );
	DEF_NATFUNC( nfGetConstraintAt );
	DEF_NATFUNC( nfSetConstraintAt );
	DEF_NATFUNC( nfRemoveConstraint );
	DEF_NATFUNC( nfRemoveConstraintFrom );
	DEF_NATFUNC( nfRemoveAllConstraints );
	
	DEF_NATFUNC( nfGetIgnoreColliderCount );
	DEF_NATFUNC( nfGetIgnoreColliderAt );
	DEF_NATFUNC( nfHasIgnoreCollider );
	DEF_NATFUNC( nfAddIgnoreCollider );
	DEF_NATFUNC( nfRemoveIgnoreCollider );
	DEF_NATFUNC( nfRemoveAllIgnoreColliders );
	
	DEF_NATFUNC( nfGetCollisionTestCount );
	DEF_NATFUNC( nfGetCollisionTestAt );
	DEF_NATFUNC( nfAddCollisionTest );
	DEF_NATFUNC( nfRemoveCollisionTest );
	DEF_NATFUNC( nfRemoveAllCollisionTests );
	
	DEF_NATFUNC( nfApplyImpuls );
	DEF_NATFUNC( nfApplyImpulsAt );
	DEF_NATFUNC( nfApplyTorqueImpuls );
	DEF_NATFUNC( nfApplyForce );
	DEF_NATFUNC( nfApplyForceAt );
	DEF_NATFUNC( nfApplyTorque );
	
	DEF_NATFUNC( nfGetColliderListener );
	DEF_NATFUNC( nfSetColliderListener );
	DEF_NATFUNC( nfColliderListenerGetCustomCanHit );
	DEF_NATFUNC( nfColliderListenerSetCustomCanHit );
	
	DEF_NATFUNC( nfGetBreakingListener );
	DEF_NATFUNC( nfSetBreakingListener );
	
	DEF_NATFUNC( nfPointInside );
	DEF_NATFUNC( nfRayHits );
	DEF_NATFUNC( nfRayHitsClosest );
	DEF_NATFUNC( nfColliderHits );
	DEF_NATFUNC( nfColliderMoveHits );
	DEF_NATFUNC( nfColliderMoveHitsClosest );
	DEF_NATFUNC( nfColliderRotateHits );
	DEF_NATFUNC( nfColliderRotateHitsClosest );
	DEF_NATFUNC( nfColliderMoveRotateHits );
	DEF_NATFUNC( nfColliderMoveRotateHitsClosest );
	
	DEF_NATFUNC( nfEquals );
	DEF_NATFUNC( nfEquals2 );
	DEF_NATFUNC( nfHashCode );
#undef DEF_NATFUNC
};

#endif
