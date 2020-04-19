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
#include "../../world/meWorld.h"
#include "../../world/terrain/meHeightTerrain.h"
#include "../../world/terrain/meHeightTerrainSector.h"
#include "../../world/terrain/meHeightTerrainTexture.h"
#include "../../world/terrain/meHeightTerrainNavSpace.h"

#include <dragengine/common/exceptions.h>



// Class meWCEntry
////////////////////

// Constructor, destructor
////////////////////////////

meWCEntry::meWCEntry( meWindowChangelog &windowChangelog, eElementTypes type ) :
igdeListItem( "" ),
pWindowChangelog( windowChangelog ),
pType( type ),
pWorld( NULL ),
pHTTexture( NULL ),
pHTNavSpace( NULL )
{
	if( type < eetWorld || type > eetHTPFCache ){
		DETHROW( deeInvalidParam );
	}
}

meWCEntry::~meWCEntry(){
	SetHTNavSpace( NULL );
	SetHTTexture( NULL );
	SetWorld( NULL );
}



// Management
///////////////

void meWCEntry::SetSector( const decPoint3 &sector ){
	pSector = sector;
}

void meWCEntry::SetWorld( meWorld *world ){
	if( world == pWorld ){
		return;
	}
	
	if( pWorld ){
		pWorld->FreeReference();
	}
	
	pWorld = world;
	
	if( world ){
		world->AddReference();
	}
}

void meWCEntry::SetHTTexture( meHeightTerrainTexture *texture ){
	if( texture == pHTTexture ){
		return;
	}
	
	if( pHTTexture ){
		pHTTexture->FreeReference();
	}
	
	pHTTexture = texture;
	
	if( texture ){
		texture->AddReference();
	}
}

void meWCEntry::SetHTNavSpace( meHeightTerrainNavSpace *navspace ){
	if( navspace == pHTNavSpace ){
		return;
	}
	
	if( pHTNavSpace ){
		pHTNavSpace->FreeReference();
	}
	
	pHTNavSpace = navspace;
	
	if( navspace ){
		navspace->AddReference();
	}
}



void meWCEntry::UpdateText(){
	const char * const textUnsaved = "< not saved yet >";
	const char * const textRemoved = "< removed >";
	decStringList &details = GetDetails();
	decString text;
	
	details.RemoveAll();
	
	switch( pType ){
	case eetWorld:
		text = "-";
		details.Add( "World" );
		
		if( pWorld->GetSaved() ){
			details.Add( pWorld->GetFilePath() );
			
		}else{
			details.Add( textUnsaved );
		}
		break;
		
	case eetHeightTerrain: {
		text = "-";
		details.Add( "Height Terrain" );
		
		if( pWorld->GetHeightTerrain()->GetSaved() ){
			if( pWorld->GetHeightTerrain()->GetPathHT().IsEmpty() ){
				details.Add( textRemoved );
				
			}else{
				details.Add( pWorld->GetHeightTerrain()->GetPathHT() );
			}
			
		}else{
			details.Add( textUnsaved );
		}
		} break;
		
	case eetHTHeight: {
		const meHeightTerrainSector * const htsector =
			pWorld->GetHeightTerrain()->GetSectorWith( decPoint( pSector.x, pSector.z ) );
		
		text.Format( "(%d,%d)", pSector.x, pSector.z );
		details.Add( "Height Image" );
		
		if( htsector && htsector->GetHeightImageSaved() ){
			if( htsector->GetPathHeightImage().IsEmpty() ){
				details.Add( textRemoved );
				
			}else{
				details.Add( htsector->GetPathHeightImage() );
			}
			
		}else{
			details.Add( textUnsaved );
		}
		} break;
		
	case eetHTVisibility: {
		const meHeightTerrainSector * const htsector =
			pWorld->GetHeightTerrain()->GetSectorWith( decPoint( pSector.x, pSector.z ) );
		
		text.Format( "(%d,%d)", pSector.x, pSector.z );
		details.Add( "Visibility Image" );
		
		if( htsector && htsector->GetVisibilitySaved() ){
			if( htsector->GetPathVisibilityImage().IsEmpty() ){
				details.Add( textRemoved );
				
			}else{
				details.Add( htsector->GetPathVisibilityImage() );
			}
			
		}else{
			details.Add( textUnsaved );
		}
		} break;
		
	case eetHTTextureMask:
		text.Format( "(%d,%d)", pSector.x, pSector.z );
		details.Add( "Texture Mask" );
		
		if( pHTTexture->GetMaskSaved() ){
			if( pHTTexture->GetPathMask().IsEmpty() ){
				details.Add( textRemoved );
				
			}else{
				details.Add( pHTTexture->GetPathMask() );
			}
			
		}else{
			details.Add( textUnsaved );
		}
		break;
		
	case meWCEntry::eetHTNavSpace:
		text = "-";
		details.Add( "Navigation Space" );
		
		if( pHTNavSpace->GetNavSpaceSaved() ){
			if( pHTNavSpace->GetPathNavSpace().IsEmpty() ){
				details.Add( textRemoved );
				
			}else{
				details.Add( pHTNavSpace->GetPathNavSpace() );
			}
			
		}else{
			details.Add( textUnsaved );
		}
		break;
		
	case eetHTPFCache: {
		const meHeightTerrainSector * const htsector =
			pWorld->GetHeightTerrain()->GetSectorWith( decPoint( pSector.x, pSector.z ) );
		
		text.Format( "(%d,%d)", pSector.x, pSector.z );
		details.Add( "Prop Field Cache" );
		
		if( htsector && htsector->GetPFCacheSaved() ){
			if( htsector->GetPathPFCacheImage().IsEmpty() ){
				details.Add( textRemoved );
				
			}else{
				details.Add( htsector->GetPathPFCacheImage() );
			}
			
		}else{
			details.Add( textUnsaved );
		}
		} break;
	}
	
	SetText( text );
}
