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

#include "gdeFilePattern.h"

#include <dragengine/common/exceptions.h>



// Class gdeFilePattern
/////////////////////////

// Constructor, destructor
////////////////////////////

gdeFilePattern::gdeFilePattern(){
}

gdeFilePattern::gdeFilePattern( const char *name, const char *pattern, const char *defaultExtension ) :
pName( name ),
pPattern( pattern ),
pDefExtension( defaultExtension ){
}

gdeFilePattern::gdeFilePattern( const gdeFilePattern &filePattern ) :
pName( filePattern.pName ),
pPattern( filePattern.pPattern ),
pDefExtension( filePattern.pDefExtension ){
}

gdeFilePattern::~gdeFilePattern(){
}



// Management
///////////////

void gdeFilePattern::SetName( const char *name ){
	pName = name;
}

void gdeFilePattern::SetPattern( const char *pattern ){
	pPattern = pattern;
}

void gdeFilePattern::SetDefaultExtension( const char *extension ){
	pDefExtension = extension;
}
