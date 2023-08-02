/* 
 * Drag[en]gine IGDE
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#ifndef _IGDEGIZMOMOUSEKEYLISTENER_H_
#define _IGDEGIZMOMOUSEKEYLISTENER_H_

#include "../event/igdeMouseKeyListener.h"

class igdeCamera;
class igdeViewRenderWindow;
class igdeGizmoManager;


/**
 * \brief Mouse key listener handling gizmos.
 * 
 * Helps handling gizmos by sending mouse events to subclasses igdeGizmoManager. Subclass
 * can check after super class implementation returns if a gizmo is in progress of editing
 * and do further event handling if required.
 */
class DE_DLL_EXPORT igdeGizmoMouseKeyListener : public igdeMouseKeyListener{
private:
	igdeGizmoManager &pGizmoManager;
	igdeViewRenderWindow &pView;
	igdeCamera *pCamera;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create gizmo mouse key listener. */
	igdeGizmoMouseKeyListener( igdeGizmoManager &gizmoManager, igdeViewRenderWindow &view );
	
	/** \brief Clean up gizmo mouse key listener. */
	virtual ~igdeGizmoMouseKeyListener() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Gizmo manager. */
	inline igdeGizmoManager &GetGizmoManager() const{ return pGizmoManager; }
	
	/** \brief View. */
	inline igdeViewRenderWindow &GetView() const{ return pView; }
	
	/** \brief Camera or nullptr. */
	inline igdeCamera *GetCamera() const{ return pCamera; }
	
	/** \brief Set camera or nullptr. */
	void SetCamera( igdeCamera *camera );
	
	
	
	/**
	 * \brief User pressed down key while view is focused.
	 * 
	 * \param[in] widget Widget sending the event.
	 * \param[in] keyCode Engine key code or deInputEvent::ekcUndefined if not mapped.
	 * \param[in] key Native OS key.
	 */
	virtual void OnKeyPress( igdeWidget *widget, deInputEvent::eKeyCodes keyCode, int key ) override;
	
	/**
	 * \brief User released down key while view is focused.
	 * 
	 * \param[in] widget Widget sending the event.
	 * \param[in] keyCode Engine key code or deInputEvent::ekcUndefined if not mapped.
	 * \param[in] key Native OS key.
	 */
	virtual void OnKeyRelease( igdeWidget *widget, deInputEvent::eKeyCodes keyCode, int key ) override;
	
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
	virtual void OnButtonPress( igdeWidget *widget, int button, const decPoint &position, int modifiers ) override;
	
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
	virtual void OnButtonRelease( igdeWidget *widget, int button, const decPoint &position, int modifiers ) override;
	
	/**
	 * \brief User moved mouse.
	 * 
	 * \param[in] widget Widget sending the event.
	 * \param[in] position Mouse position at the time of button release in local coordinates
	 * \param[in] modifiers Modifier keys pressed at the time of release. OR combination of
	 *                      values from deInputEvent::eStateModifiers.
	 */
	virtual void OnMouseMoved( igdeWidget *widget, const decPoint &position, int modifiers ) override;
	
	/**
	 * \brief User wheeled mouse.
	 * 
	 * \param[in] widget Widget sending the event.
	 * \param[in] position Mouse position where wheel is used.
	 * \param[in] change Mouse wheel change.
	 * \param[in] modifiers Modifier keys pressed at the time of release. OR combination of
	 *                      values from deInputEvent::eStateModifiers.
	 */
	virtual void OnMouseWheeled( igdeWidget *widget, const decPoint &position, const decPoint &change, int modifiers ) override;
	/*@}*/
};

#endif
