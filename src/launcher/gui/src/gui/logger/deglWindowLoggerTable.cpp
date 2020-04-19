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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "deglWindowLoggerTable.h"
#include "deglWindowLoggerTableItem.h"



// Class deglWindowLoggerTable
////////////////////////////////

FXIMPLEMENT( deglWindowLoggerTable, FXTable, NULL, 0 )

// Constructor, destructor
////////////////////////////

deglWindowLoggerTable::deglWindowLoggerTable(){
}

deglWindowLoggerTable::deglWindowLoggerTable( FXComposite *p, FXObject *tgt, FXSelector sel, FXuint opts, FXint x, FXint y, FXint w, FXint h,
FXint pl, FXint pr, FXint pt, FXint pb ) : FXTable( p, tgt, sel, opts, x, y, w, h, pl, pr, pt, pb ){
}

deglWindowLoggerTable::~deglWindowLoggerTable(){
}



// Management
///////////////

void deglWindowLoggerTable::SetCellTextProps( int row, int col, FXColor color, FXFont *font ){
	deglWindowLoggerTableItem &item = *( ( deglWindowLoggerTableItem* )getItem( row, col ) );
	
	item.SetTextColor( color );
	item.SetFont( font );
	
	updateItem( row, col );
}

FXTableItem *deglWindowLoggerTable::createItem( const FXString &text, FXIcon *icon, void *ptr ){
	return new deglWindowLoggerTableItem( text, icon, ptr );
}
