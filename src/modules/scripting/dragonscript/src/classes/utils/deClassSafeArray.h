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

#ifndef _DECLASSSAFEARRAY_H_
#define _DECLASSSAFEARRAY_H_

#include <libdscript/libdscript.h>

class deEngine;
class deScriptingDragonScript;



/**
 * \brief Safe array script class.
 * \details Array class safe to be modified while iterating using forEach.
 */
class deClassSafeArray : public dsClass{
private:
	deScriptingDragonScript *pDS;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new class. */
	deClassSafeArray( deScriptingDragonScript *ds );
	/** \brief Cleans up the class. */
	virtual ~deClassSafeArray();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Creates the class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** \brief Retrieves the scripting module. */
	inline deScriptingDragonScript *GetDS() const{ return pDS; }
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsSA, *clsVoid, *clsInt, *clsFlt, *clsStr, *clsObj, *clsBool, *clsBlock;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetCount );
	DEF_NATFUNC( nfHas );
	DEF_NATFUNC( nfIndexOf );
	DEF_NATFUNC( nfGetAt );
	DEF_NATFUNC( nfSetAt );
	DEF_NATFUNC( nfAdd );
	DEF_NATFUNC( nfAddIfAbsent );
	DEF_NATFUNC( nfRemove );
	DEF_NATFUNC( nfRemoveFrom );
	DEF_NATFUNC( nfRemoveIfPresent );
	DEF_NATFUNC( nfRemoveAll );
	DEF_NATFUNC( nfRemoveAll2 );
	
	DEF_NATFUNC( nfForEach );
	DEF_NATFUNC( nfFind );
#undef DEF_NATFUNC
};

#endif
