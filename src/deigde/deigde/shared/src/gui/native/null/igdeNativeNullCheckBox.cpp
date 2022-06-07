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

#include "igdeNativeNullCheckBox.h"


// Class igdeNativeNullCheckBox
/////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeNativeNullCheckBox::igdeNativeNullCheckBox(){
}

igdeNativeNullCheckBox::~igdeNativeNullCheckBox(){
}

igdeNativeNullCheckBox *igdeNativeNullCheckBox::CreateNativeWidget( igdeCheckBox& ){
	return new igdeNativeNullCheckBox;
}

void igdeNativeNullCheckBox::PostCreateNativeWidget(){
}

void igdeNativeNullCheckBox::DestroyNativeWidget(){
	delete this;
}



// Management
///////////////

void igdeNativeNullCheckBox::UpdateChecked(){
}

void igdeNativeNullCheckBox::UpdateStyle(){
}

void igdeNativeNullCheckBox::UpdateText(){
}

void igdeNativeNullCheckBox::UpdateDescription(){
}

void igdeNativeNullCheckBox::UpdateIcon(){
}

void igdeNativeNullCheckBox::UpdateEnabled(){
}

#endif
