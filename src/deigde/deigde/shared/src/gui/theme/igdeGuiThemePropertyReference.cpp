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

#include "igdeGuiThemeProperty.h"
#include "igdeGuiThemePropertyReference.h"



// Class igdeGuiThemePropertyReference
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeGuiThemePropertyReference::igdeGuiThemePropertyReference(){
}

igdeGuiThemePropertyReference::igdeGuiThemePropertyReference( igdeGuiThemeProperty *property ) :
deObjectReference( property ){
}

igdeGuiThemePropertyReference::igdeGuiThemePropertyReference( const igdeGuiThemePropertyReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeGuiThemePropertyReference::TakeOver( igdeGuiThemeProperty *property ){
	deObjectReference::TakeOver( property );
}

bool igdeGuiThemePropertyReference::operator!() const{
	return deObjectReference::operator!();
}

igdeGuiThemePropertyReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeGuiThemePropertyReference::operator igdeGuiThemeProperty *() const{
	return ( igdeGuiThemeProperty* )operator deObject *();
}

igdeGuiThemePropertyReference::operator igdeGuiThemeProperty &() const{
	return ( igdeGuiThemeProperty& )operator deObject &();
}

igdeGuiThemeProperty *igdeGuiThemePropertyReference::operator->() const{
	return ( igdeGuiThemeProperty* )deObjectReference::operator->();
}

igdeGuiThemePropertyReference &igdeGuiThemePropertyReference::operator=( igdeGuiThemeProperty *property ){
	deObjectReference::operator=( property );
	return *this;
}

igdeGuiThemePropertyReference &igdeGuiThemePropertyReference::operator=( const igdeGuiThemePropertyReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeGuiThemePropertyReference::operator==( igdeGuiThemeProperty *property ) const{
	return deObjectReference::operator==( property );
}

bool igdeGuiThemePropertyReference::operator!=( igdeGuiThemeProperty *property ) const{
	return deObjectReference::operator!=( property );
}

bool igdeGuiThemePropertyReference::operator==( const igdeGuiThemePropertyReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeGuiThemePropertyReference::operator!=( const igdeGuiThemePropertyReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
