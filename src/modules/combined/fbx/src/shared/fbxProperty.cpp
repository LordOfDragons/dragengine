/* 
 * FBX Model Module
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

#include "fbxProperty.h"
#include "property/fbxPropertyBool.h"
#include "property/fbxPropertyShort.h"
#include "property/fbxPropertyInteger.h"
#include "property/fbxPropertyLong.h"
#include "property/fbxPropertyFloat.h"
#include "property/fbxPropertyDouble.h"
#include "property/fbxPropertyArrayBool.h"
#include "property/fbxPropertyArrayInteger.h"
#include "property/fbxPropertyArrayLong.h"
#include "property/fbxPropertyArrayFloat.h"
#include "property/fbxPropertyArrayDouble.h"
#include "property/fbxPropertyString.h"
#include "property/fbxPropertyBinary.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decZFileReader.h>


// Class fbxProperty
//////////////////////

// Constructor, destructor
////////////////////////////

fbxProperty::fbxProperty( eType type ) : pType( type ){
}

fbxProperty::~fbxProperty(){
}



// Management
///////////////

fbxPropertyBool &fbxProperty::CastBool(){
	DETHROW( deeInvalidAction );
}

fbxPropertyShort &fbxProperty::CastShort(){
	DETHROW( deeInvalidAction );
}

fbxPropertyInteger &fbxProperty::CastInteger(){
	DETHROW( deeInvalidAction );
}

fbxPropertyLong &fbxProperty::CastLong(){
	DETHROW( deeInvalidAction );
}

fbxPropertyFloat &fbxProperty::CastFloat(){
	DETHROW( deeInvalidAction );
}

fbxPropertyDouble &fbxProperty::CastDouble(){
	DETHROW( deeInvalidAction );
}

fbxPropertyArrayBool &fbxProperty::CastArrayBool(){
	DETHROW( deeInvalidAction );
}

fbxPropertyArrayInteger &fbxProperty::CastArrayInteger(){
	DETHROW( deeInvalidAction );
}

fbxPropertyArrayLong &fbxProperty::CastArrayLong(){
	DETHROW( deeInvalidAction );
}

fbxPropertyArrayFloat &fbxProperty::CastArrayFloat(){
	DETHROW( deeInvalidAction );
}

fbxPropertyArrayDouble &fbxProperty::CastArrayDouble(){
	DETHROW( deeInvalidAction );
}

fbxPropertyString &fbxProperty::CastString(){
	DETHROW( deeInvalidAction );
}

fbxPropertyBinary &fbxProperty::CastBinary(){
	DETHROW( deeInvalidAction );
}



bool fbxProperty::GetValueAsBool() const{
	DETHROW( deeInvalidAction );
}

int fbxProperty::GetValueAsInt() const{
	DETHROW( deeInvalidAction );
}

int64_t fbxProperty::GetValueAsLong() const{
	DETHROW( deeInvalidAction );
}

float fbxProperty::GetValueAsFloat() const{
	DETHROW( deeInvalidAction );
}

double fbxProperty::GetValueAsDouble() const{
	DETHROW( deeInvalidAction );
}



int fbxProperty::GetValueCount() const{
	DETHROW( deeInvalidAction );
}

bool fbxProperty::GetValueAtAsBool( int ) const{
	DETHROW( deeInvalidAction );
}

int fbxProperty::GetValueAtAsInt( int ) const{
	DETHROW( deeInvalidAction );
}

float fbxProperty::GetValueAtAsFloat( int ) const{
	DETHROW( deeInvalidAction );
}

double fbxProperty::GetValueAtAsDouble( int ) const{
	DETHROW( deeInvalidAction );
}



decVector2 fbxProperty::GetValueAtAsVector2( int index ) const{
	DETHROW( deeInvalidAction );
}

decVector fbxProperty::GetValueAtAsVector( int index ) const{
	DETHROW( deeInvalidAction );
}



fbxProperty *fbxProperty::Read( decBaseFileReader &reader ){
	const char typeCode = reader.ReadChar();
	
	switch( typeCode ){
	case 'C':
		return new fbxPropertyBool( reader );
		
	case 'Y':
		return new fbxPropertyShort( reader );
		
	case 'I':
		return new fbxPropertyInteger( reader );
		
	case 'L':
		return new fbxPropertyLong( reader );
		
	case 'F':
		return new fbxPropertyFloat( reader );
		
	case 'D':
		return new fbxPropertyDouble( reader );
		
	case 'b':
		return new fbxPropertyArrayBool( reader );
		
	case 'i':
		return new fbxPropertyArrayInteger( reader );
		
	case 'l':
		return new fbxPropertyArrayLong( reader );
		
	case 'f':
		return new fbxPropertyArrayFloat( reader );
		
	case 'd':
		return new fbxPropertyArrayDouble( reader );
		
	case 'S':
		return new fbxPropertyString( reader );
		
	case 'R':
		return new fbxPropertyBinary( reader );
		
	default:
		DETHROW_INFO( deeInvalidFileFormat, "unknown property type code" );
	}
}

const char *fbxProperty::DebugTypeName() const{
	switch( pType ){
	case etBoolean:
		return "bool";
		
	case etShort:
		return "short";
		
	case etInteger:
		return "int";
		
	case etLong:
		return "long";
		
	case etFloat:
		return "float";
		
	case etDouble:
		return "double";
		
	case etArrayBoolean:
		return "bool[]";
		
	case etArrayInteger:
		return "int[]";
		
	case etArrayLong:
		return "long[]";
		
	case etArrayFloat:
		return "float[]";
		
	case etArrayDouble:
		return "double[]";
		
	case etString:
		return "string";
		
	case etBinary:
		return "binary";
		
	default:
		return "??";
	}
}



// Protected Functions
////////////////////////

void fbxProperty::pGetValueReader( decBaseFileReader &reader, decBaseFileReaderReference& valueReader ){
	const int encoding = reader.ReadUInt();
	const int compressedLength = reader.ReadUInt();
	
	switch( encoding ){
	case 0: // plain
		valueReader = &reader;
		break;
		
	case 1: // z-lib encoded
		(void)compressedLength;
		valueReader.TakeOver( new decZFileReader( &reader, true, compressedLength ) );
		break;
		
	default:{
		decString message( "unknown encoding: " );
		message.AppendValue( encoding );
		DETHROW_INFO( deeInvalidFileFormat, message );
		}
	}
}
