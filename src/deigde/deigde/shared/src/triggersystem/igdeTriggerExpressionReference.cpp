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

#include "igdeTriggerExpression.h"
#include "igdeTriggerExpressionReference.h"



// Class igdeTriggerExpressionReference
/////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeTriggerExpressionReference::igdeTriggerExpressionReference(){
}

igdeTriggerExpressionReference::igdeTriggerExpressionReference( igdeTriggerExpression *expression ) :
deObjectReference( expression ){
}

igdeTriggerExpressionReference::igdeTriggerExpressionReference( const igdeTriggerExpressionReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeTriggerExpressionReference::TakeOver( igdeTriggerExpression *expression ){
	deObjectReference::TakeOver( expression );
}

bool igdeTriggerExpressionReference::operator!() const{
	return deObjectReference::operator!();
}

igdeTriggerExpressionReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeTriggerExpressionReference::operator igdeTriggerExpression *() const{
	return ( igdeTriggerExpression* )operator deObject *();
}

igdeTriggerExpressionReference::operator igdeTriggerExpression &() const{
	return ( igdeTriggerExpression& )operator deObject &();
}

igdeTriggerExpression *igdeTriggerExpressionReference::operator->() const{
	return ( igdeTriggerExpression* )deObjectReference::operator->();
}

igdeTriggerExpressionReference &igdeTriggerExpressionReference::operator=( igdeTriggerExpression *expression ){
	deObjectReference::operator=( expression );
	return *this;
}

igdeTriggerExpressionReference &igdeTriggerExpressionReference::operator=( const igdeTriggerExpressionReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeTriggerExpressionReference::operator==( igdeTriggerExpression *expression ) const{
	return deObjectReference::operator==( expression );
}

bool igdeTriggerExpressionReference::operator!=( igdeTriggerExpression *expression ) const{
	return deObjectReference::operator!=( expression );
}

bool igdeTriggerExpressionReference::operator==( const igdeTriggerExpressionReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeTriggerExpressionReference::operator!=( const igdeTriggerExpressionReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
