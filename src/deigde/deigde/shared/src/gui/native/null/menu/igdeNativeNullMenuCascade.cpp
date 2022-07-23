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

#include "igdeNativeNullMenuCascade.h"


// Class igdeNativeNullMenuCascade
////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeNativeNullMenuCascade::igdeNativeNullMenuCascade(){
}

igdeNativeNullMenuCascade::~igdeNativeNullMenuCascade(){
}

void *igdeNativeNullMenuCascade::CreateNativeWidget( igdeMenuCascade& ){
	return new igdeNativeNullMenuCascade;
}

void igdeNativeNullMenuCascade::PostCreateNativeWidget( igdeMenuCascade&, void* ){
}

void igdeNativeNullMenuCascade::DestroyNativeWidget( igdeMenuCascade&, void *native ){
	delete ( igdeNativeNullMenuCascade* )native;
}

void *igdeNativeNullMenuCascade::GetNativeContainer( const igdeMenuCascade&, void *native ){
	return native;
}

void igdeNativeNullMenuCascade::UpdateTitle( const igdeMenuCascade&, void* ){
}

void igdeNativeNullMenuCascade::UpdateDescription( const igdeMenuCascade &, void* ){
}

void igdeNativeNullMenuCascade::UpdateIcon( const igdeMenuCascade &, void* ){
}

void igdeNativeNullMenuCascade::UpdateEnabled( const igdeMenuCascade&, void* ){
}

void *igdeNativeNullMenuCascade::CreateNativePopup( igdeMenuCascade&, igdeWidget& ){
	return new igdeNativeNullMenuCascade;
}

void igdeNativeNullMenuCascade::PostCreateNativePopup( igdeMenuCascade&, void* ){
}

void igdeNativeNullMenuCascade::ShowPopupWindow( igdeMenuCascade&, igdeWidget&, const decPoint & ){
}

void igdeNativeNullMenuCascade::DestroyNativePopup( igdeMenuCascade&, void *native ){
	delete ( igdeNativeNullMenuCascade* )native;
}

#endif
