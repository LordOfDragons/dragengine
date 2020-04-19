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

#ifndef _DEALDIALOGMAIN_H_
#define _DEALDIALOGMAIN_H_

#include "../dealDialog.h"


class dealWidgetTaskStatus;
class dealWidgetButton;


/**
 * \brief Main dialog.
 * 
 */
class dealDialogMain : public dealDialog{
private:
	dealWidgetTaskStatus *pTaskCheckEngine;
	dealWidgetButton *pButtonRun;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create dialog. */
	dealDialogMain( dealDisplay &display );
	
	/** \brief Clean up dialog. */
	virtual ~dealDialogMain();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Check engine task. */
	inline dealWidgetTaskStatus &GetTaskCheckEngine(){ return *pTaskCheckEngine; }
	
	
	
	/** \brief Dialog has been activated. */
	virtual void OnActivate();
	
	/** \brief Dialog has been deactivated. */
	virtual void OnDeactivate();
	
	
	
	/** \brief Set if run button is enabled. */
	void SetRunEnabled( bool enabled );
	/*@}*/
	
	
	
private:
	void pBuildContent();
};

#endif
