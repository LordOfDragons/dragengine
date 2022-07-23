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

#include "deglDPLListItemParam.h"

#include <dragengine/common/exceptions.h>



// Definitions
////////////////

#define ICON_SPACING             4    // Spacing between icon and label
#define SIDE_SPACING             6    // Left or right spacing between items
#define LINE_SPACING             4    // Line spacing between items



// Class deglDPLListItemParam
//////////////////////////////

FXIMPLEMENT( deglDPLListItemParam, FXListItem, nullptr, 0 )

// Constructor, destructor
////////////////////////////

deglDPLListItemParam::deglDPLListItemParam(){
}

deglDPLListItemParam::deglDPLListItemParam( const FXString &text, const char *parameterName, FXFont *font ) :
FXListItem( text ),
pFont( font ),
pParameterName( parameterName )
{
	if( ! parameterName || ! font ){
		DETHROW( deeInvalidParam );
	}
}

deglDPLListItemParam::~deglDPLListItemParam(){
}



// Management
///////////////

void deglDPLListItemParam::SetFont( FXFont *font ){
	if( ! font ){
		DETHROW( deeInvalidParam );
	}
	pFont = font;
}

void deglDPLListItemParam::draw( const FXList *list, FXDC &dc, FXint xx, FXint yy, FXint w, FXint h ) const {
	FXint ih = 0;
	FXint th = 0;
	
	if( icon ){
		ih = icon->getHeight();
	}
	if( ! label.empty() ){
		th = pFont->getFontHeight();
	}
	
	if( isSelected() ){
		dc.setForeground( list->getSelBackColor() );
		
	}else{
		dc.setForeground( list->getBackColor() );
	}
	dc.fillRectangle( xx, yy, w, h );
	
	if( hasFocus() ){
		dc.drawFocusRectangle( xx + 1, yy + 1, w - 2, h - 2 );
	}
	xx += SIDE_SPACING / 2;
	
	if( icon ){
		dc.drawIcon( icon, xx, yy + ( h - ih ) / 2 );
		xx += ICON_SPACING + icon->getWidth();
	}
	
	if( ! label.empty() ){
		dc.setFont( pFont );
		
		if( ! isEnabled() ){
			dc.setForeground( makeShadowColor( list->getBackColor() ) );
			
		}else if( isSelected() ){
			dc.setForeground( list->getSelTextColor() );
			
		}else{
			dc.setForeground( list->getTextColor() );
		}
		
		dc.drawText( xx, yy + ( h - th ) / 2 + pFont->getFontAscent(), label );
	}
}
