/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
