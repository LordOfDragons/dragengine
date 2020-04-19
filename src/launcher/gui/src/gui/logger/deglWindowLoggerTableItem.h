/* 
 * Drag[en]gine GUI Launcher
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

#ifndef _DEGLWINDOWLOGGERTABLEITEM_H_
#define _DEGLWINDOWLOGGERTABLEITEM_H_

#include "../foxtoolkit.h"



/**
 * @brief Logger Window Table Item.
 * Special version of the FXTableItem providing support for individual cell
 * text color and text font.
 */
class deglWindowLoggerTableItem : public FXTableItem{
	FXDECLARE( deglWindowLoggerTableItem )
	
protected:
	deglWindowLoggerTableItem();
	
private:
	FXColor pTextColor;
	FXFont *pFont;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new item. */
	deglWindowLoggerTableItem( const FXString &text, FXIcon *ic = NULL, void *ptr = NULL );
	/** Cleans up the item. */
	virtual ~deglWindowLoggerTableItem();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the text color. */
	inline FXColor GetTextColor() const{ return pTextColor; }
	/** Sets the text color. */
	void SetTextColor( FXColor color );
	/** Retrieves the text font or NULL to use the table font. */
	inline FXFont *GetFont() const{ return pFont; }
	/** Sets the text font or NULL to use the table font. */
	void SetFont( FXFont *font );
	
	/*
	/ * Create input control for editing this item. * /
	virtual FXWindow *getControlFor( FXTable *table );
	
	/ * Set value from input control. * /
	virtual void setFromControl( FXWindow *control );
	*/
	
	/** Retrieves the width of the item. */
	virtual FXint getWidth( const FXTable *table ) const;
	/** Retrieves the height of the item. */
	virtual FXint getHeight( const FXTable *table ) const;
	/** Render background. */
	virtual void drawBackground( const FXTable *table, FXDC &dc, FXint x, FXint y, FXint w, FXint h ) const;
	/** Render text. */
	virtual void drawContent( const FXTable *table, FXDC &dc, FXint x, FXint y, FXint w, FXint h ) const;
	/*@}*/
};

// end of include only once
#endif
