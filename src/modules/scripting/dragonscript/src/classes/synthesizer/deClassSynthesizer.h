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

#ifndef _DECLASSSYNTHESIZER_H_
#define _DECLASSSYNTHESIZER_H_

#include <libdscript/libdscript.h>

class deEngine;
class deSynthesizer;
class deScriptingDragonScript;



/**
 * \brief Synthesizer class.
 */
class deClassSynthesizer : public dsClass{
private:
	deScriptingDragonScript &pDS;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create script class. */
	deClassSynthesizer(deScriptingDragonScript &ds);
	
	/** \brief Clean up script class. */
	virtual ~deClassSynthesizer();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Create class members. */
	void CreateClassMembers(dsEngine *engine);
	
	/** \brief Scripting module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief synthesizer or \em NULL if object is \em NULL. */
	deSynthesizer *GetSynthesizer(dsRealObject *object) const;
	
	/** \brief Push synthesizer on the stack (pushes \em nil if instance is \em NULL). */
	void PushSynthesizer(dsRunTime *rt, deSynthesizer *synthesizer);
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsSyn;
		dsClass *clsSynS;
		dsClass *clsSynController;
		dsClass *clsCurveBezier;
		
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
	
	DEF_NATFUNC(nfGetChannelCount);
	DEF_NATFUNC(nfSetChannelCount);
	DEF_NATFUNC(nfGetSampleRate);
	DEF_NATFUNC(nfSetSampleRate);
	DEF_NATFUNC(nfGetBytesPerSample);
	DEF_NATFUNC(nfSetBytesPerSample);
	DEF_NATFUNC(nfGetSampleCount);
	DEF_NATFUNC(nfSetSampleCount);
	
	DEF_NATFUNC(nfGetControllerCount);
	DEF_NATFUNC(nfSetControllerCount);
	DEF_NATFUNC(nfGetControllerAt);
	DEF_NATFUNC(nfGetControllerNamed);
	DEF_NATFUNC(nfIndexOfControllerNamed);
	
	DEF_NATFUNC(nfGetLinkCount);
	DEF_NATFUNC(nfAddLink);
	DEF_NATFUNC(nfRemoveAllLinks);
	DEF_NATFUNC(nfLinkSetController);
	DEF_NATFUNC(nfLinkSetRepeat);
	DEF_NATFUNC(nfLinkSetCurve);
	
	DEF_NATFUNC(nfGetSourceCount);
	DEF_NATFUNC(nfAddSource);
	DEF_NATFUNC(nfGetSourceAt);
	DEF_NATFUNC(nfRemoveAllSources);
	
	DEF_NATFUNC(nfEquals);
	DEF_NATFUNC(nfEquals2);
	DEF_NATFUNC(nfHashCode);
#undef DEF_NATFUNC
};

#endif
