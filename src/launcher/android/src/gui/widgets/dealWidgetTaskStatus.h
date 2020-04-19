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

#ifndef _DEALWIDGETTASKSTATUS_H_
#define _DEALWIDGETTASKSTATUS_H_

#include "dealWidgetLayoutStack.h"
#include "../../common/string/decString.h"

class dealWidgetLabel;
class dealWidgetProgressBar;


/**
 * \brief Display status of a task for example installing something.
 */
class dealWidgetTaskStatus : public dealWidgetLayoutStack{
private:
	dealWidgetLabel *pLabelName;
	dealWidgetLabel *pLabelStatus;
	dealWidgetProgressBar *pProgressBar;
	dealWidget *pBgPending;
	dealWidget *pBgFinished;
	
	decString pName;
	decString pStatus;
	
	bool pFinished;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create widget. */
	dealWidgetTaskStatus( dealDisplay &display );
	
	/** \brief Clean up widget. */
	virtual ~dealWidgetTaskStatus();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Task name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set task name. */
	void SetName( const char *name );
	
	/** \brief Task status text. */
	inline const decString &GetStatus() const{ return pStatus; }
	
	/** \brief Set task status text. */
	void SetStatus( const char *text );
	
	/** \brief Task progress in the range from 0 to progress range. */
	float GetProgress() const;
	
	/** \brief Set task progress range. */
	void SetProgressRange( int range );
	
	/** \brief Set task progress in the range from 0 to progress range - 1. */
	void SetProgress( int progress );
	
	
	
	/** \brief Finished. */
	inline bool GetFinished() const{ return pFinished; }
	
	/** \brief Set finished. */
	void SetFinished( bool finished );
	/*@}*/
	
	
	
private:
	void pBuildContent();
};

#endif
