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

#ifndef _DEGLWINDOWLOGGERTABLE_H_
#define _DEGLWINDOWLOGGERTABLE_H_

#include "../foxtoolkit.h"



/**
 * Window Logger Table.
 * Special version of the FXTable providing support for individual cell
 * forground color.
 */
class deglWindowLoggerTable : public FXTable{
	FXDECLARE( deglWindowLoggerTable )
	
protected:
	deglWindowLoggerTable();
	
private:
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new table. */
	deglWindowLoggerTable( FXComposite *p, FXObject *tgt = nullptr, FXSelector sel = 0, FXuint opts = 0, FXint x = 0,
	FXint y = 0, FXint w = 0, FXint h = 0, FXint pl = DEFAULT_MARGIN, FXint pr = DEFAULT_MARGIN,
	FXint pt = DEFAULT_MARGIN, FXint pb = DEFAULT_MARGIN );
	/** Cleans up the table. */
	virtual ~deglWindowLoggerTable();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Set cell text color and font. */
	void SetCellTextProps( int row, int col, FXColor color, FXFont *font );
	
	/** Create item. */
	virtual FXTableItem *createItem( const FXString &text, FXIcon *icon, void *ptr );
	/*@}*/
};

// end of include only once
#endif
