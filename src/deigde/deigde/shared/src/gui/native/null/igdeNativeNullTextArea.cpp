/* 
 * Drag[en]gine IGDE
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
