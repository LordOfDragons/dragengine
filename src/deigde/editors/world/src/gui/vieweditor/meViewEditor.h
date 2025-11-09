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

#ifndef _MEVIEWEDITOR_H_
#define _MEVIEWEDITOR_H_

#include "../../world/meWorldGuiParameters.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/input/deInputEvent.h>

class meCamera;
class meConfiguration;
class meObjectList;
class meView3D;
class meWorld;
class meWorldGuiParameters;

class deCollider;
class deGraphicContext;
class deBaseScriptingCollider;
class deException;
class decCollisionFilter;


/**
 * \brief Base class for view editors.
 * 
 * A view editor defines editing behavior for a specific task using a meView3D widget.
 * Provides support for handling mouse and keyboard input directed at the meView3D
 * widget for a specific editing task.
 */
class meViewEditor{
private:
	meView3D &pView;
	
	decDMatrix pMatrixView;
	decDMatrix pMatrixViewLocal;
	decDMatrix pMatrixViewLocalInverse;
	
	decPoint pDragOrgin;
	decPoint pDragCurrent;
	decPoint pDragDistanceLast;
	decPoint pDragDistanceStart;
	
	bool pShiftStart;
	bool pControlStart;
	
	bool pDragLMB;
	bool pDragRMB;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create view editor. */
	meViewEditor( meView3D &view );
	
	/** \brief Clean up view editor. */
	virtual ~meViewEditor();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Parent view. */
	inline meView3D &GetView() const{ return pView; }
	
	/** \brief Width of the view. */
	int GetViewWidth() const;
	
	/** \brief Height of the view. */
	int GetViewHeight() const;
	
	/** \brief World. */
	meWorld &GetWorld() const;
	
	/** \brief World gui parameters. */
	meWorldGuiParameters &GetWorldGuiParameters() const;
	
	/** \brief Element mode. */
	meWorldGuiParameters::eElementModes GetElementMode() const;
	
	/** \brief Active camera. */
	meCamera &GetActiveCamera() const;
	
	/** \brief Configuration. */
	meConfiguration &GetConfiguration() const;
	
	
	
	/** \brief View matrix. */
	inline const decDMatrix &GetMatrixView() const{ return pMatrixView; }
	
	/** \brief Local view matrix. */
	inline const decDMatrix &GetMatrixViewLocal() const{ return pMatrixViewLocal; }
	
	/** \brief Inverse local view matrix. */
	inline const decDMatrix &GetMatrixViewLocalInverse() const{ return pMatrixViewLocalInverse; }
	
	
	
	/** \brief Drag origin point. */
	inline const decPoint &GetDragOrigin() const{ return pDragOrgin; }
	
	/** \brief Current drag point. */
	inline const decPoint &GetDragCurrent() const{ return pDragCurrent; }
	
	/** \brief Drag distance since the last mouse movement. */
	inline const decPoint &GetDragDistanceLast() const{ return pDragDistanceLast; }
	
	/** \brief Drag distance since the start of the dragging. */
	inline const decPoint &GetDragDistanceStart() const{ return pDragDistanceStart; }
	
	
	
	/** \brief Shift key has been pressed at the start of dragging. */
	inline bool GetShiftStart() const{ return pShiftStart; }
	
	/** \brief Shift key has been pressed at the start of dragging. */
	inline bool GetControlStart() const{ return pControlStart; }
	
	
	
	/** \brief Left mouse button started the dragging. */
	inline bool GetDragLeftMouseButton() const{ return pDragLMB; }
	
	/** \brief Right mouse button started the dragging. */
	inline bool GetDragRightMouseButton() const{ return pDragRMB; }
	
	
	
	/** \brief Update matrices. */
	void UpdateMatrices();
	
	/**
	 * \brief Check for axis locking in response to a key press event.
	 * \returns true if the key has been handled.
	 */
	bool CheckAxisLocking( deInputEvent::eKeyCodes key );
	
	/** \brief Log exception. */
	void LogException( const deException &exception ) const;
	
	
	
	/** \brief List of selected objects including all objects attached to them. */
	void GetSelectedObjectsWithAttached( meObjectList &list );
	
	
	
	/** \brief Test for collision with scene elements using a ray originating from (x,y) running along the view axis. */
	void RayTestCollision( deBaseScriptingCollider *listener, const decDVector &rayPosition,
		const decVector &rayDirection, const decCollisionFilter &collisionFilter );
	
	/** \brief Test for collision with scene elements using a collider . */
	void ColliderTestCollision( deBaseScriptingCollider *listener, deCollider *collider );
	
	
	
	/**
	 * \brief Calculate view matrix.
	 * \details Overwrite to set to a specific matrix if some element alters the matrix to use.
	 */
	virtual decDMatrix CalculateViewMatrix() const;
	
	/**
	 * \brief Calculate view local matrix.
	 * \details Overwrite to set to a specific matrix if some element alters the matrix to use.
	 */
	virtual decDMatrix CalculateViewLocalMatrix() const;
	
	
	
	/** \brief Game like frame update. */
	virtual void OnFrameUpdate( float elapsed );
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	/** \brief View size changed. */
	virtual void OnResize();
	
	/** \brief A key on the keyboard has been pressed. Return true if handled. */
	virtual bool OnKeyPress( deInputEvent::eKeyCodes key, bool shift, bool control );
	
	/** \brief A key on the keyboard has been released. Return true if handled. */
	virtual bool OnKeyRelease( deInputEvent::eKeyCodes key, bool shift, bool control );
	
	/** \brief The left mouse button has been pressed. */
	virtual void OnLeftMouseButtonPress( int x, int y, bool shift, bool control );
	
	/** \brief The left mouse button has been released. */
	virtual void OnLeftMouseButtonRelease( int x, int y, bool shift, bool control );
	
	/** \brief The right mouse button has been pressed. */
	virtual void OnRightMouseButtonPress( int x, int y, bool shift, bool control );
	
	/** \brief The right mouse button has been released. */
	virtual void OnRightMouseButtonRelease( int x, int y, bool shift, bool control );
	
	/** \brief The mouse has been moved. */
	virtual void OnMouseMove( int x, int y, bool shift, bool control );
	
	/**
	 * \brief The mouse wheel has been used. Steps contains the number of steps
	 *        up (positive) or down (negative)
	 */
	virtual void OnMouseWheel( int steps, bool shift, bool control );
	
	/** \brief The mouse entered view. */
	virtual void OnMouseEnter();
	
	/** \brief The mouse left view. */
	virtual void OnMousLeave();
	/*@}*/
};

#endif
