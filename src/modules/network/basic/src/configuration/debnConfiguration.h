/* 
 * Drag[en]gine Basic Network Module
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

#ifndef _DEBNCONFIGURATION_H_
#define _DEBNCONFIGURATION_H_

#include <dragengine/common/string/decString.h>


/**
 * Configuration.
 */
class debnConfiguration{
public:
	enum eLogLevels{
		ellError,
		ellWarning,
		ellInfo,
		ellDebug
	};
	
	
	
private:
	eLogLevels pLogLevel;
	
	float pConnectResendInterval;
	float pConnectTimeout;
	float pReliableResendInterval;
	float pReliableTimeout;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create configuration. */
	debnConfiguration();
	
	/** Clean up configuration. */
	~debnConfiguration();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Log level. */
	inline eLogLevels GetLogLevel() const{ return pLogLevel; }
	void SetLogLevel( eLogLevels level );
	
	/** Connect resent interval in seconds. */
	inline float GetConnectResendInterval() const{ return pConnectResendInterval; }
	void SetConnectResendInterval( float interval );
	
	/** Connect timeout in seconds. */
	inline float GetConnectTimeout() const{ return pConnectTimeout; }
	void SetConnectTimeout( float timeout );
	
	/** Reliable message resend interval in seconds. */
	inline float GetReliableResendInterval() const{ return pReliableResendInterval; }
	void SetReliableResendInterval( float interval );
	
	/** Reliable message timeout in seconds. */
	inline float GetReliableTimeout() const{ return pReliableTimeout; }
	void SetReliableTimeout( float timeout );
	/*@}*/
};

#endif
