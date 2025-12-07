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

#ifndef _DECLASSINPUTEVENT_H_
#define _DECLASSINPUTEVENT_H_

#include <libdscript/libdscript.h>

class deScriptingDragonScript;

class deInputEvent;



/**
 * \brief Input event script class.
 */
class deClassInputEvent : public dsClass{
private:
	deScriptingDragonScript &pDS;
	dsClass *pClsInputEventType;
	dsClass *pClsInputEventSource;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create class. */
	deClassInputEvent(deScriptingDragonScript &ds);
	
	/** \brief Clean up class. */
	virtual ~deClassInputEvent();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Creates class members. */
	void CreateClassMembers(dsEngine *engine);
	
	/** \brief InputEvent or \em NULL if myself is \em NULL. */
	const deInputEvent &GetInputEvent(dsRealObject *myself) const;
	
	/** \brief Push input event which can be \em NULL. */
	void PushInputEvent(dsRunTime *rt, const deInputEvent &event);
	
	inline dsClass *GetClassInputEventType() const{ return pClsInputEventType; }
	inline dsClass *GetClassInputEventSource() const{ return pClsInputEventSource; }
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsInputEvent;
		
		dsClass *clsVoid;
		dsClass *clsBool;
		dsClass *clsInteger;
		dsClass *clsFloat;
		dsClass *clsString;
		dsClass *clsObject;
		dsClass *clsInputEventType;
		dsClass *clsInputEventSource;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{\
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC(nfDestructor);
	
	DEF_NATFUNC(nfGetType);
	DEF_NATFUNC(nfGetDevice);
	DEF_NATFUNC(nfGetCode);
	DEF_NATFUNC(nfGetState);
	DEF_NATFUNC(nfGetKeyCode);
	DEF_NATFUNC(nfGetKeyChar);
	DEF_NATFUNC(nfGetX);
	DEF_NATFUNC(nfGetY);
	DEF_NATFUNC(nfGetValue);
	DEF_NATFUNC(nfGetTime);
	DEF_NATFUNC(nfGetSource);
	
	DEF_NATFUNC(nfEquals);
	DEF_NATFUNC(nfHashCode);
#undef DEF_NATFUNC
};

#endif
