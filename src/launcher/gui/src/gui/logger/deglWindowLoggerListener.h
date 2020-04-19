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

#ifndef _DEGLWINDOWLOGGERLISTENER_H_
#define _DEGLWINDOWLOGGERLISTENER_H_

#include "../../logger/deglLoggerHistoryListener.h"

class deglWindowLogger;



/**
 * @brief Logger Window Listener.
 */
class deglWindowLoggerListener : public deglLoggerHistoryListener{
private:
	deglWindowLogger *pWindow;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new listener. */
	deglWindowLoggerListener( deglWindowLogger *window );
	/** Cleans up the listener. */
	virtual ~deglWindowLoggerListener();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** A message has been added to the history. */
	virtual void MessageAdded( deglLoggerHistory *history, deglLoggerHistoryEntry &entry );
	/** History has been cleared. */
	virtual void HistoryCleared( deglLoggerHistory *history );
	/*@}*/
};

// end of include only once
#endif
