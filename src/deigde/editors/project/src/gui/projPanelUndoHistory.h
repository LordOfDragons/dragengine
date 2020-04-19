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

#ifndef _PROJPANELUNDOHISTORY_H_
#define _PROJPANELUNDOHISTORY_H_

#include <deigde/gui/properties/igdeWPUndoHistory.h>

class projProject;
class projPanelUndoHistoryListener;
class projWindowMain;



/**
 * \brief Profiles panel.
 */
class projPanelUndoHistory : public igdeWPUndoHistory{
private:
	projProject *pProject;
	projPanelUndoHistoryListener *pListener;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create view. */
	projPanelUndoHistory( igdeEnvironment &environment );
	
	/** \brief Clean up view. */
	virtual ~projPanelUndoHistory();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Project. */
	inline projProject *GetProject() const{ return pProject; }
	
	/** \brief Set synthesizer. */
	void SetProject( projProject *project );
	/*@}*/
};

#endif
