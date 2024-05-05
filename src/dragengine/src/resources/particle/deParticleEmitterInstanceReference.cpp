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

#include "deParticleEmitterInstance.h"
#include "deParticleEmitterInstanceReference.h"



// Class deParticleEmitterInstanceReference
/////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deParticleEmitterInstanceReference::deParticleEmitterInstanceReference(){
}

deParticleEmitterInstanceReference::deParticleEmitterInstanceReference( deParticleEmitterInstance *object ) :
deObjectReference( object ){
}

deParticleEmitterInstanceReference::deParticleEmitterInstanceReference( const deParticleEmitterInstanceReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deParticleEmitterInstanceReference::TakeOver( deParticleEmitterInstance *object ){
	deObjectReference::TakeOver( object );
}

bool deParticleEmitterInstanceReference::operator!() const{
	return deObjectReference::operator!();
}

deParticleEmitterInstanceReference::operator bool() const{
	return deObjectReference::operator bool();
}

deParticleEmitterInstanceReference::operator deParticleEmitterInstance *() const{
	return ( deParticleEmitterInstance* )operator deObject *();
}

deParticleEmitterInstanceReference::operator deParticleEmitterInstance &() const{
	return ( deParticleEmitterInstance& )operator deObject &();
}

deParticleEmitterInstance *deParticleEmitterInstanceReference::operator->() const{
	return ( deParticleEmitterInstance* )deObjectReference::operator->();
}

deParticleEmitterInstanceReference &deParticleEmitterInstanceReference::operator=( deParticleEmitterInstance *object ){
	deObjectReference::operator=( object );
	return *this;
}

deParticleEmitterInstanceReference &deParticleEmitterInstanceReference::operator=( const deParticleEmitterInstanceReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deParticleEmitterInstanceReference::operator==( deParticleEmitterInstance *object ) const{
	return deObjectReference::operator==( object );
}

bool deParticleEmitterInstanceReference::operator!=( deParticleEmitterInstance *object ) const{
	return deObjectReference::operator!=( object );
}

bool deParticleEmitterInstanceReference::operator==( const deParticleEmitterInstanceReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deParticleEmitterInstanceReference::operator!=( const deParticleEmitterInstanceReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
