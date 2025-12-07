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

#ifndef _DECLASSSOUND_H_
#define _DECLASSSOUND_H_

#include <libdscript/libdscript.h>

class deSound;
class deScriptingDragonScript;



/**
 * \brief Sound script class.
 */
class deClassSound : public dsClass{
private:
	deScriptingDragonScript &pDS;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create script class. */
	deClassSound(deScriptingDragonScript &ds);
	
	/** \brief Clean up script class. */
	virtual ~deClassSound();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Script module. */
	inline deScriptingDragonScript &GetDS() const{return pDS;}
	
	/** \brief Create class members. */
	void CreateClassMembers(dsEngine *engine);
	
	/** \brief Get sound from object or \em NULL if \em myself is \em NULL. */
	deSound *GetSound(dsRealObject *myself) const;
	
	/** \brief Push sound to stack or \em NULL if \em sound is \em NULL. */
	void PushSound(dsRunTime *rt, deSound *sound);
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsSound;
		
		dsClass *clsVoid;
		dsClass *clsBool;
		dsClass *clsString;
		dsClass *clsObject;
		dsClass *clsInteger;
		dsClass *clsFloat;
		
		dsClass *clsResNot;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{\
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC(nfNew);
	DEF_NATFUNC(nfLoadAsynchron);
	DEF_NATFUNC(nfDestructor);
	
	DEF_NATFUNC(nfGetFilename);
	DEF_NATFUNC(nfGetBytesPerSample);
	DEF_NATFUNC(nfGetSampleRate);
	DEF_NATFUNC(nfGetChannelCount);
	DEF_NATFUNC(nfGetSampleCount);
	DEF_NATFUNC(nfGetPlayTime);
	
	DEF_NATFUNC(nfEquals);
	DEF_NATFUNC(nfHashCode);
#undef DEF_NATFUNC
};

#endif
