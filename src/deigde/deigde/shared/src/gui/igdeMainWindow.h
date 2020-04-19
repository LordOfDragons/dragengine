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

#ifndef _IGDEMAINWINDOW_H_
#define _IGDEMAINWINDOW_H_

#include "igdeWindow.h"
#include "../environment/igdeEnvironment.h"

#include <dragengine/common/math/decMath.h>


class igdeEngineController;



/**
 * \brief Main window.
 */
class igdeMainWindow : public igdeWindow{
public:
	/** \brief Window state. */
	enum eWindowStates{
		ewsNormal,
		ewsMinimized,
		ewsMaximized
	};
	
	
	
private:
	igdeEngineController *pEngineController;
	decPoint pInitialSize;
	eWindowStates pWindowState;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create main window prototype. */
	igdeMainWindow( igdeEnvironment &environment, const char *title );
	
	
	
protected:
	/**
	 * \brief Clean up main window prototype.
	 * \warning Subclass is responsible to calls StopEngine to shut down the engine
	 *          before starting cleaning up widgets.
	 */
	virtual ~igdeMainWindow();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Initial size when shown or (0,0) to choose automatically. */
	inline const decPoint &GetInitialSize() const{ return pInitialSize; }
	
	/** \brief Set initial size when shown or (0,0) to choose automatically. */
	void SetInitialSize( const decPoint &initialSize );
	
	/** \brief Window state. */
	inline eWindowStates GetWindowState() const{ return pWindowState; }
	
	/** \brief Set window state. */
	void SetWindowState( eWindowStates windowState );
	
	
	
	/** \brief Get engine controller or NULL if not present. */
	inline igdeEngineController *GetMainWindowEngineController() const{ return pEngineController; }
	
	/** \brief Start engine. */
	void StartEngine();
	
	/** \brief Stop engine. */
	void StopEngine();
	
	/** \brief Hook method called before the engine starts. */
	virtual void OnBeforeEngineStart();
	
	/** \brief Hook method called after the engine has been started. */
	virtual void OnAfterEngineStart();
	
	/** \brief Hook method called before the engine is shut down. */
	virtual void OnBeforeEngineStop();
	
	/** \brief Hook method called before the engine is shut down. */
	virtual void OnAfterEngineStop();
	
	/** \brief Processes frame update messages. */
	virtual void OnFrameUpdate();
	
	/** \brief Window state changed. */
	virtual void OnWindowState();
	
	
	
	/** \brief Close window bypassing CloseWindow(). */
	virtual void Close();
	
	
	
protected:
	void CreateEngineController();
	void AddNullModules();
	void ActivateNullModules();
	
	
	
public:
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
	/*@}*/
	
	
	
protected:
	/**
	 * \brief Window title changed.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	virtual void OnTitleChanged();
	
	/**
	 * \brief Window icon changed.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	virtual void OnIconChanged();
	
	/**
	 * \brief Window size changed.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	virtual void OnSizeChanged();
	
	/**
	 * \brief Window position changed.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	virtual void OnPositionChanged();
	
	/** \brief Visible changed. */
	virtual void OnVisibleChanged();
	
	/** \brief Enabled changed. */
	virtual void OnEnabledChanged();
	
	/** \brief Window state changed. */
	virtual void OnWindowStateChanged();
	/*@}*/
};

#endif
