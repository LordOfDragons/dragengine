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

#include "deEffect.h"
#include "deEffectReference.h"



// Class deEffectReference
////////////////////////////

// Constructor, destructor
////////////////////////////

deEffectReference::deEffectReference(){
}

deEffectReference::deEffectReference( deEffect *object ) :
deObjectReference( object ){
}

deEffectReference::deEffectReference( const deEffectReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deEffectReference::TakeOver( deEffect *object ){
	deObjectReference::TakeOver( object );
}

bool deEffectReference::operator!() const{
	return deObjectReference::operator!();
}

deEffectReference::operator bool() const{
	return deObjectReference::operator bool();
}

deEffectReference::operator deEffect *() const{
	return ( deEffect* )operator deObject *();
}

deEffectReference::operator deEffect &() const{
	return ( deEffect& )operator deObject &();
}

deEffect *deEffectReference::operator->() const{
	return ( deEffect* )deObjectReference::operator->();
}

deEffectReference &deEffectReference::operator=( deEffect *object ){
	deObjectReference::operator=( object );
	return *this;
}

deEffectReference &deEffectReference::operator=( const deEffectReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deEffectReference::operator==( deEffect *object ) const{
	return deObjectReference::operator==( object );
}

bool deEffectReference::operator!=( deEffect *object ) const{
	return deObjectReference::operator!=( object );
}

bool deEffectReference::operator==( const deEffectReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deEffectReference::operator!=( const deEffectReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
