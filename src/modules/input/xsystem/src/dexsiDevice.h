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

#ifndef _DEXSIDEVICE_H_
#define _DEXSIDEVICE_H_

#include "dexsiXInclude.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/input/deInputDevice.h>
#include <dragengine/resources/image/deImage.h>

class deXSystemInput;
class dexsiDeviceAxis;
class dexsiDeviceButton;
class dexsiDeviceFeedback;
class deInputDevice;



/**
 * X-System input device.
 */
class dexsiDevice : public deObject{
public:
	/** Type holding strong reference. */
	typedef deTObjectReference<dexsiDevice> Ref;
	
	
	/** Source identifier. */
	enum eSources{
		/** X11. */
		esX11,
		
		/** XInput. */
		esXInput,
		
		/** Libevdev. */
		esLibevdev,
		
		/** Joystick API. */
		esJoystickApi
	};
	
	/** X type. */
	enum eXDeviceTypes{
		exdtKeyboard,
		exdtMouse,
		exdtTablet,
		exdtTouchscreen,
		exdtTouchpad,
		exdtBarcode,
		exdtButtonbox,
		exdtKnobBox,
		exdtOneKnob,
		exdtNineKnob,
		exdtTrackball,
		exdtQuadrature,
		exdtIdModule,
		exdtSpaceball,
		exdtDataglove,
		exdtEyetracker,
		exdtCursorkeys,
		exdtFootmouse,
		exdtJoystick,
		exdtOther
	};
	
	
	
private:
	deXSystemInput &pModule;
	
	int pIndex;
	eSources pSource;
	
	deInputDevice::eDeviceTypes pType;
	decString pID;
	decString pName;
	deImage::Ref pDisplayImage;
	decObjectOrderedSet pDisplayIcons;
	decString pDisplayText;
	
	int pXi2Id;
	eXDeviceTypes pXDeviceType;
	
	decObjectOrderedSet pButtons;
	decObjectOrderedSet pAxes;
	decObjectOrderedSet pFeedbacks;
	
	int pX11FirstKeyCode;
	int pX11KeyCodeCount;
	int *pX11KeyCodeMap;
	
	bool pDirtyAxesValues;
	
	static Atom pXDeviceTypeAtoms[dexsiDevice::exdtOther];
	static bool pXDeviceTypeAtomsReady;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create device. */
	dexsiDevice(deXSystemInput &module, eSources source);
	
	/** Create device from device information. */
	dexsiDevice(deXSystemInput &module, const XIDeviceInfo &info);
	
protected:
	/** Clean up device. */
	virtual ~dexsiDevice();
	/*@}*/
	
	
	
public:
	/** \name Module Management */
	/*@{*/
	/** X-System input module. */
	inline deXSystemInput &GetModule() const{ return pModule; }
	
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
	inline const decObjectOrderedSet &GetDisplayIcons() const{ return pDisplayIcons; }
	
	/** Set display image and icons. */
	void SetDisplayImages(const char *name);
	
	/** Display text. */
	inline const decString &GetDisplayText() const{ return pDisplayText; }
	
	/** Set display text. */
	void SetDisplayText(const char *text);
	
	
	
	/** Number of buttons. */
	int GetButtonCount() const;
	
	/** Add button. */
	void AddButton(dexsiDeviceButton *button);
	
	/** Button at index. */
	dexsiDeviceButton *GetButtonAt(int index) const;
	
	/** Button with identifier or \em NULL if absent. */
	dexsiDeviceButton *GetButtonWithID(const char *id) const;
	
	/** Button with X11 code or \em NULL if absent. */
	dexsiDeviceButton *GetButtonWithX11Code(int code) const;
	
	/** Index of button with identifier or -1 if absent. */
	int IndexOfButtonWithID(const char *id) const;
	
	/** Index of button with X11 code or -1 if absent. */
	int IndexOfButtonWithX11Code(int code) const;
	
	
	
	/** Number of axiss. */
	int GetAxisCount() const;
	
	/** Add axis. */
	void AddAxis(dexsiDeviceAxis *axis);
	
	/** Axis at index. */
	dexsiDeviceAxis *GetAxisAt(int index) const;
	
	/** Axis with identifier or \em NULL if absent. */
	dexsiDeviceAxis *GetAxisWithID(const char *id) const;
	
	/** Axis with X11 code or \em NULL if absent. */
	dexsiDeviceAxis *GetAxisWithX11Code(int code) const;
	
	/** Index of axis with identifier or -1 if absent. */
	int IndexOfAxisWithID(const char *id) const;
	
	/** Index of axis with X11 code or -1 if absent. */
	int IndexOfAxisWithX11Code(int code) const;
	
	
	
	/** Number of feedbacks. */
	int GetFeedbackCount() const;
	
	/** Add feedback. */
	void AddFeedback(dexsiDeviceFeedback *feedback);
	
	/** Feedback at index. */
	dexsiDeviceFeedback *GetFeedbackAt(int index) const;
	
	/** Feedback with identifier or \em NULL if absent. */
	dexsiDeviceFeedback *GetFeedbackWithID(const char *id) const;
	
	/** Index of feedback with identifier or -1 if absent. */
	int IndexOfFeedbackWithID(const char *id) const;
	
	
	
	/** First key code. */
	inline int GetX11FirstKeyCode() const{ return pX11FirstKeyCode; }
	
	/** Key code count. */
	inline int GetX11KeyCodeCount() const{ return pX11KeyCodeCount; }
	
	/** Reset key code map. */
	void ResetX11KeyCodeMap(int firstKeyCode, int keyCodeCount);
	
	/** Set key code map entry. */
	void SetLookupX11KeyCode(int keyCode, int button);
	
	/** Get button for key code or -1 if not found. */
	int LookupX11KeyCode(int keyCode) const;
	
	
	
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
	
	
	
private:
	void pPrepareXDeviceTypeAtoms();
	void pUpdateDeviceIndices();
};

#endif
