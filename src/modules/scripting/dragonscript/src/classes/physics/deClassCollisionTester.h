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
	deClassCollisionTester(deScriptingDragonScript &ds);
	/** \brief Cleans up the class. */
	virtual ~deClassCollisionTester();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Creates the class members. */
	void CreateClassMembers(dsEngine *engine);
	
	
	
	/** \brief Collision tested from real object or NULL if real object is NULL. */
	dedsCollisionTester *GetCollisionTester(dsRealObject *myself) const;
	
	/** \brief Push collision tester or NULL onto the stack. */
	void PushCollisionTester(dsRunTime *rt, dedsCollisionTester::Ref collisionTester);
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsCT, *clsVoid, *clsInt, *clsFlt, *clsStr, *clsBool, *clsObj;
		dsClass *clsDVec, *clsVec, *clsQuat, *clsWorld, *clsTS, *clsCF, *clsCLL;
		dsClass *clsShapeList, *clsCol;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{\
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC(nfNew);
	DEF_NATFUNC(nfNewCopy);
	DEF_NATFUNC(nfDestructor);
	
	DEF_NATFUNC(nfGetWorld);
	DEF_NATFUNC(nfSetWorld);
	DEF_NATFUNC(nfGetTouchSensor);
	DEF_NATFUNC(nfSetTouchSensor);
	DEF_NATFUNC(nfGetTouchSensorShape);
	DEF_NATFUNC(nfSetTouchSensorShape);
	
	DEF_NATFUNC(nfSetCollisionRay);
	DEF_NATFUNC(nfSetCollisionShape);
	DEF_NATFUNC(nfGetCollisionFilter);
	DEF_NATFUNC(nfSetCollisionFilter);
	DEF_NATFUNC(nfGetColliderListener);
	DEF_NATFUNC(nfSetColliderListener);
	
	DEF_NATFUNC(nfAddIgnoreCollider);
	DEF_NATFUNC(nfRemoveIgnoreCollider);
	DEF_NATFUNC(nfRemoveAllIgnoreColliders);
	
	DEF_NATFUNC(nfHasCollision);
	DEF_NATFUNC(nfGetHitDistance);
	DEF_NATFUNC(nfGetHitCollider);
	DEF_NATFUNC(nfGetHitBone);
	
	DEF_NATFUNC(nfReset);
	DEF_NATFUNC(nfRayHits);
	DEF_NATFUNC(nfColliderHits);
	DEF_NATFUNC(nfColliderHits2);
	DEF_NATFUNC(nfColliderMoveHits);
	DEF_NATFUNC(nfColliderMoveHits2);
	DEF_NATFUNC(nfColliderRotateHits);
	DEF_NATFUNC(nfColliderRotateHits2);
	DEF_NATFUNC(nfColliderMoveRotateHits);
	DEF_NATFUNC(nfColliderMoveRotateHits2);
#undef DEF_NATFUNC
};

#endif
