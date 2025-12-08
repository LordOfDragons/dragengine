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

#ifndef _IGDEWINDOWLOGGER_H_
#define _IGDEWINDOWLOGGER_H_

#include "../igdeWindow.h"
#include "../igdeTextArea.h"

class igdeLoggerHistory;
class igdeLoggerHistoryEntry;
class igdeWindowLoggerListener;



/**
 * \brief Logger window.
 */
class DE_DLL_EXPORT igdeWindowLogger : public igdeWindow{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeWindowLogger> Ref;
	
	
	/** \brief Warning style name. */
	static const char *styleWarning;
	
	/** \brief Error style name. */
	static const char *styleError;
	
	
	
private:
	igdeWindowLoggerListener::Ref pListener;
	igdeLoggerHistory::Ref pLogger;
	igdeTextArea::Ref pEditLogs;
	int pPendingAddedLogs;
	bool pPendingClearLogs;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create logger window. */
	igdeWindowLogger(igdeEnvironment &environment);
	
	/** \brief Clean up logger window. */
	virtual ~igdeWindowLogger();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Logger or NULL. */
	inline igdeLoggerHistory *GetLogger() const{ return pLogger; }
	
	/** \brief Set logger or NULL. */
	void SetLogger(igdeLoggerHistory *logger);
	
	
	
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
	void pAddLog(const igdeLoggerHistoryEntry &entry);
	void pClearLogs();
};

#endif
