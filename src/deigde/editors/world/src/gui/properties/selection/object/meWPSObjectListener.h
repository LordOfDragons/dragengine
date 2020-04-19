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

#ifndef _MEWPSOBJECTLISTENER_H_
#define _MEWPSOBJECTLISTENER_H_

#include "../../../../world/meWorldNotifier.h"

class meWPSObject;



/**
 * \brief Object panel listener.
 */
class meWPSObjectListener : public meWorldNotifier{
private:
	meWPSObject &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	meWPSObjectListener( meWPSObject &panel );
	
	/** \brief Clean up listener. */
	virtual ~meWPSObjectListener();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Trigger table changed. */
	virtual void TriggerTableChanged( meWorld *world );
	
	
	
	/** \brief Object selection changed. */
	virtual void ObjectSelectionChanged( meWorld *world );
	
	/** \brief Object changed. */
	virtual void ObjectChanged( meWorld *world, meObject *object );
	
	/** \brief Object class changed. */
	virtual void ObjectClassChanged( meWorld *world, meObject *object );
	
	/** \brief Object geometry changed. */
	virtual void ObjectGeometryChanged( meWorld *world, meObject *object );
	
	/** \brief Object properties changed. */
	virtual void ObjectPropertiesChanged( meWorld *world, meObject *object );
	
	/** \brief Object active property changed. */
	virtual void ObjectActivePropertyChanged( meWorld *world, meObject *object );
	
	/** \brief Object texture count changed. */
	virtual void ObjectTextureCountChanged( meWorld *world, meObject *object );
	
	/** \brief Object active texture changed. */
	virtual void ObjectActiveTextureChanged( meWorld *world, meObject *object );
	
	/** \brief Object texture changed. */
	virtual void ObjectTextureChanged( meWorld *world, meObject *object, meObjectTexture *texture );
	
	/** \brief Object texture properties changed. */
	virtual void ObjectTexturePropertiesChanged( meWorld *world, meObject *object, meObjectTexture *texture );
	
	/** \brief Object texture active property changed. */
	virtual void ObjectTextureActivePropertyChanged( meWorld *world, meObject *object, meObjectTexture *texture );
	/*@}*/
};

#endif
