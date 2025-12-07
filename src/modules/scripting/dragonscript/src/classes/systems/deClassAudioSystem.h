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

#ifndef _DECLASSAUDIOSYSTEM_H_
#define _DECLASSAUDIOSYSTEM_H_

#include <libdscript/libdscript.h>

#include <dragengine/common/math/decMath.h>

class deEngine;
class deScriptingDragonScript;



// audio script class
class deClassAudioSystem : public dsClass{
private:
	deScriptingDragonScript &pDS;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create script class. */
	deClassAudioSystem(deScriptingDragonScript &ds);
	
	/** \brief Clean up script class. */
	~deClassAudioSystem();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Create class members. */
	void CreateClassMembers(dsEngine *engine);
	
	/** \brief Module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsAudSys;
		dsClass *clsMic;
		dsClass *clsVoid;
		dsClass *clsInteger;
		dsClass *clsFloat;
		dsClass *clsBool;
		dsClass *clsString;
		dsClass *clsModPar;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{\
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC(nfGetActiveMicrophone);
	DEF_NATFUNC(nfSetActiveMicrophone);
	
	DEF_NATFUNC(nfGetParameterCount);
	DEF_NATFUNC(nfGetParameterInfo);
	DEF_NATFUNC(nfGetParameterInfo2);
	DEF_NATFUNC(nfGetParameterValue);
	DEF_NATFUNC(nfSetParameterValue);
	DEF_NATFUNC(nfSendCommand);
	DEF_NATFUNC(nfGetFPSRate);
#undef DEF_NATFUNC
};

#endif
