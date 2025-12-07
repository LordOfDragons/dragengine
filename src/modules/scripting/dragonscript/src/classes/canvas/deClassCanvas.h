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

#ifndef _DECLASSCANVAS_H_
#define _DECLASSCANVAS_H_

#include <libdscript/libdscript.h>

#include <dragengine/common/math/decMath.h>

class deCanvas;
class deScriptingDragonScript;



/**
 * \brief Canvas script class.
 */
class deClassCanvas : public dsClass{
private:
	deScriptingDragonScript &pDS;
	dsClass *pClsCanvasBlendMode;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new script class. */
	deClassCanvas(deScriptingDragonScript &ds);
	
	/** \brief Clean up script class. */
	virtual ~deClassCanvas();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Script module. */
	inline deScriptingDragonScript &GetDS(){return pDS;}
	inline const deScriptingDragonScript &GetDS() const{return pDS;}
	inline dsClass *GetClassCanvasBlendMode() const{return pClsCanvasBlendMode;}
	
	/** \brief Create script class members. */
	virtual void CreateClassMembers(dsEngine *engine);
	
	/**
	 * \brief Retrieve canvas from a script object.
	 * \details Returns \em NULL if \em myself is \em NULL.
	 */
	deCanvas *GetCanvas(dsRealObject *myself) const;
	
	/**
	 * \brief Push canvas onto the stack.
	 * \details Pushes a \em null object to the stack if \em collider is \em NULL.
	 */
	void PushCanvas(dsRunTime *rt, deCanvas *canvas);
	
	/** \brief Assigns canvas or \em NULL. */
	void AssignCanvas(dsRealObject *myself, deCanvas *canvas);
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsCanvas, *clsVoid, *clsInt, *clsFlt, *clsStr, *clsObj, *clsBool;
		dsClass *clsPoint, *clsTexMat2, *clsClrMat;
		dsClass *clsCanvasBlendMode;
		dsClass *clsCanvasView;
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
	DEF_NATFUNC(nfGetSize);
	DEF_NATFUNC(nfSetSize);
	DEF_NATFUNC(nfGetTransform);
	DEF_NATFUNC(nfSetTransform);
	DEF_NATFUNC(nfGetColorTransform);
	DEF_NATFUNC(nfSetColorTransform);
	DEF_NATFUNC(nfGetVisible);
	DEF_NATFUNC(nfSetVisible);
	DEF_NATFUNC(nfGetOrder);
	DEF_NATFUNC(nfSetOrder);
	DEF_NATFUNC(nfGetTransparency);
	DEF_NATFUNC(nfSetTransparency);
	DEF_NATFUNC(nfGetBlendMode);
	DEF_NATFUNC(nfSetBlendMode);
	DEF_NATFUNC(nfGetMask);
	DEF_NATFUNC(nfSetMask);
	DEF_NATFUNC(nfGetParentView);
	DEF_NATFUNC(nfGetParentMask);
	
	DEF_NATFUNC(nfEquals);
	DEF_NATFUNC(nfHashCode);
#undef DEF_NATFUNC
};

#endif
