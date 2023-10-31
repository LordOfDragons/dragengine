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

#ifndef _IGDEVIEWRENDERWINDOW_H_
#define _IGDEVIEWRENDERWINDOW_H_

#include "igdeWidget.h"

#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/resources/canvas/deCanvasReference.h>
#include <dragengine/resources/rendering/deRenderWindowReference.h>

class deCanvas;
class deCamera;
class deCanvasView;
class deCanvasPaint;
class deCanvasRenderWorld;
class igdeMouseKeyListener;



/**
 * \brief IGDE UI Widget using Game Engine Render Window.
 */
class DE_DLL_EXPORT igdeViewRenderWindow : public igdeWidget{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeViewRenderWindow> Ref;
	
	
	
private:
	deRenderWindowReference pRenderWindow;
	deCanvasReference pCanvasRenderWorld;
	deCanvasReference pCanvasBackground;
	bool pEnableRendering;
	bool pEngineRunning;
	
	decObjectOrderedSet pListeners;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create render window. */
	igdeViewRenderWindow( igdeEnvironment &environment );
	
	
	
protected:
	/**
	 * \brief Clean up widget.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeViewRenderWindow();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Render window or NULL if absent. */
	inline deRenderWindow *GetRenderWindow() const{ return pRenderWindow; }
	
	/** \brief Rendering is enabled. */
	inline bool GetEnableRendering() const{ return pEnableRendering; }
	
	/** \brief Set if rendering is enabled. */
	void SetEnableRendering( bool enable );
	
	/** \brief Rendering is possible. */
	virtual bool GetCanRender() const;
	
	/** \brief Render area size. */
	virtual decPoint GetRenderAreaSize() const;
	
	
	
	/** \brief Clear render window failed error allowing the render window to try again. */
	void ClearErrorRenderWindow();
	
	
	
	/** \brief Game engine has been started. */
	virtual void OnAfterEngineStart();
	
	/** \brief Game engine is about to be stopped. */
	virtual void OnBeforeEngineStop();
	
	/** \brief Game like frame update. */
	virtual void OnFrameUpdate( float elapsed );
	
	
	
	/**
	 * \brief Set camera to render.
	 * 
	 * Convenience call to not deal with deCanvasRenderWorld.
	 */
	void SetRenderWorld( deCamera *camera );
	
	/**
	 * \brief Add canvas.
	 * \details Convenience call to not deal with deRenderWindow, deRenderTarget and deCanvasView.
	 */
	void AddCanvas( deCanvas *canvas );
	
	/**
	 * \brief Remove canvas.
	 * \details Convenience call to not deal with deRenderWindow, deRenderTarget and deCanvasView.
	 */
	void RemoveCanvas( deCanvas *canvas );
	
	/** \brief Render window canvas. */
	deCanvasView *GetRenderWindowCanvas() const;
	
	/** \brief Render world canvas. */
	deCanvasRenderWorld *GetCanvasRenderWorld() const;
	
	/** \brief Background canvas. */
	deCanvasPaint *GetCanvasBackground() const;
	
	/**
	 * \brief Create canvas.
	 * 
	 * Called once it is safe to create canvas. This is the case if the widget is fully
	 * visible and functional on the user screen and the engine render window has been
	 * successfully created.
	 */
	virtual void CreateCanvas();
	
	/** \brief Grab mouse and keyboard input. */
	virtual void GrabInput();
	
	/** \brief Release mouse and keyboard input. */
	virtual void ReleaseInput();
	
	
	
	/** \brief Add listener. */
	void AddListener( igdeMouseKeyListener *listener );
	
	/** \brief Remove listener. */
	void RemoveListener( igdeMouseKeyListener *listener );
	
	/** \brief Notify listeners user pressed down key while view is focused. */
	virtual void NotifyKeyPress( deInputEvent::eKeyCodes keyCode, int key );
	
	/** \brief Notify listeners user released down key while view is focused. */
	virtual void NotifyKeyRelease( deInputEvent::eKeyCodes keyCode, int key );
	
	/** \brief Notify listener user pressed down mouse button. */
	virtual void NotifyButtonPress( int button, const decPoint &position, int modifiers );
	
	/** \brief Notify listeners user released down mouse button. */
	virtual void NotifyButtonRelease( int button, const decPoint &position, int modifiers );
	
	/** \brief Notify listener user double clicked mouse button. */
	virtual void NotifyDoubleClicked( int button, const decPoint &position, int modifiers );
	
	/** \brief Notify listeners user moved mouse. */
	virtual void NotifyMouseMoved( const decPoint &position, int modifiers );
	
	/** \brief Notify listeners user wheeled mouse. */
	virtual void NotifyMouseWheeled( const decPoint &position, const decPoint &change, int modifiers );
	
	/** \brief Notify listeners mouse entered widget. */
	virtual void NotifyMouseEnter();
	
	/** \brief Notify listeners mouse left widget. */
	virtual void NotifyMouseLeave();
	/*@}*/
	
	
	
	/**
	 * \name IGDE Internal Use Only
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	/*@{*/
	/**
	 * \brief Create native widget.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	virtual void CreateNativeWidget();
	
	/**
	 * \brief Drop native widget.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	virtual void DropNativeWidget();
	
	/**
	 * \brief Destroy native widget.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	virtual void DestroyNativeWidget();
	
	/**
	 * \brief Create and attach render window.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	void CreateAndAttachRenderWindow();
	/*@}*/
};

#endif
