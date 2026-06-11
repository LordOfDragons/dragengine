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

#include "igdeNativeNullListBoxFilter.h"


// Class igdeNativeNullListBoxFilter
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeNativeNullListBoxFilter::igdeNativeNullListBoxFilter(){
}

igdeNativeNullListBoxFilter::~igdeNativeNullListBoxFilter(){
}

igdeNativeNullListBoxFilter *igdeNativeNullListBoxFilter::CreateNativeWidget(igdeListBoxFilter&){
	// DELint-Allow-NewWithoutRef
	return new igdeNativeNullListBoxFilter;
}

void igdeNativeNullListBoxFilter::PostCreateNativeWidget(){
}

void igdeNativeNullListBoxFilter::DestroyNativeWidget(){
	delete this;
}



// Management
///////////////

void igdeNativeNullListBoxFilter::BuildList(){
}

void igdeNativeNullListBoxFilter::UpdateItem(int){
}

void igdeNativeNullListBoxFilter::UpdateStyles(){
}

void igdeNativeNullListBoxFilter::UpdateSelection(){
}

void igdeNativeNullListBoxFilter::Focus(){
}

void igdeNativeNullListBoxFilter::MakeItemVisible(int){
}

decPoint igdeNativeNullListBoxFilter::GetContentPosition() const{
	return {};
}

void igdeNativeNullListBoxFilter::SetContentPosition(const decPoint&){
}

void igdeNativeNullListBoxFilter::InsertItem(int){
}

void igdeNativeNullListBoxFilter::RemoveItem(int){
}

void igdeNativeNullListBoxFilter::RemoveAllItems(){
}

void igdeNativeNullListBoxFilter::MoveItem(int, int){
}

void igdeNativeNullListBoxFilter::UpdateEnabled(){
}

void igdeNativeNullListBoxFilter::UpdateRowCount(){
}

void igdeNativeNullListBoxFilter::UpdateDescription(){
}

void igdeNativeNullListBoxFilter::UpdateFilterString(){
}

#endif
