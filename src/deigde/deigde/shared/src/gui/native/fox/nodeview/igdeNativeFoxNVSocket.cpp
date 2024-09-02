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

#ifdef IGDE_TOOLKIT_FOX

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

igdeNativeFoxNVSocket::igdeNativeFoxNVSocket( FXComposite *pparent, FXObject *ttarget,
	int selector, int fflags, const igdeGuiTheme & ) :
FXFrame( pparent, fflags, 0, 0, 0, 0, 0, 0, 0, 0 ),
pChecked( false ),
pRadius( 6 ),
pColorFrame( FXRGB( 0, 0, 0 ) ),
pColorFillChecked( FXRGB( 0, 0, 255 ) ),
pColorFillUnchecked( pparent->getApp()->getBackColor() ),
pWindowShape( NULL )
{
	this->flags |= FLAG_ENABLED | FLAG_DROPTARGET;
	this->target = ttarget;
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

long igdeNativeFoxNVSocket::onPaint( FXObject*, FXSelector, void *pdata ){
	FXEvent * const event = ( FXEvent* )pdata;
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

#endif
