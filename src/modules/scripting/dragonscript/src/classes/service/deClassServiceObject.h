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

#ifndef _DEDSCLASSSERVICEOBJECT_H_
#define _DEDSCLASSSERVICEOBJECT_H_

#include <libdscript/libdscript.h>

class deScriptingDragonScript;
class deServiceObject;
class deResource;


/**
 * Service object script class.
 */
class deClassServiceObject : public dsClass{
private:
	deScriptingDragonScript &pDS;
	dsClass *pClsServiceObjectType;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create class. */
	deClassServiceObject( deScriptingDragonScript &ds );
	
	/** Clean up class. */
	virtual ~deClassServiceObject();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** Creates class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** Service object or nullptr if myself is nullptr. */
	deServiceObject *GetServiceObject( dsRealObject *myself ) const;
	
	/** Push service object which can be nullptr. */
	void PushServiceObject( dsRunTime *rt, deServiceObject *object );
	
	inline dsClass *GetClassServiceObjectType() const{ return pClsServiceObjectType; }
	
	/**
	 * Retrieve the engine resource object for a script object.
	 * 
	 * Return nullptr if the value is not an object, is nullptr or does not match a
	 * resource script class.
	 */
	deResource *GetResource( dsValue &myself ) const;
	
	/**
	 * Push engine resource onto stack as script object.
	 * 
	 * Push nullptr object if the engine resource is null or does not have a script class.
	 */
	void PushResource( dsRunTime &rt, deResource *resource );
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsServiceObject;
		
		dsClass *clsVoid;
		dsClass *clsBool;
		dsClass *clsInteger;
		dsClass *clsFloat;
		dsClass *clsString;
		dsClass *clsObject;
		dsClass *clsArray;
		
		dsClass *clsMemoryFile;
		dsClass *clsServiceObjectType;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfNewBool );
	DEF_NATFUNC( nfNewInteger );
	DEF_NATFUNC( nfNewFloat );
	DEF_NATFUNC( nfNewString );
	DEF_NATFUNC( nfNewResource );
	DEF_NATFUNC( nfNewData );
	DEF_NATFUNC( nfNewCopy );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetValueType );
	
	DEF_NATFUNC( nfIsBoolean );
	DEF_NATFUNC( nfGetBoolean );
	
	DEF_NATFUNC( nfIsInteger );
	DEF_NATFUNC( nfGetInteger );
	
	DEF_NATFUNC( nfIsFloat );
	DEF_NATFUNC( nfGetFloat );
	
	DEF_NATFUNC( nfIsString );
	DEF_NATFUNC( nfGetString );
	
	DEF_NATFUNC( nfIsResource );
	DEF_NATFUNC( nfGetResource );
	
	DEF_NATFUNC( nfIsData );
	DEF_NATFUNC( nfGetData );
	
	DEF_NATFUNC( nfIsDictionary );
	DEF_NATFUNC( nfGetChildCount );
	DEF_NATFUNC( nfGetChildrenKeys );
	DEF_NATFUNC( nfGetChildAt );
	DEF_NATFUNC( nfAddChild );
	DEF_NATFUNC( nfRemoveChild );
	DEF_NATFUNC( nfRemoveAllChildren );
	
	DEF_NATFUNC( nfHashCode );
	DEF_NATFUNC( nfEquals );
#undef DEF_NATFUNC
};

#endif
