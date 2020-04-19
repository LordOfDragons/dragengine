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

#ifndef _DECLASSCOLLISIONTESTER_H_
#define _DECLASSCOLLISIONTESTER_H_

#include <libdscript/libdscript.h>

class dedsCollisionTester;
class deScriptingDragonScript;


/**
 * \brief Collision tester script class.
 * \details Provides a native class implementation of testing for collisions and examining
 *          the results. This class covers the typical scenarios where collisions are
 *          tested outside the physics collision detection handling like testing distance
 *          against the ground or other objects in a touch sensor or testing what collider
 *          the player is looking at. All the functionality provided by this class can be
 *          implemented in scripts too but at less speed. In addition this class provides
 *          helper calculations on the found result avoiding the need to calculate the
 *          values in the game scripts.
 */
class deClassCollisionTester : public dsClass{
private:
	deScriptingDragonScript &pDS;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new class. */
	deClassCollisionTester( deScriptingDragonScript &ds );
	/** \brief Cleans up the class. */
	virtual ~deClassCollisionTester();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Creates the class members. */
	void CreateClassMembers( dsEngine *engine );
	
	
	
	/** \brief Collision tested from real object or NULL if real object is NULL. */
	dedsCollisionTester *GetCollisionTester( dsRealObject *myself ) const;
	
	/** \brief Push collision tester or NULL onto the stack. */
	void PushCollisionTester( dsRunTime *rt, dedsCollisionTester *collisionTester );
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsCT, *clsVoid, *clsInt, *clsFlt, *clsStr, *clsBool, *clsObj;
		dsClass *clsDVec, *clsVec, *clsQuat, *clsWorld, *clsTS, *clsCF, *clsCLL;
		dsClass *clsShapeList, *clsCol;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfNewCopy );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetWorld );
	DEF_NATFUNC( nfSetWorld );
	DEF_NATFUNC( nfGetTouchSensor );
	DEF_NATFUNC( nfSetTouchSensor );
	DEF_NATFUNC( nfGetTouchSensorShape );
	DEF_NATFUNC( nfSetTouchSensorShape );
	
	DEF_NATFUNC( nfSetCollisionRay );
	DEF_NATFUNC( nfSetCollisionShape );
	DEF_NATFUNC( nfGetCollisionFilter );
	DEF_NATFUNC( nfSetCollisionFilter );
	DEF_NATFUNC( nfGetColliderListener );
	DEF_NATFUNC( nfSetColliderListener );
	
	DEF_NATFUNC( nfAddIgnoreCollider );
	DEF_NATFUNC( nfRemoveIgnoreCollider );
	DEF_NATFUNC( nfRemoveAllIgnoreColliders );
	
	DEF_NATFUNC( nfHasCollision );
	DEF_NATFUNC( nfGetHitDistance );
	DEF_NATFUNC( nfGetHitCollider );
	DEF_NATFUNC( nfGetHitBone );
	
	DEF_NATFUNC( nfReset );
	DEF_NATFUNC( nfRayHits );
	DEF_NATFUNC( nfColliderHits );
	DEF_NATFUNC( nfColliderHits2 );
	DEF_NATFUNC( nfColliderMoveHits );
	DEF_NATFUNC( nfColliderMoveHits2 );
	DEF_NATFUNC( nfColliderRotateHits );
	DEF_NATFUNC( nfColliderRotateHits2 );
	DEF_NATFUNC( nfColliderMoveRotateHits );
	DEF_NATFUNC( nfColliderMoveRotateHits2 );
#undef DEF_NATFUNC
};

#endif
