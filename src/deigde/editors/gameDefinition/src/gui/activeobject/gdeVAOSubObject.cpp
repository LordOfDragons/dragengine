/* 
 * Drag[en]gine IGDE Game Definition Editor
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "gdeVAOSubObject.h"
#include "gdeViewActiveObject.h"
#include "../gdeWindowMain.h"
#include "../../configuration/gdeConfiguration.h"
#include "../../gamedef/gdeGameDefinition.h"
#include "../../gamedef/objectClass/gdeObjectClass.h"
#include "../../gamedef/property/gdeProperty.h"
#include "../../gdEditor.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/codec/igdeCodecPropertyString.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/shape/decShapeList.h>
#include <dragengine/logger/deLogger.h>



// Class gdeVAOSubObject
//////////////////////////

// Constructor, destructor
////////////////////////////

gdeVAOSubObject::gdeVAOSubObject( gdeViewActiveObject &view, const gdeObjectClass &objectClass,
	const decString &propertyPrefix ) :
pView( view ),
pObjectClass( objectClass ),
pPropertyPrefix( propertyPrefix ){
}

gdeVAOSubObject::~gdeVAOSubObject(){
}



// Management
///////////////

void gdeVAOSubObject::RebuildResources(){
}

void gdeVAOSubObject::SelectedObjectChanged(){
}

void gdeVAOSubObject::GetExtends( decVector &minExtend, decVector &maxExtend ) const{
	minExtend.SetZero();
	maxExtend.SetZero();
}

decString gdeVAOSubObject::PropertyString( const decString &name, const decString &defaultValue ) const{
	const char * const value = pPropertyValue( name );
	return value ? decString( value ) : defaultValue;
}

int gdeVAOSubObject::PropertyInt( const decString &name, int defaultValue ) const{
	const char * const value = pPropertyValue( name );
	return value ? decString( value ).ToInt() : defaultValue;
}

float gdeVAOSubObject::PropertyFloat( const decString &name, float defaultValue ) const{
	const char * const value = pPropertyValue( name );
	return value ? decString( value ).ToFloat() : defaultValue;
}

bool gdeVAOSubObject::PropertyBool( const decString &name, bool defaultValue ) const{
	const char * const value = pPropertyValue( name );
	return value ? strcmp( value, "0" ) != 0 : defaultValue;
}

decColor gdeVAOSubObject::PropertyColor( const decString &name, const decColor &defaultValue ) const{
	const char * const value = pPropertyValue( name );
	if( ! value ){
		return defaultValue;
	}
	
	const decStringList parts( decString( value ).Split( ' ' ) );
	decColor color;
	if( parts.GetCount() > 0 ){
		color.r = parts.GetAt( 0 ).ToFloat();
	}
	if( parts.GetCount() > 1 ){
		color.g = parts.GetAt( 1 ).ToFloat();
	}
	if( parts.GetCount() > 2 ){
		color.b = parts.GetAt( 2 ).ToFloat();
	}
	if( parts.GetCount() > 3 ){
		color.a = parts.GetAt( 3 ).ToFloat();
	}
	return color;
}

decVector gdeVAOSubObject::PropertyVector( const decString &name, const decVector &defaultValue ) const{
	const char * const value = pPropertyValue( name );
	if( ! value ){
		return defaultValue;
	}
	
	const decStringList parts( decString( value ).Split( ' ' ) );
	decVector vector;
	if( parts.GetCount() > 0 ){
		vector.x = parts.GetAt( 0 ).ToFloat();
	}
	if( parts.GetCount() > 1 ){
		vector.y = parts.GetAt( 1 ).ToFloat();
	}
	if( parts.GetCount() > 2 ){
		vector.z = parts.GetAt( 2 ).ToFloat();
	}
	return vector;
}

decVector2 gdeVAOSubObject::PropertyVector2( const decString &name, const decVector2 &defaultValue ) const{
	const char * const value = pPropertyValue( name );
	if( ! value ){
		return defaultValue;
	}
	
	const decStringList parts( decString( value ).Split( ' ' ) );
	decVector2 vector;
	if( parts.GetCount() > 0 ){
		vector.x = parts.GetAt( 0 ).ToFloat();
	}
	if( parts.GetCount() > 1 ){
		vector.y = parts.GetAt( 1 ).ToFloat();
	}
	return vector;
}

decQuaternion gdeVAOSubObject::PropertyQuaternion( const decString &name, const decVector &defaultValue ) const{
	return decQuaternion::CreateFromEuler( PropertyVector( name, defaultValue ) * DEG2RAD );
}

void gdeVAOSubObject::PropertyShapeList( const decString &name, decShapeList &list, const decShapeList &defaultValue ) const{
	list.RemoveAll();
	const char * const value = pPropertyValue( name );
	if( ! value ){
		list = defaultValue;
		return;
	}
	
	igdeCodecPropertyString codec;
	codec.DecodeShapeList( value, list );
}



// Private functions
//////////////////////

const char *gdeVAOSubObject::pPropertyValue( const decString &name ) const{
	if( name.IsEmpty() ){
		return NULL;
	}
	
	const gdeObjectClass &objectClass = *pView.GetObjectClass();
	const decString realName( pPropertyPrefix + name );
	
	// find in property values
	const decString *propertyValue = NULL;
	if( objectClass.GetPropertyValues().GetAt( realName, &propertyValue ) ){
		return propertyValue->GetString();
	}
	
	// find in properties
	const gdeProperty * const property = objectClass.GetProperties().GetNamed( realName );
	if( property ){
		return property->GetDefaultValue();
	}
	
	// not found
	return NULL;
}
