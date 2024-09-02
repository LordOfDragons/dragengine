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

#include "deSCCommon.h"

#include <dragengine/common/math/decMath.h>


// Class deSCCommon
/////////////////////

uint32 deSCCommon::UInt32( const deServiceObject &so ){
	return UInt32( so.GetString() );
}

deServiceObject::Ref deSCCommon::UInt32( uint32 value ){
	return deServiceObject::NewString( UInt32ToString( value ) );
}

uint32 deSCCommon::UInt32( const decString &string ){
	return ( uint32 )string.ToLongValid();
}

decString deSCCommon::UInt32ToString( uint32 value ){
	decString string;
	string.AppendValue( ( unsigned long long )value );
	return string;
}

uint64 deSCCommon::UInt64( const deServiceObject &so ){
	return UInt64( so.GetString() );
}

deServiceObject::Ref deSCCommon::UInt64( uint64 value ){
	return deServiceObject::NewString( UInt64ToString( value ) );
}

uint64 deSCCommon::UInt64( const decString &string ){
	return ( uint64 )string.ToLongValid();
}

decString deSCCommon::UInt64ToString( uint64 value ){
	decString string;
	string.AppendValue( ( long long )value );
	return string;
}

CSteamID deSCCommon::SteamID( const deServiceObject::Ref &so ){
	return CSteamID( UInt64( so->GetString() ) );
}

deServiceObject::Ref deSCCommon::SteamID( const CSteamID &id ){
	return deServiceObject::NewString( SteamIDToString( id ) );
}

CSteamID deSCCommon::SteamID( const decString &string ){
	return CSteamID( UInt64( string ) );
}

decString deSCCommon::SteamIDToString( const CSteamID &id ){
	return UInt64ToString( id.ConvertToUint64() );
}
