/* 
 * Drag[en]gine Launcher Shared
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#ifndef _DELLOGGERHISTORYLISTENER_H_
#define _DELLOGGERHISTORYLISTENER_H_

#include <dragengine/deObject.h>

class delLoggerHistory;
class delLoggerHistoryEntry;


/**
 * \brief Logger history listener.
 */
class delLoggerHistoryListener : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<delLoggerHistoryListener> Ref;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	delLoggerHistoryListener();
	
protected:
	/** \brief Clean up listener. */
	virtual ~delLoggerHistoryListener();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/**
	 * \brief Message added.
	 * \note Histroy mutex is locked while this method is called.
	 */
	virtual void MessageAdded( const delLoggerHistory &history, const delLoggerHistoryEntry &entry );
	
	/**
	 * \brief History cleared.
	 * \note Histroy mutex is locked while this method is called.
	 */
	virtual void HistoryCleared( const delLoggerHistory &history );
	/*@}*/
};

#endif
