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
