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

#ifndef _IGDETRIGGERTARGET_H_
#define _IGDETRIGGERTARGET_H_

#include <dragengine/common/collection/decObjectSet.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/deObject.h>

class igdeTriggerListener;



/**
 * \brief Trigger Target.
 */
class DE_DLL_EXPORT igdeTriggerTarget : public deObject{
private:
	decString pName;
	bool pFired;
	bool pHasFired;
	decObjectSet pListeners;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create trigger target. */
	igdeTriggerTarget( const char *name );
	
	
	
protected:
	/** \brief Clean up trigger target. */
	virtual ~igdeTriggerTarget();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Determines if the target is currently fired or not. */
	inline bool GetFired() const{ return pFired; }
	
	/** \brief Sets if the target is currently fired or not. */
	void SetFired( bool fired );
	
	/** \brief Determines if the target has ever fired. */
	inline bool GetHasFired() const{ return pHasFired; }
	
	/** \brief Sets if the target has ever fired. */
	void SetHasFired( bool hasFired );
	
	/** \brief Fire the trigger action. */
	void Fire();
	
	/** \brief Reset the trigger. */
	void Reset();
	
	/** \brief Fully resets the trigger. */
	void FullReset();
	
	/** \brief Adds a listener. */
	void AddListener( igdeTriggerListener *listener );
	
	/** \brief Removes a listener if existing. */
	void RemoveListener( igdeTriggerListener *listener );
	
	/** \brief Notify all listeners. */
	void NotifyListeners();
	/*@}*/
};

#endif
