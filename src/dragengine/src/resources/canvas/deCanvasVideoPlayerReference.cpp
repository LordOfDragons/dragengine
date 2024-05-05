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

#include "deCanvasVideoPlayer.h"
#include "deCanvasVideoPlayerReference.h"



// Class deCanvasVideoPlayerReference
////////////////////////////////

// Constructor, destructor
////////////////////////////

deCanvasVideoPlayerReference::deCanvasVideoPlayerReference(){
}

deCanvasVideoPlayerReference::deCanvasVideoPlayerReference( deCanvasVideoPlayer *object ) :
deObjectReference( object ){
}

deCanvasVideoPlayerReference::deCanvasVideoPlayerReference( const deCanvasVideoPlayerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deCanvasVideoPlayerReference::TakeOver( deCanvasVideoPlayer *object ){
	deObjectReference::TakeOver( object );
}

bool deCanvasVideoPlayerReference::operator!() const{
	return deObjectReference::operator!();
}

deCanvasVideoPlayerReference::operator bool() const{
	return deObjectReference::operator bool();
}

deCanvasVideoPlayerReference::operator deCanvasVideoPlayer *() const{
	return ( deCanvasVideoPlayer* )operator deObject *();
}

deCanvasVideoPlayerReference::operator deCanvasVideoPlayer &() const{
	return ( deCanvasVideoPlayer& )operator deObject &();
}

deCanvasVideoPlayer *deCanvasVideoPlayerReference::operator->() const{
	return ( deCanvasVideoPlayer* )deObjectReference::operator->();
}

deCanvasVideoPlayerReference &deCanvasVideoPlayerReference::operator=( deCanvasVideoPlayer *object ){
	deObjectReference::operator=( object );
	return *this;
}

deCanvasVideoPlayerReference &deCanvasVideoPlayerReference::operator=( const deCanvasVideoPlayerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deCanvasVideoPlayerReference::operator==( deCanvasVideoPlayer *object ) const{
	return deObjectReference::operator==( object );
}

bool deCanvasVideoPlayerReference::operator!=( deCanvasVideoPlayer *object ) const{
	return deObjectReference::operator!=( object );
}

bool deCanvasVideoPlayerReference::operator==( const deCanvasVideoPlayerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deCanvasVideoPlayerReference::operator!=( const deCanvasVideoPlayerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
