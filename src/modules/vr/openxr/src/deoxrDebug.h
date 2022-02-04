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

#ifndef _DEOXRDEBUG_H_
#define _DEOXRDEBUG_H_

#include "deoxrBasics.h"

class deoxrInstance;


/**
 * OpenXR debug support.
 */
class deoxrDebug{
private:
	deoxrInstance &pInstance;
	
	bool pEnabled;
	
	XrDebugUtilsMessengerEXT pMessenger;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create debug. */
	deoxrDebug( deoxrInstance &instance );
	
	/** Clean up debug. */
	~deoxrDebug();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Instance. */
	inline deoxrInstance &GetInstance() const{ return pInstance; }
	
	/** Debug mode is enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	/** Enable or disable debug mode. */
	void SetEnabled( bool enabled );
	
	
	
private:
	void pRegisterReportCallback();
	void pUnregisterReportCallback();
	
	void DebugMessage( const char *function, const char *messageId, const char *message );
	
	static XrBool32 DebugMessageCallback( XrDebugUtilsMessageSeverityFlagsEXT messageSeverity,
		XrDebugUtilsMessageTypeFlagsEXT messageTypes,
		const XrDebugUtilsMessengerCallbackDataEXT *callbackData, void *userData );
};

#endif

