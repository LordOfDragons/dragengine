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

#ifndef _DEALWIDGETLAYOUTFLOW_H_
#define _DEALWIDGETLAYOUTFLOW_H_

#include "dealWidgetLayout.h"



/**
 * \brief Flow widget layout.
 * 
 * Supports flow and box layouts along X and Y axis.
 * 
 */
class dealWidgetLayoutFlow : public dealWidgetLayout{
public:
	/** \brief Layout type. */
	enum eLayoutTypes{
		/** \brief Scale widgets relative to each other to fit into content area. */
		eltFill,
		
		/** \brief Widgets are resized to their minimum size and aligned left/top. */
		eltTop,
			
		/** \brief Widgets are resized to their minimum size and aligned left/top with last widget stretched to fill. */
		eltTopFill,
	
		/** \brief Widgets are resized to their minimum size and center aligned. */
		eltCenter,
	
		/** \brief Widgets are resized to their minimum size and center aligned and first and last widget stretched to fill. */
		eltCenterFill,
		
		/** \brief Widgets are resized to their minimum size and aligned right/bottom. */
		eltBottom,
		
		/** \brief Widgets are resized to their minimum size and aligned right/bottom and first widget stretched to fill. */
		eltBottomFill
	};
	
private:
	bool pHorizontal;
	int pGap;
	eLayoutTypes pLayoutX;
	eLayoutTypes pLayoutY;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create widget. */
	dealWidgetLayoutFlow( dealDisplay &display );
	
	/** \brief Create widget. */
	dealWidgetLayoutFlow( dealDisplay &display, bool horizontal, int gap,
		eLayoutTypes layoutX, eLayoutTypes layoutY );
	
	/** \brief Clean up widget. */
	virtual ~dealWidgetLayoutFlow();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Layout horizontal instead of vertical. */
	inline bool GetHorizontal() const{ return pHorizontal; }
	
	/** \brief Set if widgets are laid out horizontal instead of vertical. */
	void SetHorizontal( bool horizontal );
	
	/** \brief Gap in pixels between widgets. */
	inline int GetGap() const{ return pGap; }
	
	/** \brief Set gap in pxiels between widgets. */
	void SetGap( int gap );
	
	/** \brief Layout type in X direction. */
	inline eLayoutTypes GetLayoutX() const{ return pLayoutX; }
	
	/** \brief Set layout type in X direction. */
	void SetLayoutX( eLayoutTypes type );
	
	/** \brief Layout type in Y direction. */
	inline eLayoutTypes GetLayoutY() const{ return pLayoutY; }
	
	/** \brief Set layout type in Y direction. */
	void SetLayoutY( eLayoutTypes type );
	
	
	
	/** \brief Minimum size of widget. */
	virtual decPoint GetMinimumSize();
	
	/** \brief Layout child widgets. */
	virtual void LayoutWidgets();
	
	
	
	/** \brief Real minimum size of widget. */
	decPoint GetRealMinimumSize();
	/*@}*/
};

#endif
