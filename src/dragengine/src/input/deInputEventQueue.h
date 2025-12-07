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

#ifndef _DEINPUTEVENTQUEUE_H_
#define _DEINPUTEVENTQUEUE_H_

#include "../dragengine_export.h"

class deInputEvent;


/**
 * \brief Input events queue with an initial size.
 * 
 * The queue does not grow since processing a large amount of events due to a short
 * time problem is more of a problem than discarding some events in such a situation.
 */
class DE_DLL_EXPORT deInputEventQueue{
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
	deInputEventQueue(int queueSize);
	
	/** \brief Clean up the event queue. */
	~deInputEventQueue();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Determine if the queue contains any events. */
	inline bool GetHasEvents() const{return pEventCount > 0;}
	
	/** \brief Number of events in the queue. */
	inline int GetEventCount() const{return pEventCount;}
	
	/**
	 * \brief Event by index.
	 * \throws deeInvalidParam \em index is less than 0 or greater than GetEventCount()-1.
	 */
	const deInputEvent &GetEventAt(int index) const;
	
	/**
	 * \brief Add a copy of an event to the end of the queue.
	 * 
	 * If the queue is full the event is discarded.
	 * 
	 * \returns true if the event has been added otherwise false.
	 */
	bool AddEvent(const deInputEvent &event);
	
	/** \brief Remove all events from the queue. */
	void RemoveAllEvents();
	/*@}*/
};

#endif
