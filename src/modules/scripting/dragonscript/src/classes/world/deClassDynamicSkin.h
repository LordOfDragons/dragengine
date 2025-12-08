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
#ifndef _DECLASSDYNAMICSKIN_H_
#define _DECLASSDYNAMICSKIN_H_

// includes
#include <libdscript/libdscript.h>

// predefinitions
class deScriptingDragonScript;
class deDynamicSkin;



/**
 * @brief Dynamic Skin Script Class.
 *
 * Dynamic Skin Script Class.
 */
class deClassDynamicSkin : public dsClass{
private:
	deScriptingDragonScript *pDS;
	dsClass *pClsDynamicSkinRenderableType;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new class. */
	deClassDynamicSkin(deScriptingDragonScript *ds);
	/** Cleans up the class. */
	virtual ~deClassDynamicSkin();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the module. */
	inline deScriptingDragonScript *GetDS() const{ return pDS; }
	/** Creates class members. */
	void CreateClassMembers(dsEngine *engine);
	
	/** Retrieves the dynamic skin from an object. */
	deDynamicSkin *GetDynamicSkin(dsRealObject *myself) const;
	/** Pushes a dynamic skin. */
	void PushDynamicSkin(dsRunTime *rt, deDynamicSkin *dynamicSkin);
	
	inline dsClass *GetClassDynamicSkinRenderableType() const{ return pClsDynamicSkinRenderableType; }
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsDSkin, *clsVoid, *clsBool, *clsInt, *clsFlt, *clsStr, *clsObj;
		dsClass *clsClr, *clsImg, *clsCam, *clsVP, *clsCView;
		dsClass *clsDynamicSkinRenderableType;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{\
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC(nfNew);
	DEF_NATFUNC(nfDestructor);
	
	DEF_NATFUNC(nfGetRenderableCount);
	DEF_NATFUNC(nfIndexOfRenderable);
	DEF_NATFUNC(nfHasRenderable);
	DEF_NATFUNC(nfAddRenderable);
	DEF_NATFUNC(nfRemoveRenderable);
	DEF_NATFUNC(nfRemoveAllRenderables);
	
	DEF_NATFUNC(nfGetTypeAt);
	
	DEF_NATFUNC(nfGetValueAt);
	DEF_NATFUNC(nfSetValueAt);
	
	DEF_NATFUNC(nfGetColorAt);
	DEF_NATFUNC(nfSetColorAt);
	
	DEF_NATFUNC(nfGetImageAt);
	DEF_NATFUNC(nfSetImageAt);
	
	DEF_NATFUNC(nfGetCanvasAt);
	DEF_NATFUNC(nfSetCanvasAt);
	DEF_NATFUNC(nfSetCanvasAt2);
	DEF_NATFUNC(nfSetCanvasAt3);
	
	DEF_NATFUNC(nfGetCameraAt);
	DEF_NATFUNC(nfSetCameraAt);
	
	DEF_NATFUNC(nfGetVideoPlayerAt);
	DEF_NATFUNC(nfSetVideoPlayerAt);
	
	DEF_NATFUNC(nfHashCode);
	DEF_NATFUNC(nfEquals);
#undef DEF_NATFUNC
};

// end of include only once
#endif
