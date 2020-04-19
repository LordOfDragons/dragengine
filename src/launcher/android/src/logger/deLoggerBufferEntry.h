/* 
 * Drag[en]gine Android Launcher
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

#ifndef _DELOGGERBUFFERENTRY_H_
#define _DELOGGERBUFFERENTRY_H_

#include "../common/string/decString.h"



/**
 * \brief Logger buffer entry.
 */
class deLoggerBufferEntry{
public:
	/** \brief Message types. */
	enum eMessageTypes{
		/** \brief Information. */
		emtInfo,
		
		/** \brief Warning. */
		emtWarn,
		
		/** \brief Error. */
		emtError
	};
	
private:
	eMessageTypes pType;
	decString pSource;
	decString pMessage;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create logger history entry. */
	deLoggerBufferEntry();
	
	/** \brief Clean up logger history entry. */
	~deLoggerBufferEntry();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Type. */
	inline eMessageTypes GetType() const{ return pType; }
	
	/** \brief Set type. */
	void SetType( eMessageTypes type );
	
	/** \brief Source. */
	inline const decString &GetSource() const{ return pSource; }
	
	/** \brief Set source. */
	void SetSource( const char *source );
	
	/** \brief Message. */
	inline const decString &GetMessage() const{ return pMessage; }
	
	/** \brief Set message. */
	void SetMessage( const char *message );
	/*@}*/
};

#endif
