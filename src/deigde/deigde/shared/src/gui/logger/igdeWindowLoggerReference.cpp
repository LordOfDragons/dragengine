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

#include "igdeWindowLogger.h"
#include "igdeWindowLoggerReference.h"



// Class igdeWindowLoggerReference
////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeWindowLoggerReference::igdeWindowLoggerReference(){
}

igdeWindowLoggerReference::igdeWindowLoggerReference( igdeWindowLogger *window ) :
deObjectReference( window ){
}

igdeWindowLoggerReference::igdeWindowLoggerReference( const igdeWindowLoggerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeWindowLoggerReference::TakeOver( igdeWindowLogger *window ){
	deObjectReference::TakeOver( window );
}

bool igdeWindowLoggerReference::operator!() const{
	return deObjectReference::operator!();
}

igdeWindowLoggerReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeWindowLoggerReference::operator igdeWindowLogger *() const{
	return ( igdeWindowLogger* )operator deObject *();
}

igdeWindowLoggerReference::operator igdeWindowLogger &() const{
	return ( igdeWindowLogger& )operator deObject &();
}

igdeWindowLogger *igdeWindowLoggerReference::operator->() const{
	return ( igdeWindowLogger* )deObjectReference::operator->();
}

igdeWindowLoggerReference &igdeWindowLoggerReference::operator=( igdeWindowLogger *window ){
	deObjectReference::operator=( window );
	return *this;
}

igdeWindowLoggerReference &igdeWindowLoggerReference::operator=( const igdeWindowLoggerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeWindowLoggerReference::operator==( igdeWindowLogger *window ) const{
	return deObjectReference::operator==( window );
}

bool igdeWindowLoggerReference::operator!=( igdeWindowLogger *window ) const{
	return deObjectReference::operator!=( window );
}

bool igdeWindowLoggerReference::operator==( const igdeWindowLoggerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeWindowLoggerReference::operator!=( const igdeWindowLoggerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
