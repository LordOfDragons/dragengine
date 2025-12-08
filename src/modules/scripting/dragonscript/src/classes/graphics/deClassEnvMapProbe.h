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

#ifndef _DECLASSENVMAPPROBE_H_
#define _DECLASSENVMAPPROBE_H_

#include <libdscript/libdscript.h>

class deEngine;
class deEnvMapProbe;
class deScriptingDragonScript;



/**
 * @brief Environment Map Probe Class.
 */
class deClassEnvMapProbe : public dsClass{
private:
	deScriptingDragonScript *pDS;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new script class. */
	deClassEnvMapProbe(deScriptingDragonScript *ds);
	/** Cleans up the script class. */
	~deClassEnvMapProbe();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Creates the class members. */
	void CreateClassMembers(dsEngine *engine);
	/** Retrieves the scripting module. */
	inline deScriptingDragonScript *GetDS() const{ return pDS; }
	
	/** Retrieves the environment map probe or NULL if object is NULL. */
	deEnvMapProbe *GetEnvMapProbe(dsRealObject *object) const;
	/** Pushes an environment map probe on the stack (pushes null if instance is NULL). */
	void PushEnvMapProbe(dsRunTime *rt, deEnvMapProbe *envMapProbe);
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsEmp;
		
		dsClass *clsVoid;
		dsClass *clsBool;
		dsClass *clsStr;
		dsClass *clsInt;
		dsClass *clsFlt;
		dsClass *clsObj;
		
		dsClass *clsDVec;
		dsClass *clsQuat;
		dsClass *clsVec;
		dsClass *clsShaList;
		dsClass *clsLayerMask;
		dsClass *clsWorld;
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
	DEF_NATFUNC(nfGetScaling);
	DEF_NATFUNC(nfSetScaling);
	
	DEF_NATFUNC(nfGetLayerMask);
	DEF_NATFUNC(nfSetLayerMask);
	DEF_NATFUNC(nfGetParentWorld);
	
	DEF_NATFUNC(nfGetShapeListInfluence);
	DEF_NATFUNC(nfSetShapeListInfluence);
	
	DEF_NATFUNC(nfGetShapeReflection);
	DEF_NATFUNC(nfSetShapeReflection);
	DEF_NATFUNC(nfGetShapeListReflectionMask);
	DEF_NATFUNC(nfSetShapeListReflectionMask);
	
	DEF_NATFUNC(nfGetInfluenceBorderSize);
	DEF_NATFUNC(nfSetInfluenceBorderSize);
	DEF_NATFUNC(nfGetInfluencePriority);
	DEF_NATFUNC(nfSetInfluencePriority);
	
	DEF_NATFUNC(nfHashCode);
	DEF_NATFUNC(nfEquals);
	DEF_NATFUNC(nfEquals2);
#undef DEF_NATFUNC
};

#endif
