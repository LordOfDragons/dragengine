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

#ifndef _DECLASSCOLLIDERCOLLISIONTEST_H_
#define _DECLASSCOLLIDERCOLLISIONTEST_H_

#include <libdscript/libdscript.h>

class deCollider;
class deColliderCollisionTest;
class deScriptingDragonScript;


/**
 * \brief Collider collision test script class.
 */
class deClassColliderCollisionTest : public dsClass{
private:
	deScriptingDragonScript &pDS;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new class. */
	deClassColliderCollisionTest( deScriptingDragonScript &ds );
	
	/** \brief Cleans up class. */
	virtual ~deClassColliderCollisionTest();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief DragonScript module. */
	inline deScriptingDragonScript &GetDS(){ return pDS; }
	inline const deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Create class members. */
	void CreateClassMembers( dsEngine *engine );
	
	
	
	/** \brief Collider collision test from real object or \em NULL if real object is \em NULL. */
	deColliderCollisionTest *GetCollisionTest( dsRealObject *myself ) const;
	
	/** \brief Push collider collision tester or \em NULL onto the stack. */
	void PushCollisionTest( dsRunTime *rt, deColliderCollisionTest *collisionTest, deCollider *parentCollider );
	
	/** \brief Collider collision test parent from real object. */
	deCollider *GetParentCollider( dsRealObject *myself ) const;
	
	/** \brief Set collider collision test parent or \em NULL. */
	void SetParentCollider( dsRealObject *myself, deCollider *collider ) const;
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsCCT, *clsVoid, *clsInt, *clsFlt, *clsStr, *clsBool, *clsObj;
		dsClass *clsVec, *clsDVec, *clsTS, *clsCF, *clsCol, *clsCI, *clsComp;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfNewCopy );
	DEF_NATFUNC( nfNewWorld );
	DEF_NATFUNC( nfNewWorldBone );
	DEF_NATFUNC( nfNewTouchSensor );
	DEF_NATFUNC( nfNewTouchSensorBone );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetTouchSensor );
	DEF_NATFUNC( nfSetTouchSensor );
	
	DEF_NATFUNC( nfGetCollider );
	DEF_NATFUNC( nfSetCollider );
	DEF_NATFUNC( nfGetCollisionFilter );
	DEF_NATFUNC( nfSetCollisionFilter );
	
	DEF_NATFUNC( nfGetComponent );
	DEF_NATFUNC( nfSetComponent );
	DEF_NATFUNC( nfGetBone );
	DEF_NATFUNC( nfSetBone );
	DEF_NATFUNC( nfGetOrigin );
	DEF_NATFUNC( nfSetOrigin );
	DEF_NATFUNC( nfGetDirection );
	DEF_NATFUNC( nfSetDirection );
	DEF_NATFUNC( nfGetLocalDirection );
	DEF_NATFUNC( nfSetLocalDirection );
	DEF_NATFUNC( nfGetEnabled );
	DEF_NATFUNC( nfSetEnabled );
	
	DEF_NATFUNC( nfGetTestOrigin );
	DEF_NATFUNC( nfGetTestDirection );
	
	DEF_NATFUNC( nfGetHasCollision );
	DEF_NATFUNC( nfGetCollisionCount );
	DEF_NATFUNC( nfGetCollisionAt );
	DEF_NATFUNC( nfReset );
	
	DEF_NATFUNC( nfHitDistance );
	DEF_NATFUNC( nfHitPointCollider );
	DEF_NATFUNC( nfHitPointWorld );
	DEF_NATFUNC( nfHitNormalCollider );
	DEF_NATFUNC( nfHitNormalWorld );
	DEF_NATFUNC( nfHitCollider );
	DEF_NATFUNC( nfHitBone );
#undef DEF_NATFUNC
};

#endif
