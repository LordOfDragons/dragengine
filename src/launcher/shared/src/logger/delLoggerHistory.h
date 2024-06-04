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
