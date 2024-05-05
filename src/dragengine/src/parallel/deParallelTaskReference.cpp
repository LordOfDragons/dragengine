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

#include "deParallelTask.h"
#include "deParallelTaskReference.h"



// Class deParallelTaskReference
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deParallelTaskReference::deParallelTaskReference(){
}

deParallelTaskReference::deParallelTaskReference( deParallelTask *object ) :
deThreadSafeObjectReference( object ){
}

deParallelTaskReference::deParallelTaskReference( const deParallelTaskReference &reference ) :
deThreadSafeObjectReference( reference ){
}



// Management
///////////////

void deParallelTaskReference::TakeOver( deParallelTask *object ){
	deThreadSafeObjectReference::TakeOver( object );
}

bool deParallelTaskReference::operator!() const{
	return deThreadSafeObjectReference::operator!();
}

deParallelTaskReference::operator bool() const{
	return deThreadSafeObjectReference::operator bool();
}

deParallelTaskReference::operator deParallelTask *() const{
	return ( deParallelTask* )operator deThreadSafeObject *();
}

deParallelTaskReference::operator deParallelTask &() const{
	return ( deParallelTask& )operator deThreadSafeObject &();
}

deParallelTask *deParallelTaskReference::operator->() const{
	return ( deParallelTask* )deThreadSafeObjectReference::operator->();
}

deParallelTaskReference &deParallelTaskReference::operator=( deParallelTask *object ){
	deThreadSafeObjectReference::operator=( object );
	return *this;
}

deParallelTaskReference &deParallelTaskReference::operator=( const deParallelTaskReference &reference ){
	deThreadSafeObjectReference::operator=( reference );
	return *this;
}

bool deParallelTaskReference::operator==( deParallelTask *object ) const{
	return deThreadSafeObjectReference::operator==( object );
}

bool deParallelTaskReference::operator==( const deParallelTaskReference &reference ) const{
	return deThreadSafeObjectReference::operator==( reference );
}
