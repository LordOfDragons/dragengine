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

#ifndef _IGDEVIEWCURVEBEZIERLISTENER_H_
#define _IGDEVIEWCURVEBEZIERLISTENER_H_


#include <dragengine/deObject.h>


class igdeViewCurveBezier;
class igdeMenuCascade;


/**
 * \brief IGDE UI ViewCurveBezier Listener.
 */
class igdeViewCurveBezierListener : public deObject{
protected:
	/** \text Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	igdeViewCurveBezierListener();
	
	
	
protected:
	/**
	 * \brief Clean up widget.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeViewCurveBezierListener();
	/*@}*/
	
	
	
public:
	/** \text Events */
	/*@{*/
	/**
	 * \brief ViewCurveBezier curve changed.
	 * 
	 * User finished modifying curve. Called if values are changed manually or user
	 * used mouse interaction to manipulate curve and finished the interaction.
	 */
	virtual void OnCurveChanged( igdeViewCurveBezier *viewCurveBezier );
	
	/**
	 * \brief ViewCurveBezier curve is changing.
	 * 
	 * User is using mouse interaction to manipulate curve but has not yet finished.
	 * This event is send each time the curve changed while the user is actively
	 * modifying using mouse interaction. Once finished OnCurveChanged() is called.
	 */
	virtual void OnCurveChanging( igdeViewCurveBezier *viewCurveBezier );
	
	/**
	 * \brief ViewCurveBezier selected point changed.
	 * 
	 * User selected curve point.
	 */
	virtual void OnSelectedPointChanged( igdeViewCurveBezier *viewCurveBezier );
	
	/**
	 * \brief Create context menu.
	 * 
	 * Context menu entries have to be added to the menu. This allows multiple listeners
	 * to add entries to the context menu without affecting others. If entries are already
	 * present the listener should also add a separator first.
	 */
	virtual void AddContextMenuEntries( igdeViewCurveBezier *viewCurveBezier, igdeMenuCascade &menu );
	/*@}*/
};

#endif
