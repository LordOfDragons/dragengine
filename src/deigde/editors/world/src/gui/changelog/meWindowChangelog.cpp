/* 
 * Drag[en]gine IGDE World Editor
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

#include "meWCEntry.h"
#include "meWindowChangelog.h"
#include "meWindowChangelogListener.h"
#include "../meWindowMain.h"
#include "../../world/meWorld.h"
#include "../../world/terrain/meHeightTerrain.h"
#include "../../world/terrain/meHeightTerrainSector.h"
#include "../../world/terrain/meHeightTerrainTexture.h"
#include "../../world/terrain/meHeightTerrainNavSpace.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeIconListBox.h>
#include <deigde/gui/event/igdeIconListBoxListener.h>
#include <deigde/gui/model/igdeListItemReference.h>
#include <deigde/gui/model/igdeListItemSorter.h>
#include <deigde/gui/model/igdeListItemSorterReference.h>

#include <dragengine/common/exceptions.h>



// Sorting
////////////

namespace {

class cChangelogSorter : public igdeListItemSorter{
public:
	cChangelogSorter(){}
	
	virtual bool Precedes( const igdeListItem &item1, const igdeListItem &item2 ){
		const meWCEntry &entry1 = ( const meWCEntry & )item1;
		const meWCEntry &entry2 = ( const meWCEntry & )item2;
		const bool worldType1 = entry1.GetType() == meWCEntry::eetWorld;
		const bool worldType2 = entry2.GetType() == meWCEntry::eetWorld;
		
		if( worldType1 && ! worldType2 ){
			return true;
		}
		if( worldType2 && ! worldType1 ){
			return false;
		}
		
		const decPoint3 difference( entry2.GetSector() - entry1.GetSector() );
		if( difference.x < 0 ){
			return true;
			
		}else if( difference.x > 0 ){
			return false;
			
		}else if( difference.y < 0 ){
			return true;
			
		}else if( difference.y > 0 ){
			return false;
			
		}else{
			return difference.z <= 0;
		}
	}
};

class cListChangelog : public igdeIconListBoxListener{
	meWindowChangelog &pWindow;
	
public:
	cListChangelog( meWindowChangelog &window ) : pWindow( window ){}
	
	virtual void AddContextMenuEntries( igdeIconListBox*, igdeMenuCascade& ){
		(void)pWindow;
	}
};

}



// Class meWindowChangelog
/////////////////////////////

// Constructor, destructor
////////////////////////////

meWindowChangelog::meWindowChangelog( meWindowMain &windowMain ) :
igdeContainerBorder( windowMain.GetEnvironment() ),
pWindowMain( windowMain ),
pListener( NULL ),
pWorld( NULL )
{
	igdeUIHelper &helper = windowMain.GetEnvironment().GetUIHelper();
	
	pListener = new meWindowChangelogListener( *this );
	
	igdeUIHelper::sColumnHeader headers[ 3 ] = {
		igdeUIHelper::sColumnHeader( "Sector", NULL, 50 ),
		igdeUIHelper::sColumnHeader( "What", NULL, 150 ),
		igdeUIHelper::sColumnHeader( "Filename", NULL, 380 ) };
	helper.IconListBox( decPoint( 100, 150 ), headers, 3, "Changes", pListChanges,
		new cListChangelog( *this ) );
	AddChild( pListChanges, igdeContainerBorder::eaCenter );
	
	igdeListItemSorterReference sorter;
	sorter.TakeOver( new cChangelogSorter );
	pListChanges->SetSorter( sorter );
}

meWindowChangelog::~meWindowChangelog(){
	if( pWorld ){
		pWorld->RemoveNotifier( pListener );
		pWorld->FreeReference();
		pWorld = NULL;
	}
	
	if( pListener ){
		pListener->FreeReference();
	}
}



// Management
///////////////

void meWindowChangelog::SetWorld( meWorld *world ){
	if( world == pWorld ){
		return;
	}
	
	pListChanges->RemoveAllItems();
	
	if( pWorld ){
		pWorld->RemoveNotifier( pListener );
		pWorld->FreeReference();
	}
	
	pWorld = world;
	
	if( world ){
		world->AddNotifier( pListener );
		world->AddReference();
	}
	
	UpdateChangelog();
}



void meWindowChangelog::UpdateChangelog(){
	pListChanges->RemoveAllItems();
	
	if( ! pWorld ){
		return;
	}
	
	meHeightTerrain * const hterrain = pWorld->GetHeightTerrain();
	igdeListItemReference refEntry;
	int i, j;
	
	try{
		if( pWorld->GetChanged() ){
			refEntry.TakeOver( new meWCEntry( *this, meWCEntry::eetWorld ) );
			meWCEntry &entry = ( meWCEntry& )( igdeListItem& )refEntry;
			entry.SetWorld( pWorld );
			entry.UpdateText();
			
			pListChanges->AddItem( refEntry );
		}
		
		if( pWorld->GetDepChanged() ){
			const int htsectorCount = hterrain->GetSectorCount();
			
			if( pWorld->GetHeightTerrain()->GetChanged() ){
				refEntry.TakeOver( new meWCEntry( *this, meWCEntry::eetHeightTerrain ) );
				meWCEntry &entry = ( meWCEntry& )( igdeListItem& )refEntry;
				entry.SetWorld( pWorld );
				entry.UpdateText();
				pListChanges->AddItem( refEntry );
			}
			
			for( i=0; i<htsectorCount; i++ ){
				meHeightTerrainSector &htsector = *hterrain->GetSectorAt( i );
				const decPoint &scoord = htsector.GetCoordinates();
				
				if( htsector.GetHeightImageChanged() ){
					refEntry.TakeOver( new meWCEntry( *this, meWCEntry::eetHTHeight ) );
					meWCEntry &entry = ( meWCEntry& )( igdeListItem& )refEntry;
					entry.SetWorld( pWorld );
					entry.SetSector( decPoint3( scoord.x, 0, scoord.y ) );
					entry.UpdateText();
					pListChanges->AddItem( refEntry );
				}
				
				if( htsector.GetVisibilityChanged() ){
					refEntry.TakeOver( new meWCEntry( *this, meWCEntry::eetHTVisibility ) );
					meWCEntry &entry = ( meWCEntry& )( igdeListItem& )refEntry;
					entry.SetWorld( pWorld );
					entry.SetSector( decPoint3( scoord.x, 0, scoord.y ) );
					entry.UpdateText();
					pListChanges->AddItem( refEntry );
				}
				
				if( htsector.GetPFCacheChanged() ){
					refEntry.TakeOver( new meWCEntry( *this, meWCEntry::eetHTPFCache ) );
					meWCEntry &entry = ( meWCEntry& )( igdeListItem& )refEntry;
					entry.SetWorld( pWorld );
					entry.SetSector( decPoint3( scoord.x, 0, scoord.y ) );
					entry.UpdateText();
					pListChanges->AddItem( refEntry );
				}
				
				if( pWorld->GetHeightTerrain()->GetDepChanged() ){
					const int httextureCount = htsector.GetTextureCount();
					
					for( j=0; j<httextureCount; j++ ){
						meHeightTerrainTexture &httexture = *htsector.GetTextureAt( j );
						if( ! httexture.GetMaskChanged() ){
							continue;
						}
						
						refEntry.TakeOver( new meWCEntry( *this, meWCEntry::eetHTTextureMask ) );
						meWCEntry &entry = ( meWCEntry& )( igdeListItem& )refEntry;
						entry.SetHTTexture( &httexture );
						entry.SetSector( decPoint3( scoord.x, 0, scoord.y ) );
						entry.UpdateText();
						pListChanges->AddItem( refEntry );
					}
				}
				
				const int navSpaceCount = htsector.GetNavSpaceCount();
				for( j=0; j<navSpaceCount; j++ ){
					meHeightTerrainNavSpace * const navspace = htsector.GetNavSpaceAt( j );
					if( ! navspace->GetNavSpaceChanged() ){
						continue;
					}
					
					refEntry.TakeOver( new meWCEntry( *this, meWCEntry::eetHTNavSpace ) );
					meWCEntry &entry = ( meWCEntry& )( igdeListItem& )refEntry;
					entry.SetHTNavSpace( navspace );
					entry.SetSector( decPoint3( scoord.x, 0, scoord.y ) );
					entry.UpdateText();
					pListChanges->AddItem( refEntry );
				}
			}
		}
		
	}catch( const deException &e ){
		pWindowMain.DisplayException( e );
	}
	
	pListChanges->SortItems();
}



void meWindowChangelog::SaveEntry( meWCEntry* ){
}

void meWindowChangelog::SaveAllEntries(){
}
