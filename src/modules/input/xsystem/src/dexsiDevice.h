/* 
 * Drag[en]gine X System Input Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _DEXSIDEVICE_H_
#define _DEXSIDEVICE_H_

#include "dexsiXInclude.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/input/deInputDevice.h>
#include <dragengine/resources/image/deImageReference.h>

class deXSystemInput;
class dexsiDeviceAxis;
class dexsiDeviceButton;
class dexsiDeviceFeedback;
class deInputDevice;



/**
 * \brief X-System input device.
 */
class dexsiDevice : public deObject{
public:
	/** \brief Source identifier. */
	enum eSources{
		/** \brief X11. */
		esX11,
		
		/** \brief XInput. */
		esXInput,
		
		/** \brief Libevdev. */
		esLibevdev,
		
		/** \brief Joystick API. */
		esJoystickApi
	};
	
	/** \brief X type. */
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
	deImageReference pDisplayImage;
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
	
	static Atom pXDeviceTypeAtoms[ dexsiDevice::exdtOther ];
	static bool pXDeviceTypeAtomsReady;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create device. */
	dexsiDevice( deXSystemInput &module, eSources source );
	
	/** \brief Create device from device information. */
	dexsiDevice( deXSystemInput &module, const XIDeviceInfo &info );
	
protected:
	/** \brief Clean up device. */
	virtual ~dexsiDevice();
	/*@}*/
	
	
	
public:
	/** \name Module Management */
	/*@{*/
	/** \brief X-System input module. */
	inline deXSystemInput &GetModule() const{ return pModule; }
	
	/** \brief Index. */
	inline int GetIndex() const{ return pIndex; }
	
	/** \brief Set index. */
	void SetIndex( int index );
	
	/** \brief Source. */
	inline eSources GetSource() const{ return pSource; }
	
	
	
	/** \brief Device type. */
	inline deInputDevice::eDeviceTypes GetType() const{ return pType; }
	
	/** \brief Set device type. */
	void SetType( deInputDevice::eDeviceTypes type );
	
	/** \brief Identifier. */
	inline const decString &GetID() const{ return pID; }
	
	/** \brief Set identifier. */
	void SetID( const char *id );
	
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set name. */
	void SetName( const char *name );
	
	/** \brief Display image. */
	inline deImage *GetDisplayImage() const{ return pDisplayImage; }
	
	/** \brief Display icons (deImage*). */
	inline const decObjectOrderedSet &GetDisplayIcons() const{ return pDisplayIcons; }
	
	/** \brief Set display image and icons. */
	void SetDisplayImages( const char *name );
	
	/** \brief Display text. */
	inline const decString &GetDisplayText() const{ return pDisplayText; }
	
	/** \brief Set display text. */
	void SetDisplayText( const char *text );
	
	
	
	/** \brief Number of buttons. */
	int GetButtonCount() const;
	
	/** \brief Add button. */
	void AddButton( dexsiDeviceButton *button );
	
	/** \brief Button at index. */
	dexsiDeviceButton *GetButtonAt( int index ) const;
	
	/** \brief Button with identifier or \em NULL if absent. */
	dexsiDeviceButton *GetButtonWithID( const char *id ) const;
	
	/** \brief Button with X11 code or \em NULL if absent. */
	dexsiDeviceButton *GetButtonWithX11Code( int code ) const;
	
	/** \brief Index of button with identifier or -1 if absent. */
	int IndexOfButtonWithID( const char *id ) const;
	
	/** \brief Index of button with X11 code or -1 if absent. */
	int IndexOfButtonWithX11Code( int code ) const;
	
	
	
	/** \brief Number of axiss. */
	int GetAxisCount() const;
	
	/** \brief Add axis. */
	void AddAxis( dexsiDeviceAxis *axis );
	
	/** \brief Axis at index. */
	dexsiDeviceAxis *GetAxisAt( int index ) const;
	
	/** \brief Axis with identifier or \em NULL if absent. */
	dexsiDeviceAxis *GetAxisWithID( const char *id ) const;
	
	/** \brief Axis with X11 code or \em NULL if absent. */
	dexsiDeviceAxis *GetAxisWithX11Code( int code ) const;
	
	/** \brief Index of axis with identifier or -1 if absent. */
	int IndexOfAxisWithID( const char *id ) const;
	
	/** \brief Index of axis with X11 code or -1 if absent. */
	int IndexOfAxisWithX11Code( int code ) const;
	
	
	
	/** \brief Number of feedbacks. */
	int GetFeedbackCount() const;
	
	/** \brief Add feedback. */
	void AddFeedback( dexsiDeviceFeedback *feedback );
	
	/** \brief Feedback at index. */
	dexsiDeviceFeedback *GetFeedbackAt( int index ) const;
	
	/** \brief Feedback with identifier or \em NULL if absent. */
	dexsiDeviceFeedback *GetFeedbackWithID( const char *id ) const;
	
	/** \brief Index of feedback with identifier or -1 if absent. */
	int IndexOfFeedbackWithID( const char *id ) const;
	
	
	
	/** \brief First key code. */
	inline int GetX11FirstKeyCode() const{ return pX11FirstKeyCode; }
	
	/** \brief Key code count. */
	inline int GetX11KeyCodeCount() const{ return pX11KeyCodeCount; }
	
	/** \brief Reset key code map. */
	void ResetX11KeyCodeMap( int firstKeyCode, int keyCodeCount );
	
	/** \brief Set key code map entry. */
	void SetLookupX11KeyCode( int keyCode, int button );
	
	/** \brief Get button for key code or -1 if not found. */
	int LookupX11KeyCode( int keyCode ) const;
	
	
	
	/** \brief One or more axes values are dirty. */
	inline bool GetDirtyAxesValues() const{ return pDirtyAxesValues; }
	
	/** \brief Set if one or more axes values are dirty. */
	void SetDirtyAxesValues( bool dirty );
	
	
	
	/** \brief Update engine input device information. */
	void GetInfo( deInputDevice &info ) const;
	
	/** \brief Update device state. */
	virtual void Update();
	
	/** \brief Send axis events if dirty. */
	void SendDirtyAxisEvents();
	/*@}*/
	
	
	
private:
	void pPrepareXDeviceTypeAtoms();
};

#endif
