/* 
 * Drag[en]gine Android Launcher
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
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
 
#include <stdio.h>

#include "dealWidgetOptionGroup.h"
#include "dealWidgetOptionBox.h"
#include "../../common/exceptions.h"




// Class dealWidgetOptionGroup
////////////////////////////////

// Constructors, destructors
//////////////////////////////

dealWidgetOptionGroup::dealWidgetOptionGroup(){
}

dealWidgetOptionGroup::~dealWidgetOptionGroup(){
	RemoveAll();
}



// Management
///////////////

int dealWidgetOptionGroup::GetCount() const{
	return pWidgets.GetCount();
}

dealWidgetOptionBox *dealWidgetOptionGroup::GetAt( int index ) const{
	return ( dealWidgetOptionBox* )pWidgets.GetAt( index );
}

void dealWidgetOptionGroup::Add( dealWidgetOptionBox *widget ){
	if( ! widget || pWidgets.Has( widget ) ){
		DETHROW( deeInvalidParam );
	}
	
	if( GetSelected() ){
		widget->SetSelected( false );
	}
	
	pWidgets.Add( widget );
	//widget->SetGroup( this );
}

void dealWidgetOptionGroup::Remove( dealWidgetOptionBox *widget ){
	if( ! pWidgets.Has( widget ) ){
		DETHROW( deeInvalidParam );
	}
	
	//widget->SetGroup( NULL );
	pWidgets.Remove( widget );
}

void dealWidgetOptionGroup::RemoveAll(){
	while( pWidgets.GetCount() > 0 ){
		dealWidgetOptionBox * const widget = ( dealWidgetOptionBox* )pWidgets.GetAt( pWidgets.GetCount() - 1 );
		//widget->SetGroup( NULL );
		pWidgets.Remove( widget );
	}
}

dealWidgetOptionBox *dealWidgetOptionGroup::GetSelected() const{
	const int count = pWidgets.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		dealWidgetOptionBox * const option = ( dealWidgetOptionBox* )pWidgets.GetAt( i );
		if( option->GetSelected() ){
			return option;
		}
	}
	
	return NULL;
}

void dealWidgetOptionGroup::Select( dealWidgetOptionBox *option ){
	const int count = pWidgets.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		dealWidgetOptionBox * const option2 = ( dealWidgetOptionBox* )pWidgets.GetAt( i );
		option2->SetSelected( option2 == option );
	}
}
