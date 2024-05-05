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

#include "deLanguagePack.h"
#include "deLanguagePackReference.h"



// Class deLanguagePackReference
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deLanguagePackReference::deLanguagePackReference(){
}

deLanguagePackReference::deLanguagePackReference( deLanguagePack *object ) :
deObjectReference( object ){
}

deLanguagePackReference::deLanguagePackReference( const deLanguagePackReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deLanguagePackReference::TakeOver( deLanguagePack *object ){
	deObjectReference::TakeOver( object );
}

bool deLanguagePackReference::operator!() const{
	return deObjectReference::operator!();
}

deLanguagePackReference::operator bool() const{
	return deObjectReference::operator bool();
}

deLanguagePackReference::operator deLanguagePack *() const{
	return ( deLanguagePack* )operator deObject *();
}

deLanguagePackReference::operator deLanguagePack &() const{
	return ( deLanguagePack& )operator deObject &();
}

deLanguagePack *deLanguagePackReference::operator->() const{
	return ( deLanguagePack* )deObjectReference::operator->();
}

deLanguagePackReference &deLanguagePackReference::operator=( deLanguagePack *object ){
	deObjectReference::operator=( object );
	return *this;
}

deLanguagePackReference &deLanguagePackReference::operator=( const deLanguagePackReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deLanguagePackReference::operator==( deLanguagePack *object ) const{
	return deObjectReference::operator==( object );
}

bool deLanguagePackReference::operator!=( deLanguagePack *object ) const{
	return deObjectReference::operator!=( object );
}

bool deLanguagePackReference::operator==( const deLanguagePackReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deLanguagePackReference::operator!=( const deLanguagePackReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
