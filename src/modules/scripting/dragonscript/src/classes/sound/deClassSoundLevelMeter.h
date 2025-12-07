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

#ifndef _DECLASSSOUNDLEVELMETER_H_
#define _DECLASSSOUNDLEVELMETER_H_

#include <libdscript/libdscript.h>

class deEngine;
class deScriptingDragonScript;
class deSoundLevelMeter;


/**
 * \brief Sound level meter script class.
 */
class deClassSoundLevelMeter : public dsClass{
private:
	deScriptingDragonScript &pDS;
	dsClass *pClsSoundLevelMeterType;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create script class. */
	deClassSoundLevelMeter(deScriptingDragonScript &ds);
	
	/** \brief Clean up script class. */
	virtual ~deClassSoundLevelMeter();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Module. */
	inline deScriptingDragonScript &GetDS() const{return pDS;}
	
	/** \brief Create script class members. */
	void CreateClassMembers(dsEngine *engine);
	
	/** \brief Sound level meter from real object or NULL if real object is NULL. */
	deSoundLevelMeter *GetSoundLevelMeter(dsRealObject *myself) const;
	
	/** \brief Push sound level meter or NULL onto the stack. */
	void PushSoundLevelMeter(dsRunTime *rt, deSoundLevelMeter *soundLevelMeter);
	
	inline dsClass *GetClassSoundLevelMeterType() const{return pClsSoundLevelMeterType;}
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsSoundLevelMeter;
		
		dsClass *clsVoid;
		dsClass *clsInteger;
		dsClass *clsFloat;
		dsClass *clsString;
		dsClass *clsObject;
		dsClass *clsBool;
		
		dsClass *clsQuaternion;
		dsClass *clsDVector;
		dsClass *clsLayerMask;
		dsClass *clsSoundLevelMeterSpeaker;
		dsClass *clsSoundLevelMeterListener;
		dsClass *clsSoundLevelMeterType;
	};
	
#define DEF_NATFUNC(name) \
	class name : public dsFunction{\
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC(nfNew);
	DEF_NATFUNC(nfDestructor);
	
	DEF_NATFUNC(nfGetType);
	DEF_NATFUNC(nfSetType);
	DEF_NATFUNC(nfGetPosition);
	DEF_NATFUNC(nfSetPosition);
	DEF_NATFUNC(nfGetOrientation);
	DEF_NATFUNC(nfSetOrientation);
	DEF_NATFUNC(nfGetConeAngle);
	DEF_NATFUNC(nfSetConeAngle);
	DEF_NATFUNC(nfGetAudibleDistance);
	DEF_NATFUNC(nfSetAudibleDistance);
	DEF_NATFUNC(nfGetLayerMask);
	DEF_NATFUNC(nfSetLayerMask);
	DEF_NATFUNC(nfGetEnabled);
	DEF_NATFUNC(nfSetEnabled);
	
	DEF_NATFUNC(nfGetAudibleSpeakerCount);
	DEF_NATFUNC(nfGetAudibleSpeakerAt);
	
	DEF_NATFUNC(nfGetListener);
	DEF_NATFUNC(nfSetListener);
	
	DEF_NATFUNC(nfEquals);
	DEF_NATFUNC(nfHashCode);
#undef DEF_NATFUNC
};

#endif
