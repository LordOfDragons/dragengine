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

#ifndef _DEALWIDGETLAYOUTSTACK_H_
#define _DEALWIDGETLAYOUTSTACK_H_

#include "dealWidgetLayout.h"



/**
 * \brief Stack widget layout.
 * 
 * Overlay widgets ontop of each other either at their minimal size or content area size.
 * 
 */
class dealWidgetLayoutStack : public dealWidgetLayout{
private:
	bool pFullSize;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create widget. */
	dealWidgetLayoutStack( dealDisplay &display );
	
	/** \brief Create widget. */
	dealWidgetLayoutStack( dealDisplay &display, bool fullSize );
	
	/** \brief Clean up widget. */
	virtual ~dealWidgetLayoutStack();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Resize widgets to content area size. */
	inline bool GetFullSize() const{ return pFullSize; }
	
	/** \brief Set if widgets are resized to content area size. */
	void SetFullSize( bool fullSize );
	
	
	
	/** \brief Minimum size of widget. */
	virtual decPoint GetMinimumSize();
	
	/** \brief Layout child widgets. */
	virtual void LayoutWidgets();
	
	
	
	/** \brief Real minimum size of widget. */
	decPoint GetRealMinimumSize();
	/*@}*/
};

#endif
