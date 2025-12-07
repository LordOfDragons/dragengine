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

#ifndef _DEDSCLASSINPUTDEVICEFEEDBACK_H_
#define _DEDSCLASSINPUTDEVICEFEEDBACK_H_

#include <libdscript/libdscript.h>

class deScriptingDragonScript;
class dedsInputDevice;


/**
 * \brief Input device feedback script class.
 */
class deClassInputDeviceFeedback : public dsClass{
private:
	deScriptingDragonScript &pDS;
	dsClass *pClsInputDeviceFeedbackType;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create class. */
	deClassInputDeviceFeedback(deScriptingDragonScript &ds);
	
	/** \brief Clean up class. */
	virtual ~deClassInputDeviceFeedback();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Module. */
	inline deScriptingDragonScript &GetDS() const{return pDS;}
	
	/** \brief Creates class members. */
	void CreateClassMembers(dsEngine *engine);
	
	/** \brief Push feedback. */
	void PushFeedback(dsRunTime *rt, dedsInputDevice *device, int index);
	
	inline dsClass *GetClassInputDeviceFeedbackType() const{return pClsInputDeviceFeedbackType;}
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsIDFeedback;
		
		dsClass *clsVoid;
		dsClass *clsBool;
		dsClass *clsInteger;
		dsClass *clsFloat;
		dsClass *clsString;
		dsClass *clsObject;
		
		dsClass *clsInputDevice;
		dsClass *clsInputDeviceFeedbackType;
		dsClass *clsInputDeviceComponent;
		dsClass *clsImage;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{\
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC(nfDestructor);
	
	DEF_NATFUNC(nfGetInputDevice);
	DEF_NATFUNC(nfGetFeedbackIndex);
	
	DEF_NATFUNC(nfGetID);
	DEF_NATFUNC(nfGetName);
	DEF_NATFUNC(nfGetType);
	DEF_NATFUNC(nfGetComponent);
	DEF_NATFUNC(nfGetDisplayImage);
	DEF_NATFUNC(nfGetDisplayIconCount);
	DEF_NATFUNC(nfGetDisplayIconAt);
	DEF_NATFUNC(nfGetLargestDisplayIconX);
	DEF_NATFUNC(nfGetLargestDisplayIconY);
	DEF_NATFUNC(nfGetDisplayText);
	
	DEF_NATFUNC(nfGetValue);
	DEF_NATFUNC(nfSetValue);
	
	DEF_NATFUNC(nfEquals);
#undef DEF_NATFUNC
};

#endif
