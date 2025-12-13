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
#ifndef _DECLASSQUATERNION_H_
#define _DECLASSQUATERNION_H_

// includes
#include <libdscript/libdscript.h>
#include <dragengine/common/math/decMath.h>

// predefinitions
class deEngine;
class deScriptingDragonScript;

// quaternion script class
class deClassQuaternion : public dsClass{
private:
	deEngine *pGameEngine;
	deScriptingDragonScript *pScrMgr;
public:
	// constructor
	deClassQuaternion(deEngine *gameEngine, deScriptingDragonScript *scriptManager);
	~deClassQuaternion();
	// internal functions
	void CreateClassMembers(dsEngine *engine);
	inline deEngine *GetGameEngine() const{ return pGameEngine; }
	inline deScriptingDragonScript *GetScriptModule() const{ return pScrMgr; }
	const decQuaternion &GetQuaternion(dsRealObject *This) const;
	void PushQuaternion(dsRunTime *RT, const decQuaternion &vector);

private:
	struct sInitData{
		dsClass *clsQuat;
		
		dsClass *clsVoid;
		dsClass *clsBool;
		dsClass *clsStr;
		dsClass *clsObj;
		dsClass *clsInt;
		dsClass *clsFlt;
		
		dsClass *clsVec;
		dsClass *clsDVec;
		dsClass *clsFileReader;
		dsClass *clsFileWriter;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{\
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC(nfNew);
	DEF_NATFUNC(nfNew2);
	DEF_NATFUNC(nfNew3);
	DEF_NATFUNC(nfNewFromEuler);
	DEF_NATFUNC(nfNewFromEuler2);
	DEF_NATFUNC(nfNewFromEulerX);
	DEF_NATFUNC(nfNewFromEulerY);
	DEF_NATFUNC(nfNewFromEulerZ);
	DEF_NATFUNC(nfNewFromAxis);
	DEF_NATFUNC(nfDestructor);
	
	DEF_NATFUNC(nfGetX);
	DEF_NATFUNC(nfGetY);
	DEF_NATFUNC(nfGetZ);
	DEF_NATFUNC(nfGetW);
	DEF_NATFUNC(nfGet);
	DEF_NATFUNC(nfGetLength);
	DEF_NATFUNC(nfNormalize);
	DEF_NATFUNC(nfConjugate);
	DEF_NATFUNC(nfAbsolute);
	DEF_NATFUNC(nfGetEulerAngles);
	DEF_NATFUNC(nfSlerp);
	
	DEF_NATFUNC(nfIsEqualTo);
	
	DEF_NATFUNC(nfReadFromFile);
	DEF_NATFUNC(nfWriteToFile);
	
	DEF_NATFUNC(nfOpMinus);
	DEF_NATFUNC(nfOpAdd);
	DEF_NATFUNC(nfOpSubtract);
	DEF_NATFUNC(nfOpScale);
	DEF_NATFUNC(nfOpDivide);
	DEF_NATFUNC(nfOpMultiply);
	DEF_NATFUNC(nfOpMultiplyVector);
	DEF_NATFUNC(nfOpMultiplyDVector);
	
	DEF_NATFUNC(nfEquals);
	DEF_NATFUNC(nfHashCode);
	DEF_NATFUNC(nfToString);
	DEF_NATFUNC(nfToStringPrecision);
#undef DEF_NATFUNC
};

// end of include only once
#endif
