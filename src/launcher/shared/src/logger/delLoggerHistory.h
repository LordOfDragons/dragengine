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

#ifndef _DELLOGGERHISTORY_H_
#define _DELLOGGERHISTORY_H_

#include <dragengine/logger/deLogger.h>
#include <dragengine/common/string/decStringSet.h>
#include <dragengine/threading/deMutex.h>
#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/collection/decObjectList.h>

class delLoggerHistoryEntry;
class delLoggerHistoryListener;


/**
 * \brief Logger history.
 */
class DE_DLL_EXPORT delLoggerHistory : public deLogger{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<delLoggerHistory> Ref;
	
	
	
private:
	int pHistorySize;
	delLoggerHistoryEntry *pEntries;
	int pEntryCount;
	int pEntryOffset;
	
	decObjectOrderedSet pListeners;
	
	bool pLogInfo;
	bool pLogWarn;
	bool pLogError;
	decStringSet pLogSources;
	
	deMutex pMutex;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create logger history. */
	delLoggerHistory( int size = 250 );
	
protected:
	/** \brief Clean up logger history. */
	virtual ~delLoggerHistory();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Mutex to lock before accesing history entries. */
	inline deMutex &GetMutex(){ return pMutex; }
	
	/** \brief Size of history. */
	int GetHistorySize() const{ return pHistorySize; }
	
	/**
	 * \brief Set size of history.
	 * 
	 * Clears history.
	 * 
	 * \note This call does an implicit mutex lock.
	 */
	void SetHistorySize( int size );
	
	/** \brief Count of history entries. */
	inline int GetEntryCount() const{ return pEntryCount; }
	
	/** \brief Entry at index. */
	delLoggerHistoryEntry &GetEntryAt( int index );
	const delLoggerHistoryEntry &GetEntryAt( int index ) const;
	
	/**
	 * \brief Add entry to history.
	 * 
	 * Entry is added at end of list. If all entry slots are used the oldest entry is dropped.
	 */
	delLoggerHistoryEntry &AddEntry();
	
	/**
	 * \brief Clear history.
	 * \note Mutex is internally locked.
	 */
	void Clear();
	
	/** \brief Message can be added. */
	bool CanAddMessage( int type, const char *source );
	
	/**
	 * \brief Add listener.
	 * \note Mutex is internally locked.
	 */
	void AddListener( delLoggerHistoryListener *listener );
	
	/**
	 * \brief Remove listener.
	 * \note Mutex is internally locked.
	 */
	void RemoveListener( delLoggerHistoryListener *listener );
	
	/** \brief Notify listeners message has been added. */
	void NotifyMessageAdded( const delLoggerHistoryEntry &entry );
	
	
	
	/** \brief Log an information message. */
	virtual void LogInfo( const char *source, const char *message );
	
	/** \brief Log a warning message. */
	virtual void LogWarn( const char *source, const char *message );
	
	/** \brief Log an error message. */
	virtual void LogError( const char *source, const char *message );
	/*@}*/
};

#endif
