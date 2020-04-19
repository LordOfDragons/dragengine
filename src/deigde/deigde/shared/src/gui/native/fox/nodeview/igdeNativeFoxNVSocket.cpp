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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "igdeNativeFoxNVSocket.h"
#include "../../../theme/igdeGuiTheme.h"
#include "../../../theme/propertyNames.h"
#include "../../../../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



FXDEFMAP( igdeNativeFoxNVSocket ) cNativeIgdeNVSocketMap[] = {
	FXMAPFUNC( SEL_CONFIGURE, 0, igdeNativeFoxNVSocket::onResize ),
	FXMAPFUNC( SEL_PAINT, 0, igdeNativeFoxNVSocket::onPaint )
};


FXIMPLEMENT( igdeNativeFoxNVSocket, FXFrame,
	cNativeIgdeNVSocketMap, ARRAYNUMBER( cNativeIgdeNVSocketMap ) )


// class igdeNativeFoxNVSocket
///////////////////////////////

igdeNativeFoxNVSocket::igdeNativeFoxNVSocket(){
	this->flags |= FLAG_ENABLED | FLAG_DROPTARGET;
}

igdeNativeFoxNVSocket::igdeNativeFoxNVSocket( FXComposite *parent, FXObject *target,
	int selector, int flags, const igdeGuiTheme & ) :
FXFrame( parent, flags, 0, 0, 0, 0, 0, 0, 0, 0 ),
pChecked( false ),
pRadius( 6 ),
pColorFrame( FXRGB( 0, 0, 0 ) ),
pColorFillChecked( FXRGB( 0, 0, 255 ) ),
pColorFillUnchecked( parent->getApp()->getBackColor() ),
pWindowShape( NULL )
{
	this->flags |= FLAG_ENABLED | FLAG_DROPTARGET;
	this->target = target;
	message = selector;
	backColor = getApp()->getBackColor();
}

igdeNativeFoxNVSocket::~igdeNativeFoxNVSocket(){
	if( pWindowShape ){
		clearShape();
		delete pWindowShape;
	}
}



// Management
///////////////

FXint igdeNativeFoxNVSocket::getDefaultWidth(){
	return pRadius * 2;
}

FXint igdeNativeFoxNVSocket::getDefaultHeight(){
	return pRadius * 2;
}

FXbool igdeNativeFoxNVSocket::canFocus() const{
	return false;
}



void igdeNativeFoxNVSocket::SetChecked( bool checked ){
	if( checked == pChecked ){
		return;
	}
	
	pChecked = checked;
	update();
}

void igdeNativeFoxNVSocket::SetColorFrame( FXColor color ){
	if( color == pColorFrame ){
		return;
	}
	
	pColorFrame = color;
	update();
}

void igdeNativeFoxNVSocket::SetColorFillChecked( FXColor color ){
	if( color == pColorFillChecked ){
		return;
	}
	
	pColorFillChecked = color;
	update();
}

void igdeNativeFoxNVSocket::SetColorFillUnchecked( FXColor color ){
	if( color == pColorFillUnchecked ){
		return;
	}
	
	pColorFillUnchecked = color;
	update();
}



decPoint igdeNativeFoxNVSocket::GetCenter() const{
	return decPoint( getWidth() / 2, getHeight() / 2 );
}

decPoint igdeNativeFoxNVSocket::GetBorderLeft() const{
	return decPoint( getWidth() / 2 - pRadius, getHeight() / 2 );
}

decPoint igdeNativeFoxNVSocket::GetBorderRight() const{
	return decPoint( getWidth() / 2 + pRadius, getHeight() / 2 );
}

bool igdeNativeFoxNVSocket::IsInsideSocket( const decPoint &point ) const{
	return ( point - GetCenter() ).Length() < ( float )pRadius + 0.5f;
}



// Events
///////////

long igdeNativeFoxNVSocket::onResize( FXObject*, FXSelector, void* ){
	pUpdateWindowShape();
	return 1;
}

long igdeNativeFoxNVSocket::onPaint( FXObject*, FXSelector, void *data ){
	FXEvent * const event = ( FXEvent* )data;
	FXDCWindow dc( this, event );
	const decPoint center( getWidth() / 2, getHeight() / 2 );
	const decPoint position( center - decPoint( pRadius, pRadius ) );
	const decPoint size( pRadius * 2 - 1, pRadius * 2 - 1 );
	
	dc.setForeground( pColorFillUnchecked );
	dc.fillEllipse( position.x, position.y, size.x, size.y );
	
	if( pChecked ){
		dc.setForeground( pColorFillChecked );
		dc.fillEllipse( position.x + 2, position.y + 2, size.x - 4, size.y - 4 );
	}
	
	dc.setForeground( pColorFrame );
	dc.drawEllipse( position.x, position.y, size.x, size.y );
	return 1;
}



// Protected Functions
////////////////////////

void igdeNativeFoxNVSocket::pUpdateWindowShape(){
	// make sure the window shape exists and is created. should this fail for some reason we
	// simply ignore the shape which causes garbage to show around the node but it would not
	// prevent usage of the editor
	if( ! pWindowShape ){
		pWindowShape = new FXBitmap( getApp() );
		if( pWindowShape ){
			pWindowShape->create();
			setShape( pWindowShape );
		}
	}
	
	// if the window shape exists update it by drawing the node frame ontop of it
	if( ! pWindowShape ){
		return;
	}
	
	clearShape();
	
	const decPoint center( getWidth() / 2, getHeight() / 2 );
	const decPoint position( center - decPoint( pRadius, pRadius ) );
	const decPoint size( pRadius * 2 - 1, pRadius * 2 - 1 );
	
	pWindowShape->resize( getWidth(), getHeight() );
	
	{ // just to make sure the dc is killed before we attempt to render the bitmap
	FXDCWindow dc( pWindowShape );
	dc.setForeground( FX_COLOR_WINDOW_SHAPE_TRANSPARENT );
	dc.fillRectangle( 0, 0, getWidth(), getHeight() );
	
	dc.setForeground( FX_COLOR_WINDOW_SHAPE_OPAQUE );
	dc.fillEllipse( position.x, position.y, size.x, size.y );
	dc.drawEllipse( position.x, position.y, size.x, size.y );
	}
	pWindowShape->render();
	
	setShape( pWindowShape );
}
