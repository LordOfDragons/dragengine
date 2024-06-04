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

#include "igdeViewCurveBezier.h"
#include "igdeViewCurveBezierReference.h"



// Class igdeViewCurveBezierReference
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeViewCurveBezierReference::igdeViewCurveBezierReference(){
}

igdeViewCurveBezierReference::igdeViewCurveBezierReference( igdeViewCurveBezier *viewCurveBezier ) :
deObjectReference( viewCurveBezier ){
}

igdeViewCurveBezierReference::igdeViewCurveBezierReference( const igdeViewCurveBezierReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeViewCurveBezierReference::TakeOver( igdeViewCurveBezier *viewCurveBezier ){
	deObjectReference::TakeOver( viewCurveBezier );
}

bool igdeViewCurveBezierReference::operator!() const{
	return deObjectReference::operator!();
}

igdeViewCurveBezierReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeViewCurveBezierReference::operator igdeViewCurveBezier *() const{
	return ( igdeViewCurveBezier* )operator deObject *();
}

igdeViewCurveBezierReference::operator igdeViewCurveBezier &() const{
	return ( igdeViewCurveBezier& )operator deObject &();
}

igdeViewCurveBezier *igdeViewCurveBezierReference::operator->() const{
	return ( igdeViewCurveBezier* )deObjectReference::operator->();
}

igdeViewCurveBezierReference &igdeViewCurveBezierReference::operator=( igdeViewCurveBezier *viewCurveBezier ){
	deObjectReference::operator=( viewCurveBezier );
	return *this;
}

igdeViewCurveBezierReference &igdeViewCurveBezierReference::operator=( const igdeViewCurveBezierReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeViewCurveBezierReference::operator==( igdeViewCurveBezier *viewCurveBezier ) const{
	return deObjectReference::operator==( viewCurveBezier );
}

bool igdeViewCurveBezierReference::operator!=( igdeViewCurveBezier *viewCurveBezier ) const{
	return deObjectReference::operator!=( viewCurveBezier );
}

bool igdeViewCurveBezierReference::operator==( const igdeViewCurveBezierReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeViewCurveBezierReference::operator!=( const igdeViewCurveBezierReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
