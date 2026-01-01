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

// include only once
#ifndef _DECLASSTOUCHSENSOR_H_
#define _DECLASSTOUCHSENSOR_H_

// includes
#include <libdscript/libdscript.h>
#include <dragengine/common/math/decMath.h>

// predefinitions
class deEngine;
class deTouchSensor;
class deClassVector;
class deClassDVector;
class deClassCollider;
class deClassQuaternion;
class deClassTouchSensorListener;
class deScriptingDragonScript;



// scene touchSensor script class
class deClassTouchSensor : public dsClass{
private:
	deEngine *pGameEngine;
	deScriptingDragonScript *pDS;
	
	deClassTouchSensorListener *pClsTSL;
	deClassVector *pClsVec;
	deClassDVector *pClsDVec;
	deClassCollider *pClsCol;
	deClassQuaternion *pClsQuat;
	
public:
	// constructor
	deClassTouchSensor(deEngine *gameEngine, deScriptingDragonScript *scrMgr);
	~deClassTouchSensor();
	// internal functions
	void CreateClassMembers(dsEngine *engine);
	deTouchSensor *GetTouchSensor(dsRealObject *myself) const;
	void PushTouchSensor(dsRunTime *rt, deTouchSensor *touchSensor);
	inline deEngine *GetGameEngine() const{ return pGameEngine; }
	
	inline deClassTouchSensorListener *GetClassTouchSensorListener() const{ return pClsTSL; }
	inline deClassVector *GetClassVector() const{ return pClsVec; }
	inline deClassDVector *GetClassDVector() const{ return pClsDVec; }
	inline deClassCollider *GetClassCollider() const{ return pClsCol; }
	inline deClassQuaternion *GetClassQuaternion() const{ return pClsQuat; }
	inline deScriptingDragonScript *GetDS() const{ return pDS; }
	
private:
	struct sInitData{
		dsClass *clsTS, *clsVoid, *clsInt, *clsFlt, *clsStr, *clsQuat, *clsVec;
		dsClass *clsObj, *clsBool, *clsTSL, *clsTer, *clsCol, *clsDVec, *clsCF;
		dsClass *clsColLis, *clsShapeList;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{\
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC(nfNew);
	DEF_NATFUNC(nfDestructor);
	
	DEF_NATFUNC(nfGetPosition);
	DEF_NATFUNC(nfSetPosition);
	DEF_NATFUNC(nfGetOrientation);
	DEF_NATFUNC(nfSetOrientation);
	
	DEF_NATFUNC(nfGetCollisionFilter);
	DEF_NATFUNC(nfSetCollisionFilter);
	DEF_NATFUNC(nfGetTrackEnterLeave);
	DEF_NATFUNC(nfSetTrackEnterLeave);
	DEF_NATFUNC(nfGetEnabled);
	DEF_NATFUNC(nfSetEnabled);
	DEF_NATFUNC(nfGetShape);
	DEF_NATFUNC(nfSetShape);
	
	DEF_NATFUNC(nfIsEmpty);
	DEF_NATFUNC(nfGetColliderCount);
	DEF_NATFUNC(nfGetColliderAt);
	
	DEF_NATFUNC(nfGetIgnoreColliderCount);
	DEF_NATFUNC(nfGetIgnoreColliderAt);
	DEF_NATFUNC(nfHasIgnoreCollider);
	DEF_NATFUNC(nfAddIgnoreCollider);
	DEF_NATFUNC(nfRemoveIgnoreCollider);
	DEF_NATFUNC(nfRemoveAllIgnoreColliders);
	
	DEF_NATFUNC(nfPointInside);
	DEF_NATFUNC(nfAllHits);
	DEF_NATFUNC(nfRayHits);
	DEF_NATFUNC(nfRayHitsClosest);
	DEF_NATFUNC(nfColliderHits);
	DEF_NATFUNC(nfColliderMoveHits);
	DEF_NATFUNC(nfColliderMoveHitsClosest);
	DEF_NATFUNC(nfColliderRotateHits);
	DEF_NATFUNC(nfColliderRotateHitsClosest);
	DEF_NATFUNC(nfColliderMoveRotateHits);
	DEF_NATFUNC(nfColliderMoveRotateHitsClosest);
	
	DEF_NATFUNC(nfGetListener);
	DEF_NATFUNC(nfSetListener);
	
	DEF_NATFUNC(nfEquals);
	DEF_NATFUNC(nfHashCode);
#undef DEF_NATFUNC
};

// end of include only once
#endif
