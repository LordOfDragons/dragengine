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

#ifndef _IGDESTEPABLETASK_H_
#define _IGDESTEPABLETASK_H_

#include <dragengine/common/string/decString.h>



/**
 * \brief Stepable task.
 *
 * Base class for stepable tasks. Such tasks split lengthy operations into
 * manageable work units so called steps. The length of one step is not
 * defined but should be reasonably small to tell the user visually that
 * the task is still working. Each task has an info text and progress
 * value to communicate to the user the current state. After each step the
 * GUI updates with the stored information. Two changed flags help to
 * avoid updating if nothing changed. They are set by this base class
 * therefore subclasses do not have to worry about those flags. Just set
 * the status whenever it changes and leave the hard work for this class.
 * The return value of the Step call indicates if further steps are
 * required or if the task finished. Progress is measured from 0 to 1
 * usually displayed as 100 percent.
 */
class DE_DLL_EXPORT igdeStepableTask{
private:
	decString pMessage;
	float pProgress;
	bool pDirtyMessage;
	bool pDirtyProgress;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create task. */
	igdeStepableTask();
	
	/** \brief Clean up task. */
	virtual ~igdeStepableTask();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Message. */
	inline const decString &GetMessage() const{ return pMessage; }
	
	/** \brief Set message. */
	void SetMessage( const char *message );
	
	/** \brief Progress. */
	inline float GetProgress() const{ return pProgress; }
	
	/** \brief Set progress. */
	void SetProgress( float progress );
	
	/** \brief Message is dirty. */
	inline bool GetDirtyMessage() const{ return pDirtyMessage; }
	
	/** \brief Set message dirty flag. */
	void SetDirtyMessage( bool dirty );
	
	/** \brief Progress is dirty. */
	inline bool GetDirtyProgress() const{ return pDirtyProgress; }
	
	/** \brief Set if progress is dirty. */
	void SetDirtyProgress( bool dirty );
	
	
	
	/**
	 * \brief Run next step of task.
	 * 
	 * The step should consume a small amount of time to allow the GUI to stay responsive.
	 * In case of an error an exception has to be thrown which aborts the task.
	 * 
	 * \retval true Task requires more steps.
	 * \retval false Task finished after this step.
	 */
	virtual bool Step() = 0;
	/*@}*/
};

#endif
