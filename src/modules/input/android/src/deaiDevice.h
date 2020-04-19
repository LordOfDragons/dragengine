/* 
 * Drag[en]gine Android Input Module
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

#ifndef _DEAIDEVICE_H_
#define _DEAIDEVICE_H_

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/input/deInputDevice.h>
#include <dragengine/input/deInputEvent.h>

class deAndroidInput;
class deaiDeviceAxis;
class deaiDeviceButton;
class deaiDeviceFeedback;
class deInputDevice;



/**
 * \brief Android input device.
 */
class deaiDevice : public deObject{
public:
	/** \brief Source identifier. */
	enum eSources{
		/** \brief Android. */
		esAndroid
	};
	
	
	
private:
	deAndroidInput &pModule;
	
	int pIndex;
	eSources pSource;
	
	deInputDevice::eDeviceTypes pType;
	decString pID;
	decString pName;
	
	int pButtonCount;
	deaiDeviceButton *pButtons;
	
	int pAxisCount;
	deaiDeviceAxis *pAxes;
	
	int pFeedbackCount;
	deaiDeviceFeedback *pFeedbacks;
	
	bool pDirtyAxesValues;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create device. */
	deaiDevice( deAndroidInput &module, eSources source );
	
protected:
	/** \brief Clean up device. */
	virtual ~deaiDevice();
	/*@}*/
	
	
	
public:
	/** \name Module Management */
	/*@{*/
	/** \brief X-System input module. */
	inline deAndroidInput &GetModule() const{ return pModule; }
	
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
	
	
	
	/** \brief Number of buttons. */
	inline int GetButtonCount() const{ return pButtonCount; }
	
	/** \brief Set number of buttons. */
	void SetButtonCount( int count );
	
	/** \brief Button at index. */
	deaiDeviceButton &GetButtonAt( int index ) const;
	
	/** \brief Button with identifier or \em NULL if absent. */
	deaiDeviceButton *GetButtonWithID( const char *id ) const;
	
	/** \brief Button with android code or \em NULL if absent. */
	deaiDeviceButton *GetButtonWithAICode( int code ) const;
	
	/** \brief Index of button with identifier or -1 if absent. */
	int IndexOfButtonWithID( const char *id ) const;
	
	/** \brief Index of button with key code or -1 if absent. */
	int IndexOfButtonWithKeyCode( deInputEvent::eKeyCodes code ) const;
	
	/** \brief Index of button with android code or -1 if absent. */
	int IndexOfButtonWithAICode( int code ) const;
	
	
	
	/** \brief Number of axiss. */
	inline int GetAxisCount() const{ return pAxisCount; }
	
	/** \brief Set number of axiss. */
	void SetAxisCount( int count );
	
	/** \brief Axis at index. */
	deaiDeviceAxis &GetAxisAt( int index ) const;
	
	/** \brief Axis with identifier or \em NULL if absent. */
	deaiDeviceAxis *GetAxisWithID( const char *id ) const;
	
	/** \brief Axis with android code or \em NULL if absent. */
	deaiDeviceAxis *GetAxisWithAICode( int code ) const;
	
	/** \brief Index of axis with identifier or -1 if absent. */
	int IndexOfAxisWithID( const char *id ) const;
	
	/** \brief Index of axis with android code or -1 if absent. */
	int IndexOfAxisWithAICode( int code ) const;
	
	
	
	/** \brief Number of feedbacks. */
	inline int GetFeedbackCount() const{ return pFeedbackCount; }
	
	/** \brief Set number of feedbacks. */
	void SetFeedbackCount( int count );
	
	/** \brief Feedback at index. */
	deaiDeviceFeedback &GetFeedbackAt( int index ) const;
	
	/** \brief Feedback with identifier or \em NULL if absent. */
	deaiDeviceFeedback *GetFeedbackWithID( const char *id ) const;
	
	/** \brief Index of feedback with identifier or -1 if absent. */
	int IndexOfFeedbackWithID( const char *id ) const;
	
	
	
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
};

#endif
