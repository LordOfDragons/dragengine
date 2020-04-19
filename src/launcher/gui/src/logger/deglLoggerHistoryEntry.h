/* 
 * Drag[en]gine GUI Launcher
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

#ifndef _DEGLLOGGERHISTORYENTRY_H_
#define _DEGLLOGGERHISTORYENTRY_H_

#include <dragengine/common/string/decString.h>



/**
 * @brief Logger History Entry.
 * Message entry in a logger history.
 */
class deglLoggerHistoryEntry{
public:
	/** Message types. */
	enum eMessageTypes{
		/** Information. */
		emtInfo,
		/** Warning. */
		emtWarn,
		/** Error. */
		emtError
	};
	
private:
	// time stamp...
	int pType;
	decString pSource;
	decString pMessage;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new logger history entry. */
	deglLoggerHistoryEntry();
	/** Cleans up the logger history entry. */
	~deglLoggerHistoryEntry();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the type. */
	inline int GetType() const{ return pType; }
	/** Sets the type. */
	void SetType( int type );
	/** Retrieves the source. */
	inline const decString &GetSource() const{ return pSource; }
	inline decString &GetSource(){ return pSource; }
	/** Retrieves the message. */
	inline const decString &GetMessage() const{ return pMessage; }
	inline decString &GetMessage(){ return pMessage; }
	/** Clears the entry. */
	void Clear();
	/** Cleans up the message entry by removing extra blanks and newlines at the end. */
	void CleanUpMessage();
	/*@}*/
};

// end of include only once
#endif
