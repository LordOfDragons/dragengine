/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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

public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeMouseKeyListener> Ref;
	
	
	
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
	
	/**
	 * \brief Mouse entered widget.
	 * 
	 * \param[in] widget Widget sending the event.
	 */
	virtual void OnMouseEnter( igdeWidget *widget );
	
	/**
	 * \brief Mouse left widget.
	 * 
	 * \param[in] widget Widget sending the event.
	 */
	virtual void OnMouseLeave( igdeWidget *widget );
	/*@}*/
};

#endif
