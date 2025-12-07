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

#ifndef _DECLASSSYNTHESIZERINSTANCE_H_
#define _DECLASSSYNTHESIZERINSTANCE_H_

#include <libdscript/libdscript.h>

class deEngine;
class deSynthesizerInstance;
class deScriptingDragonScript;



/**
 * \brief Synthesizer instance script class.
 */
class deClassSynthesizerInstance : public dsClass{
private:
	deScriptingDragonScript &pDS;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create script class. */
	deClassSynthesizerInstance(deScriptingDragonScript &ds);
	
	/** \brief Clean up script class. */
	virtual ~deClassSynthesizerInstance();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Create class members. */
	void CreateClassMembers(dsEngine *engine);
	
	/** \brief Scripting module. */
	inline deScriptingDragonScript &GetDS() const{return pDS;}
	
	/** \brief Synthesizer instance for object or \em NULL if object is \em NULL. */
	deSynthesizerInstance *GetSynthesizerInstance(dsRealObject *object) const;
	
	/** \brief Push animator instance or NULL on the stack. */
	void PushSynthesizerInstance(dsRunTime *rt, deSynthesizerInstance *instance);
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsSynI;
		dsClass *clsSyn;
		dsClass *clsSynController;
		
		dsClass *clsVoid;
		dsClass *clsBool;
		dsClass *clsString;
		dsClass *clsInt;
		dsClass *clsFloat;
		dsClass *clsObject;
	};
	#define DEF_NATFUNC(name) \
	class name : public dsFunction{\
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC(nfNew);
	DEF_NATFUNC(nfDestructor);
	
	DEF_NATFUNC(nfGetSynthesizer);
	DEF_NATFUNC(nfSetSynthesizer);
	DEF_NATFUNC(nfGetSampleCount);
	DEF_NATFUNC(nfSetSampleCount);
	
	DEF_NATFUNC(nfGetControllerCount);
	DEF_NATFUNC(nfSetControllerCount);
	DEF_NATFUNC(nfGetControllerAt);
	DEF_NATFUNC(nfGetControllerNamed);
	DEF_NATFUNC(nfIndexOfControllerNamed);
	DEF_NATFUNC(nfCopyControllerStates);
	DEF_NATFUNC(nfCopyControllerStates2);
	DEF_NATFUNC(nfCopyNamedControllerStates);
	
	DEF_NATFUNC(nfEquals);
	DEF_NATFUNC(nfHashCode);
#undef DEF_NATFUNC
};

#endif
