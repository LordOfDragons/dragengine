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

#include "deSoundLevelMeter.h"
#include "deSoundLevelMeterReference.h"



// Class deSoundLevelMeterReference
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

deSoundLevelMeterReference::deSoundLevelMeterReference(){
}

deSoundLevelMeterReference::deSoundLevelMeterReference( deSoundLevelMeter *object ) :
deObjectReference( object ){
}

deSoundLevelMeterReference::deSoundLevelMeterReference( const deSoundLevelMeterReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deSoundLevelMeterReference::TakeOver( deSoundLevelMeter *object ){
	deObjectReference::TakeOver( object );
}

bool deSoundLevelMeterReference::operator!() const{
	return deObjectReference::operator!();
}

deSoundLevelMeterReference::operator bool() const{
	return deObjectReference::operator bool();
}

deSoundLevelMeterReference::operator deSoundLevelMeter *() const{
	return ( deSoundLevelMeter* )operator deObject *();
}

deSoundLevelMeterReference::operator deSoundLevelMeter &() const{
	return ( deSoundLevelMeter& )operator deObject &();
}

deSoundLevelMeter *deSoundLevelMeterReference::operator->() const{
	return ( deSoundLevelMeter* )deObjectReference::operator->();
}

deSoundLevelMeterReference &deSoundLevelMeterReference::operator=( deSoundLevelMeter *object ){
	deObjectReference::operator=( object );
	return *this;
}

deSoundLevelMeterReference &deSoundLevelMeterReference::operator=( const deSoundLevelMeterReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deSoundLevelMeterReference::operator==( deSoundLevelMeter *object ) const{
	return deObjectReference::operator==( object );
}

bool deSoundLevelMeterReference::operator!=( deSoundLevelMeter *object ) const{
	return deObjectReference::operator!=( object );
}

bool deSoundLevelMeterReference::operator==( const deSoundLevelMeterReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deSoundLevelMeterReference::operator!=( const deSoundLevelMeterReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
