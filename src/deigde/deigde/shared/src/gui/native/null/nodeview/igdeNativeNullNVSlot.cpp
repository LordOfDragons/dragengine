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

#include "igdeNativeNullNVSlot.h"


// class igdeNativeNullNVSlot
///////////////////////////////

igdeNativeNullNVSlot::igdeNativeNullNVSlot(){
}

igdeNativeNullNVSlot::~igdeNativeNullNVSlot(){
}

igdeNativeNullNVSlot *igdeNativeNullNVSlot::CreateNativeWidget( igdeNVSlot& ){
	return new igdeNativeNullNVSlot;
}

void igdeNativeNullNVSlot::PostCreateNativeWidget(){
}

void igdeNativeNullNVSlot::DestroyNativeWidget(){
	delete this;
}



// Management
///////////////

void igdeNativeNullNVSlot::UpdateText(){
}

void igdeNativeNullNVSlot::UpdateDescription(){
}

void igdeNativeNullNVSlot::UpdateEnabled(){
}

void igdeNativeNullNVSlot::UpdateColor(){
}

void igdeNativeNullNVSlot::UpdateLinkedState(){
}



decPoint igdeNativeNullNVSlot::GetCenter() const{
	return decPoint();
}

decPoint igdeNativeNullNVSlot::GetCenterNode() const{
	return decPoint();
}

decPoint igdeNativeNullNVSlot::GetCenterBoard() const{
	return decPoint();
}

decPoint igdeNativeNullNVSlot::GetConnector() const{
	return decPoint();
}

decPoint igdeNativeNullNVSlot::GetConnectorNode() const{
	return decPoint();
}

decPoint igdeNativeNullNVSlot::GetConnectorBoard() const{
	return decPoint();
}

#endif
