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

#include "deLogger.h"
#include "deLoggerReference.h"



// Class deLoggerReference
////////////////////////////

// Constructor, destructor
////////////////////////////

deLoggerReference::deLoggerReference(){
}

deLoggerReference::deLoggerReference( deLogger *object ) :
deObjectReference( object ){
}

deLoggerReference::deLoggerReference( const deLoggerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deLoggerReference::TakeOver( deLogger *object ){
	deObjectReference::TakeOver( object );
}

bool deLoggerReference::operator!() const{
	return deObjectReference::operator!();
}

deLoggerReference::operator bool() const{
	return deObjectReference::operator bool();
}

deLoggerReference::operator deLogger *() const{
	return ( deLogger* )operator deObject *();
}

deLoggerReference::operator deLogger &() const{
	return ( deLogger& )operator deObject &();
}

deLogger *deLoggerReference::operator->() const{
	return ( deLogger* )deObjectReference::operator->();
}

deLoggerReference &deLoggerReference::operator=( deLogger *object ){
	deObjectReference::operator=( object );
	return *this;
}

deLoggerReference &deLoggerReference::operator=( const deLoggerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deLoggerReference::operator==( deLogger *object ) const{
	return deObjectReference::operator==( object );
}

bool deLoggerReference::operator!=( deLogger *object ) const{
	return deObjectReference::operator!=( object );
}

bool deLoggerReference::operator==( const deLoggerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deLoggerReference::operator!=( const deLoggerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
