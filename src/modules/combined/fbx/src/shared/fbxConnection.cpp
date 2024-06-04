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
#include <inttypes.h>

#include "fbxConnection.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deBaseModule.h>


// Class fbxConnection
////////////////////////

// Constructor, destructor
////////////////////////////

fbxConnection::fbxConnection( int64_t source, int64_t target, const char *property ) :
pSource( source ),
pTarget( target ),
pProperty( property ){
}

fbxConnection::~fbxConnection(){
}



// Management
///////////////

int64_t fbxConnection::OtherID( int64_t id ) const{
	if( id == pSource ){
		return pTarget;
		
	}else if( id == pTarget ){
		return pSource;
		
	}else{
		DETHROW( deeInvalidParam );
	}
}



void fbxConnection::DebugPrintStructure( deBaseModule &module, const decString &prefix ) const{
#if defined __MINGW32__ || defined __MINGW64__
	#ifdef PRId64
		#undef PRId64
	#endif
	#define PRId64 "I64u"
#endif
	if( pProperty.IsEmpty() ){
		module.LogInfoFormat( "%sConnection: %" PRId64 " -> %" PRId64,
			prefix.GetString(), pSource, pTarget );
		
	}else{
		module.LogInfoFormat( "%sConnection: %" PRId64 " -> %" PRId64 " (%s)",
			prefix.GetString(), pSource, pTarget, pProperty.GetString() );
	}
}
