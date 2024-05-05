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

#include "igdeGDProperty.h"

#include <dragengine/common/exceptions.h>



// Class igdeGDProperty
/////////////////////////

// Constructor, destructor
////////////////////////////

igdeGDProperty::igdeGDProperty( const char *name ) :
pName( name ),
pType( eptString ),
pMinimumValue( 0.0f ),
pMaximumValue( 1.0f ),
pPathPatternType( epptAll ),
pIdentifierUsage( false )
{
	if( pName.IsEmpty() ){
		DETHROW( deeInvalidParam );
	}
}

igdeGDProperty::igdeGDProperty( const igdeGDProperty &property ) :
pName( property.pName ),
pDescription( property.pDescription ),
pType( property.pType ),
pMinimumValue( property.pMinimumValue ),
pMaximumValue( property.pMaximumValue ),
pDefaultValue( property.pDefaultValue ),
pOptions( property.pOptions ),
pPathPatternType( property.pPathPatternType ),
pCustomPathPattern( property.pCustomPathPattern ),
pIdentifierGroup( property.pIdentifierGroup ),
pIdentifierUsage( property.pIdentifierUsage ){
}

igdeGDProperty::~igdeGDProperty(){
}



// Management
///////////////

void igdeGDProperty::SetName( const char *name ){
	pName = name;
}

void igdeGDProperty::SetDescription( const char *description ){
	pDescription = description;
}

void igdeGDProperty::SetType( ePropertyTypes type ){
	if( type < 0 || type >= CountTypes ){
		DETHROW( deeInvalidParam );
	}
	pType = type;
}

void igdeGDProperty::SetMinimumValue( float value ){
	pMinimumValue = value;
}

void igdeGDProperty::SetMaximumValue( float value ){
	pMaximumValue = value;
}

void igdeGDProperty::SetPathPatternType( ePathPatternTypes type ){
	if( type < 0 || type >= CountPatternTypes ){
		DETHROW( deeInvalidParam );
	}
	pPathPatternType = type;
}

void igdeGDProperty::SetIdentifierGroup( const char *group ){
	pIdentifierGroup = group;
}

void igdeGDProperty::SetIdentifierUsage( bool usage ){
	pIdentifierUsage = usage;
}

void igdeGDProperty::SetDefaultValue( const char *value ){
	pDefaultValue = value;
}



igdeGDProperty &igdeGDProperty::operator=( const igdeGDProperty &property ){
	pName = property.pName;
	pDescription = property.pDescription;
	pType = property.pType;
	pMinimumValue = property.pMinimumValue;
	pMaximumValue = property.pMaximumValue;
	pDefaultValue = property.pDefaultValue;
	pOptions = property.pOptions;
	pPathPatternType = property.pPathPatternType;
	pIdentifierGroup = property.pIdentifierGroup;
	pIdentifierUsage = property.pIdentifierUsage;
	pCustomPathPattern = property.pCustomPathPattern;
	return *this;
}
