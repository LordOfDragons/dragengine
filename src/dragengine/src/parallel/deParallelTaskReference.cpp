/* 
 * Drag[en]gine Game Engine
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
