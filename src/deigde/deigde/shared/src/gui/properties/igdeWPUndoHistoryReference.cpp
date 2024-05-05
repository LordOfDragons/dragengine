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

#include "igdeWPUndoHistory.h"
#include "igdeWPUndoHistoryReference.h"



// Class igdeWPUndoHistoryReference
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeWPUndoHistoryReference::igdeWPUndoHistoryReference(){
}

igdeWPUndoHistoryReference::igdeWPUndoHistoryReference( igdeWPUndoHistory *widget ) :
deObjectReference( widget ){
}

igdeWPUndoHistoryReference::igdeWPUndoHistoryReference( const igdeWPUndoHistoryReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeWPUndoHistoryReference::TakeOver( igdeWPUndoHistory *widget ){
	deObjectReference::TakeOver( widget );
}

bool igdeWPUndoHistoryReference::operator!() const{
	return deObjectReference::operator!();
}

igdeWPUndoHistoryReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeWPUndoHistoryReference::operator igdeWPUndoHistory *() const{
	return ( igdeWPUndoHistory* )operator deObject *();
}

igdeWPUndoHistoryReference::operator igdeWPUndoHistory &() const{
	return ( igdeWPUndoHistory& )operator deObject &();
}

igdeWPUndoHistory *igdeWPUndoHistoryReference::operator->() const{
	return ( igdeWPUndoHistory* )deObjectReference::operator->();
}

igdeWPUndoHistoryReference &igdeWPUndoHistoryReference::operator=( igdeWPUndoHistory *widget ){
	deObjectReference::operator=( widget );
	return *this;
}

igdeWPUndoHistoryReference &igdeWPUndoHistoryReference::operator=( const igdeWPUndoHistoryReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeWPUndoHistoryReference::operator==( igdeWPUndoHistory *widget ) const{
	return deObjectReference::operator==( widget );
}

bool igdeWPUndoHistoryReference::operator!=( igdeWPUndoHistory *widget ) const{
	return deObjectReference::operator!=( widget );
}

bool igdeWPUndoHistoryReference::operator==( const igdeWPUndoHistoryReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeWPUndoHistoryReference::operator!=( const igdeWPUndoHistoryReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
