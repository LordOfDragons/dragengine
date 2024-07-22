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

#ifndef _DESCCOMMON_H_
#define _DESCCOMMON_H_

#include "steam_api.h"

#include <dragengine/resources/service/deServiceObject.h>


/**
 * Convert common data.
 */
class deSCCommon{
private:
	deSCCommon() = default;
	
public:
	/** Convert uint32. */
	static uint32 UInt32( const deServiceObject &so );
	static deServiceObject::Ref UInt32( uint32 value );
	
	static uint32 UInt32( const decString &string );
	static decString UInt32ToString( uint32 value );
	
	/** Convert uint64. */
	static uint64 UInt64( const deServiceObject &so );
	static deServiceObject::Ref UInt64( uint64 value );
	
	static uint64 UInt64( const decString &string );
	static decString UInt64ToString( uint64 value );
	
	/** Convert SteamID. */
	static CSteamID SteamID( const deServiceObject::Ref &so );
	static deServiceObject::Ref SteamID( const CSteamID &id );
	
	static CSteamID SteamID( const decString &string );
	static decString SteamIDToString( const CSteamID &id );
};

#endif
