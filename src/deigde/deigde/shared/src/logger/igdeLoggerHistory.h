/* 
 * Drag[en]gine IGDE
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

#ifndef _IGDELOGGERHISTORY_H_
#define _IGDELOGGERHISTORY_H_

#include <dragengine/logger/deLogger.h>
#include <dragengine/common/string/decStringSet.h>
#include <dragengine/common/collection/decObjectSet.h>
#include <dragengine/threading/deMutex.h>

class igdeLoggerHistoryEntry;
class igdeLoggerHistoryListener;



/**
 * \brief Logger History.
 * 
 * This class is thread-safe in general. Listeners have to be careful about accessing
 * the data. Methods doing an implicite lock are marked. Furthermore all listener
 * notification calls are done while inside a locked mutex. Do not try to lock the
 * mutex while your listener is invoked. To access the history outside a listener
 * notification lock the mutex yourself.
 */
class DE_DLL_EXPORT igdeLoggerHistory : public deLogger{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeLoggerHistory> Ref;
	
	
	
private:
	int pHistorySize;
	igdeLoggerHistoryEntry *pEntries;
	int pEntryPointer;
	int pEntryCount;
	
	decObjectSet pListeners;
	
	bool pLogInfo;
	bool pLogWarn;
	bool pLogError;
	decStringSet pLogSources;
	
	deMutex pMutex;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create logger history. */
	igdeLoggerHistory();
	
	/** \brief Clean up logger history. */
	virtual ~igdeLoggerHistory();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Mutex. Lock before working with the history entries in any way. */
	inline deMutex &GetMutex(){ return pMutex; }
	
	/** \brief Size of the history. */
	int GetHistorySize() const{ return pHistorySize; }
	
	/**
	 * \brief Set size of the history. This clears the history.
	 * \note This call does an implicite lock/unlock on the mutex.
	 */
	void SetHistorySize( int size );
	
	/** \brief Number of history entries. */
	inline int GetEntryCount() const{ return pEntryCount; }
	
	/** \brief Entry at the given position. */
	igdeLoggerHistoryEntry &GetEntryAt( int index );
	const igdeLoggerHistoryEntry &GetEntryAt( int index ) const;
	
	/**
	 * \brief Adds a new entry to the history.
	 * 
	 * The entry is added at the end of the list. If all entry slots are used already the
	 * oldest entry is overwritten and the pointer advanced by one.
	 */
	igdeLoggerHistoryEntry &AddEntry();
	
	/**
	 * \brief Clears the history.
	 * \note This call does an implicite lock/unlock on the mutex.
	 */
	void Clear();
	
	/** \brief Determines if a message can be added. */
	bool CanAddMessage( int type, const char *source );
	
	/** \brief Adds a listener. */
	void AddListener( igdeLoggerHistoryListener *listener );
	
	/** \brief Removes a listener. */
	void RemoveListener( igdeLoggerHistoryListener *listener );
	
	/** \brief Notifies all listeners that a message has been added. */
	void NotifyMessageAdded( igdeLoggerHistoryEntry &entry );
	
	/** \brief Log an information message. */
	virtual void LogInfo( const char *source, const char *message );
	
	/** \brief Log a warning message. */
	virtual void LogWarn( const char *source, const char *message );
	
	/** \brief Log an error message. */
	virtual void LogError( const char *source, const char *message );
	/*@}*/
};

#endif
