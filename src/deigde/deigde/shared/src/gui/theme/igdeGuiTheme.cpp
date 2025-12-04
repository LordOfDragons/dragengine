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

#include "igdeGuiTheme.h"
#include "igdeGuiThemeProperty.h"
#include "igdeGuiThemeProperty.h"

#include <dragengine/common/exceptions.h>



// Class igdeGuiTheme
///////////////////////

// Constructor, destructor
////////////////////////////

igdeGuiTheme::igdeGuiTheme( const char *name ) :
pName( name ){
}

igdeGuiTheme::igdeGuiTheme( const char *name, const igdeGuiTheme &guitheme ) :
pName( name ),
pProperties( guitheme.pProperties ){
}

igdeGuiTheme::~igdeGuiTheme(){
}



// Management
///////////////

int igdeGuiTheme::GetPropertyCount() const{
	return pProperties.GetCount();
}

igdeGuiThemeProperty *igdeGuiTheme::GetProperty( const char *name ) const{
	deObject *property = NULL;
	return pProperties.GetAt( name, &property ) ? ( igdeGuiThemeProperty* )property : NULL;
}

bool igdeGuiTheme::HasProperty( const char *property ) const{
	return pProperties.Has( property );
}

void igdeGuiTheme::SetProperty( igdeGuiThemeProperty *property ){
	if( ! property ){
		DETHROW( deeInvalidParam );
	}
	pProperties.SetAt( property->GetName(), property );
}

void igdeGuiTheme::RemoveProperty( const char *name ){
	pProperties.Remove( name );
}

void igdeGuiTheme::RemovePropertyIfPresent( const char *name ){
	pProperties.RemoveIfPresent( name );
}

void igdeGuiTheme::RemoveAllProperties(){
	pProperties.RemoveAll();
}

decStringList igdeGuiTheme::GetPropertyNames() const{
	return pProperties.GetKeys();
}



int igdeGuiTheme::GetIntProperty( const char *name, int defaultValue ) const{
	const igdeGuiThemeProperty * const property = GetProperty( name );
	return property ? property->GetInt() : defaultValue;
}

float igdeGuiTheme::GetFloatProperty( const char *name, float defaultValue ) const{
	const igdeGuiThemeProperty * const property = GetProperty( name );
	return property ? property->GetFloat() : defaultValue;
}

const decPoint &igdeGuiTheme::GetPointProperty( const char *name, const decPoint &defaultValue ) const{
	const igdeGuiThemeProperty * const property = GetProperty( name );
	return property ? property->GetPoint() : defaultValue;
}

const decColor &igdeGuiTheme::GetColorProperty( const char *name, const decColor &defaultValue ) const{
	const igdeGuiThemeProperty * const property = GetProperty( name );
	return property ? property->GetColor() : defaultValue;
}

const decString &igdeGuiTheme::GetStringProperty( const char *name, const decString &defaultValue ) const{
	const igdeGuiThemeProperty * const property = GetProperty( name );
	return property ? property->GetString() : defaultValue;
}



void igdeGuiTheme::SetIntProperty( const char *name, int value ){
	igdeGuiThemeProperty::Ref property;
	property.TakeOver( new igdeGuiThemeProperty( name, value ) );
	pProperties.SetAt( name, property.operator->() );
}

void igdeGuiTheme::SetFloatProperty( const char *name, float value ){
	igdeGuiThemeProperty::Ref property;
	property.TakeOver( new igdeGuiThemeProperty( name, value ) );
	pProperties.SetAt( name, property.operator->() );
}

void igdeGuiTheme::SetPointProperty( const char *name, const decPoint &value ){
	igdeGuiThemeProperty::Ref property;
	property.TakeOver( new igdeGuiThemeProperty( name, value ) );
	pProperties.SetAt( name, property.operator->() );
}

void igdeGuiTheme::SetColorProperty( const char *name, const decColor &value ){
	igdeGuiThemeProperty::Ref property;
	property.TakeOver( new igdeGuiThemeProperty( name, value ) );
	pProperties.SetAt( name, property.operator->() );
}

void igdeGuiTheme::SetStringProperty( const char *name, const decString &value ){
	igdeGuiThemeProperty::Ref property;
	property.TakeOver( new igdeGuiThemeProperty( name, value ) );
	pProperties.SetAt( name, property.operator->() );
}



igdeGuiTheme &igdeGuiTheme::operator=( const igdeGuiTheme &guitheme ){
	pProperties = guitheme.pProperties;
	return *this;
}
