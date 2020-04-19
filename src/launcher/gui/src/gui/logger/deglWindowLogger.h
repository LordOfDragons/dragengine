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

#ifndef _DEGLWINDOWLOGGER_H_
#define _DEGLWINDOWLOGGER_H_

#include "../foxtoolkit.h"

class deglLoggerHistory;
class deglLoggerHistoryEntry;
class deglWindowLoggerListener;
class deglWindowLoggerTable;



/**
 * @brief Logger Window.
 * Display the logging history.
 */
class deglWindowLogger : public FXTopWindow{
	FXDECLARE( deglWindowLogger )
	
protected:
	deglWindowLogger();
	
public:
	enum eFoxIDs{
		ID_SELF = FXTopWindow::ID_LAST,
		ID_TABLE_LOGS,
		ID_LAST
	};
	
private:
	deglWindowLoggerListener *pListener;
	deglLoggerHistory *pLogger;
	FXFont *pFontNormal;
	FXFont *pFontBold;
	
	deglWindowLoggerTable *pTableLogs;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new logger window. */
	deglWindowLogger( FXApp *app );
	/** Cleans up the logger window. */
	virtual ~deglWindowLogger();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Create window. */
	virtual void create();
	
	/** Retrieves the logger to monitor or NULL if none. */
	inline deglLoggerHistory *GetLogger() const{ return pLogger; }
	/** Sets the logger to monitor or NULL if none. */
	void SetLogger( deglLoggerHistory *logger );
	
	/** Updates the logs table. */
	void UpdateLogs();
	/** Add log to table. */
	void AddLogToTable( const deglLoggerHistoryEntry &entry );
	/** Clear logs table. */
	void ClearLogsTable();
	/*@}*/
	
	/** @name Events */
	/*@{*/
	long onMap( FXObject *sender, FXSelector selector, void *data );
	long onClose( FXObject *sender, FXSelector selector, void *data );
	/*@}*/
};

// end of include only once
#endif
