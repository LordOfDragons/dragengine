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

#ifndef _DEAINPDEVICE_H_
#define _DEAINPDEVICE_H_

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/input/deInputDevice.h>
#include <dragengine/input/deInputEvent.h>

class deAndroidInput;
class deainpDeviceAxis;
class deainpDeviceButton;
class deainpDeviceFeedback;
class deInputDevice;



/**
 * \brief Android input device.
 */
class deainpDevice : public deObject{
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
	deainpDeviceButton *pButtons;
	
	int pAxisCount;
	deainpDeviceAxis *pAxes;
	
	int pFeedbackCount;
	deainpDeviceFeedback *pFeedbacks;
	
	bool pDirtyAxesValues;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create device. */
	deainpDevice( deAndroidInput &module, eSources source );
	
protected:
	/** \brief Clean up device. */
	virtual ~deainpDevice();
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
	deainpDeviceButton &GetButtonAt( int index ) const;
	
	/** \brief Button with identifier or \em NULL if absent. */
	deainpDeviceButton *GetButtonWithID( const char *id ) const;
	
	/** \brief Button with android code or \em NULL if absent. */
	deainpDeviceButton *GetButtonWithAICode( int code ) const;
	
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
	deainpDeviceAxis &GetAxisAt( int index ) const;
	
	/** \brief Axis with identifier or \em NULL if absent. */
	deainpDeviceAxis *GetAxisWithID( const char *id ) const;
	
	/** \brief Axis with android code or \em NULL if absent. */
	deainpDeviceAxis *GetAxisWithAICode( int code ) const;
	
	/** \brief Index of axis with identifier or -1 if absent. */
	int IndexOfAxisWithID( const char *id ) const;
	
	/** \brief Index of axis with android code or -1 if absent. */
	int IndexOfAxisWithAICode( int code ) const;
	
	
	
	/** \brief Number of feedbacks. */
	inline int GetFeedbackCount() const{ return pFeedbackCount; }
	
	/** \brief Set number of feedbacks. */
	void SetFeedbackCount( int count );
	
	/** \brief Feedback at index. */
	deainpDeviceFeedback &GetFeedbackAt( int index ) const;
	
	/** \brief Feedback with identifier or \em NULL if absent. */
	deainpDeviceFeedback *GetFeedbackWithID( const char *id ) const;
	
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
