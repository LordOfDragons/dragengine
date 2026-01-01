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

#ifndef _IGDELOGGERHISTORYLISTENER_H_
#define _IGDELOGGERHISTORYLISTENER_H_

#include <dragengine/deObject.h>

class igdeLoggerHistory;
class igdeLoggerHistoryEntry;



/**
 * \brief Logger History Listener.
 * 
 * Listener for a logger history object. Supports notification about
 * entries being added to the history.
 */
class DE_DLL_EXPORT igdeLoggerHistoryListener : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeLoggerHistoryListener> Ref;
	
	
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	igdeLoggerHistoryListener();
	
protected:
	/** \brief Clean up listener. */
	virtual ~igdeLoggerHistoryListener();
	
public:
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief A message has been added to the history. */
	virtual void MessageAdded(igdeLoggerHistory *history, igdeLoggerHistoryEntry &entry);
	
	/** \brief History has been cleared. */
	virtual void HistoryCleared(igdeLoggerHistory *history);
	/*@}*/
};

#endif
