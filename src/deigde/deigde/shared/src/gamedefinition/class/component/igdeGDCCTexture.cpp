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

#include "igdeGDCCTexture.h"

#include <dragengine/common/exceptions.h>



// Class igdeGDCCTexture
//////////////////////////

// Constructor, destructor
////////////////////////////

igdeGDCCTexture::igdeGDCCTexture() :
pScale( 1.0f, 1.0f ),
pRotation( 0.0f ),
pColorTint( 1.0f, 1.0f, 1.0f ){
}

igdeGDCCTexture::igdeGDCCTexture( const igdeGDCCTexture &texture ) :
pName( texture.pName ),
pPathSkin( texture.pPathSkin ),
pOffset( texture.pOffset ),
pScale( texture.pScale ),
pRotation( texture.pRotation ),
pColorTint( texture.pColorTint ),
pProperties( texture.pProperties ){
}

igdeGDCCTexture::~igdeGDCCTexture(){
}



// Management
///////////////

void igdeGDCCTexture::SetName( const char *name ){
	pName = name;
}

void igdeGDCCTexture::SetPathSkin( const char *path ){
	pPathSkin = path;
}

void igdeGDCCTexture::SetOffset( const decVector2 &offset ){
	pOffset = offset;
}

void igdeGDCCTexture::SetScale( const decVector2 &scale ){
	pScale = scale;
}

void igdeGDCCTexture::SetRotation( float rotation ){
	pRotation = rotation;
}

void igdeGDCCTexture::SetColorTint( const decColor &color ){
	pColorTint = color;
}

void igdeGDCCTexture::SetProperties( const decStringDictionary &properties ){
	pProperties = properties;
}
