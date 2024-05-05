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

#include "deAnimatorRule.h"
#include "deAnimatorRuleReference.h"



// Class deAnimatorRuleReference
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deAnimatorRuleReference::deAnimatorRuleReference(){
}

deAnimatorRuleReference::deAnimatorRuleReference( deAnimatorRule *object ) :
deObjectReference( object ){
}

deAnimatorRuleReference::deAnimatorRuleReference( const deAnimatorRuleReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deAnimatorRuleReference::TakeOver( deAnimatorRule *object ){
	deObjectReference::TakeOver( object );
}

bool deAnimatorRuleReference::operator!() const{
	return deObjectReference::operator!();
}

deAnimatorRuleReference::operator bool() const{
	return deObjectReference::operator bool();
}

deAnimatorRuleReference::operator deAnimatorRule *() const{
	return ( deAnimatorRule* )operator deObject *();
}

deAnimatorRuleReference::operator deAnimatorRule &() const{
	return ( deAnimatorRule& )operator deObject &();
}

deAnimatorRule *deAnimatorRuleReference::operator->() const{
	return ( deAnimatorRule* )deObjectReference::operator->();
}

deAnimatorRuleReference &deAnimatorRuleReference::operator=( deAnimatorRule *object ){
	deObjectReference::operator=( object );
	return *this;
}

deAnimatorRuleReference &deAnimatorRuleReference::operator=( const deAnimatorRuleReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deAnimatorRuleReference::operator==( deAnimatorRule *object ) const{
	return deObjectReference::operator==( object );
}

bool deAnimatorRuleReference::operator!=( deAnimatorRule *object ) const{
	return deObjectReference::operator!=( object );
}

bool deAnimatorRuleReference::operator==( const deAnimatorRuleReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deAnimatorRuleReference::operator!=( const deAnimatorRuleReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
