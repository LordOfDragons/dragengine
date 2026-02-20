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

#ifndef _IGDEGIZMOMANAGER_H_
#define _IGDEGIZMOMANAGER_H_

#include "igdeGizmo.h"

#include <dragengine/common/utils/decCollisionFilter.h>
#include <dragengine/input/deInputEvent.h>

class deWorld;
class igdeCamera;
class igdeViewRenderWindow;


/**
 * \brief IGDE interactive 3D gizmo manager.
 * 
 * Simplifies handling interactive 3D gizmos. Typically an instance of this class is used
 * in a igdeViewRenderWindow subclass as member. Upon receiving mouse events the subclass
 * can call into this class if gizmo handling is allowed.
 * 
 * To further simplify usage mouse events can be directly send to this class. The caller
 * has to check after calling the mouse event handling classes if he has to continue
 * processing the events or not. Basically this means if the manager is editing a gizmo
 * after the event handling returns the caller has to not process it further.
 * 
 * To use simplified mouse event handling the following has to be done:
 * - subclass and overwrite GetEditingGizmo() to obtain igdeGizmo instances from collider
 *   user pointers. typically editor specific visitors are used for this
 * - set collision filter to use for ray casting
 */
class DE_DLL_EXPORT igdeGizmoManager{
private:
	igdeGizmo::Ref pEditingGizmo;
	igdeGizmo::Ref pHoverGizmo;
	
	float pRayLength;
	decCollisionFilter pRayCollisionFilter;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create gizmo manager. */
	igdeGizmoManager();
	
	/** \brief Clean up gizmo manager. */
	virtual ~igdeGizmoManager();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/**
	 * \brief Hover gizmo.
	 * 
	 * Call this when a gizmo is under the mouse pointer.
	 * 
	 * \param[in] rayOrigin Origin of ray hitting gizmo.
	 * \param[in] rayDirection Direction of ray (including length) hitting gizmo.
	 * \param[in] distance Distance from 0 to 1 along rayDirection up to hit point.
	 * \param[in] bone Index of bone hit by ray or -1.
	 * \param[in] shape Index of shape hit by ray or -1.
	 * \param[in] modifiers Modifier keys pressed at the time of release. OR combination of
	 *                      values from deInputEvent::eStateModifiers.
	 */
	void Hover(igdeGizmo *gizmo, const decDVector &rayOrigin, const decDVector &rayDirection,
		double distance, int bone, int shape, int modifiers);
	
	/**
	 * \brief Clear hover gizmo
	 * 
	 * Call this when no gizmo is under the mouse pointer.
	 */
	void ClearHover();
	
	/** \brief Hover gizmo or nullptr. */
	inline const igdeGizmo::Ref &GetHoverGizmo() const{ return pHoverGizmo; }
	
	/** \brief Hover gizmo is present. */
	inline bool HasHoverGizmo() const{ return pHoverGizmo; }
	
	
	
	/**
	 * \brief Start editing gizmo.
	 * 
	 * If editing started true is returned. Caller has to grab input on affected widget
	 * or otherwise ensure user can drag the mouse to send UpdateEditing() accordingly.
	 * If a gizmo is already editing false is returned.
	 * 
	 * \param[in] rayOrigin Origin of ray hitting gizmo.
	 * \param[in] rayDirection Direction of ray (including length) hitting gizmo.
	 * \param[in] viewMatrix View oriented matrix.
	 * \param[in] distance Distance from 0 to 1 along rayDirection up to hit point.
	 * \param[in] bone Index of bone hit by ray or -1.
	 * \param[in] shape Index of shape hit by ray or -1.
	 * \param[in] modifiers Modifier keys pressed at the time of release. OR combination of
	 *                      values from deInputEvent::eStateModifiers.
	 */
	bool StartEditing(igdeGizmo *gizmo, const decDVector &rayOrigin,
		const decDVector &rayDirection, const decDMatrix &viewMatrix,
		double distance, int bone, int shape, int modifiers);
	
	/** \brief Editing gizmo or nullptr. */
	inline const igdeGizmo::Ref &GetEditingGizmo() const{ return pEditingGizmo; }
	
	/** \brief Editing gizmo is present. */
	inline bool HasEditingGizmo() const{ return pEditingGizmo; }
	
	/**
	 * \brief Update editing.
	 * 
	 * Call if mouse moved and provide new ray position and direction for editing gizmo
	 * to update editing state. If a gizmo is editing it is updated otherwise nothing is done.
	 * 
	 * \param[in] rayOrigin Origin of ray.
	 * \param[in] rayDirection Normalized direction of ray.
	 * \param[in] viewMatrix View oriented matrix.
	 * \param[in] modifiers Modifier keys pressed at the time of release. OR combination of
	 *                      values from deInputEvent::eStateModifiers.
	 */
	void UpdateEditing(const decDVector &rayOrigin, const decDVector &rayDirection,
		const decDMatrix &viewMatrix, int modifiers);
	
	/**
	 * \brief User wheeled mouse while editing.
	 * 
	 * \param[in] rayOrigin Origin of ray.
	 * \param[in] rayDirection Normalized direction of ray.
	 * \param[in] viewMatrix View oriented matrix.
	 * \param[in] modifiers Modifier keys pressed at the time of event. OR combination of
	 *                      values from deInputEvent::eStateModifiers.
	 */
	void MouseWheeledEditing(const decDVector &rayOrigin, const decDVector &rayDirection,
		const decDMatrix &viewMatrix, const decPoint &change, int modifiers);
	
	/**
	 * \brief Frame update.
	 * 
	 * Call this always on igdeWidget::OnFrameUpdate(). If a gizmo is editing it is updated.
	 * 
	 * \param[in] elapsed Elapsed time.
	 */
	void OnFrameUpdate(float elapsed);
	
	/**
	 * \brief Stop editing gizmo if one is editing right now.
	 * 
	 * \param[in] cancel Cancel editing if possible.
	 */
	void StopEditing(bool cancel);
	
	
	
	/**
	 * \brief User pressed down mouse button.
	 * 
	 * \param[in] view View sending the event.
	 * \param[in] camera Camera used in view.
	 * \param[in] button Button code which is deInputEvent::eMouseButtonCodes for engine
	 *                   known buttons or higher values for native OS button.
	 * \param[in] position Mouse position at the time of button press in local coordinates
	 * \param[in] modifiers Modifier keys pressed at the time of press. OR combination of
	 *                      values from deInputEvent::eStateModifiers.
	 */
	void OnButtonPress(const igdeViewRenderWindow &view, const igdeCamera &camera,
		int button, const decPoint &position, int modifiers);
	
	/**
	 * \brief User released mouse button.
	 * 
	 * \param[in] view View sending the event.
	 * \param[in] camera Camera used in view.
	 * \param[in] button Button code which is deInputEvent::eMouseButtonCodes for engine
	 *                   known buttons or higher values for native OS button.
	 * \param[in] position Mouse position at the time of button release in local coordinates
	 * \param[in] modifiers Modifier keys pressed at the time of release. OR combination of
	 *                      values from deInputEvent::eStateModifiers.
	 */
	void OnButtonRelease(const igdeViewRenderWindow &view, const igdeCamera &camera,
		int button, const decPoint &position, int modifiers);
	
	/**
	 * \brief User moved mouse.
	 * 
	 * \param[in] view View sending the event.
	 * \param[in] camera Camera used in view.
	 * \param[in] position Mouse position at the time of button release in local coordinates
	 * \param[in] modifiers Modifier keys pressed at the time of release. OR combination of
	 *                      values from deInputEvent::eStateModifiers.
	 */
	void OnMouseMoved(const igdeViewRenderWindow &view, const igdeCamera &camera,
		const decPoint &position, int modifiers);
	
	/**
	 * \brief User wheeled mouse.
	 * 
	 * \param[in] view View sending the event.
	 * \param[in] camera Camera used in view.
	 * \param[in] position Mouse position where wheel is used.
	 * \param[in] change Mouse wheel change.
	 * \param[in] modifiers Modifier keys pressed at the time of release. OR combination of
	 *                      values from deInputEvent::eStateModifiers.
	 */
	void OnMouseWheeled(const igdeViewRenderWindow &view, const igdeCamera &camera,
		const decPoint &position, const decPoint &change, int modifiers);
	
	/**
	 * \brief User pressed down key while view is focused.
	 * 
	 * \param[in] keyCode Engine key code or deInputEvent::ekcUndefined if not mapped.
	 * \param[in] key Native OS key.
	 */
	void OnKeyPress(deInputEvent::eKeyCodes keyCode, int key);
	
	/**
	 * \brief User released down key while view is focused.
	 * 
	 * \param[in] keyCode Engine key code or deInputEvent::ekcUndefined if not mapped.
	 * \param[in] key Native OS key.
	 */
	void OnKeyRelease(deInputEvent::eKeyCodes keyCode, int key);
	
	/** \brief Length of ray to use for simplified mouse event handling ray casting. */
	inline float GetRayLength() const{ return pRayLength; }
	
	/**
	 * \brief Set length of ray to use for simplified mouse event handling.
	 * 
	 * Default length is 1km.
	 */
	void SetRayLength(float length);
	
	/** \brief Collision filter to use for simplified mouse event handling ray casting. */
	inline const decCollisionFilter &GetRayCollisionFilter() const{ return pRayCollisionFilter; }
	
	/**
	 * \brief Set collision filter to use for simplified mouse event handling ray casting.
	 * 
	 * Default collision filter is empty.
	 */
	void SetRayCollisionFilter(const decCollisionFilter &collisionFilter);
	
	/**
	 * \brief Get igdeGizmo from collider user pointer or nullptr if not a igdeGizmo.
	 * 
	 * Subclass has to implement this function to allow using the mouse event handling.
	 * Default implementation returns nullptr.
	 * 
	 * \param[in] userPointer Collider user pointer. This is never nullptr.
	 */
	virtual igdeGizmo *GizmoFromColliderUserPointer(void *userPointer);
	/*@}*/
};

#endif
