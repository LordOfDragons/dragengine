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

#include "igdeNativeNullIconListBox.h"


// class igdeNativeNullIconListBox
////////////////////////////////////

igdeNativeNullIconListBox::igdeNativeNullIconListBox(){
}

igdeNativeNullIconListBox::~igdeNativeNullIconListBox(){
}

igdeNativeNullIconListBox *igdeNativeNullIconListBox::CreateNativeWidget( igdeIconListBox& ){
	return new igdeNativeNullIconListBox;
}

void igdeNativeNullIconListBox::PostCreateNativeWidget(){
}

void igdeNativeNullIconListBox::DestroyNativeWidget(){
	delete this;
}



// Management
///////////////

void igdeNativeNullIconListBox::BuildHeader(){
}

void igdeNativeNullIconListBox::BuildList(){
}

void igdeNativeNullIconListBox::InsertItem( int, igdeListItem & ){
}

void igdeNativeNullIconListBox::UpdateItem( int ){
}

void igdeNativeNullIconListBox::MoveItem( int, int ){
}

void igdeNativeNullIconListBox::RemoveItem( int ){
}

void igdeNativeNullIconListBox::UpdateSelection(){
}

void igdeNativeNullIconListBox::UpdateHeader(){
}

void igdeNativeNullIconListBox::UpdateStyles(){
}

void igdeNativeNullIconListBox::UpdateDescription(){
}

void igdeNativeNullIconListBox::UpdateEnabled(){
}

void igdeNativeNullIconListBox::UpdateMinimumSize(){
}

void igdeNativeNullIconListBox::Focus(){
}

void igdeNativeNullIconListBox::MakeItemVisible( int ){
}

void igdeNativeNullIconListBox::RemoveAllItems(){
}

#endif
