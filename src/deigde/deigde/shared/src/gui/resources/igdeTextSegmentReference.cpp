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

#include "igdeTextSegment.h"
#include "igdeTextSegmentReference.h"



// Class igdeTextSegmentReference
///////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeTextSegmentReference::igdeTextSegmentReference(){
}

igdeTextSegmentReference::igdeTextSegmentReference( igdeTextSegment *textSegment ) :
deObjectReference( textSegment ){
}

igdeTextSegmentReference::igdeTextSegmentReference( const igdeTextSegmentReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeTextSegmentReference::TakeOver( igdeTextSegment *textSegment ){
	deObjectReference::TakeOver( textSegment );
}

bool igdeTextSegmentReference::operator!() const{
	return deObjectReference::operator!();
}

igdeTextSegmentReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeTextSegmentReference::operator igdeTextSegment *() const{
	return ( igdeTextSegment* )operator deObject *();
}

igdeTextSegmentReference::operator igdeTextSegment &() const{
	return ( igdeTextSegment& )operator deObject &();
}

igdeTextSegment *igdeTextSegmentReference::operator->() const{
	return ( igdeTextSegment* )deObjectReference::operator->();
}

igdeTextSegmentReference &igdeTextSegmentReference::operator=( igdeTextSegment *textSegment ){
	deObjectReference::operator=( textSegment );
	return *this;
}

igdeTextSegmentReference &igdeTextSegmentReference::operator=( const igdeTextSegmentReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeTextSegmentReference::operator==( igdeTextSegment *textSegment ) const{
	return deObjectReference::operator==( textSegment );
}

bool igdeTextSegmentReference::operator!=( igdeTextSegment *textSegment ) const{
	return deObjectReference::operator!=( textSegment );
}

bool igdeTextSegmentReference::operator==( const igdeTextSegmentReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeTextSegmentReference::operator!=( const igdeTextSegmentReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
