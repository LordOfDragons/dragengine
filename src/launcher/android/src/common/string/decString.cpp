/* 
 * Drag[en]gine Android Launcher
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
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "decString.h"
#include "decStringList.h"
#include "../exceptions.h"

#if defined OS_BEOS && ! defined va_copy
#define va_copy __va_copy
#endif



// Options for fxfilematch
enum {
	FILEMATCH_PERIOD      = 4,        // Leading `.' is matched only explicitly
//	FILEMATCH_CASEFOLD    = 16        // Compare without regard to case
};



// Class decString
/////////////////////

// Constructor, destructor
////////////////////////////

decString::decString(){
	pString = new char[ 1 ];
	if( ! pString ) DETHROW( deeOutOfMemory );
	pString[ 0 ] = '\0';
}

decString::decString( const char *string ){
	if( ! string ){
		DETHROW( deeInvalidParam );
	}
	int length = strlen( string );
	
	pString = new char[ length + 1 ];
	if( ! pString ) DETHROW( deeOutOfMemory );
	strncpy( pString, string, length );
	pString[ length ] = '\0';
}

decString::decString( const decString &string ){
	int length = strlen( string.pString );
	
	pString = new char[ length + 1 ];
	if( ! pString ) DETHROW( deeOutOfMemory );
	strncpy( pString, string.pString, length );
	pString[ length ] = '\0';
}

decString::decString( const decString &string1, const decString &string2 ){
	int length1 = strlen( string1.pString );
	int length2 = strlen( string2.pString );
	
	pString = new char[ length1 + length2 + 1 ];
	if( ! pString ) DETHROW( deeOutOfMemory );
	strncpy( pString, string1.pString, length1 );
	strncpy( pString + length1, string2.pString, length2 );
	pString[ length1 + length2 ] = '\0';
}

decString::decString( const decString &string1, const char *string2 ){
	if( ! string2 ) DETHROW( deeInvalidParam );
	int length1 = strlen( string1.pString );
	int length2 = strlen( string2 );
	
	pString = new char[ length1 + length2 + 1 ];
	if( ! pString ) DETHROW( deeOutOfMemory );
	strncpy( pString, string1.pString, length1 );
	strncpy( pString + length1, string2, length2 );
	pString[ length1 + length2 ] = '\0';
}

decString::~decString(){
	if( pString ) delete [] pString;
}



// Management
///////////////

bool decString::IsEmpty() const{
	return pString[ 0 ] == '\0';
}

void decString::Empty(){
	if( pString[ 0 ] != '\0' ){
		char *newString = new char[ 1 ];
		if( ! newString ) DETHROW( deeOutOfMemory );
		newString[ 0 ] = '\0';
		
		delete [] pString;
		pString = newString;
	}
}

int decString::GetLength() const{
	return strlen( pString );
}

int decString::GetAt( int position ) const{
	const int len = GetLength();
	
	if( position < 0 ){
		position += len;
	}
	
	if( position < 0 || position >= len ){
		DETHROW( deeInvalidParam );
	}
	
	return pString[ position ];
}

void decString::SetAt( int position, int character ){
	const int len = GetLength();
	
	if( position < 0 ){
		position += len;
	}
	
	if( position < 0 || position >= len ){
		DETHROW( deeInvalidParam );
	}
	
	if( character < 0 || character > 255 ){
		DETHROW( deeInvalidParam );
	}
	
	pString[ position ] = character;
}



void decString::Set( const decString &string ){
	int length = strlen( string.pString );
	
	char *newString = new char[ length + 1 ];
	if( ! newString ) DETHROW( deeOutOfMemory );
	strncpy( newString, string.pString, length );
	newString[ length ] = '\0';
	
	delete [] pString;
	pString = newString;
}

void decString::Set( const char *string ){
	if( ! string ){
		DETHROW( deeInvalidParam );
	}
	
	int length = strlen( string );
	
	char *newString = new char[ length + 1 ];
	if( ! newString ) DETHROW( deeOutOfMemory );
	strncpy( newString, string, length );
	newString[ length ] = '\0';
	
	delete [] pString;
	pString = newString;
}

void decString::Set( int character, int count ){
	if( character < 0 || character > 255 || count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	char * const newString = new char[ count + 1 ];
	memset( newString, character, count );
	newString[ count ] = '\0';
	
	delete [] pString;
	pString = newString;
}

void decString::SetValue( char value ){
#ifdef OS_W32
    int length = snprintf( NULL, 0, "%hi", value );
#else
	int length = snprintf( NULL, 0, "%hhi", value );
#endif
	if( length < 0 ) DETHROW( deeInvalidParam ); // broken snprintf implementation
	
	char *newString = new char[ length + 1 ];
	if( ! newString ) DETHROW( deeOutOfMemory );
#ifdef OS_W32
	snprintf( newString, length + 1, "%hi", value );
#else
	snprintf( newString, length + 1, "%hhi", value );
#endif
	newString[ length ] = '\0';
	
	delete [] pString;
	pString = newString;
}

void decString::SetValue( unsigned char value ){
#ifdef OS_W32
	int length = snprintf( NULL, 0, "%hu", value );
#else
	int length = snprintf( NULL, 0, "%hhu", value );
#endif
	if( length < 0 ) DETHROW( deeInvalidParam ); // broken snprintf implementation
	
	char *newString = new char[ length + 1 ];
	if( ! newString ) DETHROW( deeOutOfMemory );
#ifdef OS_W32
	snprintf( newString, length + 1, "%hu", value );
#else
	snprintf( newString, length + 1, "%hhu", value );
#endif
	newString[ length ] = '\0';
	
	delete [] pString;
	pString = newString;
}

void decString::SetValue( short value ){
	int length = snprintf( NULL, 0, "%hi", value );
	if( length < 0 ) DETHROW( deeInvalidParam ); // broken snprintf implementation
	
	char *newString = new char[ length + 1 ];
	if( ! newString ) DETHROW( deeOutOfMemory );
	snprintf( newString, length + 1, "%hi", value );
	newString[ length ] = '\0';
	
	delete [] pString;
	pString = newString;
}

void decString::SetValue( unsigned short value ){
	int length = snprintf( NULL, 0, "%hu", value );
	if( length < 0 ) DETHROW( deeInvalidParam ); // broken snprintf implementation
	
	char *newString = new char[ length + 1 ];
	if( ! newString ) DETHROW( deeOutOfMemory );
	snprintf( newString, length + 1, "%hu", value );
	newString[ length ] = '\0';
	
	delete [] pString;
	pString = newString;
}

void decString::SetValue( int value ){
	int length = snprintf( NULL, 0, "%i", value );
	if( length < 0 ) DETHROW( deeInvalidParam ); // broken snprintf implementation
	
	char *newString = new char[ length + 1 ];
	if( ! newString ) DETHROW( deeOutOfMemory );
	snprintf( newString, length + 1, "%i", value );
	newString[ length ] = '\0';
	
	delete [] pString;
	pString = newString;
}

void decString::SetValue( unsigned int value ){
	int length = snprintf( NULL, 0, "%u", value );
	if( length < 0 ) DETHROW( deeInvalidParam ); // broken snprintf implementation
	
	char *newString = new char[ length + 1 ];
	if( ! newString ) DETHROW( deeOutOfMemory );
	snprintf( newString, length + 1, "%u", value );
	newString[ length ] = '\0';
	
	delete [] pString;
	pString = newString;
}

void decString::SetValue( float value ){
	int length = snprintf( NULL, 0, "%g", value );
	if( length < 0 ) DETHROW( deeInvalidParam ); // broken snprintf implementation
	
	char *newString = new char[ length + 1 ];
	if( ! newString ) DETHROW( deeOutOfMemory );
	snprintf( newString, length + 1, "%g", value );
	newString[ length ] = '\0';
	
	delete [] pString;
	pString = newString;
}

void decString::SetValue( double value ){
	int length = snprintf( NULL, 0, "%g", value );
	if( length < 0 ) DETHROW( deeInvalidParam ); // broken snprintf implementation
	
	char *newString = new char[ length + 1 ];
	if( ! newString ) DETHROW( deeOutOfMemory );
	snprintf( newString, length + 1, "%g", value );
	newString[ length ] = '\0';
	
	delete [] pString;
	pString = newString;
}

void decString::Format( const char *format, ... ){
	va_list list;
	va_start( list, format );
	FormatUsing( format, list );
	va_end( list );
}

void decString::FormatUsing( const char *format, va_list args ){
	va_list copyargs;
	
	va_copy( copyargs, args );
	int length = vsnprintf( NULL, 0, format, copyargs );
	va_end( copyargs );
	
	if( length < 0 ) DETHROW( deeInvalidParam ); // broken vsnprintf implementation
	
	char *newString = new char[ length + 1 ];
	if( ! newString ) DETHROW( deeInvalidParam );
	
	if( vsnprintf( newString, length + 1, format, args ) != length ) DETHROW( deeInvalidParam ); // broken vsnprintf implementation
	
	delete [] pString;
	pString = newString;
}



void decString::Append( const decString &string ){
	int length1 = strlen( pString );
	int length2 = strlen( string.pString );
	
	char *newString = new char[ length1 + length2 + 1 ];
	if( ! newString ) DETHROW( deeOutOfMemory );
	strncpy( newString, pString, length1 );
	strncpy( newString + length1, string.pString, length2 );
	newString[ length1 + length2 ] = '\0';
	
	delete [] pString;
	pString = newString;
}

void decString::Append( const char *string ){
	if( ! string ){
		DETHROW( deeInvalidParam );
	}
	
	int length1 = strlen( pString );
	int length2 = strlen( string );
	
	char *newString = new char[ length1 + length2 + 1 ];
	if( ! newString ) DETHROW( deeOutOfMemory );
	strncpy( newString, pString, length1 );
	strncpy( newString + length1, string, length2 );
	newString[ length1 + length2 ] = '\0';
	
	delete [] pString;
	pString = newString;
}

void decString::AppendCharacter( char character ){
	int length = strlen( pString );
	
	char *newString = new char[ length + 2 ];
	if( ! newString ) DETHROW( deeOutOfMemory );
	strncpy( newString, pString, length );
	newString[ length ] = character;
	newString[ length + 1 ] = '\0';
	
	delete [] pString;
	pString = newString;
}

void decString::AppendCharacter( unsigned char character ){
	AppendCharacter( ( char )character );
}

void decString::AppendCharacter( int character ){
	AppendCharacter( ( unsigned char )character );
}

void decString::AppendValue( char value ){
	int length1 = strlen( pString );
#ifdef OS_W32
	int length2 = snprintf( NULL, 0, "%hi", value );
#else
	int length2 = snprintf( NULL, 0, "%hhi", value );
#endif
	if( length2 < 0 ) DETHROW( deeInvalidParam ); // broken snprintf implementation
	
	char *newString = new char[ length1 + length2 + 1 ];
	if( ! newString ) DETHROW( deeOutOfMemory );
	strncpy( newString, pString, length1 );
#ifdef OS_W32
	if( snprintf( newString + length1, length2 + 1, "%hi", value ) != length2 ) DETHROW( deeInvalidParam ); // broken vsnprintf implementation
#else
	if( snprintf( newString + length1, length2 + 1, "%hhi", value ) != length2 ) DETHROW( deeInvalidParam ); // broken vsnprintf implementation
#endif
	newString[ length1 + length2 ] = '\0';
	
	delete [] pString;
	pString = newString;
}

void decString::AppendValue( unsigned char value ){
	int length1 = strlen( pString );
#ifdef OS_W32
	int length2 = snprintf( NULL, 0, "%hu", value );
#else
	int length2 = snprintf( NULL, 0, "%hhu", value );
#endif
	if( length2 < 0 ) DETHROW( deeInvalidParam ); // broken snprintf implementation
	
	char *newString = new char[ length1 + length2 + 1 ];
	if( ! newString ) DETHROW( deeOutOfMemory );
	strncpy( newString, pString, length1 );
#ifdef OS_W32
	if( snprintf( newString + length1, length2 + 1, "%hu", value ) != length2 ) DETHROW( deeInvalidParam ); // broken vsnprintf implementation
#else
	if( snprintf( newString + length1, length2 + 1, "%hhu", value ) != length2 ) DETHROW( deeInvalidParam ); // broken vsnprintf implementation
#endif
	newString[ length1 + length2 ] = '\0';
	
	delete [] pString;
	pString = newString;
}

void decString::AppendValue( short value ){
	int length1 = strlen( pString );
	int length2 = snprintf( NULL, 0, "%hi", value );
	if( length2 < 0 ) DETHROW( deeInvalidParam ); // broken snprintf implementation
	
	char *newString = new char[ length1 + length2 + 1 ];
	if( ! newString ) DETHROW( deeOutOfMemory );
	strncpy( newString, pString, length1 );
	if( snprintf( newString + length1, length2 + 1, "%hi", value ) != length2 ) DETHROW( deeInvalidParam ); // broken vsnprintf implementation
	newString[ length1 + length2 ] = '\0';
	
	delete [] pString;
	pString = newString;
}

void decString::AppendValue( short unsigned value ){
	int length1 = strlen( pString );
	int length2 = snprintf( NULL, 0, "%hu", value );
	if( length2 < 0 ) DETHROW( deeInvalidParam ); // broken snprintf implementation
	
	char *newString = new char[ length1 + length2 + 1 ];
	if( ! newString ) DETHROW( deeOutOfMemory );
	strncpy( newString, pString, length1 );
	if( snprintf( newString + length1, length2 + 1, "%hu", value ) != length2 ) DETHROW( deeInvalidParam ); // broken vsnprintf implementation
	newString[ length1 + length2 ] = '\0';
	
	delete [] pString;
	pString = newString;
}

void decString::AppendValue( int value ){
	int length1 = strlen( pString );
	int length2 = snprintf( NULL, 0, "%i", value );
	if( length2 < 0 ) DETHROW( deeInvalidParam ); // broken snprintf implementation
	
	char *newString = new char[ length1 + length2 + 1 ];
	if( ! newString ) DETHROW( deeOutOfMemory );
	strncpy( newString, pString, length1 );
	if( snprintf( newString + length1, length2 + 1, "%i", value ) != length2 ) DETHROW( deeInvalidParam ); // broken vsnprintf implementation
	newString[ length1 + length2 ] = '\0';
	
	delete [] pString;
	pString = newString;
}

void decString::AppendValue( unsigned int value ){
	int length1 = strlen( pString );
	int length2 = snprintf( NULL, 0, "%u", value );
	if( length2 < 0 ) DETHROW( deeInvalidParam ); // broken snprintf implementation
	
	char *newString = new char[ length1 + length2 + 1 ];
	if( ! newString ) DETHROW( deeOutOfMemory );
	strncpy( newString, pString, length1 );
	if( snprintf( newString + length1, length2 + 1, "%u", value ) != length2 ) DETHROW( deeInvalidParam ); // broken vsnprintf implementation
	newString[ length1 + length2 ] = '\0';
	
	delete [] pString;
	pString = newString;
}

void decString::AppendValue( float value ){
	int length1 = strlen( pString );
	int length2 = snprintf( NULL, 0, "%g", value );
	if( length2 < 0 ) DETHROW( deeInvalidParam ); // broken snprintf implementation
	
	char *newString = new char[ length1 + length2 + 1 ];
	if( ! newString ) DETHROW( deeOutOfMemory );
	strncpy( newString, pString, length1 );
	if( snprintf( newString + length1, length2 + 1, "%g", value ) != length2 ) DETHROW( deeInvalidParam ); // broken vsnprintf implementation
	newString[ length1 + length2 ] = '\0';
	
	delete [] pString;
	pString = newString;
}

void decString::AppendValue( double value ){
	int length1 = strlen( pString );
	int length2 = snprintf( NULL, 0, "%g", value );
	if( length2 < 0 ) DETHROW( deeInvalidParam ); // broken snprintf implementation
	
	char *newString = new char[ length1 + length2 + 1 ];
	if( ! newString ) DETHROW( deeOutOfMemory );
	strncpy( newString, pString, length1 );
	if( snprintf( newString + length1, length2 + 1, "%g", value ) != length2 ) DETHROW( deeInvalidParam ); // broken vsnprintf implementation
	newString[ length1 + length2 ] = '\0';
	
	delete [] pString;
	pString = newString;
}

void decString::AppendFormat( const char *format, ... ){
	va_list list;
	va_start( list, format );
	AppendFormatUsing( format, list );
	va_end( list );
}

void decString::AppendFormatUsing( const char *format, va_list args ){
	int length1 = strlen( pString );
	va_list copyargs;
	
	va_copy( copyargs, args );
	int length2 = vsnprintf( NULL, 0, format, copyargs );
	va_end( copyargs );
	
	if( length2 < 0 ) DETHROW( deeInvalidParam ); // broken vsnprintf implementation
	
	char *newString = new char[ length1 + length2 + 1 ];
	if( ! newString ) DETHROW( deeInvalidParam );
	
	strncpy( newString, pString, length1 );
	if( vsnprintf( newString + length1, length2 + 1, format, args ) != length2 ) DETHROW( deeInvalidParam ); // broken vsnprintf implementation
	
	delete [] pString;
	pString = newString;
}



int decString::Find( int character ) const{
	return Find( character, 0, GetLength() );
}

int decString::Find( int character, int start ) const{
	return Find( character, start, GetLength() );
}

int decString::Find( int character, int start, int end ) const{
	const int len = GetLength();
	int i;
	
	if( start < 0 ){
		start = len + start;
	}
	if( end < 0 ){
		end = len + end;
	}
	
	if( start < 0 ){
		start = 0;
	}
	if( end > len ){
		end = len;
	}
	
	for( i=start; i<end; i++ ){
		if( pString[ i ] == character ){
			return i;
		}
	}
	
	return -1;
}

int decString::Find( const char *characters ) const{
	return Find( characters, 0, GetLength() );
}

int decString::Find( const char *characters, int start ) const{
	return Find( characters, start, GetLength() );
}

int decString::Find( const char *characters, int start, int end ) const{
	if( ! characters ){
		DETHROW( deeInvalidParam );
	}
	
	const int ccount = strlen( characters );
	int i, found, foundBest = -1;
	
	for( i=0; i<ccount; i++ ){
		found = Find( characters[ i ], start, end );
		
		if( found != -1 && ( foundBest == -1 || found < foundBest ) ){
			foundBest = found;
		}
	}
	
	return foundBest;
}

int decString::Find( const decString &characters ) const{
	return Find( characters.GetString(), 0, GetLength() );
}

int decString::Find( const decString &characters, int start ) const{
	return Find( characters.GetString(), start, GetLength() );
}

int decString::Find( const decString &characters, int start, int end ) const{
	return Find( characters.GetString(), start, end );
}

int decString::FindReverse( int character ) const{
	return FindReverse( character, 0, GetLength() );
}

int decString::FindReverse( int character, int start ) const{
	return FindReverse( character, start, GetLength() );
}

int decString::FindReverse( int character, int start, int end ) const{
	const int len = GetLength();
	int i;
	
	if( start < 0 ){
		start = len + start;
	}
	if( end < 0 ){
		end = len + end;
	}
	
	if( start < 0 ){
		start = 0;
	}
	if( end > len ){
		end = len;
	}
	
	for( i=end-1; i>=start; i-- ){
		if( pString[ i ] == character ){
			return i;
		}
	}
	
	return -1;
}

int decString::FindReverse( const char *characters ) const{
	return FindReverse( characters, 0, GetLength() );
}

int decString::FindReverse( const char *characters, int start ) const{
	return FindReverse( characters, start, GetLength() );
}

int decString::FindReverse( const char *characters, int start, int end ) const{
	if( ! characters ){
		DETHROW( deeInvalidParam );
	}
	
	const int ccount = strlen( characters );
	int i, found, foundBest = -1;
	
	for( i=0; i<ccount; i++ ){
		found = FindReverse( characters[ i ], start, end );
		
		if( found != -1 && found > foundBest ){
			foundBest = found;
		}
	}
	
	return foundBest;
}

int decString::FindReverse( const decString &characters ) const{
	return FindReverse( characters.GetString(), 0, GetLength() );
}

int decString::FindReverse( const decString &characters, int start ) const{
	return FindReverse( characters.GetString(), start, GetLength() );
}

int decString::FindReverse( const decString &characters, int start, int end ) const{
	return FindReverse( characters.GetString(), start, end );
}

int decString::FindString( const char *string ) const{
	return FindString( string, 0, GetLength() );
}

int decString::FindString( const char *string, int start ) const{
	return FindString( string, start, GetLength() );
}

int decString::FindString( const char *string, int start, int end ) const{
	if( ! string ){
		DETHROW( deeInvalidParam );
	}
	
	const int slen = strlen( string );
	const int rlen = GetLength();
	int i;
	
	if( rlen < slen ){
		return -1;
	}
	
	if( start < 0 ){
		start = rlen + start;
	}
	if( end < 0 ){
		end = rlen + end;
	}
	
	if( start < 0 ){
		start = 0;
	}
	if( end > rlen ){
		end = rlen;
	}
	
	for( i=start; i<end; i++ ){
		if( strncmp( pString + i, string, slen ) == 0 ){
			return i;
		}
	}
	
	return -1;
}

int decString::FindString( const decString &string ) const{
	return FindString( string.GetString(), 0, GetLength() );
}

int decString::FindString( const decString &string, int start ) const{
	return FindString( string.GetString(), start, GetLength() );
}

int decString::FindString( const decString &string, int start, int end ) const{
	return FindString( string.GetString(), start, end );
}

int decString::FindStringReverse( const char *string ) const{
	return FindStringReverse( string, 0, GetLength() );
}

int decString::FindStringReverse( const char *string, int start ) const{
	return FindStringReverse( string, start, GetLength() );
}

int decString::FindStringReverse( const char *string, int start, int end ) const{
	if( ! string ){
		DETHROW( deeInvalidParam );
	}
	
	const int slen = strlen( string );
	const int rlen = GetLength();
	int i;
	
	if( rlen < slen ){
		return -1;
	}
	
	if( start < 0 ){
		start = rlen + start;
	}
	if( end < 0 ){
		end = rlen + end;
	}
	
	if( start < 0 ){
		start = 0;
	}
	if( end > rlen ){
		end = rlen;
	}
	
	for( i=end-1; i>=start; i-- ){
		if( strncmp( pString + i, string, slen ) == 0 ){
			return i;
		}
	}
	
	return -1;
}

int decString::FindStringReverse( const decString &string ) const{
	return FindStringReverse( string.GetString(), 0, GetLength() );
}

int decString::FindStringReverse( const decString &string, int start ) const{
	return FindStringReverse( string.GetString(), start, GetLength() );
}

int decString::FindStringReverse( const decString &string, int start, int end ) const{
	return FindStringReverse( string.GetString(), start, end );
}



decString decString::GetLeft( int count ) const{
	if( count < 0 ){
		count = 0;
	}
	
	return GetMiddle( 0, count );
}

decString decString::GetRight( int count ) const{
	const int len = GetLength();
	
	if( count < 0 ){
		count = 0;
	}
	if( count > len ){
		count = len;
	}
	
	return GetMiddle( len - count, len );
}

decString decString::GetMiddle( int start ) const{
	return GetMiddle( start, GetLength() );
}

decString decString::GetMiddle( int start, int end ) const{
	const int len = GetLength();
	decString string;
	
	if( start < 0 ){
		start = len + start;
	}
	if( end < 0 ){
		end = len + end;
	}
	
	if( start < 0 ){
		start = 0;
	}
	if( end > len ){
		end = len;
	}
	
	if( start < end ){
		const int count = end - start;
		
		string.Set( ' ', count );
		strncpy( string.pString, pString + start, count );
	}
	
	return string;
}



void decString::Reverse(){
	int start, end, swap;
	
	for( start=0, end=GetLength()-1; start<end; start++, end-- ){
		swap = pString[ start ];
		pString[ start ] = pString[ end ];
		pString[ end ] = swap;
	}
}

decString decString::GetReversed() const{
	int len = GetLength();
	decString string;
	
	if( len > 0 ){
		int from, to;
		
		string.Set( ' ', len );
		
		for( from=len-1, to=0; to<len; from--, to++ ){
			string.pString[ to ] = pString[ from ];
		}
	}
	
	return string;
}

decStringList decString::Split( int character ) const{
	const int len = GetLength();
	decStringList list;
	int i, start = -1;
	
	for( i=0; i<len; i++ ){
		if( start == -1 ){
			if( pString[ i ] != character ){
				start = i;
			}
			
		}else{
			if( pString[ i ] == character ){
				list.Add( GetMiddle( start, i ) );
				start = -1;
			}
		}
	}
	
	if( start != -1 ){
		list.Add( GetMiddle( start, len ) );
	}
	
	return list;
}

decStringList decString::Split( const char *characters ) const{
	if( ! characters ){
		DETHROW( deeInvalidParam );
	}
	
	const int clen = strlen( characters );
	const int len = GetLength();
	decStringList list;
	int i, j, start = -1;
	
	for( i=0; i<len; i++ ){
		if( start == -1 ){
			for( j=0; j<clen; j++ ){
				if( pString[ i ] == characters[ j ] ){
					break;
				}
			}
			if( j == clen ){
				start = i;
			}
			
		}else{
			for( j=0; j<clen; j++ ){
				if( pString[ i ] == characters[ j ] ){
					list.Add( GetMiddle( start, i ) );
					start = -1;
					break;
				}
			}
		}
	}
	
	if( start != -1 ){
		list.Add( GetMiddle( start, len ) );
	}
	
	return list;
}

decStringList decString::Split( const decString &characters ) const{
	return Split( characters.GetString() );
}



void decString::Replace( int replaceCharacter, int withCharacter ){
	const char rc = ( char )replaceCharacter;
	const char wc = ( char )withCharacter;
	const int len = GetLength();
	int i;
	
	for( i=0; i<len; i++ ){
		if( pString[ i ] == rc ){
			pString[ i ] = wc;
		}
	}
}

void decString::Replace( const char *replaceCharacters, int withCharacter ){
	if( ! replaceCharacters ){
		DETHROW( deeInvalidParam );
	}
	
	const int len = strlen( replaceCharacters );
	int i;
	
	for( i=0; i<len; i++ ){
		Replace( replaceCharacters[ i ], withCharacter );
	}
}

void decString::Replace( const decString &replaceCharacters, int withCharacter ){
	Replace( replaceCharacters.GetString(), withCharacter );
}

void decString::ReplaceString( const char *replaceString, const char *withString ){
	if( ! replaceString || ! withString ){
		DETHROW( deeInvalidParam );
	}
	
	const int rlen = strlen( replaceString );
	const int len = GetLength() - rlen + 1;
	
	if( rlen > 0 && len > 0 ){
		const int wlen = strlen( withString );
		const int difflen = wlen - rlen;
		const decString string = *this;
		int newlen = len + rlen;
		int i, npos;
		
		for( i=0; i<len; i++ ){
			if( strncmp( string.pString + i, replaceString, rlen ) == 0 ){
				i += rlen - 1;
				newlen += difflen;
			}
		}
		
		Set( ' ', newlen );
		
		for( npos=0, i=0; i<len; i++ ){
			if( strncmp( string.pString + i, replaceString, rlen ) == 0 ){
				if( wlen > 0 ){
					strncpy( pString + npos, withString, wlen );
					npos += wlen;
				}
				i += rlen - 1;
				
			}else{
				pString[ npos++ ] = string.pString[ i ];
			}
		}
		for( i=0; i<rlen; i++ ){
			pString[ npos + i ] = string.pString[ len + i ];
		}
	}
}

void decString::ReplaceString( const decString &replaceString, const decString &withString ){
	ReplaceString( replaceString.GetString(), withString.GetString() );
}

decString decString::GetReplaced( int replaceCharacter, int withCharacter ) const{
	const int len = GetLength();
	decString string;
	
	if( len > 0 ){
		const char rc = ( char )replaceCharacter;
		const char wc = ( char )withCharacter;
		int i;
		
		string.Set( ' ', len );
		
		for( i=0; i<len; i++ ){
			if( pString[ i ] == rc ){
				string.pString[ i ] = wc;
				
			}else{
				string.pString[ i ] = pString[ i ];
			}
		}
	}
	
	return string;
}

decString decString::GetReplaced( const char *replaceCharacters, int withCharacter ) const{
	if( ! replaceCharacters ){
		DETHROW( deeInvalidParam );
	}
	
	const int len = strlen( replaceCharacters );
	decString string;
	
	if( len > 0 ){
		int i;
		
		string = GetReplaced( replaceCharacters[ 0 ], withCharacter );
		
		for( i=1; i<len; i++ ){
			string.Replace( replaceCharacters[ i ], withCharacter );
		}
	}
	
	return string;
}

decString decString::GetReplaced( const decString &replaceCharacters, int withCharacter ) const{
	return GetReplaced( replaceCharacters.GetString(), withCharacter );
}

decString decString::GetReplacedString( const char *replaceString, const char *withString ) const{
	if( ! replaceString || ! withString ){
		DETHROW( deeInvalidParam );
	}
	
	const int rlen = strlen( replaceString );
	const int len = GetLength() - rlen + 1;
	decString string;
	
	if( rlen > 0 && len > 0 ){
		const int wlen = strlen( withString );
		const int difflen = wlen - rlen;
		int newlen = len + rlen;
		int i, npos;
		
		for( i=0; i<len; i++ ){
			if( strncmp( pString + i, replaceString, rlen ) == 0 ){
				i += rlen - 1;
				newlen += difflen;
			}
		}
		
		string.Set( ' ', newlen );
		
		for( npos=0, i=0; i<len; i++ ){
			if( strncmp( pString + i, replaceString, rlen ) == 0 ){
				if( wlen > 0 ){
					strncpy( string.pString + npos, withString, wlen );
					npos += wlen;
				}
				i += rlen - 1;
				
			}else{
				string.pString[ npos++ ] = pString[ i ];
			}
		}
		for( i=0; i<rlen; i++ ){
			string.pString[ npos + i ] = pString[ len + i ];
		}
		
	}else{
		string = *this;
	}
	
	return string;
}

decString decString::GetReplacedString( const decString &replaceString, const decString &withString ) const{
	return GetReplacedString( replaceString.GetString(), withString.GetString() );
}



void decString::TrimLeft(){
	const int len = GetLength();
	int i, j;
	
	for( i=0; i<len; i++ ){
		if( isspace( pString[ i ] ) == 0 ){
			break;
		}
	}
	
	for( j=i; j<len; j++ ){
		pString[ j - i ] = pString[ j ];
	}
	pString[ len - i ] = '\0';
}

decString decString::GetTrimmedLeft() const{
	const int len = GetLength();
	int i;
	
	for( i=0; i<len; i++ ){
		if( isspace( pString[ i ] ) == 0 ){
			break;
		}
	}
	
	return GetMiddle( i, len );
}

void decString::TrimRight(){
	const int len = GetLength();
	int i;
	
	for( i=len-1; i>=0; i-- ){
		if( isspace( pString[ i ] ) == 0 ){
			pString[ i + 1 ] = '\0';
			break;
		}
	}
}

decString decString::GetTrimmedRight() const{
	const int len = GetLength();
	int i;
	
	for( i=len-1; i>=0; i-- ){
		if( isspace( pString[ i ] ) == 0 ){
			break;
		}
	}
	
	return GetLeft( i + 1 );
}

void decString::Trim(){
	const int len = GetLength();
	int start, end, i;
	
	for( start=0; start<len; start++ ){
		if( isspace( pString[ start ] ) == 0 ){
			break;
		}
	}
	
	for( end=len-1; end>=start; end-- ){
		if( isspace( pString[ end ] ) == 0 ){
			break;
		}
	}
	
	for( i=start; i<=end; i++ ){
		pString[ i - start ] = pString[ i ];
	}
	pString[ end - start + 1 ] = '\0';
}

decString decString::GetTrimmed() const{
	const int len = GetLength();
	int start, end;
	
	for( start=0; start<len; start++ ){
		if( isspace( pString[ start ] ) == 0 ){
			break;
		}
	}
	
	for( end=len-1; end>=start; end-- ){
		if( isspace( pString[ end ] ) == 0 ){
			break;
		}
	}
	
	return GetMiddle( start, end + 1 );
}



void decString::ToLower(){
	const int len = GetLength();
	int i;
	
	for( i=0; i<len; i++ ){
		pString[ i ] = ( char )tolower( pString[ i ] );
	}
}

decString decString::GetLower() const{
	const int len = GetLength();
	decString string;
	
	if( len > 0 ){
		int i;
		
		string.Set( ' ', len );
		
		for( i=0; i<len; i++ ){
			string.pString[ i ] = ( char )tolower( pString[ i ] );
		}
	}
	
	return string;
}

void decString::ToUpper(){
	const int len = GetLength();
	int i;
	
	for( i=0; i<len; i++ ){
		pString[ i ] = ( char )toupper( pString[ i ] );
	}
}

decString decString::GetUpper() const{
	const int len = GetLength();
	decString string;
	
	if( len > 0 ){
		int i;
		
		string.Set( ' ', len );
		
		for( i=0; i<len; i++ ){
			string.pString[ i ] = ( char )toupper( pString[ i ] );
		}
	}
	
	return string;
}



int decString::ToInt() const{
	return ( int )strtol( pString, NULL, 10 );
}

long long decString::ToLong() const{
	return strtol( pString, NULL, 10 );
}

float decString::ToFloat() const{
	return strtof( pString, NULL );
}

double decString::ToDouble() const{
	return strtod( pString, NULL );
}

const char *decString::GetString() const{
	return ( const char * )pString;
}



bool decString::Equals( const decString &string ) const{
	return pCompare( string.pString ) == 0;
}

bool decString::Equals( const char *string ) const{
	if( ! string ){
		DETHROW( deeInvalidParam );
	}
	
	return pCompare( string ) == 0;
}

bool decString::EqualsInsensitive( const decString &string ) const{
	return pCompareInsensitive( string.pString ) == 0;
}

bool decString::EqualsInsensitive( const char *string ) const{
	if( ! string ){
		DETHROW( deeInvalidParam );
	}
	
	return pCompareInsensitive( string ) == 0;
}

int decString::Compare( const decString &string ) const{
	return pCompare( string.pString );
}

int decString::Compare( const char *string ) const{
	if( ! string ){
		DETHROW( deeInvalidParam );
	}
	
	return pCompare( string );
}

int decString::CompareInsensitive( const decString &string ) const{
	return pCompareInsensitive( string.pString );
}

int decString::CompareInsensitive( const char *string ) const{
	if( ! string ){
		DETHROW( deeInvalidParam );
	}
	
	return pCompareInsensitive( string );
}



bool decString::MatchesPattern( const decString &pattern ) const{
	return decString::fxfilematch( pattern.GetString(), pString, FILEMATCH_PERIOD );
}

bool decString::MatchesPattern( const char *pattern ) const{
	if( ! pattern ) DETHROW( deeInvalidParam );
	
	return decString::fxfilematch( pattern, pString, FILEMATCH_PERIOD );
}

bool decString::StringMatchesPattern( const char *string, const char *pattern ){
	if( ! string || ! pattern ) DETHROW( deeInvalidParam );
	
	return decString::fxfilematch( pattern, string, FILEMATCH_PERIOD );
}

unsigned int decString::Hash() const{
	return Hash( pString );
}

unsigned int decString::Hash( const char *string ){
	// SDBM hash algorithm
	unsigned int hash = 0;
	int c;
	
	if( string ){
		c = *string++;
		while( c ){
			hash = c + ( hash << 6 ) + ( hash << 16 ) - hash;
			c = *string++;
		}
	}
	
	return hash;
}



// Operators
//////////////

char decString::operator[]( int position ) const{
	const int len = GetLength();
	
	if( position < 0 ){
		position += len;
	}
	
	if( position < 0 || position >= len ){
		DETHROW( deeInvalidParam );
	}
	
	return pString[ position ];
}

char &decString::operator[]( int position ){
	const int len = GetLength();
	
	if( position < 0 ){
		position += len;
	}
	
	if( position < 0 || position >= len ){
		DETHROW( deeInvalidParam );
	}
	
	return pString[ position ];
}

bool decString::operator!() const{
	return pString[ 0 ] == '\0';
}

bool decString::operator==( const decString &string ) const{
	return pCompare( string.pString ) == 0;
}

bool decString::operator==( const char *string ) const{
	if( ! string ){
		DETHROW( deeInvalidParam );
	}
	
	return pCompare( string ) == 0;
}

bool decString::operator!=( const decString &string ) const{
	return pCompare( string.pString ) != 0;
}

bool decString::operator!=( const char *string ) const{
	if( ! string ){
		DETHROW( deeInvalidParam );
	}
	return pCompare( string ) != 0;
}

bool decString::operator<( const decString &string ) const{
	return pCompare( string.pString ) < 0;
}

bool decString::operator<( const char *string ) const{
	if( ! string ){
		DETHROW( deeInvalidParam );
	}
	
	return pCompare( string ) < 0;
}

bool decString::operator<=( const decString &string ) const{
	return pCompare( string.pString ) <= 0;
}

bool decString::operator<=( const char *string ) const{
	if( ! string ){
		DETHROW( deeInvalidParam );
	}
	
	return pCompare( string ) <= 0;
}

bool decString::operator>( const decString &string ) const{
	return pCompare( string.pString ) > 0;
}

bool decString::operator>( const char *string ) const{
	if( ! string ){
		DETHROW( deeInvalidParam );
	}
	
	return pCompare( string ) > 0;
}

bool decString::operator>=( const decString &string ) const{
	return pCompare( string.pString ) >= 0;
}

bool decString::operator>=( const char *string ) const{
	if( ! string ){
		DETHROW( deeInvalidParam );
	}
	
	return pCompare( string ) >= 0;
}



decString decString::operator+( const decString &string ) const{
	return decString( *this, string );
}

decString decString::operator+( const char *string ) const{
	return decString( *this, string );
}



decString &decString::operator=( const decString &string ){
	Set( string );
	return *this;
}

decString &decString::operator=( const char *string ){
	Set( string );
	return *this;
}

decString &decString::operator+=( const decString &string ){
	Append( string );
	return *this;
}

decString &decString::operator+=( const char *string ){
	Append( string );
	return *this;
}



decString::operator const char*() const{
	return ( const char * )pString;
}



// Private Functions
//////////////////////

int decString::pCompare( const char *string ) const{
	return strcmp( pString, string );
}

int decString::pCompareInsensitive( const char *string ) const{
	int compare, i = 0;
	
	do{
		compare = tolower( pString[ i ] ) - tolower( string[ i ] );
		if( compare != 0 ){
			return compare;
		}
		
	} while( pString[ i++ ] );
	
	return 0;
}



/*
 * File matching function borrowed from the fox toolkit ( www.fox-toolkit.org ).
 * Original file is fxfilematch.cpp .
 *
 * Modified the sources to match the specific search requirements.
 * - remove class storage specifiers ( better safe than sorry )
 * - removed the flags since the test is already carried out on path components
 *   so path separaters have not to be tested for
 * - tidied up the formating
 */

/*
  Notes:
  - This is "upward compatible" from the standard fnmatch function in glibc,
    in addition to the basic matching, fxfilematch can also handle alternatives.

  - Match patterns are as follows:

    ?           Matches single character.
    *           Matches zero or more characters.
    [abc]       Matches a single character, which must be a, b, or c.
    [^abc]      Matches a single character, which must be anything other than a, b, or c.
    [!abc]      Ditto.
    [a-zA-Z]    Matches single character, which must be one of a-z or A-Z.
    [^a-zA-Z]   Matches single character, which must be anything other than a-z or A-Z.
    [!a-zA-Z]   Ditto.
    pat1|pat2   Matches either pat1 or pat2.
    pat1,pat2   Ditto.
    (pat1|pat2) Matches either pat1 or pat2; patterns may be nested.
    (pat1,pat2) Ditto.

  - Examples:

    *.cpp|*.cc|*.cxx|*.C  Matches some common extensions for C++ source files.

    image.(bmp,gif,jpg)   Matches a file called image given as either bmp, gif, or jpg.

    *.[^o]                Matches any file except object files.

  - You can escape meta characters like '?', '*', '(', ')', '|', '^', '!', and ','
    with the backslash '\'.

  - Match modes:

    FILEMATCH_FILE_NAME     No wildcard can ever match "/" (or "\","/" under Windows).
    FILEMATCH_NOESCAPE      Backslashes don't quote special chars ("\" is treated as "\").
    FILEMATCH_PERIOD        Leading "." is matched only explicitly (Useful to match hidden files on Unix).
    FILEMATCH_LEADING_DIR   Ignore "/..." after a match.
    FILEMATCH_CASEFOLD      Compare without regard to case.

  - Note that under Windows, FILEMATCH_NOESCAPE must be passed
*/

// If folding case, make lower case
//#define FOLD(c)          ( ( flags & FILEMATCH_CASEFOLD ) ? Ascii::toLower( c ) : ( c ) )
#define FOLD(c)			( c )

bool decString::domatch( const char *pattern, const char *string, int flags ){
	char c, cs, ce, cc, neg;
	const char *p = pattern;
	const char *q = string;
	//const char *s;
	int level;
	
	while( ( c = *p++ ) != '\0' ){
		switch( c ){
			case '?':
				if( *q == '\0' ) return false;
				if( ( flags & FILEMATCH_PERIOD ) && ( *q == '.' ) && ( q == string ) ) return false;
				q++;
				break;
			
			case '*':
				c = *p;
				while( c == '*' ){
					c = *++p;
				}
				if( ( flags & FILEMATCH_PERIOD ) && ( *q == '.' ) && ( q == string ) ) return false;
				if( c == '\0'){    // Optimize for case of trailing '*'
					return true;
				}
				while( ! domatch( p, q, flags & ~FILEMATCH_PERIOD ) ){
					if( *q++ == '\0' ) return false;
				}
				return true;
			
			case '[':
				if( *q == '\0' ) return false;
				if( ( flags & FILEMATCH_PERIOD ) && ( *q == '.' ) && ( q == string ) ) return false;
				cc = FOLD( *q );
				neg = ( ( *p == '!' ) || ( *p == '^' ) );
				if( neg ) p++;
				c = *p++;
				do{
					if( c == '\\' ) c = *p++;
					cs = ce = FOLD( c );
					if( c == '\0' ) return false;
					c = *p++;
					c = FOLD( c );
					if( c == '-' && *p != ']' ){
						c = *p++;
						if( c == '\\' ) c = *p++;
						if( c == '\0' ) return false;
						ce = FOLD( c );
						c = *p++;
					}
					if( ( cs <= cc ) && ( cc <= ce ) ) goto match;
				}
				while( c != ']' );
				if( ! neg ) return false;
				q++;
				break;
				
match:			while( c != ']' ){
					if( c == '\0' ) return false;
					c = *p++;
					if( c == '\\' ) p++;
				}
				if( neg ) return false;
				q++;
				break;
			
			case '(':
nxt:			if( domatch( p, q, flags ) ) return true;
				for( level = 0; *p && 0 <= level; ){
					switch( *p++ ){
					case '\\':
						if( *p ) p++;
						break;
					
					case '(':
						level++;
						break;
					
					case ')':
						level--;
						break;
					
					case '|':
					case ',':
						if( level == 0 ) goto nxt;
					}
				}
				return false;
			
			case ')':
				break;
			
			case '|':
			case ',':
				for( level = 0; *p && 0 <= level; ){
					switch( *p++ ){
					case '\\':
						if( *p ) p++;
						break;
					
					case '(':
						level++;
						break;
					
					case ')':
						level--;
						break;
					}
				}
				break;
			
			case '\\':
				if( *p ) c = *p++;   // Trailing escape represents itself
			
			default:
				if( FOLD( c ) != FOLD( *q ) ) return false;
				q++;
				break;
		}
	}
	return ( *q == '\0' );
}

bool decString::fxfilematch( const char *pattern, const char *string, int flags ){
	const char *p = pattern;
	const char *q = string;
	int level;
	
	if( p && q ){
nxt:	if( decString::domatch( p, q, flags ) ) return true;
		for( level = 0; *p && 0 <= level; ){
			switch( *p++ ){
			case '\\':
				if( *p ) p++;
				break;
			
			case '(':
				level++;
				break;
			
			case ')':
				level--;
				break;
			
			case '|':
			case ',':
				if( level == 0 ) goto nxt;
			}
		}
	}
	
	return false;
}
