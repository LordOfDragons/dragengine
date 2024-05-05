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

#include "igdeWidget.h"
#include "igdeWidgetReference.h"



// Class igdeWidgetReference
//////////////////////////////

// Constructor, destructor
////////////////////////////

igdeWidgetReference::igdeWidgetReference(){
}

igdeWidgetReference::igdeWidgetReference( igdeWidget *widget ) :
deObjectReference( widget ){
}

igdeWidgetReference::igdeWidgetReference( const igdeWidgetReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeWidgetReference::TakeOver( igdeWidget *widget ){
	deObjectReference::TakeOver( widget );
}

bool igdeWidgetReference::operator!() const{
	return deObjectReference::operator!();
}

igdeWidgetReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeWidgetReference::operator igdeWidget *() const{
	return ( igdeWidget* )operator deObject *();
}

igdeWidgetReference::operator igdeWidget &() const{
	return ( igdeWidget& )operator deObject &();
}

igdeWidget *igdeWidgetReference::operator->() const{
	return ( igdeWidget* )deObjectReference::operator->();
}

igdeWidgetReference &igdeWidgetReference::operator=( igdeWidget *widget ){
	deObjectReference::operator=( widget );
	return *this;
}

igdeWidgetReference &igdeWidgetReference::operator=( const igdeWidgetReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeWidgetReference::operator==( igdeWidget *widget ) const{
	return deObjectReference::operator==( widget );
}

bool igdeWidgetReference::operator!=( igdeWidget *widget ) const{
	return deObjectReference::operator!=( widget );
}

bool igdeWidgetReference::operator==( const igdeWidgetReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeWidgetReference::operator!=( const igdeWidgetReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
