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

#ifndef _DECLASSEDITABLEIMAGE_H_
#define _DECLASSEDITABLEIMAGE_H_

#include <libdscript/libdscript.h>


class deEngine;
class deImage;
class deScriptingDragonScript;
class deClassResourceListener;


/**
 * \brief Editable image class.
 */
class deClassEditableImage : public dsClass{
private:
	deScriptingDragonScript &pDS;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create script class. */
	deClassEditableImage(deScriptingDragonScript &ds);
	
	/** \brief Clean up script class. */
	virtual ~deClassEditableImage();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Create class members. */
	void CreateClassMembers(dsEngine *engine);
	
	/** \brief Scripting module. */
	inline deScriptingDragonScript &GetDS() const{return pDS;}
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsEditableImage;
		dsClass *clsVoid;
		dsClass *clsBool;
		dsClass *clsString;
		dsClass *clsInteger;
		dsClass *clsObject;
		dsClass *clsImage;
		dsClass *clsColor;
		dsClass *clsArray;
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
	DEF_NATFUNC(nfGetWidth);
	DEF_NATFUNC(nfGetHeight);
	DEF_NATFUNC(nfGetDepth);
	
	DEF_NATFUNC(nfGetAt);
	DEF_NATFUNC(nfGetAt2);
	DEF_NATFUNC(nfGetRange);
	DEF_NATFUNC(nfGetRange2);
	
	DEF_NATFUNC(nfClear);
	DEF_NATFUNC(nfSetAt);
	DEF_NATFUNC(nfSetAt2);
	DEF_NATFUNC(nfSetRange);
	DEF_NATFUNC(nfSetRange2);
	DEF_NATFUNC(nfContentChanged);
#undef DEF_NATFUNC
};

#endif
