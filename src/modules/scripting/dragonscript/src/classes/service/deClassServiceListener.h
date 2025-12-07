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

#ifndef _DEDSCLASSSERVICELISTENER_H_
#define _DEDSCLASSSERVICELISTENER_H_

#include <libdscript/libdscript.h>

class deScriptingDragonScript;


/**
 * \brief Service listener script interface.
 */
class deClassServiceListener : public dsClass{
private:
	deScriptingDragonScript &pDS;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create class. */
	deClassServiceListener(deScriptingDragonScript &ds);
	
	/** \brief Clean up class. */
	virtual ~deClassServiceListener();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Creates class members. */
	void CreateClassMembers(dsEngine *engine);
	
	/** \brief Function index for requestResponse(). */
	inline int GetFuncIndexRequestResponse() const{ return pFuncIndexRequestResponse; }
	
	/** \brief Function index for requestFailed(). */
	inline int GetFuncIndexRequestFailed() const{ return pFuncIndexRequestFailed; }
	
	/** \brief Function index for eventReceived(). */
	inline int GetFuncIndexEventReceived() const{ return pFuncIndexEventReceived; }
	/*@}*/
	
	
	
private:
	int pFuncIndexRequestResponse;
	int pFuncIndexRequestFailed;
	int pFuncIndexEventReceived;
	
	struct sInitData{
		dsClass *clsServiceListener;
		
		dsClass *clsVoid;
		dsClass *clsBoolean;
		
		dsClass *clsUniqueID;
		dsClass *clsService;
		dsClass *clsServiceObject;
	};
	
#define DEF_NATFUNC(name) \
	class name : public dsFunction{\
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC(nfRequestResponse);
	DEF_NATFUNC(nfRequestFailed);
	DEF_NATFUNC(nfEventReceived);
#undef DEF_NATFUNC
};

#endif
