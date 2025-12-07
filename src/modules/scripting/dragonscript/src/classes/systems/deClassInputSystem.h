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

#ifndef _DECLASSINPUTSYSTEM_H_
#define _DECLASSINPUTSYSTEM_H_

#include <dragengine/common/collection/decObjectList.h>

#include <libdscript/libdscript.h>

class deScriptingDragonScript;
class dedsInputDevice;



/**
 * Input system script class.
 */
class deClassInputSystem : public dsClass{
private:
	deScriptingDragonScript &pDS;
	
	decObjectList pCachedDevices;
	bool pCacheDirty;
	dsClass *pClsInputEventKeyLocation;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create script class. */
	deClassInputSystem(deScriptingDragonScript &ds);
	
	/** Clean up script class. */
	virtual ~deClassInputSystem();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Create class members. */
	virtual void CreateClassMembers(dsEngine *engine);
	
	/** Script module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** Cached input device count. */
	int GetCachedDeviceCount();
	
	/** Cached device. */
	dedsInputDevice *GetCachedDeviceAt(int index);
	
	/** Invalidate cached devices. */
	void InvalidCachedDevices();
	
	/** Frame update. */
	void OnFrameUpdate();
	
	inline dsClass *GetClassInputEventKeyLocation() const{ return pClsInputEventKeyLocation; }
	/*@}*/
	
	
	
private:
	void pUpdateCachedDevices();
	
	
	struct sInitData{
		dsClass *clsInpSys;
		dsClass *clsVoid;
		dsClass *clsInteger;
		dsClass *clsFloat;
		dsClass *clsBool;
		dsClass *clsString;
		
		dsClass *clsModPar;
		dsClass *clsInputDevice;
		dsClass *clsInputEventKeyLocation;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{\
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	
	DEF_NATFUNC(nfGetCaptureInputDevices);
	DEF_NATFUNC(nfSetCaptureInputDevices);
	
	DEF_NATFUNC(nfGetDeviceCount);
	DEF_NATFUNC(nfGetDeviceAt);
	DEF_NATFUNC(nfIndexOfDeviceWithID);
	DEF_NATFUNC(nfIndexOfButtonWithID);
	DEF_NATFUNC(nfIndexOfAxisWithID);
	DEF_NATFUNC(nfIndexOfFeedbackWithID);
	DEF_NATFUNC(nfGetButtonPressed);
	DEF_NATFUNC(nfGetButtonTouched);
	DEF_NATFUNC(nfGetButtonNear);
	DEF_NATFUNC(nfGetAxisValue);
	DEF_NATFUNC(nfGetFeedbackValue);
	DEF_NATFUNC(nfSetFeedbackValue);
	DEF_NATFUNC(nfButtonMatchingKeyCode);
	DEF_NATFUNC(nfButtonMatchingKeyChar);
	DEF_NATFUNC(nfButtonMatchingKeyCode2);
	DEF_NATFUNC(nfButtonMatchingKeyChar2);
	
	DEF_NATFUNC(nfGetParameterCount);
	DEF_NATFUNC(nfGetParameterInfo);
	DEF_NATFUNC(nfGetParameterInfo2);
	DEF_NATFUNC(nfGetParameterValue);
	DEF_NATFUNC(nfSetParameterValue);
	DEF_NATFUNC(nfSendCommand);
#undef DEF_NATFUNC
};

#endif
