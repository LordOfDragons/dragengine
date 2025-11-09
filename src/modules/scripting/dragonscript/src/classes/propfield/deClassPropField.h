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
#ifndef _DEDSCLASSPROPFIELD_H_
#define _DEDSCLASSPROPFIELD_H_

// includes
#include <libdscript/libdscript.h>

// predefinitions
class deScriptingDragonScript;

class dePropField;



/**
 * @brief Prop Field Script Class.
 *
 * Native script class providing access to prop fields.
 */
class deClassPropField : public dsClass{
private:
	deScriptingDragonScript *pDS;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new class. */
	deClassPropField( deScriptingDragonScript *ds );
	/** Cleans up the class. */
	virtual ~deClassPropField();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the module. */
	inline deScriptingDragonScript *GetDS() const{ return pDS; }
	/** Creates class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** Retrieves the prop field or NULL if myself is NULL. */
	dePropField *GetPropField( dsRealObject *myself ) const;
	/** Pushes the prop field which can be NULL. */
	void PushPropField( dsRunTime *rt, dePropField *propfield );
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsPF, *clsVoid, *clsBool, *clsInt, *clsFlt, *clsStr, *clsObj;
		dsClass *clsDVec, *clsVec, *clsMdl, *clsSkin, *clsPFL, *clsCF;
		dsClass *clsWorld;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfSetPosition );
	DEF_NATFUNC(nfGetParentWorld);
	
	DEF_NATFUNC( nfAddType );
	DEF_NATFUNC( nfRemoveAllTypes );
	DEF_NATFUNC( nfSetInstanceAt );
	
	DEF_NATFUNC( nfGetListener );
	DEF_NATFUNC( nfSetListener );
	
	DEF_NATFUNC( nfHashCode );
	DEF_NATFUNC( nfEquals );
#undef DEF_NATFUNC
};

// end of include only once
#endif
