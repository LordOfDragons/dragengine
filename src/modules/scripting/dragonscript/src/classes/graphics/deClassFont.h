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

#ifndef _DECLASSFONT_H_
#define _DECLASSFONT_H_

#include <libdscript/libdscript.h>

class deFont;
class deEngine;
class deScriptingDragonScript;



/**
 * \brief Font script class.
 */
class deClassFont : public dsClass{
private:
	deScriptingDragonScript &pDS;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create script class. */
	deClassFont(deScriptingDragonScript &ds);
	
	/** \brief Clean up script class. */
	virtual ~deClassFont();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Create class members. */
	void CreateClassMembers(dsEngine *engine);
	
	/** \brief Scripting module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Font or \em NULL if object is \em NULL. */
	deFont *GetFont(dsRealObject *object) const;
	
	/** \brief Font size or 0 if object is \em NULL. */
	int GetFontSize(dsRealObject *object) const;
	
	/** \brief Pushe font on stack or null object if \em font is \em NULL. */
	void PushFont(dsRunTime *rt, deFont::Ref font, int size);
	
	
	
private:
	struct sInitData{
		dsClass *clsFont;
		
		dsClass *clsVoid;
		dsClass *clsInteger;
		dsClass *clsFloat;
		dsClass *clsBool;
		dsClass *clsByte;
		dsClass *clsString;
		dsClass *clsObject;
		
		dsClass *clsPoint;
		dsClass *clsUnicodeString;
		dsClass *clsResourceListener;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{\
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC(nfNew);
	DEF_NATFUNC(nfNewSize);
	DEF_NATFUNC(nfLoadAsynchron);
	DEF_NATFUNC(nfDestructor);
	
	DEF_NATFUNC(nfGetFilename);
	DEF_NATFUNC(nfGetSize);
	DEF_NATFUNC(nfGetBaseLine);
	
	DEF_NATFUNC(nfGetTextSize);
	DEF_NATFUNC(nfGetCharWidth);
	DEF_NATFUNC(nfGetCharAdvance);
	DEF_NATFUNC(nfGetCharBearing);
	DEF_NATFUNC(nfGetMaxWidth);
	DEF_NATFUNC(nfHasGlyph);
	
	DEF_NATFUNC(nfEquals);
	DEF_NATFUNC(nfHashCode);
#undef DEF_NATFUNC
};

#endif
