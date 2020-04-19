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

#ifndef _DEGLLOGGERHISTORYLISTENER_H_
#define _DEGLLOGGERHISTORYLISTENER_H_

#include <dragengine/deObject.h>

class deglLoggerHistory;
class deglLoggerHistoryEntry;



/**
 * @brief Logger History Listener.
 * Listener for a logger history object. Supports notification about
 * entries being added to the history.
 */
class deglLoggerHistoryListener : public deObject{
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new listener. */
	deglLoggerHistoryListener();
	/** Cleans up the listener. */
	virtual ~deglLoggerHistoryListener();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** A message has been added to the history. */
	virtual void MessageAdded( deglLoggerHistory *history, deglLoggerHistoryEntry &entry );
	/** History has been cleared. */
	virtual void HistoryCleared( deglLoggerHistory *history );
	/*@}*/
};

#endif
