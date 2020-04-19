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

#ifndef _DEALWIDGETPROGRESSBAR_H_
#define _DEALWIDGETPROGRESSBAR_H_

#include "dealWidgetLayoutStack.h"
#include "../../common/string/decString.h"

class dealWidgetLabel;


/**
 * \brief Progress bar with custom integer range.
 */
class dealWidgetProgressBar : public dealWidgetLayoutStack{
private:
	dealWidget *pBoxProgress;
	dealWidgetLabel *pLabelProgress;
	
	int pMinProgress;
	int pMaxProgress;
	int pProgress;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create widget. */
	dealWidgetProgressBar( dealDisplay &display );
	
	/** \brief Create widget. */
	dealWidgetProgressBar( dealDisplay &display, int minProgress, int maxProgress );
	
	/** \brief Clean up widget. */
	virtual ~dealWidgetProgressBar();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Minimum progress range. */
	inline int GetMinProgress() const{ return pMinProgress; }
	
	/** \brief Maximum progress range. */
	inline int GetMaxProgress() const{ return pMaxProgress; }
	
	/** \brief Set progress range. */
	void SetRange( int minProgress, int maxProgress );
	
	/** \brief Progress. */
	inline int GetProgress() const{ return pProgress; }
	
	/** \brief Set progress. */
	void SetProgress( int progress );
	
	/** \brief Progress as percentage from 0 to 1. */
	float GetProgressPercentage() const;
	
	
	
	/** \brief Size changed. */
	virtual void OnSizeChanged();
	
	/** \brief Padding changed. */
	virtual void OnPaddingChanged();
	/*@}*/
	
	
	
private:
	void pBuildContent();
	void pUpdateContent();
};

#endif
