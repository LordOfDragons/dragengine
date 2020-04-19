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

#ifndef _DEGLLOGGERHISTORY_H_
#define _DEGLLOGGERHISTORY_H_

#include <dragengine/logger/deLogger.h>
#include <dragengine/common/string/decStringSet.h>
#include <dragengine/threading/deMutex.h>
#include <dragengine/common/collection/decObjectList.h>

class deglLoggerHistoryEntry;
class deglLoggerHistoryListener;



/**
 * @brief Logger History.
 * Logger history.
 */
class deglLoggerHistory : public deLogger{
private:
	int pHistorySize;
	deglLoggerHistoryEntry *pEntries;
	int pEntryPointer;
	int pEntryCount;
	
	decObjectList pListeners;
	
	bool pLogInfo;
	bool pLogWarn;
	bool pLogError;
	decStringSet pLogSources;
	
	deMutex pMutex;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new logger history. */
	deglLoggerHistory();
	/** Cleans up the logger history. */
	virtual ~deglLoggerHistory();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** \brief Retrieves the mutex. Lock before working with the history entries in any way. */
	inline deMutex &GetMutex(){ return pMutex; }
	
	/** \brief Retrieves the size of the history. */
	int GetHistorySize() const{ return pHistorySize; }
	/**
	 * \brief Sets the size of the history. This clears the history.
	 * \note This call does an implicite lock/unlock on the mutex.
	 */
	void SetHistorySize( int size );
	/** Retrieves the number of history entries. */
	inline int GetEntryCount() const{ return pEntryCount; }
	/** Retrieves the entry at the given position. */
	deglLoggerHistoryEntry &GetEntryAt( int index );
	const deglLoggerHistoryEntry &GetEntryAt( int index ) const;
	/**
	 * \brief Adds a new entry to the history.
	 * \details The entry is added at the end of the list. If all entry slots are used already the
	 *          oldest entry is overwritten and the pointer advanced by one.
	 */
	deglLoggerHistoryEntry &AddEntry();
	/**
	 * \brief Clears the history.
	 * \note This call does an implicite lock/unlock on the mutex.
	 */
	void Clear();
	
	/** Determines if a message can be added. */
	bool CanAddMessage( int type, const char *source );
	
	/** \brief Adds a listener. */
	void AddListener( deglLoggerHistoryListener *listener );
	/** \brief Removes a listener. */
	void RemoveListener( deglLoggerHistoryListener *listener );
	/** \brief Notifies all listeners that a message has been added. */
	void NotifyMessageAdded( deglLoggerHistoryEntry &entry );
	
	/** Log an information message. */
	virtual void LogInfo( const char *source, const char *message );
	/** Log a warning message. */
	virtual void LogWarn( const char *source, const char *message );
	/** Log an error message. */
	virtual void LogError( const char *source, const char *message );
	/*@}*/
};

#endif
