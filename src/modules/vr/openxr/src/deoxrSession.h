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

#ifndef _DEOXRSESSION_H_
#define _DEOXRSESSION_H_

#include "deoxrBasics.h"

#include <dragengine/deObject.h>

class deoxrSystem;


/**
 * Oxr session.
 */
class deoxrSession : public deObject{
public:
	/** Reference. */
	typedef deTObjectReference<deoxrSession> Ref;
	
	
	
private:
	deoxrSystem &pSystem;
	
	XrSession pSession;
	bool pRunning;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create session. */
	deoxrSession( deoxrSystem &system );
	
protected:
	/** Clean up space. */
	virtual ~deoxrSession();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** System. */
	inline deoxrSystem &GetSystem() const{ return pSystem; }
	
	/** Session. */
	inline XrSession GetSession() const{ return pSession; }
	
	/** Session is running. */
	inline bool GetRunning() const{ return pRunning; }
	
	/** Begin. */
	void Begin();
	
	/** End. */
	void End();
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif

