/* 
 * Drag[en]gine IGDE
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "igdeGuiThemeProperty.h"

#include <dragengine/common/exceptions.h>



// Class igdeGuiThemeProperty
///////////////////////////////

// Constructor, destructor
////////////////////////////

igdeGuiThemeProperty::igdeGuiThemeProperty( const char *name, int value ) :
pName( name ),
pType( etInteger ),
pInteger( value ),
pFloat( 0.0f ){
}

igdeGuiThemeProperty::igdeGuiThemeProperty( const char *name, float value ) :
pName( name ),
pType( etFloat ),
pInteger( 0 ),
pFloat( value ){
}

igdeGuiThemeProperty::igdeGuiThemeProperty( const char *name, const decPoint &value ) :
pName( name ),
pType( etPoint ),
pInteger( 0 ),
pFloat( 0.0f ),
pPoint( value ){
}

igdeGuiThemeProperty::igdeGuiThemeProperty( const char *name, const decColor &value ) :
pName( name ),
pType( etColor ),
pInteger( 0 ),
pFloat( 0.0f ),
pColor( value ){
}

igdeGuiThemeProperty::igdeGuiThemeProperty( const char *name, const decString &value ) :
pName( name ),
pType( etString ),
pInteger( 0 ),
pFloat( 0.0f ),
pString( value ){
}

igdeGuiThemeProperty::~igdeGuiThemeProperty(){
}



// Management
///////////////

int igdeGuiThemeProperty::GetInt() const{
	if( pType != etInteger ){
		DETHROW( deeInvalidParam );
	}
	return pInteger;
}

float igdeGuiThemeProperty::GetFloat() const{
	if( pType != etFloat ){
		DETHROW( deeInvalidParam );
	}
	return pFloat;
}

const decPoint &igdeGuiThemeProperty::GetPoint() const{
	if( pType != etPoint ){
		DETHROW( deeInvalidParam );
	}
	return pPoint;
}

const decColor &igdeGuiThemeProperty::GetColor() const{
	if( pType != etColor ){
		DETHROW( deeInvalidParam );
	}
	return pColor;
}

const decString &igdeGuiThemeProperty::GetString() const{
	if( pType != etString ){
		DETHROW( deeInvalidParam );
	}
	return pString;
}
