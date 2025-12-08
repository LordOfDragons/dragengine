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

#ifndef _DEDSCLASSSERVICE_H_
#define _DEDSCLASSSERVICE_H_

#include <libdscript/libdscript.h>

#include <dragengine/common/utils/decUniqueID.h>

class deScriptingDragonScript;
class deService;


/**
 * Service script class.
 */
class deClassService : public dsClass{
private:
	deScriptingDragonScript &pDS;
	decUniqueID pNextId;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create class. */
	deClassService(deScriptingDragonScript &ds);
	
	/** Clean up class. */
	virtual ~deClassService();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** Creates class members. */
	void CreateClassMembers(dsEngine *engine);
	
	/** Service or nullptr if myself is nullptr. */
	deService *GetService(dsRealObject *myself) const;
	
	/** Push service which can be nullptr. */
	void PushService(dsRunTime *rt, deService::Ref service);
	
	/** Next identifier. */
	decUniqueID NextId();
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsService;
		
		dsClass *clsVoid;
		dsClass *clsBool;
		dsClass *clsInteger;
		dsClass *clsString;
		dsClass *clsObject;
		
		dsClass *clsUniqueID;
		dsClass *clsServiceObject;
		dsClass *clsServiceListener;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{\
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC(nfNew);
	DEF_NATFUNC(nfNew2);
	DEF_NATFUNC(nfDestructor);
	
	DEF_NATFUNC(nfGetName);
	
	DEF_NATFUNC(nfGetListener);
	DEF_NATFUNC(nfSetListener);
	
	DEF_NATFUNC(nfNextId);
	DEF_NATFUNC(nfStartRequest);
	DEF_NATFUNC(nfStartRequest2);
	DEF_NATFUNC(nfCancelRequest);
	DEF_NATFUNC(nfRunAction);
	
	DEF_NATFUNC(nfHashCode);
	DEF_NATFUNC(nfEquals);
#undef DEF_NATFUNC
};

#endif
