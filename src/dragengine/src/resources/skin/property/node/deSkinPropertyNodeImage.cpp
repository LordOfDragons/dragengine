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

#include "deSkinPropertyNodeImage.h"
#include "deSkinPropertyNodeVisitor.h"
#include "../../../image/deImage.h"
#include "../../../../common/exceptions.h"



// Class deSkinPropertyNodeImage
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deSkinPropertyNodeImage::deSkinPropertyNodeImage() :
pRepeat( 1, 1 ){
}

deSkinPropertyNodeImage::~deSkinPropertyNodeImage(){
}



// Management
///////////////

void deSkinPropertyNodeImage::SetPath( const char *path ){
	pPath = path;
}

void deSkinPropertyNodeImage::SetImage( deImage *image ){
	pImage = image;
}

void deSkinPropertyNodeImage::SetRepeat( const decPoint &count ){
	DEASSERT_TRUE( count >= decPoint( 1, 1 ) )
	pRepeat = count;
}



// Visiting
/////////////

void deSkinPropertyNodeImage::Visit( deSkinPropertyNodeVisitor &visitor ){
	visitor.VisitImage( *this );
}
