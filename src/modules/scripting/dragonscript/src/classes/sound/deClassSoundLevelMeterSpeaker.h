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

#ifndef _DECLASSSOUNDLEVELMETERSPEAKER_H_
#define _DECLASSSOUNDLEVELMETERSPEAKER_H_

#include <dragengine/resources/sensor/deSoundLevelMeter.h>

#include <libdscript/libdscript.h>

class deEngine;
class deScriptingDragonScript;


/**
 * \brief Sound level meter speaker script class.
 */
class deClassSoundLevelMeterSpeaker : public dsClass{
private:
	deScriptingDragonScript &pDS;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates script class. */
	deClassSoundLevelMeterSpeaker(deScriptingDragonScript &ds);
	
	/** \brief Clean up script class. */
	virtual ~deClassSoundLevelMeterSpeaker();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Create script class members. */
	void CreateClassMembers(dsEngine *engine);
	
	/** \brief Push sound level meter or NULL onto the stack. */
	void PushSoundLevelMeterSpeaker(dsRunTime *rt, const deSoundLevelMeter::cAudibleSpeaker &speaker);
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsSoundLevelMeterSpeaker;
		
		dsClass *clsVoid;
		dsClass *clsInteger;
		dsClass *clsFloat;
		dsClass *clsString;
		dsClass *clsObject;
		dsClass *clsBool;
		
		dsClass *clsSpeaker;
	};
	
#define DEF_NATFUNC(name) \
	class name : public dsFunction{\
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC(nfNew);
	DEF_NATFUNC(nfDestructor);
	
	DEF_NATFUNC(nfGetSpeaker);
	DEF_NATFUNC(nfGetVolume);
#undef DEF_NATFUNC
};

#endif
