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

#include "deDynamicSkin.h"
#include "deDynamicSkinReference.h"



// Class deDynamicSkinReference
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deDynamicSkinReference::deDynamicSkinReference(){
}

deDynamicSkinReference::deDynamicSkinReference( deDynamicSkin *object ) :
deObjectReference( object ){
}

deDynamicSkinReference::deDynamicSkinReference( const deDynamicSkinReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deDynamicSkinReference::TakeOver( deDynamicSkin *object ){
	deObjectReference::TakeOver( object );
}

bool deDynamicSkinReference::operator!() const{
	return deObjectReference::operator!();
}

deDynamicSkinReference::operator bool() const{
	return deObjectReference::operator bool();
}

deDynamicSkinReference::operator deDynamicSkin *() const{
	return ( deDynamicSkin* )operator deObject *();
}

deDynamicSkinReference::operator deDynamicSkin &() const{
	return ( deDynamicSkin& )operator deObject &();
}

deDynamicSkin *deDynamicSkinReference::operator->() const{
	return ( deDynamicSkin* )deObjectReference::operator->();
}

deDynamicSkinReference &deDynamicSkinReference::operator=( deDynamicSkin *object ){
	deObjectReference::operator=( object );
	return *this;
}

deDynamicSkinReference &deDynamicSkinReference::operator=( const deDynamicSkinReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deDynamicSkinReference::operator==( deDynamicSkin *object ) const{
	return deObjectReference::operator==( object );
}

bool deDynamicSkinReference::operator!=( deDynamicSkin *object ) const{
	return deObjectReference::operator!=( object );
}

bool deDynamicSkinReference::operator==( const deDynamicSkinReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deDynamicSkinReference::operator!=( const deDynamicSkinReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
