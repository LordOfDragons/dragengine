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

#include "deVideoPlayer.h"
#include "deVideoPlayerReference.h"



// Class deVideoPlayerReference
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deVideoPlayerReference::deVideoPlayerReference(){
}

deVideoPlayerReference::deVideoPlayerReference( deVideoPlayer *object ) :
deObjectReference( object ){
}

deVideoPlayerReference::deVideoPlayerReference( const deVideoPlayerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deVideoPlayerReference::TakeOver( deVideoPlayer *object ){
	deObjectReference::TakeOver( object );
}

bool deVideoPlayerReference::operator!() const{
	return deObjectReference::operator!();
}

deVideoPlayerReference::operator bool() const{
	return deObjectReference::operator bool();
}

deVideoPlayerReference::operator deVideoPlayer *() const{
	return ( deVideoPlayer* )operator deObject *();
}

deVideoPlayerReference::operator deVideoPlayer &() const{
	return ( deVideoPlayer& )operator deObject &();
}

deVideoPlayer *deVideoPlayerReference::operator->() const{
	return ( deVideoPlayer* )deObjectReference::operator->();
}

deVideoPlayerReference &deVideoPlayerReference::operator=( deVideoPlayer *object ){
	deObjectReference::operator=( object );
	return *this;
}

deVideoPlayerReference &deVideoPlayerReference::operator=( const deVideoPlayerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deVideoPlayerReference::operator==( deVideoPlayer *object ) const{
	return deObjectReference::operator==( object );
}

bool deVideoPlayerReference::operator!=( deVideoPlayer *object ) const{
	return deObjectReference::operator!=( object );
}

bool deVideoPlayerReference::operator==( const deVideoPlayerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deVideoPlayerReference::operator!=( const deVideoPlayerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
