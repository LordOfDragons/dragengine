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

#ifndef _DEDSCLASSNAVIGATOR_H_
#define _DEDSCLASSNAVIGATOR_H_

#include <libdscript/libdscript.h>

class deScriptingDragonScript;

class deNavigator;



/**
 * @brief Navigator Script Class.
 */
class deClassNavigator : public dsClass{
private:
	deScriptingDragonScript *pDS;
	dsClass *pClsNavigationSpaceType;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new class. */
	deClassNavigator(deScriptingDragonScript *ds);
	/** Cleans up the class. */
	virtual ~deClassNavigator();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the module. */
	inline deScriptingDragonScript *GetDS() const{ return pDS; }
	/** Creates class members. */
	void CreateClassMembers(dsEngine *engine);
	
	/** Retrieves the navigator or NULL if myself is NULL. */
	deNavigator *GetNavigator(dsRealObject *myself) const;
	/** Pushes the navigator which can be NULL. */
	void PushNavigator(dsRunTime *rt, deNavigator::Ref navigator);
	
	inline dsClass *GetClassNavigationSpaceType() const{ return pClsNavigationSpaceType; }
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsNavigator;
		
		dsClass *clsBool;
		dsClass *clsFloat;
		dsClass *clsInteger;
		dsClass *clsObject;
		dsClass *clsString;
		dsClass *clsVoid;
		
		dsClass *clsCollider;
		dsClass *clsDVector;
		dsClass *clsNavInfo;
		dsClass *clsQuaternion;
		dsClass *clsVector;
		dsClass *clsNavPath;
		dsClass *clsNavigationSpaceType;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{\
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC(nfNew);
	DEF_NATFUNC(nfDestructor);
	
	DEF_NATFUNC(nfGetLayer);
	DEF_NATFUNC(nfSetLayer);
	DEF_NATFUNC(nfGetSpaceType);
	DEF_NATFUNC(nfSetSpaceType);
	DEF_NATFUNC(nfGetMaxOutsideDistance);
	DEF_NATFUNC(nfSetMaxOutsideDistance);
	
	DEF_NATFUNC(nfGetDefaultFixCost);
	DEF_NATFUNC(nfSetDefaultFixCost);
	DEF_NATFUNC(nfGetDefaultCostPerMeter);
	DEF_NATFUNC(nfSetDefaultCostPerMeter);
	DEF_NATFUNC(nfGetBlockingCost);
	DEF_NATFUNC(nfSetBlockingCost);
	
	DEF_NATFUNC(nfGetTypeFixCost);
	DEF_NATFUNC(nfSetTypeFixCost);
	DEF_NATFUNC(nfGetTypeCostPerMeter);
	DEF_NATFUNC(nfSetTypeCostPerMeter);
	DEF_NATFUNC(nfRemoveType);
	DEF_NATFUNC(nfRemoveAllTypes);
	
	DEF_NATFUNC(nfFindPath);
	
	DEF_NATFUNC(nfNearestPoint);
	DEF_NATFUNC(nfLineCollide);
	
	DEF_NATFUNC(nfPathCollideRay);
	DEF_NATFUNC(nfPathCollideRay2);
	DEF_NATFUNC(nfPathCollideShape);
	DEF_NATFUNC(nfPathCollideShape2);
	
	DEF_NATFUNC(nfHashCode);
	DEF_NATFUNC(nfEquals);
#undef DEF_NATFUNC
};

#endif
