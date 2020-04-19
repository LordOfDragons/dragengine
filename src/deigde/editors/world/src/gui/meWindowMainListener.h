/* 
 * Drag[en]gine IGDE World Editor
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

#ifndef _MEWINDOWMAINLISTENER_H_
#define _MEWINDOWMAINLISTENER_H_

#include "../world/meWorldNotifier.h"

class meWindowMain;



/**
 * \brief Window Main Listener.
 */
class meWindowMainListener : public meWorldNotifier{
private:
	meWindowMain &pWindowMain;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	meWindowMainListener( meWindowMain &windowMain );
	
	/** \brief Clean up listener. */
	virtual ~meWindowMainListener();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Element or work mode changed. */
	virtual void ModeChanged( meWorld *world );
	
	/** \brief Changed or saved state changed. */
	virtual void StateChanged( meWorld *world );
	
	/** \brief Height terrain state changed. */
	virtual void HTStateChanged( meWorld *world );
	
	/** \brief Undos changed. */
	virtual void UndoChanged( meWorld *world );
	
	/** \brief Lighting parameters changed. */
	virtual void LightingChanged( meWorld *world );
	
	/** \brief Editing parameters changed. */
	virtual void EditingChanged( meWorld *world );
	
	/** \brief Object selection changed. */
	virtual void ObjectSelectionChanged( meWorld *world );
	
	/** \brief Object changed. */
	virtual void ObjectChanged( meWorld *world, meObject *object );
	
	/** \brief Decal selection changed. */
	virtual void DecalSelectionChanged( meWorld *world );
	
	/** \brief Decal changed. */
	virtual void DecalChanged( meWorld *world, meDecal *decal );
	/*@}*/
};

#endif
