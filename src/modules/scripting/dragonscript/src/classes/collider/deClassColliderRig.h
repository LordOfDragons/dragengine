/* 
 * Drag[en]gine DragonScript Script Module
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
