/* 
 * Drag[en]gine IGDE
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
#include <stdlib.h>
#include <string.h>

#include "igdeSpacer.h"
#include "native/toolkit.h"
#include "igdeContainer.h"
#include "native/fox/igdeNativeFoxSpacer.h"
#include "../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Class igdeSpacer
/////////////////////

// Constructor, destructor
////////////////////////////

igdeSpacer::igdeSpacer( igdeEnvironment &environment, const decPoint &size ) :
igdeWidget( environment ),
pSize( size )
{
	if( ! ( size >= decPoint() ) ){
		DETHROW( deeInvalidParam );
	}
}

igdeSpacer::~igdeSpacer(){
	DestroyNativeWidget();
}



// Management
///////////////

void igdeSpacer::SetSize( const decPoint &size ){
	if( size == pSize ){
		return;
	}
	
	if( ! ( size >= decPoint() ) ){
		DETHROW( deeInvalidParam );
	}
	
	pSize = size;
	OnSizeChanged();
}



void igdeSpacer::CreateNativeWidget(){
	if( GetNativeWidget() ){
		return;
	}
	
	if( ! GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	FXComposite * const nativeParent = ( FXComposite* )GetParent()->GetNativeContainer();
	if( ! nativeParent ){
		DETHROW( deeInvalidParam );
	}
	
	igdeNativeFoxSpacer * const native = new igdeNativeFoxSpacer( nativeParent,
		pSize.x, pSize.y, igdeUIFoxHelper::GetChildLayoutFlags( this ) );
	SetNativeWidget( native );
	if( nativeParent->id() ){
		native->create();
	}
}

void igdeSpacer::DestroyNativeWidget(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	delete ( igdeNativeFoxSpacer* )GetNativeWidget();
	DropNativeWidget();
}



void igdeSpacer::OnSizeChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeFoxSpacer* )GetNativeWidget() )->SetSize( pSize.x, pSize.y );
	}
}
