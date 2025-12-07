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
#ifndef _DEDSCLASSCURVE2D_H_
#define _DEDSCLASSCURVE2D_H_

// includes
#include <libdscript/libdscript.h>

#include <dragengine/common/curve/decCurve2D.h>

// predefinitions
class deScriptingDragonScript;

class deParticleSystem;



/**
 * @brief 2D-Curve Script Class.
 *
 * Native script class providing access to 2D-Curves.
 */
class deClassCurve2D : public dsClass{
private:
	deScriptingDragonScript *pDS;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new class. */
	deClassCurve2D(deScriptingDragonScript *ds);
	/** Cleans up the class. */
	virtual ~deClassCurve2D();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the module. */
	inline deScriptingDragonScript *GetDS() const{return pDS;}
	/** Creates class members. */
	void CreateClassMembers(dsEngine *engine);
	
	/** Retrieves the curve. */
	const decCurve2D &GetCurve(dsRealObject *myself) const;
	/** Pushes the curve. */
	void PushCurve(dsRunTime *rt, const decCurve2D &curve);
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsCurve2D, *clsVoid, *clsBool, *clsInt, *clsFlt, *clsStr, *clsObj;
		dsClass *clsVec2;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{\
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC(nfNew);
	DEF_NATFUNC(nfDestructor);
	
	DEF_NATFUNC(nfGetPointCount);
	DEF_NATFUNC(nfGetPointAt);
	DEF_NATFUNC(nfIndexOfPointClosestTo);
	DEF_NATFUNC(nfIndexOfPointBefore);
	DEF_NATFUNC(nfAddPoint);
	DEF_NATFUNC(nfAddPoint2);
	DEF_NATFUNC(nfRemovePointFrom);
	DEF_NATFUNC(nfRemoveAllPoints);
	
	DEF_NATFUNC(nfSetDefaultCurve);
	DEF_NATFUNC(nfOffsetPointsBy);
	DEF_NATFUNC(nfScalePointsBy);
	
	DEF_NATFUNC(nfEvaluateConstant);
	DEF_NATFUNC(nfEvaluateLinear);
	
	DEF_NATFUNC(nfHashCode);
	DEF_NATFUNC(nfEquals);
#undef DEF_NATFUNC
};

// end of include only once
#endif
