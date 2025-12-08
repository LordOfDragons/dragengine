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
	deClassColliderCollisionTest(deScriptingDragonScript &ds);
	
	/** \brief Cleans up class. */
	virtual ~deClassColliderCollisionTest();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief DragonScript module. */
	inline deScriptingDragonScript &GetDS(){ return pDS; }
	inline const deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Create class members. */
	void CreateClassMembers(dsEngine *engine);
	
	
	
	/** \brief Collider collision test from real object or \em NULL if real object is \em NULL. */
	deColliderCollisionTest *GetCollisionTest(dsRealObject *myself) const;
	
	/** \brief Push collider collision tester or \em NULL onto the stack. */
	void PushCollisionTest(dsRunTime *rt, deColliderCollisionTest::Ref collisionTest, deCollider::Ref parentCollider);
	
	/** \brief Collider collision test parent from real object. */
	deCollider *GetParentCollider(dsRealObject *myself) const;
	
	/** \brief Set collider collision test parent or \em NULL. */
	void SetParentCollider(dsRealObject *myself, deCollider::Ref collider) const;
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsCCT, *clsVoid, *clsInt, *clsFlt, *clsStr, *clsBool, *clsObj;
		dsClass *clsVec, *clsDVec, *clsTS, *clsCF, *clsCol, *clsCI, *clsComp;
		dsClass *clsQuaternion;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{\
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC(nfNew);
	DEF_NATFUNC(nfNewCopy);
	DEF_NATFUNC(nfNewWorld);
	DEF_NATFUNC(nfNewWorldBone);
	DEF_NATFUNC(nfNewTouchSensor);
	DEF_NATFUNC(nfNewTouchSensorBone);
	DEF_NATFUNC(nfDestructor);
	
	DEF_NATFUNC(nfGetTouchSensor);
	DEF_NATFUNC(nfSetTouchSensor);
	
	DEF_NATFUNC(nfGetCollider);
	DEF_NATFUNC(nfSetCollider);
	DEF_NATFUNC(nfGetCollisionFilter);
	DEF_NATFUNC(nfSetCollisionFilter);
	
	DEF_NATFUNC(nfGetComponent);
	DEF_NATFUNC(nfSetComponent);
	DEF_NATFUNC(nfGetBone);
	DEF_NATFUNC(nfSetBone);
	DEF_NATFUNC(nfGetOrigin);
	DEF_NATFUNC(nfSetOrigin);
	DEF_NATFUNC(nfGetOrientation);
	DEF_NATFUNC(nfSetOrientation);
	DEF_NATFUNC(nfGetDirection);
	DEF_NATFUNC(nfSetDirection);
	DEF_NATFUNC(nfGetLocalDirection);
	DEF_NATFUNC(nfSetLocalDirection);
	DEF_NATFUNC(nfGetEnabled);
	DEF_NATFUNC(nfSetEnabled);
	
	DEF_NATFUNC(nfGetTestOrigin);
	DEF_NATFUNC(nfGetTestOrientation);
	DEF_NATFUNC(nfGetTestDirection);
	
	DEF_NATFUNC(nfGetHasCollision);
	DEF_NATFUNC(nfGetCollisionCount);
	DEF_NATFUNC(nfGetCollisionAt);
	DEF_NATFUNC(nfReset);
	
	DEF_NATFUNC(nfHitDistance);
	DEF_NATFUNC(nfHitPointCollider);
	DEF_NATFUNC(nfHitPointWorld);
	DEF_NATFUNC(nfHitNormalCollider);
	DEF_NATFUNC(nfHitNormalWorld);
	DEF_NATFUNC(nfHitCollider);
	DEF_NATFUNC(nfHitBone);
#undef DEF_NATFUNC
};

#endif
