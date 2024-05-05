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

void decBaseFileWriter::WriteVarUInt( uint32_t value ){
	if( value < 64 ){
		WriteByte( ( uint8_t )value );
		
	} else if( value < 16384 ){
		const uint8_t bytes[ 2 ] = {
			( uint8_t )( ( 1 << 6 ) | ( ( value >> 8 ) & 0x3f ) ),
			( uint8_t )( value ) };
		Write( bytes, 2 );
		
	} else if( value < 4194304 ){
		const uint8_t bytes[ 3 ] = {
			( uint8_t )( ( 2 << 6 ) | ( ( value >> 16 ) & 0x3f ) ),
			( uint8_t )( value >> 8 ),
			( uint8_t )( value ) };
		Write( bytes, 3 );
		
	} else if( value < 1073741824 ){
		const uint8_t bytes[ 4 ] = {
			( uint8_t )( ( 3 << 6 ) | ( ( value >> 24 ) & 0x3f ) ),
			( uint8_t )( value >> 16 ),
			( uint8_t )( value >> 8 ),
			( uint8_t )( value ) };
		Write( bytes, 4 );
		
	} else {
		DETHROW_INFO( deeInvalidParam, "value > 1073741823" );
	}
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

void decBaseFileWriter::WriteString32( const char *string ){
	const unsigned int len = strlen( string );
	if ( len > 2147483647 ) {
		DETHROW_INFO( deeInvalidParam, "string length > 2147483647" );
	}
	WriteUInt( ( uint32_t )len );
	Write( string, len );
}

void decBaseFileWriter::WriteVarString( const char *string ){
	const unsigned int len = strlen( string );
	if ( len > 1073741823 ) {
		DETHROW_INFO( deeInvalidParam, "string length > 1073741823" );
	}
	WriteVarUInt( ( uint32_t )len );
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
