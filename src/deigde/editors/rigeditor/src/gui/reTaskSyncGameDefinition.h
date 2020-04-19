/* 
 * Drag[en]gine IGDE Rig Editor
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

#ifndef _RAETASKSYNCGAMEDEFINITION_H_
#define _RETASKSYNCGAMEDEFINITION_H_

#include <deigde/gui/igdeStepableTask.h>

class reWindowMain;



/**
 * \brief Synchronize game definition task.
 */
class reTaskSyncGameDefinition : public igdeStepableTask{
private:
	reWindowMain &pWindowMain;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create task. */
	reTaskSyncGameDefinition( reWindowMain &windowMain );
	
	/** \brief Clean up task. */
	virtual ~reTaskSyncGameDefinition();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Run next step of task.
	 * 
	 * The step should consume a small amount of time to allow the GUI to stay responsive.
	 * In case of an error an exception has to be thrown which aborts the task.
	 * 
	 * \retval true Task requires more steps.
	 * \retval false Task finished after this step.
	 */
	virtual bool Step();
	/*@}*/
};

#endif
