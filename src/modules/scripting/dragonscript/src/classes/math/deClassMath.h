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

#ifndef _DECLASSMATH_H_
#define _DECLASSMATH_H_

#include <libdscript/libdscript.h>
#include <dragengine/common/math/decMath.h>

class deScriptingDragonScript;



/**
 * @brief Math Routines.
 * Script class providing math routines. This class provides all the functions the DragonScript
 * Math class does but differs in that they operate using degrees instead of radians. In
 * addition some helper functions are provided for common operations to reduce the scripting work.
 */
class deClassMath : public dsClass{
private:
	deScriptingDragonScript *pDS;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new script class. */
	deClassMath(deScriptingDragonScript *ds);
	/** Cleans up the script class. */
	virtual ~deClassMath();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the module. */
	inline deScriptingDragonScript *GetDS() const{ return pDS; }
	/** Creates class members. */
	void CreateClassMembers(dsEngine *engine);
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsMath, *clsVoid, *clsBool, *clsStr, *clsInt, *clsFlt, *clsObj;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{\
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	
	DEF_NATFUNC(nfacos);
	DEF_NATFUNC(nfasin);
	DEF_NATFUNC(nfatan);
	DEF_NATFUNC(nfatan2);
	DEF_NATFUNC(nfcos);
	DEF_NATFUNC(nfcosh);
	DEF_NATFUNC(nfsin);
	DEF_NATFUNC(nfsinh);
	DEF_NATFUNC(nftan);
	DEF_NATFUNC(nftanh);
	
	DEF_NATFUNC(nfDegToRad);
	DEF_NATFUNC(nfRadToDeg);
	DEF_NATFUNC(nfLinearStep);
	DEF_NATFUNC(nfLinearStep2);
	DEF_NATFUNC(nfSmoothStep);
	DEF_NATFUNC(nfSmoothStep2);
	DEF_NATFUNC(nfNormalize);
	DEF_NATFUNC(nfNormalize2);
	DEF_NATFUNC(nfMix);
	DEF_NATFUNC(nfRandom);
	DEF_NATFUNC(nfRandomInt);
	DEF_NATFUNC(nfRandomFloat);
	DEF_NATFUNC(nfProbability);
#undef DEF_NATFUNC
};

#endif
