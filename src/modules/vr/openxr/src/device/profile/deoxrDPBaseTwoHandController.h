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

#ifndef _DEOXRDPDPBASETWOHANDCONTROLLER_H_
#define _DEOXRDPDPBASETWOHANDCONTROLLER_H_

#include "deoxrDeviceProfile.h"
#include "../deoxrDevice.h"


/**
 * Base class for two hand controller profile.
 */
class deoxrDPBaseTwoHandController : public deoxrDeviceProfile{
protected:
	deoxrDevice::Ref pDeviceLeft;
	deoxrDevice::Ref pDeviceRight;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create device profile. */
	deoxrDPBaseTwoHandController( deoxrInstance &instance, const deoxrPath &path, const char *name );
	
protected:
	/** Clean up device profile. */
	virtual ~deoxrDPBaseTwoHandController();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Check attached. */
	virtual void CheckAttached();
	
	/** Suggest bindings. */
	virtual void SuggestBindings();
	
	/** Clear actions. */
	virtual void ClearActions();
	
	/** Remove device if matching type. */
	virtual void RemoveDevice( deInputDevice::eDeviceTypes type );
	/*@}*/
	
	
	
protected:
	virtual bool pProfileEnabled() const;
	virtual void pSuggestBindings() = 0;
	
	virtual void pAddDevice( bool left ) = 0;
	
	void pRemoveDevice( bool left );
};

#endif

