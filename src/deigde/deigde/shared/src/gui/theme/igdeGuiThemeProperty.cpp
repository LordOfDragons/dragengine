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
