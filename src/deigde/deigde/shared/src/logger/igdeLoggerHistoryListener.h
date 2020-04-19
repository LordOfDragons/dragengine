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
class igdeLoggerHistoryListener : public deObject{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	igdeLoggerHistoryListener();
	
	/** \brief Clean up listener. */
	virtual ~igdeLoggerHistoryListener();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief A message has been added to the history. */
	virtual void MessageAdded( igdeLoggerHistory *history, igdeLoggerHistoryEntry &entry );
	
	/** \brief History has been cleared. */
	virtual void HistoryCleared( igdeLoggerHistory *history );
	/*@}*/
};

#endif
