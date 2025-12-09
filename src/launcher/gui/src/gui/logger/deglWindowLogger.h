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

#ifndef _DEGLWINDOWLOGGER_H_
#define _DEGLWINDOWLOGGER_H_

#include "../foxtoolkit.h"

#include <delauncher/logger/delLoggerHistory.h>
#include <delauncher/logger/delLoggerHistoryListener.h>

class deglWindowLoggerListener;
class deglWindowLoggerTable;


/**
 * Logger Window. Display the logging history.
 */
class deglWindowLogger : public FXTopWindow{
	FXDECLARE(deglWindowLogger)
	
protected:
	deglWindowLogger();
	
public:
	enum eFoxIDs{
		ID_SELF = FXTopWindow::ID_LAST,
		ID_TABLE_LOGS,
		ID_LAST
	};
	
	
	
private:
	delLoggerHistoryListener::Ref pListener;
	delLoggerHistory::Ref pLogger;
	FXFont *pFontNormal;
	FXFont *pFontBold;
	
	deglWindowLoggerTable *pTableLogs;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create logger window. */
	deglWindowLogger(FXApp *app);
	
	/** Clean up logger window. */
	virtual ~deglWindowLogger();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Create window. */
	virtual void create();
	
	/** Logger to monitor or nullptr if none. */
	inline const delLoggerHistory::Ref &GetLogger() const{ return pLogger; }
	
	/** Set logger to monitor or nullptr if none. */
	void SetLogger(delLoggerHistory *logger);
	
	/** Update logs table. */
	void UpdateLogs();
	
	/** Add log to table. */
	void AddLogToTable(const delLoggerHistoryEntry &entry);
	
	/** Clear logs table. */
	void ClearLogsTable();
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	long onMap(FXObject *sender, FXSelector selector, void *data);
	long onClose(FXObject *sender, FXSelector selector, void *data);
	/*@}*/
};

#endif
