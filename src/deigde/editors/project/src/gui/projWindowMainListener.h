/* 
 * Drag[en]gine IGDE Project Editor
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

#ifndef _PROJWINDOWMAINLISTENER_H_
#define _PROJWINDOWMAINLISTENER_H_

#include "../project/projProjectListener.h"

class projWindowMain;



/**
 * \brief Main window listener.
 */
class projWindowMainListener : public projProjectListener{
private:
	projWindowMain &pWindow;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	projWindowMainListener( projWindowMain &window );
	
	/** \brief Clean up listener. */
	virtual ~projWindowMainListener();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Changed or saved state changed. */
	virtual void StateChanged( projProject *project );
	
	/** \brief Undos changed. */
	virtual void UndoChanged( projProject *project );
	
	/** \brief Project changed. */
	virtual void ProjectChanged( projProject *project );
	
	/** \brief Profile count or order changed. */
	virtual void ProfileStructureChanged( projProject *project );
	
	/** \brief Active profile changed. */
	virtual void ActiveProfileChanged( projProject *project );
	/*@}*/
};

#endif
