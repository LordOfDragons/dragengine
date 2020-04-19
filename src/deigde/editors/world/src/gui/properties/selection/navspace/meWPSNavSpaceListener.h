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

#ifndef _MEWPSNAVSPACELISTENER_H_
#define _MEWPSNAVSPACELISTENER_H_

#include "../../../../world/meWorldNotifier.h"

class meWPSNavSpace;



/**
 * \brief Navigation Space Panel Listener.
 */
class meWPSNavSpaceListener : public meWorldNotifier{
private:
	meWPSNavSpace &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	meWPSNavSpaceListener( meWPSNavSpace &panel );
	
	/** \brief Clean up listener. */
	virtual ~meWPSNavSpaceListener();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Navigation space selection changed. */
	virtual void NavSpaceSelectionChanged( meWorld *world );
	
	/** \brief Navigation space changed. */
	virtual void NavSpaceChanged( meWorld *world, meNavigationSpace *navspace );
	
	/** \brief Navigation space geometry changed. */
	virtual void NavSpaceGeometryChanged( meWorld *world, meNavigationSpace *navspace );
	
	/** \brief Navigation space used cost types changed. */
	virtual void NavSpaceUsedCostTypesChanged( meWorld *world, meNavigationSpace *navspace );
	/*@}*/
};

#endif
