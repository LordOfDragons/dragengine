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

#include "igdeNativeNullNVBoard.h"


// class igdeNativeNullNVBoard
////////////////////////////////

igdeNativeNullNVBoard::igdeNativeNullNVBoard(){
}

igdeNativeNullNVBoard::~igdeNativeNullNVBoard(){
}

igdeNativeNullNVBoard *igdeNativeNullNVBoard::CreateNativeWidget( igdeNVBoard& ){
	return new igdeNativeNullNVBoard;
}

void igdeNativeNullNVBoard::PostCreateNativeWidget(){
}

void igdeNativeNullNVBoard::DestroyNativeWidget(){
	delete this;
}



// Management
///////////////

void igdeNativeNullNVBoard::UpdateEnabled(){
}

void igdeNativeNullNVBoard::UpdateColors(){
}

void igdeNativeNullNVBoard::UpdateNodes(){
}

void igdeNativeNullNVBoard::UpdateLinks(){
}

void igdeNativeNullNVBoard::UpdateOffset(){
}

decPoint igdeNativeNullNVBoard::GetSize(){
	return decPoint( 256, 256 );
}



void igdeNativeNullNVBoard::BeginCreateLink( igdeNativeNullNVSlot* ){
}

void igdeNativeNullNVBoard::SetCreateLinkPosition( const decPoint & ){
}

void igdeNativeNullNVBoard::SetCreateLinkTarget( igdeNativeNullNVSlot* ){
}

void igdeNativeNullNVBoard::FinishCreateLink(){
}

igdeNVLink *igdeNativeNullNVBoard::ClosestLinkNear( const decPoint &, float ) const{
	return nullptr;
}

void igdeNativeNullNVBoard::SetHoverLink( igdeNVLink* ){
}

#endif
