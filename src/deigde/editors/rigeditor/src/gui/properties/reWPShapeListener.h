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

#ifndef _REWPSHAPELISTENER_H_
#define _REWPSHAPELISTENER_H_

#include "../../rig/reRigNotifier.h"

class reWPShape;



/**
 * \brief Shape panel rig listener.
 */
class reWPShapeListener : public reRigNotifier{
private:
	reWPShape &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	reWPShapeListener( reWPShape &panel );
	
	/** \brief Clean up listener. */
	virtual ~reWPShapeListener();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief A shape changed. */
	virtual void ShapeChanged( reRig *rig, reRigShape *shape );
	
	/** \brief A shape changed selection state. */
	virtual void ShapeSelectedChanged( reRig *rig, reRigShape *shape );
	
	/** \brief All shapes have been deselected. */
	virtual void AllShapesDeselected( reRig *rig );
	
	/** \brief The active shape changed. */
	virtual void ActiveShapeChanged( reRig *rig );
	/*@}*/
};

#endif
