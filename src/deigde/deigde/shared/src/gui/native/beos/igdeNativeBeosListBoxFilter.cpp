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

#ifdef IGDE_TOOLKIT_BEOS

#include "igdeNativeBeosListBoxFilter.h"
#include "../../igdeListBoxFilter.h"
#include <dragengine/common/exceptions.h>


// Class igdeNativeBeosListBoxFilter
//////////////////////////////////////

igdeNativeBeosListBoxFilter::igdeNativeBeosListBoxFilter() = default;
igdeNativeBeosListBoxFilter::~igdeNativeBeosListBoxFilter() = default;


void *igdeNativeBeosListBoxFilter::CreateNativeWidget(igdeListBoxFilter &owner){
	// ListBoxFilter combines ListBox with text filtering of items
	// Implementation builds on ListBox with incremental search capability
	DEASSERT_NOTNULL(owner.GetParent())
	// DELint-Allow-NewWithoutRef
	return new igdeNativeBeosListBoxFilter;
}

void igdeNativeBeosListBoxFilter::PostCreateNativeWidget(igdeListBoxFilter &owner, void *native){
	// ListBoxFilter post-creation setup
}

void igdeNativeBeosListBoxFilter::DestroyNativeWidget(igdeListBoxFilter &owner, void *native){
	delete (igdeNativeBeosListBoxFilter*)native;
}

#endif
