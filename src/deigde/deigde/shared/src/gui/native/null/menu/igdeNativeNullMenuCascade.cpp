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

#ifdef IGDE_TOOLKIT_NULL

#include "igdeNativeNullMenuCascade.h"


// Class igdeNativeNullMenuCascade
////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeNativeNullMenuCascade::igdeNativeNullMenuCascade(){
}

igdeNativeNullMenuCascade::~igdeNativeNullMenuCascade(){
}

void *igdeNativeNullMenuCascade::CreateNativeWidget(igdeMenuCascade&){
	return new igdeNativeNullMenuCascade;
}

void igdeNativeNullMenuCascade::PostCreateNativeWidget(igdeMenuCascade&, void*){
}

void igdeNativeNullMenuCascade::DestroyNativeWidget(igdeMenuCascade&, void *native){
	delete (igdeNativeNullMenuCascade*)native;
}

void *igdeNativeNullMenuCascade::GetNativeContainer(const igdeMenuCascade&, void *native){
	return native;
}

void igdeNativeNullMenuCascade::UpdateTitle(const igdeMenuCascade&, void*){
}

void igdeNativeNullMenuCascade::UpdateDescription(const igdeMenuCascade &, void*){
}

void igdeNativeNullMenuCascade::UpdateIcon(const igdeMenuCascade &, void*){
}

void igdeNativeNullMenuCascade::UpdateEnabled(const igdeMenuCascade&, void*){
}

void *igdeNativeNullMenuCascade::CreateNativePopup(igdeMenuCascade&, igdeWidget&){
	return new igdeNativeNullMenuCascade;
}

void igdeNativeNullMenuCascade::PostCreateNativePopup(igdeMenuCascade&, void*){
}

void igdeNativeNullMenuCascade::ShowPopupWindow(igdeMenuCascade&, igdeWidget&, const decPoint &){
}

void igdeNativeNullMenuCascade::DestroyNativePopup(igdeMenuCascade&, void *native){
	delete (igdeNativeNullMenuCascade*)native;
}

#endif
