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
 
