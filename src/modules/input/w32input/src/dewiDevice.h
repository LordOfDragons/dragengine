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

#ifndef _DEWIDEVICE_H_
#define _DEWIDEVICE_H_

#include "dewiInclude.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decTOrderedSet.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/input/deInputDevice.h>
#include <dragengine/resources/image/deImage.h>

class deWindowsInput;
class dewiDeviceAxis;
class dewiDeviceButton;
class dewiDeviceFeedback;
class deInputDevice;



/**
 * Windows input device.
 */
class dewiDevice : public deObject{
public:
	/** Type holding strong reference. */
	typedef deTObjectReference<dewiDevice> Ref;
	
	
	/** Source identifier. */
	enum eSources{
		/** Windows. */
		esWindows,

		/** WinRT GameInput Controller. */
		esWinRTController
	};
	
	
	
private:
	deWindowsInput &pModule;
	
	int pIndex;
	eSources pSource;
	
	deInputDevice::eDeviceTypes pType;
	decString pID;
	decString pName;
	deImage::Ref pDisplayImage;
	decTObjectOrderedSet<deImage> pDisplayIcons;
	decString pDisplayText;
	
	decTObjectOrderedSet<dewiDeviceButton> pButtons;
	decTObjectOrderedSet<dewiDeviceAxis> pAxes;
	decTObjectOrderedSet<dewiDeviceFeedback> pFeedbacks;
	
	bool pDirtyAxesValues;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create device. */
	dewiDevice(deWindowsInput &module, eSources source);
	
protected:
	/** Clean up device. */
	virtual ~dewiDevice();
	/*@}*/
	
	
	
public:
	/** \name Module Management */
	/*@{*/
	/** Windows input module. */
	inline deWindowsInput &GetModule() const{ return pModule; }
	
	/** Index. */
	inline int GetIndex() const{ return pIndex; }
	
	/** Set index. */
	void SetIndex(int index);
	
	/** Source. */
	inline eSources GetSource() const{ return pSource; }
	
	
	
	/** Device type. */
	inline deInputDevice::eDeviceTypes GetType() const{ return pType; }
	
	/** Set device type. */
	void SetType(deInputDevice::eDeviceTypes type);
	
	/** Identifier. */
	inline const decString &GetID() const{ return pID; }
	
	/** Set identifier. */
	void SetID(const char *id);
	
	/** Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** Set name. */
	void SetName(const char *name);
	
	/** Display image. */
	inline const deImage::Ref &GetDisplayImage() const{ return pDisplayImage; }
	
	/** Display icons (deImage*). */
	inline const decTObjectOrderedSet<deImage> &GetDisplayIcons() const{ return pDisplayIcons; }
	
	/** Set display image and icons. */
	void SetDisplayImages(const char *name);
	
	/** Display text. */
	inline const decString &GetDisplayText() const{ return pDisplayText; }
	
	/** Set display text. */
	void SetDisplayText(const char *text);
	
	
	
	/** Buttons. */
	inline const decTObjectOrderedSet<dewiDeviceButton> &GetButtons() const{ return pButtons; }
	
	/** Add button. */
	void AddButton(dewiDeviceButton *button);
	
	/** Button with identifier or \em NULL if absent. */
	dewiDeviceButton *GetButtonWithID(const char *id) const;
	
	/** Button with WI code or \em NULL if absent. */
	dewiDeviceButton *GetButtonWithWICode(int code) const;
	
	/** Index of button with identifier or -1 if absent. */
	int IndexOfButtonWithID(const char *id) const;
	
	/** Index of button with WI code or -1 if absent. */
	int IndexOfButtonWithWICode(int code) const;
	
	
	
	/** Axes. */
	inline const decTObjectOrderedSet<dewiDeviceAxis> &GetAxes() const{ return pAxes; }
	
	/** Add axis. */
	void AddAxis(dewiDeviceAxis *axis);
	
	/** Axis with identifier or \em NULL if absent. */
	dewiDeviceAxis *GetAxisWithID(const char *id) const;
	
	/** Axis with WI code or \em NULL if absent. */
	dewiDeviceAxis *GetAxisWithWICode(int code) const;
	
	/** Index of axis with identifier or -1 if absent. */
	int IndexOfAxisWithID(const char *id) const;
	
	/** Index of axis with WI code or -1 if absent. */
	int IndexOfAxisWithWICode(int code) const;
	
	
	
	/** Feedback. */
	inline const decTObjectOrderedSet<dewiDeviceFeedback> &GetFeedbacks() const{ return pFeedbacks; }
	
	/** Add feedback. */
	void AddFeedback(dewiDeviceFeedback *feedback);
	
	/** Feedback with identifier or \em NULL if absent. */
	dewiDeviceFeedback *GetFeedbackWithID(const char *id) const;
	
	/** Index of feedback with identifier or -1 if absent. */
	int IndexOfFeedbackWithID(const char *id) const;
	
	
	
	/** One or more axes values are dirty. */
	inline bool GetDirtyAxesValues() const{ return pDirtyAxesValues; }
	
	/** Set if one or more axes values are dirty. */
	void SetDirtyAxesValues(bool dirty);
	
	
	
	/** Update engine input device information. */
	void GetInfo(deInputDevice &info) const;
	
	/** Update device state. */
	virtual void Update();
	
	/** Send axis events if dirty. */
	void SendDirtyAxisEvents();
	/*@}*/
};

#endif
