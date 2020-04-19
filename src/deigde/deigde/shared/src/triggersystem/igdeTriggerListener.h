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

#ifndef _IGDETRIGGERLISTENER_H_
#define _IGDETRIGGERLISTENER_H_

#include <dragengine/deObject.h>

class igdeTriggerTarget;



/**
 * \brief Trigger Listener.
 */
class igdeTriggerListener : public deObject{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create trigger listener. */
	igdeTriggerListener();
	
	/** \brief Clean up trigger listener. */
	virtual ~igdeTriggerListener();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Trigger target changed. */
	virtual void TriggerTargetChanged( igdeTriggerTarget *target );
	/*@}*/
};

#endif
