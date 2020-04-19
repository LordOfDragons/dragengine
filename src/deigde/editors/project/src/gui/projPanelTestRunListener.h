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

#ifndef _PROJPANELTESTRUNLISTENER_H_
#define _PROJPANELTESTRUNLISTENER_H_

#include "../project/projProjectListener.h"

class projPanelTestRun;



/**
 * \brief Panel test-run listener.
 */
class projPanelTestRunListener : public projProjectListener{
private:
	projPanelTestRun &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	projPanelTestRunListener( projPanelTestRun &panel );
	
	/** \brief Clean up listener. */
	virtual ~projPanelTestRunListener();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Project changed. */
	virtual void ProjectChanged( projProject *project );
	
	
	
	/** \brief Profile count or order changed. */
	virtual void ProfileStructureChanged( projProject *project );
	
	/** \brief Profile name changed. */
	virtual void ProfileNameChanged( projProject *project, projProfile *profile );
	
	
	
	/** \brief Launcher profile changed. */
	virtual void ActiveLaunchProfileChanged( projProject *project );
	/*@}*/
};

#endif
