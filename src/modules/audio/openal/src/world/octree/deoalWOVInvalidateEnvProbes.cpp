/* 
 * Drag[en]gine OpenAL Audio Module
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

#include "deoalWorldOctree.h"
#include "deoalWOVInvalidateEnvProbes.h"
#include "../../environment/deoalEnvProbe.h"

#include <dragengine/common/exceptions.h>



// Class deoalWOVInvalidateEnvProbes
//////////////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoalWOVInvalidateEnvProbes::deoalWOVInvalidateEnvProbes()
{
	SetVisitAll( false );
	SetVisitEnvProbes( true );
}

deoalWOVInvalidateEnvProbes::deoalWOVInvalidateEnvProbes( const decDVector &minExtend,
	const decDVector &maxExtend, const decLayerMask &layerMask ) :
pMinExtend( minExtend ),
pMaxExtend( maxExtend ),
pLayerMask( layerMask )
{
	SetVisitAll( false );
	SetVisitEnvProbes( true );
}

deoalWOVInvalidateEnvProbes::~deoalWOVInvalidateEnvProbes(){
}



// Visiting
/////////////

void deoalWOVInvalidateEnvProbes::SetExtends( const decDVector &minExtend, const decDVector &maxExtend ){
	pMinExtend = minExtend;
	pMaxExtend = maxExtend;
}

void deoalWOVInvalidateEnvProbes::SetLayerMask( const decLayerMask &layerMask ){
	pLayerMask = layerMask;
}



void deoalWOVInvalidateEnvProbes::VisitNode( deoalDOctree *node, int ){
	deoalWorldOctree &sonode = *( ( deoalWorldOctree* )node );
	int i, count = sonode.GetEnvProbeCount();
	
	// collect environment probes to remove
	pRemoveEnvProbes.RemoveAll();
	
	for( i=0; i<count; i++ ){
		deoalEnvProbe &envProbe = *sonode.GetEnvProbeAt( i );
		if( envProbe.GetLayerMask().MatchesNot( pLayerMask ) ){
			continue;
		}
		if( envProbe.GetMaxExtend() < pMinExtend || envProbe.GetMinExtend() > pMaxExtend ){
			continue;
		}
		pRemoveEnvProbes.Add( &envProbe );
	}
	
	// remove collected environment probes
	count = pRemoveEnvProbes.GetCount();
	for( i=0; i<count; i++ ){
		deoalEnvProbe * const envProbe = ( deoalEnvProbe* )pRemoveEnvProbes.GetAt( i );
		sonode.RemoveEnvProbe( envProbe );
		envProbe->SetOctreeNode( NULL );
		envProbe->Invalidate();
	}
}
