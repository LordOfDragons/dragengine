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

#include "igdeWPSky.h"
#include "igdeWPSkyReference.h"



// Class igdeWPSkyReference
/////////////////////////////

// Constructor, destructor
////////////////////////////

igdeWPSkyReference::igdeWPSkyReference(){
}

igdeWPSkyReference::igdeWPSkyReference( igdeWPSky *widget ) :
deObjectReference( widget ){
}

igdeWPSkyReference::igdeWPSkyReference( const igdeWPSkyReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeWPSkyReference::TakeOver( igdeWPSky *widget ){
	deObjectReference::TakeOver( widget );
}

bool igdeWPSkyReference::operator!() const{
	return deObjectReference::operator!();
}

igdeWPSkyReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeWPSkyReference::operator igdeWPSky *() const{
	return ( igdeWPSky* )operator deObject *();
}

igdeWPSkyReference::operator igdeWPSky &() const{
	return ( igdeWPSky& )operator deObject &();
}

igdeWPSky *igdeWPSkyReference::operator->() const{
	return ( igdeWPSky* )deObjectReference::operator->();
}

igdeWPSkyReference &igdeWPSkyReference::operator=( igdeWPSky *widget ){
	deObjectReference::operator=( widget );
	return *this;
}

igdeWPSkyReference &igdeWPSkyReference::operator=( const igdeWPSkyReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeWPSkyReference::operator==( igdeWPSky *widget ) const{
	return deObjectReference::operator==( widget );
}

bool igdeWPSkyReference::operator!=( igdeWPSky *widget ) const{
	return deObjectReference::operator!=( widget );
}

bool igdeWPSkyReference::operator==( const igdeWPSkyReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeWPSkyReference::operator!=( const igdeWPSkyReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
