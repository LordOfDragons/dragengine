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

#ifndef _DECLASSSSSOUND_H_
#define _DECLASSSSSOUND_H_

#include <libdscript/libdscript.h>

class deSynthesizer;
class deSynthesizerSourceSound;
class deScriptingDragonScript;



/**
 * \brief Class synthesizer source sound.
 * \details Script class wrapping an attached or detached synthesizer source. Objects of
 *          this class hold a pointer to the source and optional an synthesizer. If the
 *          synthesizer is non-NULL the source pointer is held by the object. Otherwise
 *          only a reference to the synthesizer is held and the pointer to the source
 *          is not held. In addition an index is stored used to update the source in
 *          the synthesizer if existing. This index is not necessary the index of the
 *          source itself but the index of the top most source altered by changes made
 *          to this source.
 */
class deClassSSSound : public dsClass{
public:
	enum eTargets{
		etBlendFactor,
		etVolume,
		etPanning,
		etSpeed,
		etPlay
	};
	
	
	
private:
	deScriptingDragonScript &pDS;
	dsClass *pClsSSSoundTarget;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create script class. */
	deClassSSSound(deScriptingDragonScript &ds);
	
	/** \brief Clean up script class. */
	virtual ~deClassSSSound();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Script module. */
	inline deScriptingDragonScript &GetDS() const{return pDS;}
	
	/** \brief Create class members. */
	void CreateClassMembers(dsEngine *engine);
	
	/** \brief Source or \em NULL if deleted or myself is \em NULL. */
	deSynthesizerSourceSound *GetSource(dsRealObject *myself) const;
	
	/** \brief Assigns synthesizer or \em NULL. */
	void AssignSynthesizer(dsRealObject *myself, deSynthesizer *synthesizer);
	
	/** \brief Pushes a source. */
	void PushSource(dsRunTime *rt, deSynthesizer *synthesizer, deSynthesizerSourceSound *source);
	
	inline dsClass *GetClassSSSoundTarget() const{return pClsSSSoundTarget;}
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsSSSound;
		dsClass *clsSound;
		
		dsClass *clsVoid;
		dsClass *clsBool;
		dsClass *clsString;
		dsClass *clsInt;
		dsClass *clsFloat;
		dsClass *clsObject;
		dsClass *clsSSSoundTarget;
	};
	#define DEF_NATFUNC(name) \
	class name : public dsFunction{\
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC(nfNew);
	DEF_NATFUNC(nfDestructor);
	
	DEF_NATFUNC(nfTargetAddLink);
	DEF_NATFUNC(nfTargetRemoveAllLinks);
	
	DEF_NATFUNC(nfSetSound);
	DEF_NATFUNC(nfSetMinSpeed);
	DEF_NATFUNC(nfSetMaxSpeed);
	DEF_NATFUNC(nfSetLooping);
#undef DEF_NATFUNC
};

#endif
