/* 
 * Drag[en]gine IGDE Game Definition Editor
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

#include "gdeWPSTIMOCSubObject.h"
#include "../../../gdeWindowMain.h"
#include "../../../../gamedef/gdeGameDefinition.h"
#include "../../../../gamedef/objectClass/gdeObjectClass.h"

#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

gdeWPSTIMOCSubObject::gdeWPSTIMOCSubObject( gdeWPSTreeModel &tree, eTypes type,
	gdeObjectClass *objectClass, int index ) :
gdeWPSTreeItemModel( tree, type ),
pObjectClass( NULL ),
pIndex( index )
{
	if( ! objectClass ){
		DETHROW( deeInvalidParam );
	}
	
	SetIcon( GetWindowMain().GetEnvironment().GetStockIcon( igdeEnvironment::esiNew ) );
	
	pObjectClass = objectClass;
	objectClass->AddReference();
}

gdeWPSTIMOCSubObject::~gdeWPSTIMOCSubObject(){
	if( pObjectClass ){
		pObjectClass->FreeReference();
	}
}



// Management
///////////////

bool gdeWPSTIMOCSubObject::IsValid() const{
	return true;
}

void gdeWPSTIMOCSubObject::ValidateObjectClassName(){
}

int gdeWPSTIMOCSubObject::Compare( const gdeWPSTreeItemModel &item ) const{
	const gdeWPSTIMOCSubObject &other = ( const gdeWPSTIMOCSubObject & )item;
	
	if( GetType() < item.GetType() ){
		return -1;
		
	}else if( GetType() > item.GetType() ){
		return 1;
		
	}else if( pIndex < other.pIndex ){
		return -1;
		
	}else if( pIndex > other.pIndex ){
		return 1;
		
	}else{
		return 0;
	}
}

void gdeWPSTIMOCSubObject::SelectBestMatching( const char *string ){
	if( GetParent() ){
		( ( gdeWPSTreeItemModel* )GetParent() )->SelectBestMatching( string );
	}
}
