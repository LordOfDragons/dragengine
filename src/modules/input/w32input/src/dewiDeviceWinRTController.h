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

#ifndef _DEWIDEVICEWINRTCONTROLLER_H_
#define _DEWIDEVICEWINRTCONTROLLER_H_

#include "dewiDevice.h"

#include <memory>
#include <vector>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Gaming.Input.h>

#include <dragengine/input/deInputEvent.h>
#include <winrt/Windows.Devices.Power.h>

namespace wrf = winrt::Windows::Foundation;
namespace wrgi = winrt::Windows::Gaming::Input;
namespace wrdp = winrt::Windows::Devices::Power;



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

	wrdp::BatteryReport pBatteryReport;
	
	

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

	/** Battery report. Can be nullptr. */
	inline const wrdp::BatteryReport &GetBatteryReport() const{ return pBatteryReport; }
	/*@}*/
};

#endif
