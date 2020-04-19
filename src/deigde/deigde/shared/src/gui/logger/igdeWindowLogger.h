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

#ifndef _IGDEWINDOWLOGGER_H_
#define _IGDEWINDOWLOGGER_H_

#include "../igdeWindow.h"
#include "../igdeTextAreaReference.h"

class igdeLoggerHistory;
class igdeLoggerHistoryEntry;
class igdeWindowLoggerListener;



/**
 * \brief Logger window.
 */
class igdeWindowLogger : public igdeWindow{
public:
	/** \brief Warning style name. */
	static const char *styleWarning;
	
	/** \brief Error style name. */
	static const char *styleError;
	
	
	
private:
	igdeWindowLoggerListener *pListener;
	igdeLoggerHistory *pLogger;
	igdeTextAreaReference pEditLogs;
	int pPendingAddedLogs;
	bool pPendingClearLogs;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create logger window. */
	igdeWindowLogger( igdeEnvironment &environment );
	
	/** \brief Clean up logger window. */
	virtual ~igdeWindowLogger();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Logger or NULL. */
	inline igdeLoggerHistory *GetLogger() const{ return pLogger; }
	
	/** \brief Set logger or NULL. */
	void SetLogger( igdeLoggerHistory *logger );
	
	
	
	/**
	 * \brief Check for log updates.
	 * 
	 * Call this during owner window OnFrameUpdate().
	 */
	virtual void OnFrameUpdate();
	
	
	
	/**
	 * \brief Request to close window due to clicking on close button.
	 * 
	 * Returns \em true to close window or \em false to keep it open. Default implementation
	 * returns \em true.
	 */
	virtual bool CloseWindow();
	
	
	
	/**
	 * \brief Log has been added.
	 * \warning For internal use by igdeWindowLoggerListener only! Do not call on your own!
	 */
	void OnLogAdded();
	
	/**
	 * \brief Logs have been cleared.
	 * \warning For internal use by igdeWindowLoggerListener only! Do not call on your own!
	 */
	void OnLogsCleared();
	
	
	
protected:
	/** \brief Visible changed. */
	virtual void OnVisibleChanged();
	/*@}*/
	
	
	
private:
	void pRemoveOldLines();
	void pAddLog( const igdeLoggerHistoryEntry &entry );
	void pClearLogs();
};

#endif
