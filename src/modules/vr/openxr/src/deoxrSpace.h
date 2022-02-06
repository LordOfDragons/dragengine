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

#ifndef _DEOXRSPACE_H_
#define _DEOXRSPACE_H_

#include "deoxrBasics.h"

#include <dragengine/deObject.h>

class deoxrSession;


/**
 * Oxr space.
 */
class deoxrSpace : public deObject{
public:
	/** Reference. */
	typedef deTObjectReference<deoxrSpace> Ref;
	
	
	
private:
	deoxrSession &pSession;
	
	XrSpace pSpace;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create space. */
	deoxrSpace( deoxrSession &session );
	
protected:
	/** Clean up space. */
	virtual ~deoxrSpace();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Session. */
	inline deoxrSession &GetSession() const{ return pSession; }
	
	/** Space. */
	inline XrSpace GetSpace() const{ return pSpace; }
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif

