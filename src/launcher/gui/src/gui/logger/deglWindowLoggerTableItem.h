/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _DEGLWINDOWLOGGERTABLEITEM_H_
#define _DEGLWINDOWLOGGERTABLEITEM_H_

#include "../foxtoolkit.h"



/**
 * Logger Window Table Item.
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
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new item. */
	deglWindowLoggerTableItem( const FXString &text, FXIcon *ic = nullptr, void *ptr = nullptr );
	/** Cleans up the item. */
	virtual ~deglWindowLoggerTableItem();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the text color. */
	inline FXColor GetTextColor() const{ return pTextColor; }
	/** Sets the text color. */
	void SetTextColor( FXColor color );
	/** Retrieves the text font or nullptr to use the table font. */
	inline FXFont *GetFont() const{ return pFont; }
	/** Sets the text font or nullptr to use the table font. */
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
