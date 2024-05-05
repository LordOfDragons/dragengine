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

#include "igdeNativeNullTextArea.h"


// Class igdeNativeNullTextArea
/////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeNativeNullTextArea::igdeNativeNullTextArea(){
}

igdeNativeNullTextArea::~igdeNativeNullTextArea(){
}

igdeNativeNullTextArea *igdeNativeNullTextArea::CreateNativeWidget( igdeTextArea& ){
	return new igdeNativeNullTextArea;
}

void igdeNativeNullTextArea::PostCreateNativeWidget(){
}

void igdeNativeNullTextArea::DestroyNativeWidget(){
	delete this;
}



// Management
///////////////

void igdeNativeNullTextArea::UpdateStyles(){
}

void igdeNativeNullTextArea::ApplyStyles(){
}

void igdeNativeNullTextArea::UpdateText(){
}

void igdeNativeNullTextArea::UpdateEnabled(){
}

void igdeNativeNullTextArea::UpdateDescription(){
}

void igdeNativeNullTextArea::UpdateEditable(){
}

void igdeNativeNullTextArea::Focus(){
}

int igdeNativeNullTextArea::GetCursorPosition() const{
	return 0;
}

void igdeNativeNullTextArea::SetCursorPosition( int ){
}

int igdeNativeNullTextArea::GetCursorColumn() const{
	return 0;
}

int igdeNativeNullTextArea::GetCursorRow() const{
	return 0;
}

void igdeNativeNullTextArea::SetCursorColumn( int ){
}

void igdeNativeNullTextArea::SetCursorRow( int ){
}

int igdeNativeNullTextArea::GetTopLine() const{
	return 0;
}

void igdeNativeNullTextArea::SetTopLine( int ){
}

int igdeNativeNullTextArea::GetBottomLine() const{
	return 0;
}

void igdeNativeNullTextArea::SetBottomLine( int ){
}

int igdeNativeNullTextArea::GetLineCount() const{
	return 1;
}

void igdeNativeNullTextArea::UpdateColumns(){
}

void igdeNativeNullTextArea::UpdateRows(){
}

#endif
