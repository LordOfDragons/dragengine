/*
 * MIT License
 *
 * Copyright (C) 2025, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _DEGLB_WINDOWLOGGER_H_
#define _DEGLB_WINDOWLOGGER_H_

#include <Window.h>
#include <ListView.h>
#include <ScrollView.h>
#include <ListItem.h>

#include <delauncher/logger/delLoggerHistory.h>
#include <delauncher/logger/delLoggerHistoryListener.h>

class deglbWindowLoggerListener;
class delLoggerHistoryEntry;


/**
 * \brief Logger Window. Display the logging history.
 */
class deglbWindowLogger : public BWindow{
private:
	delLoggerHistoryListener::Ref pListener;
	delLoggerHistory::Ref pLogger;
	
	BListView *pListView;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create logger window. */
	deglbWindowLogger();
	
	/** \brief Clean up logger window. */
	~deglbWindowLogger() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Logger to monitor or nullptr if none. */
	inline const delLoggerHistory::Ref &GetLogger() const{ return pLogger; }
	
	/** \brief Set logger to monitor or nullptr if none. */
	void SetLogger(delLoggerHistory *logger);
	
	/** \brief Add log entry (call with window locked). */
	void AddLogEntry(const delLoggerHistoryEntry &entry);
	
	/** \brief Clear log (call with window locked). */
	void ClearLog();
	
	/** \brief Update all logs from history (call with window locked). */
	void UpdateLogs();
	/*@}*/
	
	
	
	/** \name BWindow */
	/*@{*/
	bool QuitRequested() override;
	/*@}*/
};

#endif
