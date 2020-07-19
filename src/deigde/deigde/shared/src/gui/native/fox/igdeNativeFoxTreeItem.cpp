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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "igdeNativeFoxTreeItem.h"
#include "../../model/igdeTreeItem.h"
#include "../../resources/igdeIcon.h"
#include "../../resources/igdeFont.h"
#include "../../../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



FXIMPLEMENT( igdeNativeFoxTreeItem, FXTreeItem, NULL, 0 )


// class igdeNativeFoxTreeItem
////////////////////////////////

igdeNativeFoxTreeItem::igdeNativeFoxTreeItem(){ }

igdeNativeFoxTreeItem::igdeNativeFoxTreeItem( igdeTreeItem &owner ) :
pOwner( &owner )
{
}

igdeNativeFoxTreeItem::~igdeNativeFoxTreeItem(){
}



// Management
///////////////

#endif
