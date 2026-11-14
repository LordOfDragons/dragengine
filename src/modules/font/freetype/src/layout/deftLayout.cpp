/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#include "deftLayout.h"

#include <dragengine/resources/font/deFontSize.h>


// Class deftLayout
///////////////////////

// Constructor, destructor
////////////////////////////

deftLayout::deftLayout() = default;
deftLayout::~deftLayout() = default;


// Management
///////////////

void deftLayout::SetImageSize(const decPoint3 &size){
	pImageSize = size;
}

void deftLayout::Apply(deFontSize &fontSize) const{
	pGlyphs.VisitIndexed([&](int index, const Glyph &glyph){
		auto &fg = fontSize.GetGlyphAt(index);
		fg.SetUnicode(glyph.unicode);
		fg.SetX(glyph.position.x);
		fg.SetY(glyph.position.y);
		fg.SetZ(glyph.position.z);
		fg.SetWidth(glyph.size.x);
		fg.SetHeight(glyph.size.y);
		fg.SetBearing(glyph.bearing.x);
		fg.SetBearingY(glyph.bearing.y);
		fg.SetAdvance(glyph.advance);
	});
}
