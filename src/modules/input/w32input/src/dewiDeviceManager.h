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

#ifndef _DEWIDEVICEMANAGER_H_
#define _DEWIDEVICEMANAGER_H_

#include "dewiInclude.h"
#include "dewiDeviceKeyboard.h"
#include "dewiDeviceMouse.h"

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Gaming.Input.h>

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/threading/deMutex.h>

class deWindowsInput;
class dewiDevice;
class dewiDeviceWinRTController;

namespace wrf = winrt::Windows::Foundation;
namespace wrgi = winrt::Windows::Gaming::Input;


/**
 * Windows input devices.
 */
class dewiDeviceManager : public deObject{
public:
	typedef deTObjectReference<dewiDeviceManager> Ref;
	
	
private:
	deWindowsInput &pModule;
	
	decObjectOrderedSet pDevices;
	
	dewiDeviceMouse::Ref pMouse;
	dewiDeviceKeyboard::Ref pKeyboard;
	
	deMutex pMutex;
	std::vector<wrgi::RawGameController> pAddControllers;
	std::vector<wrgi::RawGameController> pRemoveControllers;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create device list. */
	dewiDeviceManager( deWindowsInput &module );
	
protected:
	/** Clean up device list. */
	virtual ~dewiDeviceManager();
	/*@}*/
	
	
	
public:
	/** \name Module Management */
	/*@{*/
	/** Update list of available devices. */
	void UpdateDeviceList();
	
	
	
	/** Count of devices. */
	int GetCount() const;
	
	/** Device at index. */
	dewiDevice *GetAt( int index ) const;
	
	/** Device with identifier or \em NULL if absent. */
	dewiDevice *GetWithID( const char *id ) const;
	
	/** Index of device with identifier or -1 if absent. */
	int IndexOfWithID( const char *id ) const;
	
	/** Input device matching controller. */
	dewiDeviceWinRTController *GetWithController( wrgi::RawGameController const &controller ) const;
	
	
	
	/** Mouse device. */
	inline const dewiDeviceMouse::Ref &GetMouse() const{ return pMouse; }
	
	/** Keyboard device. */
	inline const dewiDeviceKeyboard::Ref &GetKeyboard() const{ return pKeyboard; }
	

	
	/** Log list of input devices. */
	void LogDevices() const;

	/** Log input device. */
	void LogDevice( const dewiDevice &device ) const;
	


	/** Update devices and track attach/detach events. */
	void Update();

	
	
	/** Normalize identifier. */
	static decString NormalizeID( const char *id );
	/*@}*/
	
	
	
private:
	class sEventHandlerController : public winrt::implements<sEventHandlerController, wrf::IInspectable>{
	private:
		deMutex pMutex;
		dewiDeviceManager *pManager;

	public:
		sEventHandlerController( dewiDeviceManager *manager );
		void DropManager();

	private:
		void pOnControllerAdded( wrf::IInspectable const &sender, wrgi::RawGameController const &controller );
		void pOnControllerRemoved( wrf::IInspectable const &sender, wrgi::RawGameController const &controller );
	};

	winrt::com_ptr<sEventHandlerController> pEventHandlerController;

	friend class sEventHandlerController;

	void pCleanUp();

	void pCreateDevices();
	void pCreateControllers();
	void pProcessAddRemoveDevices();
	void pUpdateDeviceIndices();

	void pOnControllerAdded( wrgi::RawGameController const &controller );
	void pOnControllerRemoved( wrgi::RawGameController const &controller );
};

#endif
 
