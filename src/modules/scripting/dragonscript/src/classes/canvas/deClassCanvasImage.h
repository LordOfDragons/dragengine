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

#ifndef _DECLASSCANVASIMAGE_H_
#define _DECLASSCANVASIMAGE_H_

#include <libdscript/libdscript.h>

#include <dragengine/common/math/decMath.h>

class deCanvasImage;
class deScriptingDragonScript;



/**
 * \brief Canvas image script class.
 */
class deClassCanvasImage : public dsClass{
private:
	deScriptingDragonScript &pDS;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new script class. */
	deClassCanvasImage(deScriptingDragonScript &ds);
	
	/** \brief Clean up script class. */
	virtual ~deClassCanvasImage();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Script module. */
	inline deScriptingDragonScript &GetDS(){return pDS;}
	inline const deScriptingDragonScript &GetDS() const{return pDS;}
	
	/** \brief Create script class members. */
	virtual void CreateClassMembers(dsEngine *engine);
	
	/**
	 * \brief Retrieve canvas image from a script object.
	 * \details Returns \em NULL if \em myself is \em NULL.
	 */
	deCanvasImage *GetCanvas(dsRealObject *myself) const;
	
	/**
	 * \brief Push canvas image onto the stack.
	 * \details Pushes a \em null object to the stack if \em collider is \em NULL.
	 */
	void PushCanvas(dsRunTime *rt, deCanvasImage *canvas);
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsCImage, *clsVoid, *clsInt, *clsFlt, *clsStr, *clsObj, *clsBool;
		dsClass *clsImage;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{\
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC(nfNew);
	DEF_NATFUNC(nfDestructor);
	
	DEF_NATFUNC(nfGetImage);
	DEF_NATFUNC(nfSetImage);
	DEF_NATFUNC(nfGetRepeatX);
	DEF_NATFUNC(nfSetRepeatX);
	DEF_NATFUNC(nfGetRepeatY);
	DEF_NATFUNC(nfSetRepeatY);
	
	DEF_NATFUNC(nfEquals);
	DEF_NATFUNC(nfHashCode);
#undef DEF_NATFUNC
};

#endif
