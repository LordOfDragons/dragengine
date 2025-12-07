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

#ifndef _DECLASSSYNTHESIZERSOURCE_H_
#define _DECLASSSYNTHESIZERSOURCE_H_

#include <libdscript/libdscript.h>

class deSynthesizer;
class deSynthesizerSource;
class deScriptingDragonScript;



/**
 * \brief Class Synthesizer source.
 * \details Script class wrapping an attached or detached synthesizer source. Objects of
 *          this class hold a pointer to the source and optional an synthesizer. If the
 *          synthesizer is non-NULL the source pointer is held by the object. Otherwise
 *          only a reference to the synthesizer is held and the pointer to the source
 *          is not held. In addition an index is stored used to update the source in
 *          the synthesizer if existing. This index is not necessary the index of the
 *          source itself but the index of the top most source altered by changes made
 *          to this source.
 */
class deClassSynthesizerSource : public dsClass{
private:
	deScriptingDragonScript &pDS;
	dsClass *pClsSynthesizerSourceMix;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create script class. */
	deClassSynthesizerSource(deScriptingDragonScript &ds);
	
	/** \brief Clean up script class. */
	virtual ~deClassSynthesizerSource();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Script module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Create class members. */
	void CreateClassMembers(dsEngine *engine);
	
	/** \brief Source or \em NULL if deleted or myself is \em NULL. */
	deSynthesizerSource *GetSource(dsRealObject *myself) const;
	
	/** \brief Assigns source or \em NULL. */
	void AssignSource(dsRealObject *myself, deSynthesizerSource *source);
	
	/** \brief Assigns synthesizer or \em NULL. */
	void AssignSynthesizer(dsRealObject *myself, deSynthesizer *synthesizer);
	
	/**
	 * \brief Pushes a source.
	 * \details Delegates the call to the matching source script class.
	 */
	void PushSource(dsRunTime *rt, deSynthesizer *synthesizer, deSynthesizerSource *source);
	
	inline dsClass *GetClassSynthesizerSourceMix() const{ return pClsSynthesizerSourceMix; }
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsSynS;
		
		dsClass *clsSynEff;
		
		dsClass *clsVoid;
		dsClass *clsBool;
		dsClass *clsString;
		dsClass *clsInt;
		dsClass *clsFloat;
		dsClass *clsObject;
		dsClass *clsSynthesizerSourceMix;
	};
	#define DEF_NATFUNC(name) \
	class name : public dsFunction{\
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC(nfNew);
	DEF_NATFUNC(nfDestructor);
	
	DEF_NATFUNC(nfSetEnabled);
	DEF_NATFUNC(nfSetMixMode);
	DEF_NATFUNC(nfSetBlendFactor);
	
	DEF_NATFUNC(nfSetMinVolume);
	DEF_NATFUNC(nfSetMaxVolume);
	DEF_NATFUNC(nfSetMinPanning);
	DEF_NATFUNC(nfSetMaxPanning);
	
	DEF_NATFUNC(nfAddEffect);
	DEF_NATFUNC(nfRemoveEffect);
	DEF_NATFUNC(nfRemoveAllEffects);
#undef DEF_NATFUNC
};

#endif
