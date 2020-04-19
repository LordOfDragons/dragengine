/* 
 * Drag[en]gine Game Engine
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

#ifndef _DEINPUTEVENTQUEUE_H_
#define _DEINPUTEVENTQUEUE_H_

#include <sys/time.h>

class deInputEvent;


/**
 * \brief Input events queue with an initial size.
 * 
 * The queue does not grow since processing a large amount of events due to a short
 * time problem is more of a problem than discarding some events in such a situation.
 */
class deInputEventQueue{
private:
	deInputEvent *pEvents;
	int pEventCount;
	int pEventSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create a new event queue.
	 * \throws deeInvalidParam \em queueSize is less than 1.
	 */
	deInputEventQueue( int queueSize );
	
	/** \brief Clean up the event queue. */
	~deInputEventQueue();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Determine if the queue contains any events. */
	inline bool GetHasEvents() const{ return pEventCount > 0; }
	
	/** \brief Number of events in the queue. */
	inline int GetEventCount() const{ return pEventCount; }
	
	/**
	 * \brief Event by index.
	 * \throws deeInvalidParam \em index is less than 0 or greater than GetEventCount()-1.
	 */
	const deInputEvent &GetEventAt( int index ) const;
	
	/**
	 * \brief Add a copy of an event to the end of the queue.
	 * 
	 * If the queue is full the event is discarded.
	 * 
	 * \returns true if the event has been added otherwise false.
	 */
	bool AddEvent( const deInputEvent &event );
	
	/** \brief Remove all events from the queue. */
	void RemoveAllEvents();
	/*@}*/
};

#endif
