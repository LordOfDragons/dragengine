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

#ifndef _IGDEMOUSEKEYLISTENER_H_
#define _IGDEMOUSEKEYLISTENER_H_


#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/input/deInputEvent.h>


class igdeWidget;


/**
 * \brief IGDE UI Mouse/Key Listener.
 */
class DE_DLL_EXPORT igdeMouseKeyListener : public deObject{
protected:
	/** \text Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	igdeMouseKeyListener();
	
	
	
protected:
	/**
	 * \brief Clean up listener.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeMouseKeyListener();
	/*@}*/
	
	
	
public:
	/** \text Events */
	/*@{*/
	/**
	 * \brief User pressed down key while view is focused.
	 * 
	 * \param[in] widget Widget sending the event.
	 * \param[in] keyCode Engine key code or deInputEvent::ekcUndefined if not mapped.
	 * \param[in] key Native OS key.
	 */
	virtual void OnKeyPress( igdeWidget *widget, deInputEvent::eKeyCodes keyCode, int key );
	
	/**
	 * \brief User released down key while view is focused.
	 * 
	 * \param[in] widget Widget sending the event.
	 * \param[in] keyCode Engine key code or deInputEvent::ekcUndefined if not mapped.
	 * \param[in] key Native OS key.
	 */
	virtual void OnKeyRelease( igdeWidget *widget, deInputEvent::eKeyCodes keyCode, int key );
	
	/**
	 * \brief User pressed down mouse button.
	 * 
	 * \param[in] widget Widget sending the event.
	 * \param[in] button Button code which is deInputEvent::eMouseButtonCodes for engine
	 *                   known buttons or higher values for native OS button.
	 * \param[in] position Mouse position at the time of button press in local coordinates
	 * \param[in] modifiers Modifier keys pressed at the time of press. OR combination of
	 *                      values from deInputEvent::eStateModifiers.
	 */
	virtual void OnButtonPress( igdeWidget *widget, int button, const decPoint &position, int modifiers );
	
	/**
	 * \brief User released mouse button.
	 * 
	 * \param[in] widget Widget sending the event.
	 * \param[in] button Button code which is deInputEvent::eMouseButtonCodes for engine
	 *                   known buttons or higher values for native OS button.
	 * \param[in] position Mouse position at the time of button release in local coordinates
	 * \param[in] modifiers Modifier keys pressed at the time of release. OR combination of
	 *                      values from deInputEvent::eStateModifiers.
	 */
	virtual void OnButtonRelease( igdeWidget *widget, int button, const decPoint &position, int modifiers );
	
	/**
	 * \brief User double clicked mouse button.
	 * 
	 * Double click event will not be send as OnButtonPress/OnButtonRelease too.
	 * 
	 * \param[in] widget Widget sending the event.
	 * \param[in] button Button code which is deInputEvent::eMouseButtonCodes for engine
	 *                   known buttons or higher values for native OS button.
	 * \param[in] position Mouse position at the time of button press in local coordinates
	 * \param[in] modifiers Modifier keys pressed at the time of press. OR combination of
	 *                      values from deInputEvent::eStateModifiers.
	 */
	virtual void OnDoubleClicked( igdeWidget *widget, int button, const decPoint &position, int modifiers );
	
	/**
	 * \brief User moved mouse.
	 * 
	 * \param[in] widget Widget sending the event.
	 * \param[in] position Mouse position at the time of button release in local coordinates
	 * \param[in] modifiers Modifier keys pressed at the time of release. OR combination of
	 *                      values from deInputEvent::eStateModifiers.
	 */
	virtual void OnMouseMoved( igdeWidget *widget, const decPoint &position, int modifiers );
	
	/**
	 * \brief User wheeled mouse.
	 * 
	 * \param[in] widget Widget sending the event.
	 * \param[in] position Mouse position where wheel is used.
	 * \param[in] change Mouse wheel change.
	 * \param[in] modifiers Modifier keys pressed at the time of release. OR combination of
	 *                      values from deInputEvent::eStateModifiers.
	 */
	virtual void OnMouseWheeled( igdeWidget *widget, const decPoint &position, const decPoint &change, int modifiers );
	/*@}*/
};

#endif
