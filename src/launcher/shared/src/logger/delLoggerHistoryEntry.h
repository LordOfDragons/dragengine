/* 
 * Drag[en]gine Launcher Shared
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#ifndef _DELLOGGERHISTORYENTRY_H_
#define _DELLOGGERHISTORYENTRY_H_

#include <dragengine/common/string/decString.h>
#include <dragengine/common/utils/decDateTime.h>


/**
 * \brief Logger History Entry.
 */
class DE_DLL_EXPORT delLoggerHistoryEntry{
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
	decDateTime pTimeStamp;
	eMessageTypes pType;
	decString pSource;
	decString pMessage;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create logger history entry. */
	delLoggerHistoryEntry();
	
	/** \brief Create copy of logger history entry. */
	delLoggerHistoryEntry( const delLoggerHistoryEntry &entry );
	
	/** \brief Clean up logger history entry. */
	virtual ~delLoggerHistoryEntry();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Time stamp. */
	inline const decDateTime &GetTimeStamp() const{ return pTimeStamp; }
	
	/** \brief Set time stamp. */
	void SetTimeStamp( const decDateTime &timestamp );
	
	/** \brief Type. */
	inline eMessageTypes GetType() const{ return pType; }
	
	/** \brief Set type. */
	void SetType( eMessageTypes type );
	
	/** \brief Source. */
	inline const decString &GetSource() const{ return pSource; }
	
	/** \brief Set source. */
	void SetSource( const decString &source );
	
	/** \brief Message. */
	inline const decString &GetMessage() const{ return pMessage; }
	
	/** \brief Set message. */
	void SetMessage( const decString &message );
	
	/** \brief Clear entry. */
	void Clear();
	
	/** \brief Clean up message entry removing extra blanks and newlines at the end. */
	void CleanUpMessage();
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy entry. */
	delLoggerHistoryEntry &operator=( const delLoggerHistoryEntry &entry );
	/*@}*/
};

#endif
