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

#ifndef _DEMOIDEVICE_H_
#define _DEMOIDEVICE_H_

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/input/deInputDevice.h>

class deMacOSInput;
class demoiDeviceAxis;
class demoiDeviceButton;
class deInputDevice;



/**
 * \brief MacOS input device.
 */
class demoiDevice : public deObject{
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<demoiDevice> Ref;


public:
    /** \brief Source identifier. */
    enum eSources{
        /** \brief MacOS. */
        esMO
    };
    
    
    
private:
	deMacOSInput &pModule;
	
    int pIndex;
    eSources pSource;
    
	deInputDevice::eDeviceTypes pType;
	decString pID;
	decString pName;
	
	int pButtonCount;
	demoiDeviceButton *pButtons;
	
	int pAxisCount;
	demoiDeviceAxis *pAxes;
	
    bool pDirtyAxisValues;
    
    
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create device. */
	demoiDevice(deMacOSInput &module, eSources source);
	
protected:
	/** \brief Clean up device. */
	virtual ~demoiDevice();
	/*@}*/
	
	
	
public:
	/** \name Module Management */
	/*@{*/
	/** \brief MacOS input module. */
	inline deMacOSInput &GetModule() const{return pModule;}
	
    /** \brief Index. */
    inline int GetIndex() const{return pIndex;}
    
    /** \brief Set index. */
    void SetIndex(int index);
    
    /** \brief Source. */
    inline eSources GetSource() const{return pSource;}
    
	
	
	/** \brief Device type. */
	inline deInputDevice::eDeviceTypes GetType() const{return pType;}
	
	/** \brief Set device type. */
	void SetType(deInputDevice::eDeviceTypes type);
	
	/** \brief Identifier. */
 	inline const decString &GetID() const{return pID;}
	
	/** \brief Set identifier. */
 	void SetID(const char *id);
	
	/** \brief Name. */
	inline const decString &GetName() const{return pName;}
	
	/** \brief Set name. */
	void SetName(const char *name);
	
	
	
	/** \brief Number of buttons. */
	inline int GetButtonCount() const{return pButtonCount;}
	
	/** \brief Set number of buttons. */
	void SetButtonCount(int count);
	
	/** \brief Button at index. */
	demoiDeviceButton &GetButtonAt(int index) const;
	
	/** \brief Button with identifier or \em NULL if absent. */
	demoiDeviceButton *GetButtonWithID(const char *id) const;
	
	/** \brief Index of button with identifier or -1 if absent. */
	int IndexOfButtonWithID(const char *id) const;
	
    /** \brief Index of button with MO code or -1 if absent. */
    int IndexOfButtonWithMOCode(int code) const;
    
	
	
	/** \brief Number of axiss. */
	inline int GetAxisCount() const{return pAxisCount;}
	
	/** \brief Set number of axiss. */
	void SetAxisCount(int count);
	
	/** \brief Axis at index. */
	demoiDeviceAxis &GetAxisAt(int index) const;
	
	/** \brief Axis with identifier or \em NULL if absent. */
	demoiDeviceAxis *GetAxisWithID(const char *id) const;
	
	/** \brief Index of axis with identifier or -1 if absent. */
	int IndexOfAxisWithID(const char *id) const;
	
    /** \brief Index of axis with MO code or -1 if absent. */
    int IndexOfAxisWithMOCode(int code) const;
    
    
    
    /** \brief One or more axis values are dirty. */
    inline bool GetDirtyAxisValues() const{return pDirtyAxisValues;}
    
    /** \brief Set if one or more axis values are dirty. */
    void SetDirtyAxisValues(bool dirty);
    
	
	
	/** \brief Update engine input device information. */
	void GetInfo(deInputDevice &info) const;
    
    /** \brief Update device state. */
    virtual void Update();
    
    /** \brief Send axis events if dirty. */
    void SendDirtyAxisEvents();
	/*@}*/
};

#endif
