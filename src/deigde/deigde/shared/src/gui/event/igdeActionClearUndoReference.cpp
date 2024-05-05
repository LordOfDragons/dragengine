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

#include "igdeActionClearUndo.h"
#include "igdeActionClearUndoReference.h"



// Class igdeActionClearUndoReference
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeActionClearUndoReference::igdeActionClearUndoReference(){
}

igdeActionClearUndoReference::igdeActionClearUndoReference( igdeActionClearUndo *action ) :
deObjectReference( action ){
}

igdeActionClearUndoReference::igdeActionClearUndoReference( const igdeActionClearUndoReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeActionClearUndoReference::TakeOver( igdeActionClearUndo *action ){
	deObjectReference::TakeOver( action );
}

bool igdeActionClearUndoReference::operator!() const{
	return deObjectReference::operator!();
}

igdeActionClearUndoReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeActionClearUndoReference::operator igdeActionClearUndo *() const{
	return ( igdeActionClearUndo* )operator deObject *();
}

igdeActionClearUndoReference::operator igdeActionClearUndo &() const{
	return ( igdeActionClearUndo& )operator deObject &();
}

igdeActionClearUndo *igdeActionClearUndoReference::operator->() const{
	return ( igdeActionClearUndo* )deObjectReference::operator->();
}

igdeActionClearUndoReference &igdeActionClearUndoReference::operator=( igdeActionClearUndo *action ){
	deObjectReference::operator=( action );
	return *this;
}

igdeActionClearUndoReference &igdeActionClearUndoReference::operator=( const igdeActionClearUndoReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeActionClearUndoReference::operator==( igdeActionClearUndo *action ) const{
	return deObjectReference::operator==( action );
}

bool igdeActionClearUndoReference::operator!=( igdeActionClearUndo *action ) const{
	return deObjectReference::operator!=( action );
}

bool igdeActionClearUndoReference::operator==( const igdeActionClearUndoReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeActionClearUndoReference::operator!=( const igdeActionClearUndoReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
