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

#include "igdeGuiTheme.h"
#include "igdeGuiThemeReference.h"



// Class igdeGuiThemeReference
////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeGuiThemeReference::igdeGuiThemeReference(){
}

igdeGuiThemeReference::igdeGuiThemeReference( igdeGuiTheme *guitheme ) :
deObjectReference( guitheme ){
}

igdeGuiThemeReference::igdeGuiThemeReference( const igdeGuiThemeReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeGuiThemeReference::TakeOver( igdeGuiTheme *guitheme ){
	deObjectReference::TakeOver( guitheme );
}

bool igdeGuiThemeReference::operator!() const{
	return deObjectReference::operator!();
}

igdeGuiThemeReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeGuiThemeReference::operator igdeGuiTheme *() const{
	return ( igdeGuiTheme* )operator deObject *();
}

igdeGuiThemeReference::operator igdeGuiTheme &() const{
	return ( igdeGuiTheme& )operator deObject &();
}

igdeGuiTheme *igdeGuiThemeReference::operator->() const{
	return ( igdeGuiTheme* )deObjectReference::operator->();
}

igdeGuiThemeReference &igdeGuiThemeReference::operator=( igdeGuiTheme *guitheme ){
	deObjectReference::operator=( guitheme );
	return *this;
}

igdeGuiThemeReference &igdeGuiThemeReference::operator=( const igdeGuiThemeReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeGuiThemeReference::operator==( igdeGuiTheme *guitheme ) const{
	return deObjectReference::operator==( guitheme );
}

bool igdeGuiThemeReference::operator!=( igdeGuiTheme *guitheme ) const{
	return deObjectReference::operator!=( guitheme );
}

bool igdeGuiThemeReference::operator==( const igdeGuiThemeReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeGuiThemeReference::operator!=( const igdeGuiThemeReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
