/* 
 * Drag[en]gine Android Launcher
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

#ifndef _DEALDIALOG_H_
#define _DEALDIALOG_H_

#include "../deObject.h"
#include "../common/string/decString.h"

class dealDisplay;
class dealWidgetLayoutFlow;


/**
 * \brief Dialog displaying a layout widget in a display.
 * 
 * Only one dialog can be the root dialog covering the entire display with one
 * additional popup dialog not filling the entire view. 
 * 
 */
class dealDialog : public deObject{
private:
	dealDisplay &pDisplay;
	
	decString pTitle;
	dealWidgetLayoutFlow *pContent;
	dealDialog *pPopupDialog;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create dialog. */
	dealDialog( dealDisplay &display, const char *title );
	
	/** \brief Clean up dialog. */
	virtual ~dealDialog();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Display. */
	inline dealDisplay &GetDisplay() const{ return pDisplay; }
	
	
	
	/** \brief Title. */
	inline const decString &GetTitle() const{ return pTitle; }
	
	/** \brief Set title. */
	void SetTitle( const char *title );
	
	/** \brief Content layout. */
	inline dealWidgetLayoutFlow *GetContent() const{ return pContent; }
	
	/** \brief Popup dialog or \em NULL if not set. */
	inline dealDialog *GetPopupDialog() const{ return pPopupDialog; }
	
	/** \brief Pop up or down dialog replacing the previous one if set. */
	void SetPopupDialog( dealDialog *dialog );
	
	
	
	/** \brief Dialog has been activated. */
	virtual void OnActivate();
	
	/** \brief Dialog has been deactivated. */
	virtual void OnDeactivate();
	
	/** \brief Frame update. */
	virtual void OnFrameUpdate();
	
	/**
	 * \brief Dialog to display on back key pressed or \em NULL to close application.
	 * \details Default implementation returns \em NULL.
	 */
	virtual dealDialog *BackKeyDialog();
	/*@}*/
};

#endif
