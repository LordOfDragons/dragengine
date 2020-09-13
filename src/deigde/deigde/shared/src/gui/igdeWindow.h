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

#ifndef _IGDEWINDOW_H_
#define _IGDEWINDOW_H_

#include <stddef.h>

#include "igdeContainer.h"
#include "resources/igdeIconReference.h"

#include <dragengine/common/string/decString.h>

class igdeContainerReference;
class igdeWidget;



/**
 * \brief IGDE UI Window.
 */
class igdeWindow : public igdeContainer{
private:
	decString pTitle;
	igdeIconReference pIcon;
	bool pCanResize;
	decPoint pPosition;
	decPoint pSize;
	bool pEnabled;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create window. */
	igdeWindow( igdeEnvironment &environment, const char *title,
		igdeIcon *icon = NULL, bool canResize = true );
	
	
	
protected:
	/** \brief Clean up window. */
	virtual ~igdeWindow();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Title. */
	inline const decString &GetTitle() const{ return pTitle; }
	
	/** \brief Set title. */
	void SetTitle( const char *title );
	
	/** \brief Icon or NULL. */
	inline igdeIcon *GetIcon() const{ return pIcon; }
	
	/** \brief Set icon or NULL. */
	void SetIcon( igdeIcon *icon );
	
	/** \brief Window can be resized by user. */
	inline bool GetCanResize() const{ return pCanResize; }
	
	/** \brief Window size. */
	inline const decPoint &GetSize() const{ return pSize; }
	
	/** \brief Set window size. */
	void SetSize( const decPoint &size );
	
	/** \brief Window is enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	/** \brief Set if window is enabled. */
	void SetEnabled( bool enabled );
	
	/** \brief Window position. */
	inline const decPoint &GetPosition() const{ return pPosition; }
	
	/** \brief Set window position. */
	void SetPosition( const decPoint &position );
	
	/** \brief Raise and activate window. */
	virtual void RaiseAndActivate();
	
	
	
	/**
	 * \brief Add child.
	 * 
	 * Windows can contain only one widget. This is usually a container.
	 */
	virtual void AddChild( igdeWidget *child );
	
	
	
	/**
	 * \brief Request to close window due to clicking on close button.
	 * 
	 * Returns \em true to close window or \em false to keep it open. Default implementation
	 * returns \em true.
	 */
	virtual bool CloseWindow();
	
	/** \brief Close window bypassing CloseWindow(). */
	virtual void Close();
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
	 * \brief Destroy native widget.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	virtual void DestroyNativeWidget();
	
	
	
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
	
	/** \brief Raise and activate window. */
	virtual void OnRaiseAndActivate();
	/*@}*/
};

#endif
