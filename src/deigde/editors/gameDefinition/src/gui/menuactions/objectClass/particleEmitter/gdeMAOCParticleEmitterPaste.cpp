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

#include "gdeMAOCParticleEmitterPaste.h"
#include "../../../gdeWindowMain.h"
#include "../../../../clipboard/gdeClipboardDataOCParticleEmitter.h"
#include "../../../../gamedef/gdeGameDefinition.h"
#include "../../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../../gamedef/objectClass/particleemitter/gdeOCParticleEmitter.h"
#include "../../../../undosys/objectClass/particleemitter/gdeUOCAddParticleEmitter.h"

#include <deigde/clipboard/igdeClipboardDataReference.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class gdeMAOCParticleEmitterPaste
////////////////////////////////

// Constructor
////////////////

gdeMAOCParticleEmitterPaste::gdeMAOCParticleEmitterPaste( gdeWindowMain &windowMain ) :
gdeBaseMAOCSubObject( windowMain, "Paste Object Class Particle Emitter",
	windowMain.GetEnvironment().GetStockIcon( igdeEnvironment::esiPaste ),
	"Paste object class particle emitter" )
{
}



// Management
///////////////

igdeUndo *gdeMAOCParticleEmitterPaste::OnActionSubObject( gdeGameDefinition&, gdeObjectClass &objectClass ){
	igdeClipboardDataReference clip( pWindowMain.GetClipboard()
		.GetWithTypeName( gdeClipboardDataOCParticleEmitter::TYPE_NAME ) );
	if( ! clip ){
		return NULL;
	}
	
	const gdeClipboardDataOCParticleEmitter &clipOCParticleEmitter =
		( const gdeClipboardDataOCParticleEmitter & )( igdeClipboardData& )clip;
	
	deObjectReference particleEmitter;
	particleEmitter.TakeOver( new gdeOCParticleEmitter( *clipOCParticleEmitter.GetParticleEmitter() ) );
	
	igdeUndo * const undo = new gdeUOCAddParticleEmitter( &objectClass,
		( gdeOCParticleEmitter* )( deObject* )particleEmitter );
	undo->SetShortInfo( "Paste object class particle emitter" );
	return undo;
}

void gdeMAOCParticleEmitterPaste::Update(){
	SetEnabled( GetActiveObjectClass() != NULL
		&& pWindowMain.GetClipboard().HasWithTypeName( gdeClipboardDataOCParticleEmitter::TYPE_NAME ) );
}
