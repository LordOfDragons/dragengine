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

// include only once
#ifndef _DECLASSCOLLIDERCONSTRAINT_H_
#define _DECLASSCOLLIDERCONSTRAINT_H_

// includes
#include <libdscript/libdscript.h>

// predefinitions
class deEngine;
class deColliderConstraint;
class deClassVector;
class deClassQuaternion;
class deScriptingDragonScript;



/**
 * @brief Collider Constraint Class.
 * Script class for collider constraints. This is only a working copy and does not
 * wrap an engine constraint.
 */
class deClassColliderConstraint : public dsClass{
private:
	deEngine *pGameEngine;
	deScriptingDragonScript *pScrDS;
	deClassVector *pClsVec;
	deClassQuaternion *pClsQuat;
	dsClass *pClsColliderConstraintDof;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new script class. */
	deClassColliderConstraint( deEngine *gameEngine, deScriptingDragonScript *scrDS );
	/** Cleans up the script class. */
	virtual ~deClassColliderConstraint();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Creates the class members. */
	virtual void CreateClassMembers( dsEngine *engine );
	
	/** Retrieves the constraint. */
	deColliderConstraint *GetConstraint( dsRealObject *myself ) const;
	/**
	 * Creates a new constraint object and pushes it onto the stack. Copies the state of
	 * the given constraint to the newly created constraint.
	 */
	void PushConstraint( dsRunTime *rt, deColliderConstraint *constraint );
	
	/** Retrieves the game engine. */
	inline deEngine *GetGameEngine() const{ return pGameEngine; }
	/** Retrieves the scripting module. */
	inline deScriptingDragonScript *GetScriptModule() const{ return pScrDS; }
	
	/** Retrieves the vector script class. */
	inline deClassVector *GetClassVector() const{ return pClsVec; }
	/** Retrieves the quaternion script class. */
	inline deClassQuaternion *GetClassQuaternion() const{ return pClsQuat; }
	
	inline dsClass *GetClassColliderConstraintDof() const{ return pClsColliderConstraintDof; }
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsCCon, *clsVoid, *clsInt, *clsFlt, *clsStr;
		dsClass *clsVec, *clsQuat, *clsObj, *clsBool, *clsCol;
		dsClass *clsColliderConstraintDof;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetTargetCollider );
	DEF_NATFUNC( nfSetTargetCollider );
	DEF_NATFUNC( nfGetTargetBone );
	DEF_NATFUNC( nfSetTargetBone );
	
	DEF_NATFUNC( nfGetPosition1 );
	DEF_NATFUNC( nfSetPosition1 );
	DEF_NATFUNC( nfGetOrientation1 );
	DEF_NATFUNC( nfSetOrientation1 );
	DEF_NATFUNC( nfGetPosition2 );
	DEF_NATFUNC( nfSetPosition2 );
	DEF_NATFUNC( nfGetOrientation2 );
	DEF_NATFUNC( nfSetOrientation2 );
	
	DEF_NATFUNC( nfGetLowerLimitFor );
	DEF_NATFUNC( nfGetUpperLimitFor );
	DEF_NATFUNC( nfSetLimitsFor );
	DEF_NATFUNC( nfSetLockedFor );
	DEF_NATFUNC( nfSetFreeFor );
	
	DEF_NATFUNC( nfGetLinearLowerLimits );
	DEF_NATFUNC( nfGetLinearUpperLimits );
	DEF_NATFUNC( nfSetLinearLimits );
	DEF_NATFUNC( nfGetAngularLowerLimits );
	DEF_NATFUNC( nfGetAngularUpperLimits );
	DEF_NATFUNC( nfSetAngularLimits );
	
	DEF_NATFUNC( nfGetStaticFrictionFor );
	DEF_NATFUNC( nfSetStaticFrictionFor );
	DEF_NATFUNC( nfGetKinematicFrictionFor );
	DEF_NATFUNC( nfSetKinematicFrictionFor );
	DEF_NATFUNC( nfGetSpringStiffnessFor );
	DEF_NATFUNC( nfSetSpringStiffnessFor );
	
	DEF_NATFUNC( nfGetLinearDamping );
	DEF_NATFUNC( nfSetLinearDamping );
	DEF_NATFUNC( nfGetAngularDamping );
	DEF_NATFUNC( nfSetAngularDamping );
	DEF_NATFUNC( nfGetSpringDamping );
	DEF_NATFUNC( nfSetSpringDamping );
	
	DEF_NATFUNC( nfLockAll );
	DEF_NATFUNC( nfSetToBallJoint );
	DEF_NATFUNC( nfSetToHingeJoint );
	DEF_NATFUNC( nfSetToPistonJoint );
	
	DEF_NATFUNC( nfGetIsRope );
	DEF_NATFUNC( nfSetIsRope );
	
	DEF_NATFUNC( nfGetBreakingThreshold );
	DEF_NATFUNC( nfSetBreakingThreshold );
	
	DEF_NATFUNC( nfGetBone );
	DEF_NATFUNC( nfSetBone );
	
	DEF_NATFUNC( nfEquals );
	DEF_NATFUNC( nfHashCode );
#undef DEF_NATFUNC
};

// end of include only once
#endif
