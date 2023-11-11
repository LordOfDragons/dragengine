/* 
 * Drag[en]gine Windows Input Module
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEWIDEVICEWINRTCONTROLLER_H_
#define _DEWIDEVICEWINRTCONTROLLER_H_

#include "dewiDevice.h"

#include <memory>
#include <vector>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Gaming.Input.h>

#include <dragengine/input/deInputEvent.h>

namespace wrf = winrt::Windows::Foundation;
namespace wrgi = winrt::Windows::Gaming::Input;



/**
 * WinRT GameInput controller input device.
 */
class dewiDeviceWinRTController : public dewiDevice{
public:
	/** Type holding strong reference. */
	typedef deTObjectReference<dewiDeviceWinRTController> Ref;



private:
	const wrgi::RawGameController pController;
	const wrgi::Gamepad pGamepad;
	
	std::vector<double> pReadingAxis;
	std::vector<wrgi::GameControllerSwitchPosition> pReadingSwitch;
	std::unique_ptr<bool[]> pReadingButton;
	int pReadingButtonSize;
	DWORD pReadingTime;

	

public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create device. */
	dewiDeviceWinRTController( deWindowsInput &module, wrgi::RawGameController const &controller );
	
protected:
	/** Clean up device. */
	virtual ~dewiDeviceWinRTController();
	/*@}*/
	
	
	
public:
	/** \name Module Management */
	/*@{*/
	/** NonRoamableId. */
	inline const wrgi::RawGameController &GetController() const{ return pController; }

	/** Update device state. */
	virtual void Update();

	/** Axis reading. */
	double GetReadingAxis( int index ) const;

	/** Switch position reading. */
	wrgi::GameControllerSwitchPosition GetReadingSwitch( int index ) const;

	/** Button reading. */
	bool GetReadingButton( int index ) const;

	/** Reading time. */
	inline DWORD GetReadingTime() const{ return pReadingTime; }
	/*@}*/
};

#endif
