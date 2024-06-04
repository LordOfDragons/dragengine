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

#ifndef _DECLASSRUNTIMEMETER_H_
#define _DECLASSRUNTIMEMETER_H_

#include <libdscript/libdscript.h>

class deEngine;
class deScriptingDragonScript;



/**
 * \brief Runtime meter script class.
 */
class deClassRuntimeMeter : public dsClass{
private:
	deScriptingDragonScript *pDS;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new class. */
	deClassRuntimeMeter( deScriptingDragonScript *ds );
	/** \brief Cleans up the class. */
	virtual ~deClassRuntimeMeter();
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
		dsClass *clsRTM, *clsVoid, *clsInt, *clsFlt, *clsStr, *clsObj, *clsBool;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfReset );
	DEF_NATFUNC( nfReset2 );
	DEF_NATFUNC( nfElapsed );
	DEF_NATFUNC( nfElapsedMicroSec );
	
	DEF_NATFUNC( nfCounterReset );
	DEF_NATFUNC( nfCounterReset2 );
	DEF_NATFUNC( nfCounterAdd );
	DEF_NATFUNC( nfCounterTime );
	DEF_NATFUNC( nfCounterTimeSeconds );
	DEF_NATFUNC( nfCounterUpdates );
	DEF_NATFUNC( nfCounterAverageTime );
	DEF_NATFUNC( nfCounterAverageTimeSeconds );
#undef DEF_NATFUNC
};

#endif
