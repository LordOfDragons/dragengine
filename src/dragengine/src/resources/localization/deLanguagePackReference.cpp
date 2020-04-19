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

#include "deLanguagePack.h"
#include "deLanguagePackReference.h"



// Class deLanguagePackReference
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deLanguagePackReference::deLanguagePackReference(){
}

deLanguagePackReference::deLanguagePackReference( deLanguagePack *object ) :
deObjectReference( object ){
}

deLanguagePackReference::deLanguagePackReference( const deLanguagePackReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deLanguagePackReference::TakeOver( deLanguagePack *object ){
	deObjectReference::TakeOver( object );
}

bool deLanguagePackReference::operator!() const{
	return deObjectReference::operator!();
}

deLanguagePackReference::operator bool() const{
	return deObjectReference::operator bool();
}

deLanguagePackReference::operator deLanguagePack *() const{
	return ( deLanguagePack* )operator deObject *();
}

deLanguagePackReference::operator deLanguagePack &() const{
	return ( deLanguagePack& )operator deObject &();
}

deLanguagePack *deLanguagePackReference::operator->() const{
	return ( deLanguagePack* )deObjectReference::operator->();
}

deLanguagePackReference &deLanguagePackReference::operator=( deLanguagePack *object ){
	deObjectReference::operator=( object );
	return *this;
}

deLanguagePackReference &deLanguagePackReference::operator=( const deLanguagePackReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deLanguagePackReference::operator==( deLanguagePack *object ) const{
	return deObjectReference::operator==( object );
}

bool deLanguagePackReference::operator!=( deLanguagePack *object ) const{
	return deObjectReference::operator!=( object );
}

bool deLanguagePackReference::operator==( const deLanguagePackReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deLanguagePackReference::operator!=( const deLanguagePackReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
