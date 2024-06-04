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

#include "deOcclusionMesh.h"
#include "deOcclusionMeshReference.h"



// Class deOcclusionMeshReference
///////////////////////////////////

// Constructor, destructor
////////////////////////////

deOcclusionMeshReference::deOcclusionMeshReference(){
}

deOcclusionMeshReference::deOcclusionMeshReference( deOcclusionMesh *object ) :
deObjectReference( object ){
}

deOcclusionMeshReference::deOcclusionMeshReference( const deOcclusionMeshReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deOcclusionMeshReference::TakeOver( deOcclusionMesh *object ){
	deObjectReference::TakeOver( object );
}

bool deOcclusionMeshReference::operator!() const{
	return deObjectReference::operator!();
}

deOcclusionMeshReference::operator bool() const{
	return deObjectReference::operator bool();
}

deOcclusionMeshReference::operator deOcclusionMesh *() const{
	return ( deOcclusionMesh* )operator deObject *();
}

deOcclusionMeshReference::operator deOcclusionMesh &() const{
	return ( deOcclusionMesh& )operator deObject &();
}

deOcclusionMesh *deOcclusionMeshReference::operator->() const{
	return ( deOcclusionMesh* )deObjectReference::operator->();
}

deOcclusionMeshReference &deOcclusionMeshReference::operator=( deOcclusionMesh *object ){
	deObjectReference::operator=( object );
	return *this;
}

deOcclusionMeshReference &deOcclusionMeshReference::operator=( const deOcclusionMeshReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deOcclusionMeshReference::operator==( deOcclusionMesh *object ) const{
	return deObjectReference::operator==( object );
}

bool deOcclusionMeshReference::operator!=( deOcclusionMesh *object ) const{
	return deObjectReference::operator!=( object );
}

bool deOcclusionMeshReference::operator==( const deOcclusionMeshReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deOcclusionMeshReference::operator!=( const deOcclusionMeshReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
