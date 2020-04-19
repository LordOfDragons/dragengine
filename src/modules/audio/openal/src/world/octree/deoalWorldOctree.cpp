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
#include "deoalWorldOctreeVisitor.h"

#include "../../component/deoalAComponent.h"
#include "../../environment/deoalEnvProbe.h"
#include "../../microphone/deoalAMicrophone.h"
#include "../../speaker/deoalASpeaker.h"
#include "../../soundLevelMeter/deoalASoundLevelMeter.h"
#include "../../utils/collision/deoalDCollisionBox.h"
#include "../../utils/collision/deoalDCollisionVolume.h"
#include "../../utils/collision/deoalDCollisionDetection.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/sound/deMicrophone.h>
#include <dragengine/resources/sound/deSpeaker.h>




// Class deoalWorldOctree
///////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoalWorldOctree::deoalWorldOctree( const decDVector &center, const decDVector &halfSize ) :
deoalDOctree( center, halfSize ){
}

deoalWorldOctree::~deoalWorldOctree(){
	// WARNING do NOT call RemoveAll*() here! destructor is used during deoalAWorld destructor
	//         which cleared octree content using PrepareQuickDispose() calls. by calling
	//         RemoveAll*() here objects are accessed which do not exist anymore. if somebody
	//         needs to clear the octree before deleting it ClearAll() has to be called
}



// Management
///////////////

deoalDOctree *deoalWorldOctree::CreateOctree( int octant ) const{
	const decDVector halfSize( GetHalfSize() * 0.5 );
	const decDVector &center = GetCenter();
	decDVector nc;
	
	if( ( octant & 4 ) == 4 ){
		nc.x = center.x + halfSize.x;
		
	}else{
		nc.x = center.x - halfSize.x;
	}
	
	if( ( octant & 2 ) == 2 ){
		nc.y = center.y + halfSize.y;
		
	}else{
		nc.y = center.y - halfSize.y;
	}
	
	if( ( octant & 1 ) == 1 ){
		nc.z = center.z + halfSize.z;
		
	}else{
		nc.z = center.z - halfSize.z;
	}
	
	return new deoalWorldOctree( nc, halfSize );
}

void deoalWorldOctree::ClearNodeContent(){
	RemoveAllSoundLevelMeters();
	RemoveAllMicrophones();
	RemoveAllComponents();
	RemoveAllSpeakers();
	RemoveAllEnvProbes();
}

void deoalWorldOctree::ClearComponents(){
	RemoveAllComponents();
	
	int i;
	for( i=0; i<8; i++ ){
		deoalDOctree * const node = GetNodeAt( i );
		if( node ){
			( ( deoalWorldOctree* )node )->ClearComponents();
		}
	}
}

void deoalWorldOctree::ClearMicrophones(){
	RemoveAllMicrophones();
	
	int i;
	for( i=0; i<8; i++ ){
		deoalDOctree * const node = GetNodeAt( i );
		if( node ){
			( ( deoalWorldOctree* )node )->ClearMicrophones();
		}
	}
}

void deoalWorldOctree::ClearSpeakers(){
	RemoveAllSpeakers();
	
	int i;
	for( i=0; i<8; i++ ){
		deoalDOctree * const node = GetNodeAt( i );
		if( node ){
			( ( deoalWorldOctree* )node )->ClearSpeakers();
		}
	}
}

void deoalWorldOctree::ClearEnvProbes(){
	RemoveAllEnvProbes();
	
	int i;
	for( i=0; i<8; i++ ){
		deoalDOctree * const node = GetNodeAt( i );
		if( node ){
			( ( deoalWorldOctree* )node )->ClearEnvProbes();
		}
	}
}

void deoalWorldOctree::ClearSoundLevelMeters(){
	RemoveAllSoundLevelMeters();
	
	int i;
	for( i=0; i<8; i++ ){
		deoalDOctree * const node = GetNodeAt( i );
		if( node ){
			( ( deoalWorldOctree* )node )->ClearSoundLevelMeters();
		}
	}
}

void deoalWorldOctree::ClearAll(){
	void ClearComponents();
	void ClearSpeakers();
	void ClearMicrophones();
	void ClearEnvProbes();
	void ClearSoundLevelMeters();
}

void deoalWorldOctree::InsertComponentIntoTree( deoalAComponent *component, int maxDepth ){
	if( ! component || maxDepth < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	const decDVector &minExtend = component->GetMinExtend();
	const decDVector &maxExtend = component->GetMaxExtend();
	const decDVector center( ( minExtend + maxExtend ) * 0.5 );
	const decDVector halfExtends( ( maxExtend - minExtend ) * 0.5 );
	
	deoalWorldOctree *currentNode = component->GetOctreeNode();
	
	// if component is already in the octree relocate it if required
	if( currentNode ){
		// if component is not in the same node as before relocate it
		if( ! currentNode->ContainsBox( center, halfExtends ) ){
			// remove from the current node
			currentNode->RemoveComponent( component );
			currentNode = NULL;
		}
	}
	
	// add component if not in tree
	if( ! currentNode ){
		currentNode = pGetNodeFor( center, halfExtends, maxDepth );
		currentNode->AddComponent( component );
	}
}

void deoalWorldOctree::InsertMicrophoneIntoTree( deoalAMicrophone *microphone, int maxDepth ){
	if( ! microphone || maxDepth < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	const decDVector &position = microphone->GetPosition();
	const decDVector halfExtends( 0.01, 0.01, 0.01 ); // microphone has no range
	
	deoalWorldOctree *currentNode = microphone->GetOctreeNode();
	
	// if microphone is already in the octree relocate it if required
	if( currentNode ){
		// if microphone is not in the same node as before relocate it
		if( ! currentNode->ContainsBox( position, halfExtends ) ){
			// remove from the current node
			currentNode->RemoveMicrophone( microphone );
			currentNode = NULL;
		}
	}
	
	// add microphone if not in tree
	if( ! currentNode ){
		currentNode = pGetNodeFor( position, halfExtends, maxDepth );
		currentNode->AddMicrophone( microphone );
	}
}

void deoalWorldOctree::InsertSpeakerIntoTree( deoalASpeaker *speaker, int maxDepth ){
	if( ! speaker || maxDepth < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	const double range = speaker->GetRange();
	const decDVector &position = speaker->GetPosition();
	const decDVector halfExtends( range, range, range );
	
	deoalWorldOctree *currentNode = speaker->GetOctreeNode();
	
	// if speaker is already in the octree relocate it if required
	if( currentNode ){
		// if speaker is not in the same node as before relocate it
		if( ! currentNode->ContainsBox( position, halfExtends ) ){
			// remove from the current node
			currentNode->RemoveSpeaker( speaker );
			currentNode = NULL;
		}
	}
	
	// add speaker if not in tree
	if( ! currentNode ){
		currentNode = pGetNodeFor( position, halfExtends, maxDepth );
		currentNode->AddSpeaker( speaker );
	}
}

void deoalWorldOctree::InsertEnvProbeIntoTree( deoalEnvProbe *envProbe, int maxDepth ){
	if( ! envProbe || maxDepth < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	const decDVector center( ( envProbe->GetMinExtend() + envProbe->GetMaxExtend() ) * 0.5 );
	const decDVector halfExtends( ( envProbe->GetMaxExtend() - envProbe->GetMinExtend() ) * 0.5 );
	
	deoalWorldOctree *currentNode = envProbe->GetOctreeNode();
	
	// if environment probe is already in the octree relocate it if required
	if( currentNode ){
		// if environment probe is not in the same node as before relocate it
		if( ! currentNode->ContainsBox( center, halfExtends ) ){
			// remove from the current node
			currentNode->RemoveEnvProbe( envProbe );
			currentNode = NULL;
		}
	}
	
	// add environment probe if not in tree
	if( ! currentNode ){
		currentNode = pGetNodeFor( center, halfExtends, maxDepth );
		currentNode->AddEnvProbe( envProbe );
	}
}

void deoalWorldOctree::InsertSoundLevelMeterIntoTree(
deoalASoundLevelMeter *soundLevelMeter, int maxDepth ){
	if( ! soundLevelMeter || maxDepth < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	const double range = soundLevelMeter->GetAudibleDistance();
	const decDVector &position = soundLevelMeter->GetPosition();
	const decDVector halfExtends( range, range, range );
	
	deoalWorldOctree *currentNode = soundLevelMeter->GetOctreeNode();
	
	// if soundLevelMeter is already in the octree relocate it if required
	if( currentNode ){
		// if soundLevelMeter is not in the same node as before relocate it
		if( ! currentNode->ContainsBox( position, halfExtends ) ){
			// remove from the current node
			currentNode->RemoveSoundLevelMeter( soundLevelMeter );
			currentNode = NULL;
		}
	}
	
	// add soundLevelMeter if not in tree
	if( ! currentNode ){
		currentNode = pGetNodeFor( position, halfExtends, maxDepth );
		currentNode->AddSoundLevelMeter( soundLevelMeter );
	}
}



// Components
///////////////

int deoalWorldOctree::GetComponentCount() const{
	return pComponents.GetCount();
}

deoalAComponent *deoalWorldOctree::GetComponentAt( int index ) const{
	return ( deoalAComponent* )pComponents.GetAt( index );
}

int deoalWorldOctree::IndexOfComponent( deoalAComponent *component ) const{
	return pComponents.IndexOf( component );
}

void deoalWorldOctree::AddComponent( deoalAComponent *component ){
	if( ! component /* || component->GetOctreeNode() */ ){
		DETHROW( deeInvalidParam );
	}
	
	pComponents.Add( component );
	component->SetOctreeNode( this );
}

void deoalWorldOctree::RemoveComponent( deoalAComponent *component ){
	pComponents.RemoveFrom( pComponents.IndexOf( component ) );
	component->SetOctreeNode( NULL );
}

void deoalWorldOctree::RemoveAllComponents(){
	int count = pComponents.GetCount();
	while( count > 0 ){
		count--;
		( ( deoalAComponent* )pComponents.GetAt( count ) )->SetOctreeNode( NULL );
	}
	pComponents.RemoveAll();
}



// Microphones
////////////////

int deoalWorldOctree::GetMicrophoneCount() const{
	return pMicrophones.GetCount();
}

deoalAMicrophone *deoalWorldOctree::GetMicrophoneAt( int index ) const{
	return ( deoalAMicrophone* )pMicrophones.GetAt( index );
}

int deoalWorldOctree::IndexOfMicrophone( deoalAMicrophone *microphone ) const{
	return pMicrophones.IndexOf( microphone );
}

void deoalWorldOctree::AddMicrophone( deoalAMicrophone *microphone ){
	if( ! microphone /* || microphone->GetOctreeNode() */ ){
		DETHROW( deeInvalidParam );
	}
	
	pMicrophones.Add( microphone );
	microphone->SetOctreeNode( this );
}

void deoalWorldOctree::RemoveMicrophone( deoalAMicrophone *microphone ){
	pMicrophones.RemoveFrom( pMicrophones.IndexOf( microphone ) );
	microphone->SetOctreeNode( NULL );
}

void deoalWorldOctree::RemoveAllMicrophones(){
	int count = pMicrophones.GetCount();
	while( count > 0 ){
		count--;
		( ( deoalAMicrophone* )pMicrophones.GetAt( count ) )->SetOctreeNode( NULL );
	}
	pMicrophones.RemoveAll();
}



// Speakers
/////////////

int deoalWorldOctree::GetSpeakerCount() const{
	return pSpeakers.GetCount();
}

deoalASpeaker *deoalWorldOctree::GetSpeakerAt( int index ) const{
	return ( deoalASpeaker* )pSpeakers.GetAt( index );
}

int deoalWorldOctree::IndexOfSpeaker( deoalASpeaker *speaker ) const{
	return pSpeakers.IndexOf( speaker );
}

void deoalWorldOctree::AddSpeaker( deoalASpeaker *speaker ){
	if( ! speaker || speaker->GetOctreeNode() ){
		DETHROW( deeInvalidParam );
	}
	
	pSpeakers.Add( speaker );
	speaker->SetOctreeNode( this );
}

void deoalWorldOctree::RemoveSpeaker( deoalASpeaker *speaker ){
	pSpeakers.RemoveFrom( pSpeakers.IndexOf( speaker ) );
	speaker->SetOctreeNode( NULL );
}

void deoalWorldOctree::RemoveAllSpeakers(){
	int count = pSpeakers.GetCount();
	while( count > 0 ){
		count--;
		( ( deoalASpeaker* )pSpeakers.GetAt( count ) )->SetOctreeNode( NULL );
	}
	pSpeakers.RemoveAll();
}



// EnvProbes
//////////////

int deoalWorldOctree::GetEnvProbeCount() const{
	return pEnvProbes.GetCount();
}

deoalEnvProbe *deoalWorldOctree::GetEnvProbeAt( int index ) const{
	return ( deoalEnvProbe* )pEnvProbes.GetAt( index );
}

int deoalWorldOctree::IndexOfEnvProbe( deoalEnvProbe *envProbe ) const{
	return pEnvProbes.IndexOf( envProbe );
}

void deoalWorldOctree::AddEnvProbe( deoalEnvProbe *envProbe ){
	if( ! envProbe /* || envProbe->GetOctreeNode() */ ){
		DETHROW( deeInvalidParam );
	}
	
	pEnvProbes.Add( envProbe );
	envProbe->SetOctreeNode( this );
}

void deoalWorldOctree::RemoveEnvProbe( deoalEnvProbe *envProbe ){
	try{
		pEnvProbes.RemoveFrom( pEnvProbes.IndexOf( envProbe ) );
	}catch( const deException &e ){
		e.PrintError();
	}
	envProbe->SetOctreeNode( NULL );
}

void deoalWorldOctree::RemoveAllEnvProbes(){
	int count = pEnvProbes.GetCount();
	while( count > 0 ){
		count--;
		( ( deoalEnvProbe* )pEnvProbes.GetAt( count ) )->SetOctreeNode( NULL );
	}
	pEnvProbes.RemoveAll();
}



// Sound level meters
///////////////////////

int deoalWorldOctree::GetSoundLevelMeterCount() const{
	return pSoundLevelMeters.GetCount();
}

deoalASoundLevelMeter *deoalWorldOctree::GetSoundLevelMeterAt( int index ) const{
	return ( deoalASoundLevelMeter* )pSoundLevelMeters.GetAt( index );
}

int deoalWorldOctree::IndexOfSoundLevelMeter( deoalASoundLevelMeter *soundLevelMeter ) const{
	return pSoundLevelMeters.IndexOf( soundLevelMeter );
}

void deoalWorldOctree::AddSoundLevelMeter( deoalASoundLevelMeter *soundLevelMeter ){
	if( ! soundLevelMeter || soundLevelMeter->GetOctreeNode() ){
		DETHROW( deeInvalidParam );
	}
	
	pSoundLevelMeters.Add( soundLevelMeter );
	soundLevelMeter->SetOctreeNode( this );
}

void deoalWorldOctree::RemoveSoundLevelMeter( deoalASoundLevelMeter *soundLevelMeter ){
	pSoundLevelMeters.RemoveFrom( pSoundLevelMeters.IndexOf( soundLevelMeter ) );
	soundLevelMeter->SetOctreeNode( NULL );
}

void deoalWorldOctree::RemoveAllSoundLevelMeters(){
	int count = pSoundLevelMeters.GetCount();
	while( count > 0 ){
		count--;
		( ( deoalASoundLevelMeter* )pSoundLevelMeters.GetAt( count ) )->SetOctreeNode( NULL );
	}
	pSoundLevelMeters.RemoveAll();
}



// Private Functions
//////////////////////

deoalWorldOctree *deoalWorldOctree::pGetNodeFor( const decDVector &center,
const decDVector &halfSize, int maxDepth ){
	deoalDOctree *curNode = this;
	int i;
	
	for( i=0; i<maxDepth; i++ ){
		deoalDOctree * const nextNode = curNode->GetNodeAtBox( center, halfSize );
		if( ! nextNode ){
			break;
		}
		curNode = nextNode;
	}
	
	return ( deoalWorldOctree* )curNode;
}

deoalWorldOctree *deoalWorldOctree::pGetNodeFor( const decDVector &position, int maxDepth ){
	deoalDOctree *curNode = this;
	int i;
	
	for( i=0; i<maxDepth; i++ ){
		const int octant = curNode->FindOctantAtPoint( position );
		if( octant == deoalDOctree::eoNotFound ){
			break;
		}
		
		deoalDOctree *nextNode = curNode->GetNodeAt( octant );
		if( ! nextNode ){
			nextNode = curNode->CreateOctree( octant );
			curNode->SetNodeAt( octant, nextNode );
		}
		
		curNode = nextNode;
	}
	
	return ( deoalWorldOctree* )curNode;
}
