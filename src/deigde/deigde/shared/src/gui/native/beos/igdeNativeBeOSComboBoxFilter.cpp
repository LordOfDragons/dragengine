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

#ifdef IGDE_TOOLKIT_FOX

#include "igdeNativeBeOSComboBoxFilter.h"
#include "../../igdeComboBoxFilter.h"
#include "../../igdeContainer.h"
#include "../../igdeCommonDialogs.h"
#include "../../event/igdeAction.h"
#include "../../theme/igdeGuiTheme.h"
#include "../../theme/propertyNames.h"
#include "../../model/igdeListItem.h"
#include "../../resources/igdeIcon.h"
#include "../../../environment/igdeEnvironment.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Class igdeNativeBeOSComboBoxFilter
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeNativeBeOSComboBoxFilter::igdeNativeBeOSComboBoxFilter( igdeComboBoxFilter &owner, const igdeGuiTheme &guitheme ) :
igdeNativeBeOSComboBox( owner, guitheme )
// pLabelFilter( NULL ),
// pEditFilter( NULL )
{
}

igdeNativeBeOSComboBoxFilter::~igdeNativeBeOSComboBoxFilter(){
}

igdeNativeBeOSComboBoxFilter *igdeNativeBeOSComboBoxFilter::CreateNativeWidget( igdeComboBoxFilter &owner ){
	igdeNativeBeOSComboBoxFilter * const native = new igdeNativeBeOSComboBoxFilter( owner, *owner.GetGuiTheme() );
	igdeUIBeOSHelper::AddViewToParent( native, owner.GetParent() );
	return native;
}



// Management
///////////////

void igdeNativeBeOSComboBoxFilter::UpdateRowCount(){
	// automatic in haiku if I'm not mistaken
	/*
	const int count = decMath::max( decMath::min( pOwner.GetRows(),
		decMath::max( pOwner.GetItemCount(), pOwner.GetFilterItemCount() ) ) ), 1 );
	if( count == getNumVisible() ){
		return;
	}
	
	setNumVisible( count );
	
	// FOX needs this otherwise the drop-down list can stay at a wrong size
	layout();
	*/
}

void igdeNativeBeOSComboBoxFilter::UpdateFilterString(){
// 	pEditFilter->setText( pOwner.GetFilterString().GetString() );
}



void igdeNativeBeOSComboBoxFilter::MessageReceived( BMessage *message ){
	if( message->what == eeFilterChanged ){
		if( ! pOwner.GetEnabled() ){
			return;
		}
		
		try{
// 			pOwner.SetFilterString( pEditFilter->getText().text() );
			
		}catch( const deException &e ){
			pOwner.GetLogger()->LogException( "IGDE", e );
			igdeCommonDialogs::Exception( &pOwner, e );
		}
		
	}else{
		igdeNativeBeOSComboBox::MessageReceived( message );
	}
}
	
	return 1;
}

#endif
