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

#include "deTouchSensor.h"
#include "deTouchSensorReference.h"



// Class deTouchSensorReference
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deTouchSensorReference::deTouchSensorReference(){
}

deTouchSensorReference::deTouchSensorReference( deTouchSensor *object ) :
deObjectReference( object ){
}

deTouchSensorReference::deTouchSensorReference( const deTouchSensorReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deTouchSensorReference::TakeOver( deTouchSensor *object ){
	deObjectReference::TakeOver( object );
}

bool deTouchSensorReference::operator!() const{
	return deObjectReference::operator!();
}

deTouchSensorReference::operator bool() const{
	return deObjectReference::operator bool();
}

deTouchSensorReference::operator deTouchSensor *() const{
	return ( deTouchSensor* )operator deObject *();
}

deTouchSensorReference::operator deTouchSensor &() const{
	return ( deTouchSensor& )operator deObject &();
}

deTouchSensor *deTouchSensorReference::operator->() const{
	return ( deTouchSensor* )deObjectReference::operator->();
}

deTouchSensorReference &deTouchSensorReference::operator=( deTouchSensor *object ){
	deObjectReference::operator=( object );
	return *this;
}

deTouchSensorReference &deTouchSensorReference::operator=( const deTouchSensorReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deTouchSensorReference::operator==( deTouchSensor *object ) const{
	return deObjectReference::operator==( object );
}

bool deTouchSensorReference::operator!=( deTouchSensor *object ) const{
	return deObjectReference::operator!=( object );
}

bool deTouchSensorReference::operator==( const deTouchSensorReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deTouchSensorReference::operator!=( const deTouchSensorReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
