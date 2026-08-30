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

#include "igdeNativeBeosComboBoxFilter.h"
#include "../../igdeComboBoxFilter.h"
#include <dragengine/common/exceptions.h>


// Class igdeNativeBeosComboBoxFilter
///////////////////////////////////////

igdeNativeBeosComboBoxFilter::igdeNativeBeosComboBoxFilter() = default;
igdeNativeBeosComboBoxFilter::~igdeNativeBeosComboBoxFilter() = default;


void *igdeNativeBeosComboBoxFilter::CreateNativeWidget(igdeComboBoxFilter &owner){
	// ComboBoxFilter combines ComboBox with text filtering of items
	// Implementation builds on ComboBox with incremental search capability
	DEASSERT_NOTNULL(owner.GetParent())
	// DELint-Allow-NewWithoutRef
	return new igdeNativeBeosComboBoxFilter;
}

void igdeNativeBeosComboBoxFilter::PostCreateNativeWidget(igdeComboBoxFilter &owner, void *native){
	// ComboBoxFilter post-creation setup
}

void igdeNativeBeosComboBoxFilter::DestroyNativeWidget(igdeComboBoxFilter &owner, void *native){
	delete (igdeNativeBeosComboBoxFilter*)native;
}

#endif
