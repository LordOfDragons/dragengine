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

#ifndef _DECLASSCOLLIDERRIG_H_
#define _DECLASSCOLLIDERRIG_H_

#include <libdscript/libdscript.h>

#include <dragengine/common/math/decMath.h>

class deColliderRig;
class deScriptingDragonScript;



/**
 * \brief Rig collider script class.
 */
class deClassColliderRig : public dsClass{
private:
	deScriptingDragonScript &pDS;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new script class. */
	deClassColliderRig( deScriptingDragonScript &ds );
	/** \brief Clean up the script class. */
	virtual ~deClassColliderRig();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Script module owning the script class. */
	inline deScriptingDragonScript &GetDS(){ return pDS; }
	inline const deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Create script class members. */
	virtual void CreateClassMembers( dsEngine *engine );
	
	/**
	 * \brief Retrieve collider from a script object.
	 * \details Returns NULL if \em myself is NULL.
	 */
	deColliderRig *GetCollider( dsRealObject *myself ) const;
	
	/**
	 * \brief Push collider onto the stack.
	 * \details Pushes a \em null object to the stack if \em collider is NULL.
	 */
	void PushCollider( dsRunTime *rt, deColliderRig *collider );
	
	/** \brief Assigns collider or \em NULL. */
	void AssignCollider( dsRealObject *myself, deColliderRig *collider );
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsColRig, *clsVoid, *clsInt, *clsFlt, *clsStr, *clsQuat;
		dsClass *clsVec, *clsObj, *clsBool, *clsDVec, *clsDMat, *clsRig;
		dsClass *clsCI;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfAttachRig );
	DEF_NATFUNC( nfAttachBone );
	DEF_NATFUNC( nfAttachBone2 );
	DEF_NATFUNC( nfAttachWeight );
	
	DEF_NATFUNC( nfBoneApplyImpuls );
	DEF_NATFUNC( nfBoneApplyImpulsAt );
	DEF_NATFUNC( nfBoneApplyTorqueImpuls );
	DEF_NATFUNC( nfBoneApplyForce );
	DEF_NATFUNC( nfBoneApplyForceAt );
	DEF_NATFUNC( nfBoneApplyTorque );
	
	DEF_NATFUNC( nfGetRig );
	DEF_NATFUNC( nfSetRig );
	
	DEF_NATFUNC( nfCopyStatesFromCollider );
	DEF_NATFUNC( nfCopyStateFromCollider );
	DEF_NATFUNC( nfCopyStateFromCollider2 );
	DEF_NATFUNC( nfBoneGetPosition );
	DEF_NATFUNC( nfBoneSetPosition );
	DEF_NATFUNC( nfBoneGetOrientation );
	DEF_NATFUNC( nfBoneSetOrientation );
	DEF_NATFUNC( nfBoneGetLinearVelocity );
	DEF_NATFUNC( nfBoneSetLinearVelocity );
	DEF_NATFUNC( nfBoneGetAngularVelocity );
	DEF_NATFUNC( nfBoneSetAngularVelocity );
	DEF_NATFUNC( nfBoneGetMatrix );
	DEF_NATFUNC( nfBoneGetInverseMatrix );
	
	DEF_NATFUNC( nfEnableRigBoneConstraints );
	
	DEF_NATFUNC( nfEquals );
	DEF_NATFUNC( nfEquals2 );
	DEF_NATFUNC( nfHashCode );
#undef DEF_NATFUNC
};

#endif
