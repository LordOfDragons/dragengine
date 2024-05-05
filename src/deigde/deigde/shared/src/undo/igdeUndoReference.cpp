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

#include "igdeUndo.h"
#include "igdeUndoReference.h"



// Class igdeUndoReference
////////////////////////////

// Constructor, destructor
////////////////////////////

igdeUndoReference::igdeUndoReference(){
}

igdeUndoReference::igdeUndoReference( igdeUndo *undo ) :
deObjectReference( undo ){
}

igdeUndoReference::igdeUndoReference( const igdeUndoReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeUndoReference::TakeOver( igdeUndo *undo ){
	deObjectReference::TakeOver( undo );
}

bool igdeUndoReference::operator!() const{
	return deObjectReference::operator!();
}

igdeUndoReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeUndoReference::operator igdeUndo *() const{
	return ( igdeUndo* )operator deObject *();
}

igdeUndoReference::operator igdeUndo &() const{
	return ( igdeUndo& )operator deObject &();
}

igdeUndo *igdeUndoReference::operator->() const{
	return ( igdeUndo* )deObjectReference::operator->();
}

igdeUndoReference &igdeUndoReference::operator=( igdeUndo *undo ){
	deObjectReference::operator=( undo );
	return *this;
}

igdeUndoReference &igdeUndoReference::operator=( const igdeUndoReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeUndoReference::operator==( igdeUndo *undo ) const{
	return deObjectReference::operator==( undo );
}

bool igdeUndoReference::operator!=( igdeUndo *undo ) const{
	return deObjectReference::operator!=( undo );
}

bool igdeUndoReference::operator==( const igdeUndoReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeUndoReference::operator!=( const igdeUndoReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
