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

#include "igdeActionContextMenu.h"
#include "igdeActionContextMenuReference.h"



// Class igdeActionContextMenuReference
/////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeActionContextMenuReference::igdeActionContextMenuReference(){
}

igdeActionContextMenuReference::igdeActionContextMenuReference( igdeActionContextMenu *action ) :
deObjectReference( action ){
}

igdeActionContextMenuReference::igdeActionContextMenuReference( const igdeActionContextMenuReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeActionContextMenuReference::TakeOver( igdeActionContextMenu *action ){
	deObjectReference::TakeOver( action );
}

bool igdeActionContextMenuReference::operator!() const{
	return deObjectReference::operator!();
}

igdeActionContextMenuReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeActionContextMenuReference::operator igdeActionContextMenu *() const{
	return ( igdeActionContextMenu* )operator deObject *();
}

igdeActionContextMenuReference::operator igdeActionContextMenu &() const{
	return ( igdeActionContextMenu& )operator deObject &();
}

igdeActionContextMenu *igdeActionContextMenuReference::operator->() const{
	return ( igdeActionContextMenu* )deObjectReference::operator->();
}

igdeActionContextMenuReference &igdeActionContextMenuReference::operator=( igdeActionContextMenu *action ){
	deObjectReference::operator=( action );
	return *this;
}

igdeActionContextMenuReference &igdeActionContextMenuReference::operator=( const igdeActionContextMenuReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeActionContextMenuReference::operator==( igdeActionContextMenu *action ) const{
	return deObjectReference::operator==( action );
}

bool igdeActionContextMenuReference::operator!=( igdeActionContextMenu *action ) const{
	return deObjectReference::operator!=( action );
}

bool igdeActionContextMenuReference::operator==( const igdeActionContextMenuReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeActionContextMenuReference::operator!=( const igdeActionContextMenuReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
