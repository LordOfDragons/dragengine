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

#ifndef _DEMCCOMMON_H_
#define _DEMCCOMMON_H_

#include "../gdk_include.h"

#include <dragengine/resources/service/deServiceObject.h>


/**
 * Convert common data.
 */
class deMCCommon{
private:
	deMCCommon() = default;
	
public:
	/** Convert uint32. */
	static uint32_t UInt32( const deServiceObject &so );
	static deServiceObject::Ref UInt32( uint32_t value );
	
	static uint32_t UInt32( const decString &string );
	static decString UInt32ToString( uint32_t value );
	
	/** Convert uint64. */
	static uint64_t UInt64( const deServiceObject &so );
	static deServiceObject::Ref UInt64( uint64_t value );
	
	static uint64_t UInt64( const decString &string );
	static decString UInt64ToString( uint64_t value );
};

#endif
