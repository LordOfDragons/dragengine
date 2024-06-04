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
