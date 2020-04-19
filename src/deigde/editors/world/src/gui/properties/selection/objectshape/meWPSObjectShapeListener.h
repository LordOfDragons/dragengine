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

#ifndef _MEWPSOBJECTSHAPELISTENER_H_
#define _MEWPSOBJECTSHAPELISTENER_H_

#include "../../../../world/meWorldNotifier.h"

class meWPSObjectShape;



/**
 * \brief Object Shape Panel Listener.
 */
class meWPSObjectShapeListener : public meWorldNotifier{
private:
	meWPSObjectShape &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	meWPSObjectShapeListener( meWPSObjectShape &panel );
	
	/** \brief Clean up listener. */
	virtual ~meWPSObjectShapeListener();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Object selection changed. */
	virtual void ObjectSelectionChanged( meWorld *world );
	
	/** \brief Object class changed. */
	virtual void ObjectClassChanged( meWorld *world, meObject *object );
	
	/** \brief Object geometry changed. */
	virtual void ObjectGeometryChanged( meWorld *world, meObject *object );
	
	/** \brief Object properties changed. */
	virtual void ObjectPropertiesChanged( meWorld *world, meObject *object );
	
	/** \brief Object active property changed. */
	virtual void ObjectActivePropertyChanged( meWorld *world, meObject *object );
	
	/** \brief Object shape list changed. */
	virtual void ObjectShapeListChanged( meWorld *world );
	
	/** \brief Object shape selection changed. */
	virtual void ObjectShapeSelectionChanged( meWorld *world );
	/*@}*/
};

#endif
