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

#ifndef _DEALWIDGETBORDERIMAGE_H_
#define _DEALWIDGETBORDERIMAGE_H_

#include "dealWidgetImage.h"


/**
 * \brief 9-part border image widget.
 */
class dealWidgetBorderImage : public dealWidgetImage{
private:
	int pBorderLeft;
	int pBorderTop;
	int pBorderRight;
	int pBorderBottom;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create widget. */
	dealWidgetBorderImage( dealDisplay &display, dealImage *image = NULL, int borderLeft = 0,
		int borderTop = 0, int borderRight = 0, int borderBottom = 0 );
	
	/** \brief Clean up widget. */
	virtual ~dealWidgetBorderImage();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Left border size. */
	inline int getBorderLeft() const{ return pBorderLeft; }
	
	/** \brief Set left border size. */
	void SetBorderLeft( int size );
	
	/** \brief Top border size. */
	inline int getBorderTop() const{ return pBorderTop; }
	
	/** \brief Set top border size. */
	void SetBorderTop( int size );
	
	/** \brief Right border size. */
	inline int getBorderRight() const{ return pBorderRight; }
	
	/** \brief Set right border size. */
	void SetBorderRight( int size );
	
	/** \brief Bottom border size. */
	inline int getBorderBottom() const{ return pBorderBottom; }
	
	/** \brief Set bottom border size. */
	void SetBorderBottom( int size );
	
	/** \brief Set borders. */
	void SetBorders( int left, int top, int right, int bottom );
	
	
	
	/** \brief Render content. */
	virtual void RenderContent( const sRenderContext &context );
	/*@}*/
};

#endif
