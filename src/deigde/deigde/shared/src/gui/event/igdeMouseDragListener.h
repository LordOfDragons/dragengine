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

#ifndef _IGDEMOUSEDRAGLISTENER_H_
#define _IGDEMOUSEDRAGLISTENER_H_

#include "igdeMouseKeyListener.h"


/**
 * \brief Drag Mouse IGDE UI Mouse/Key Listener.
 * 
 * Supports draging with left/right button and shift/control modifier keys.
 */
class DE_DLL_EXPORT igdeMouseDragListener : public igdeMouseKeyListener{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeMouseDragListener> Ref;
	
	
	/** \brief Drag state. */
	enum eDragState{
		/** \brief No dragging in progress. */
		edsNone,
		
		/** \brief Dragging using the left mouse button. */
		edsLeft,
		
		/** \brief Dragging using the right mouse button. */
		edsRight
	};
	
	
	
private:
	decPoint pDragOrigin;
	decPoint pDragPosition;
	eDragState pDragState;
	bool pShiftOrigin;
	bool pShiftNow;
	bool pControlOrigin;
	bool pControlNow;
	
	
	
protected:
	/** \text Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	igdeMouseDragListener();
	
	
	
protected:
	/**
	 * \brief Clean up listener.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeMouseDragListener();
	/*@}*/
	
	
	
public:
	/** \text Management */
	/*@{*/
	/** \brief Drag origin position. */
	inline const decPoint &GetDragOrigin() const{return pDragOrigin;}
	
	/** \brief Set drag origin position. */
	void SetDragOrigin(const decPoint &origin);
	
	/** \brief Current drag position. */
	inline const decPoint &GetDragPosition() const{return pDragPosition;}
	
	/** \brief Set current drag position. */
	void SetDragPosition(const decPoint &position);
	
	/** \brief Current distance from drag position to drag origin position. */
	decPoint GetDragDistance() const;
	
	
	
	/** \brief Drag state. */
	inline eDragState GetDragState() const{return pDragState;}
	
	/** \brief Set drag state. */
	void SetDragState(eDragState state);
	
	/**
	 * \brief Cancel dragging.
	 * 
	 * If a drag is in progress OnDragFinish(false) is called.
	 */
	void Cancel();
	
	
	
	/** \brief Shift modifier button has been pressed at the beginning of dragging. */
	inline bool GetShiftOrigin() const{return pShiftOrigin;}
	
	/** \brief Set if shift modifier button has been pressed at the beginning of dragging. */
	void SetShiftOrigin(bool pressed);
	
	/** \brief Shift modifier button is pressed now. */
	inline bool GetShiftNow() const{return pShiftNow;}
	
	/** \brief Set if shift modifier button is pressed now. */
	void SetShiftNow(bool pressed);
	
	/** \brief Shift modifier button is pressed now or had been pressed at the beginning of dragging. */
	inline bool GetShift() const{return pShiftNow | pShiftOrigin;}
	
	/** \brief Control modifier button has been pressed at the beginning of dragging. */
	inline bool GetControlOrigin() const{return pControlOrigin;}
	
	/** \brief Set if control modifier button has been pressed at the beginning of dragging. */
	void SetControlOrigin(bool pressed);
	
	/** \brief Control modifier button is pressed now. */
	inline bool GetControlNow() const{return pControlNow;}
	
	/** \brief Set if control modifier button is pressed now. */
	void SetControlNow(bool pressed);
	
	/** \brief Control modifier button is pressed now or had been pressed at the beginning of dragging. */
	inline bool GetControl() const{return pControlNow | pControlOrigin;}
	/*@}*/
	
	
	
	/** \text Dragging events */
	/*@{*/
	/**
	 * \brief Dragging begins.
	 * 
	 * Called when a mouse button has been pressed while the state is edsNone. Subclass has
	 * to start dragging operation if possible. Drag state is set to edsLeft or edsRight
	 * depending on the pressed button.
	 * 
	 * \returns true to start drag operation or false to abort without calling OnDragFinish.
	 */
	virtual bool OnDragBegin();
	
	/**
	 * \brief Update dragging.
	 * 
	 * Called if state is not edsNone and mouse has been moved. Subclass has to update the
	 * dragging process.
	 */
	virtual void OnDragUpdate();
	
	/**
	 * \brief Dragging ends.
	 * 
	 * Called if state is not edsNone and mouse button used to start draggins has been
	 * released. Subclass has to finish dragging operation.
	 */
	virtual void OnDragFinish(bool cancelled);
	/*@}*/
	
	
	
	/** \text Events */
	/*@{*/
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
	virtual void OnButtonPress(igdeWidget *widget, int button, const decPoint &position, int modifiers);
	
	/**
	 * \brief User released down mouse button.
	 * 
	 * \param[in] widget Widget sending the event.
	 * \param[in] button Button code which is deInputEvent::eMouseButtonCodes for engine
	 *                   known buttons or higher values for native OS button.
	 * \param[in] position Mouse position at the time of button release in local coordinates
	 * \param[in] modifiers Modifier keys pressed at the time of release. OR combination of
	 *                      values from deInputEvent::eStateModifiers.
	 */
	virtual void OnButtonRelease(igdeWidget *widget, int button, const decPoint &position, int modifiers);
	
	/**
	 * \brief User moved mouse.
	 * 
	 * \param[in] widget Widget sending the event.
	 * \param[in] position Mouse position at the time of button release in local coordinates
	 * \param[in] modifiers Modifier keys pressed at the time of release. OR combination of
	 *                      values from deInputEvent::eStateModifiers.
	 */
	virtual void OnMouseMoved(igdeWidget *widget, const decPoint &position, int modifiers);
	/*@}*/
};

#endif
