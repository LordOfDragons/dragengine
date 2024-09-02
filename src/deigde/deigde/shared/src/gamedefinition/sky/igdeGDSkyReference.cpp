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

#include "igdeGDSky.h"
#include "igdeGDSkyReference.h"



// Class igdeGDSkyReference
/////////////////////////////

// Constructor, destructor
////////////////////////////

igdeGDSkyReference::igdeGDSkyReference(){
}

igdeGDSkyReference::igdeGDSkyReference( igdeGDSky *gdsky ) :
deObjectReference( gdsky ){
}

igdeGDSkyReference::igdeGDSkyReference( const igdeGDSkyReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeGDSkyReference::TakeOver( igdeGDSky *gdsky ){
	deObjectReference::TakeOver( gdsky );
}

bool igdeGDSkyReference::operator!() const{
	return deObjectReference::operator!();
}

igdeGDSkyReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeGDSkyReference::operator igdeGDSky *() const{
	return ( igdeGDSky* )operator deObject *();
}

igdeGDSkyReference::operator igdeGDSky &() const{
	return ( igdeGDSky& )operator deObject &();
}

igdeGDSky *igdeGDSkyReference::operator->() const{
	return ( igdeGDSky* )deObjectReference::operator->();
}

igdeGDSkyReference &igdeGDSkyReference::operator=( igdeGDSky *gdsky ){
	deObjectReference::operator=( gdsky );
	return *this;
}

igdeGDSkyReference &igdeGDSkyReference::operator=( const igdeGDSkyReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeGDSkyReference::operator==( igdeGDSky *gdsky ) const{
	return deObjectReference::operator==( gdsky );
}

bool igdeGDSkyReference::operator!=( igdeGDSky *gdsky ) const{
	return deObjectReference::operator!=( gdsky );
}

bool igdeGDSkyReference::operator==( const igdeGDSkyReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeGDSkyReference::operator!=( const igdeGDSkyReference &reference ) const{
	return deObjectReference::operator!=( reference );
}

