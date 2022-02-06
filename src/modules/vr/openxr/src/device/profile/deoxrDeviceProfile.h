/* 
 * Drag[en]gine OpenXR
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEOXRDEVICEPROFILE_H_
#define _DEOXRDEVICEPROFILE_H_

#include "../../deoxrBasics.h"
#include "../../deoxrInstance.h"
#include "../../deoxrPath.h"
#include "../../deVROpenXR.h"

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>

class deoxrInstance;


/**
 * Oxr device profile.
 */
class deoxrDeviceProfile : public deObject{
public:
	/** Reference. */
	typedef deTObjectReference<deoxrDeviceProfile> Ref;
	
	
	
private:
	deoxrInstance &pInstance;
	
	const deoxrPath pPath;
	const decString pName;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create device profile. */
	deoxrDeviceProfile( deoxrInstance &instance, const deoxrPath &path, const char *name );
	
protected:
	/** Clean up device profile. */
	virtual ~deoxrDeviceProfile();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Instance. */
	inline deoxrInstance &GetInstance() const{ return pInstance; }
	
	/** Path. */
	inline const deoxrPath &GetPath() const{ return pPath; }
	
	/** Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** Suggest bindings. */
	virtual void SuggestBindings() = 0;
	/*@}*/
	
	
	
protected:
	void pAdd( deoxrInstance::sSuggestBinding *&bindings,
		deVROpenXR::eInputActions inputAction, const char *path ) const;
};

#endif

