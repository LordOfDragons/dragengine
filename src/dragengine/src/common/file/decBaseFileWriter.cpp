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

#include <stdlib.h>
#include <string.h>

#include "decBaseFileWriter.h"



// Class decBaseFileWriter
////////////////////////////

// Constructor, Destructor
////////////////////////////

decBaseFileWriter::decBaseFileWriter(){
}

decBaseFileWriter::~decBaseFileWriter(){
}


// Writing
////////////

void decBaseFileWriter::WriteChar( int8_t value ){
	Write( &value, 1 );
}

void decBaseFileWriter::WriteByte( uint8_t value ){
	Write( &value, 1 );
}

void decBaseFileWriter::WriteShort( int16_t value ){
	uint16_t realValue;
	memcpy( &realValue, &value, 2 );
	WriteUShort( realValue );
}

void decBaseFileWriter::WriteUShort( uint16_t value ){
	uint8_t buffer[ 2 ];
	buffer[ 0 ] = ( uint8_t )value;
	buffer[ 1 ] = ( uint8_t )( value >> 8 );
	Write( buffer, 2 );
}

void decBaseFileWriter::WriteInt( int32_t value ){
	uint32_t realValue;
	memcpy( &realValue, &value, 4 );
	WriteUInt( realValue );
}

void decBaseFileWriter::WriteUInt( uint32_t value ){
	uint8_t buffer[ 4 ];
	buffer[ 0 ] = ( uint8_t )value;
	buffer[ 1 ] = ( uint8_t )( value >> 8 );
	buffer[ 2 ] = ( uint8_t )( value >> 16 );
	buffer[ 3 ] = ( uint8_t )( value >> 24 );
	Write( buffer, 4 );
}

void decBaseFileWriter::WriteLong( int64_t value ){
	uint64_t realValue;
	memcpy( &realValue, &value, 8 );
	WriteULong( realValue );
}

void decBaseFileWriter::WriteULong( uint64_t value ){
	uint8_t buffer[ 8 ];
	buffer[ 0 ] = ( uint8_t )value;
	buffer[ 1 ] = ( uint8_t )( value >> 8 );
	buffer[ 2 ] = ( uint8_t )( value >> 16 );
	buffer[ 3 ] = ( uint8_t )( value >> 24 );
	buffer[ 4 ] = ( uint8_t )( value >> 32 );
	buffer[ 5 ] = ( uint8_t )( value >> 40 );
	buffer[ 6 ] = ( uint8_t )( value >> 48 );
	buffer[ 7 ] = ( uint8_t )( value >> 56 );
	Write( buffer, 8 );
}

void decBaseFileWriter::WriteFloat( float value ){
	uint32_t realValue;
	memcpy( &realValue, &value, 4 );
	WriteUInt( realValue );
}

void decBaseFileWriter::WriteDouble( double value ){
	uint64_t realValue;
	memcpy( &realValue, &value, 8 );
	WriteULong( realValue );
}

void decBaseFileWriter::WriteString( const char *string ){
	Write( string, ( int )strlen( string ) );
}

void decBaseFileWriter::WriteString8( const char *string ){
	const int len = ( int )strlen( string );
	if ( len > 255 ) {
		DETHROW_INFO( deeInvalidParam, "string length > 255" );
	}
	WriteByte( ( uint8_t )len );
	Write( string, len );
}

void decBaseFileWriter::WriteString16( const char *string ){
	const int len = ( int )strlen( string );
	if ( len > 65535 ) {
		DETHROW_INFO( deeInvalidParam, "string length > 65535" );
	}
	WriteUShort( ( uint16_t )len );
	Write( string, len );
}

void decBaseFileWriter::WriteVector( const decVector &vector ){
	WriteFloat( vector.x );
	WriteFloat( vector.y );
	WriteFloat( vector.z );
}

void decBaseFileWriter::WriteVector2( const decVector2 &vector ){
	WriteFloat( vector.x );
	WriteFloat( vector.y );
}

void decBaseFileWriter::WriteQuaternion( const decQuaternion &quaternion ){
	WriteFloat( quaternion.x );
	WriteFloat( quaternion.y );
	WriteFloat( quaternion.z );
	WriteFloat( quaternion.w );
}

void decBaseFileWriter::WritePoint( const decPoint &point ){
	WriteInt( point.x );
	WriteInt( point.y );
}

void decBaseFileWriter::WritePoint3( const decPoint3 &point ){
	WriteInt( point.x );
	WriteInt( point.y );
	WriteInt( point.z );
}

void decBaseFileWriter::WriteDVector( const decDVector &vector ){
	WriteDouble( vector.x );
	WriteDouble( vector.y );
	WriteDouble( vector.z );
}

void decBaseFileWriter::WriteColor( const decColor &color ){
	WriteFloat( color.r );
	WriteFloat( color.g );
	WriteFloat( color.b );
	WriteFloat( color.a );
}

void decBaseFileWriter::WriteColor3( const decColor &color ){
	WriteFloat( color.r );
	WriteFloat( color.g );
	WriteFloat( color.b );
}
