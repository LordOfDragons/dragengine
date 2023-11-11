/* 
 * Drag[en]gine Windows Input Module
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

#ifndef _DEWIDEVICE_H_
#define _DEWIDEVICE_H_

#include "dewiInclude.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/input/deInputDevice.h>
#include <dragengine/resources/image/deImageReference.h>

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
	deImageReference pDisplayImage;
	decObjectOrderedSet pDisplayIcons;
	decString pDisplayText;
	
	decObjectOrderedSet pButtons;
	decObjectOrderedSet pAxes;
	decObjectOrderedSet pFeedbacks;
	
	bool pDirtyAxesValues;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create device. */
	dewiDevice( deWindowsInput &module, eSources source );
	
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
	void SetIndex( int index );
	
	/** Source. */
	inline eSources GetSource() const{ return pSource; }
	
	
	
	/** Device type. */
	inline deInputDevice::eDeviceTypes GetType() const{ return pType; }
	
	/** Set device type. */
	void SetType( deInputDevice::eDeviceTypes type );
	
	/** Identifier. */
	inline const decString &GetID() const{ return pID; }
	
	/** Set identifier. */
	void SetID( const char *id );
	
	/** Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** Set name. */
	void SetName( const char *name );
	
	/** Display image. */
	inline deImage *GetDisplayImage() const{ return pDisplayImage; }
	
	/** Display icons (deImage*). */
	inline const decObjectOrderedSet &GetDisplayIcons() const{ return pDisplayIcons; }
	
	/** Set display image and icons. */
	void SetDisplayImages( const char *name );
	
	/** Display text. */
	inline const decString &GetDisplayText() const{ return pDisplayText; }
	
	/** Set display text. */
	void SetDisplayText( const char *text );
	
	
	
	/** Number of buttons. */
	int GetButtonCount() const;
	
	/** Add button. */
	void AddButton( dewiDeviceButton *button );
	
	/** Button at index. */
	dewiDeviceButton *GetButtonAt( int index ) const;
	
	/** Button with identifier or \em NULL if absent. */
	dewiDeviceButton *GetButtonWithID( const char *id ) const;
	
	/** Button with WI code or \em NULL if absent. */
	dewiDeviceButton *GetButtonWithWICode( int code ) const;
	
	/** Index of button with identifier or -1 if absent. */
	int IndexOfButtonWithID( const char *id ) const;
	
	/** Index of button with WI code or -1 if absent. */
	int IndexOfButtonWithWICode( int code ) const;
	
	
	
	/** Number of axiss. */
	int GetAxisCount() const;
	
	/** Add axis. */
	void AddAxis( dewiDeviceAxis *axis );
	
	/** Axis at index. */
	dewiDeviceAxis *GetAxisAt( int index ) const;
	
	/** Axis with identifier or \em NULL if absent. */
	dewiDeviceAxis *GetAxisWithID( const char *id ) const;
	
	/** Axis with WI code or \em NULL if absent. */
	dewiDeviceAxis *GetAxisWithWICode( int code ) const;
	
	/** Index of axis with identifier or -1 if absent. */
	int IndexOfAxisWithID( const char *id ) const;
	
	/** Index of axis with WI code or -1 if absent. */
	int IndexOfAxisWithWICode( int code ) const;
	
	
	
	/** Number of feedbacks. */
	int GetFeedbackCount() const;
	
	/** Add feedback. */
	void AddFeedback( dewiDeviceFeedback *feedback );
	
	/** Feedback at index. */
	dewiDeviceFeedback *GetFeedbackAt( int index ) const;
	
	/** Feedback with identifier or \em NULL if absent. */
	dewiDeviceFeedback *GetFeedbackWithID( const char *id ) const;
	
	/** Index of feedback with identifier or -1 if absent. */
	int IndexOfFeedbackWithID( const char *id ) const;
	
	
	
	/** One or more axes values are dirty. */
	inline bool GetDirtyAxesValues() const{ return pDirtyAxesValues; }
	
	/** Set if one or more axes values are dirty. */
	void SetDirtyAxesValues( bool dirty );
	
	
	
	/** Update engine input device information. */
	void GetInfo( deInputDevice &info ) const;
	
	/** Update device state. */
	virtual void Update();
	
	/** Send axis events if dirty. */
	void SendDirtyAxisEvents();
	/*@}*/
};

#endif
