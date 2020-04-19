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

#ifndef _IGDENATIVEFOXNVSOCKET_H_
#define _IGDENATIVEFOXNVSOCKET_H_

#include "../foxtoolkit.h"


class igdeGuiTheme;


/**
 * \brief FOX toolkit Native NodeView Socket.
 */
class igdeNativeFoxNVSocket : public FXFrame{
	FXDECLARE( igdeNativeFoxNVSocket )
protected:
	   igdeNativeFoxNVSocket();
	
private:
	bool pChecked;
	int pRadius;
	FXColor pColorFrame;
	FXColor pColorFillChecked;
	FXColor pColorFillUnchecked;
	FXBitmap *pWindowShape;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeFoxNVSocket( FXComposite *parent, FXObject *target, int selector,
		int flags, const igdeGuiTheme &guitheme );
	
	/** \brief Clean up native widget. */
	virtual ~igdeNativeFoxNVSocket();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	virtual FXint getDefaultWidth();
	virtual FXint getDefaultHeight();
	virtual FXbool canFocus() const;
	
	
	/** \brief Checked. */
	inline bool GetChecked() const{ return pChecked; }
	
	/** \brief Set checked. */
	void SetChecked( bool checked );
	
	/** \brief Frame color. */
	inline FXColor GetColorFrame() const{ return pColorFrame; }
	
	/** \brief Set frame color. */
	void SetColorFrame( FXColor color );
	
	/** \brief Checked fill color. */
	inline FXColor GetColorFillChecked() const{ return pColorFillChecked; }
	
	/** \brief Set checked fill color. */
	void SetColorFillChecked( FXColor color );
	
	/** \brief Unchecked fill color. */
	inline FXColor GetColorFillUnchecked() const{ return pColorFillUnchecked; }
	
	/** \brief Set unchecked fill color. */
	void SetColorFillUnchecked( FXColor color );
	
	
	
	/** \brief Center position. */
	decPoint GetCenter() const;
	
	/** \brief Left border position. */
	decPoint GetBorderLeft() const;
	
	/** \brief Right border position. */
	decPoint GetBorderRight() const;
	
	/** \brief Is point inside socket. */
	bool IsInsideSocket( const decPoint &point ) const;
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	long onResize( FXObject *sender, FXSelector selector, void *data );
	long onPaint( FXObject *sender, FXSelector selector, void *data );
	/*@}*/
	
	
	
protected:
	void pUpdateWindowShape();
};

#endif
