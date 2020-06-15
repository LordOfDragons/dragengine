/* 
 * FBX Model Module
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

#include "fbxNode.h"
#include "fbxProperty.h"
#include "property/fbxPropertyBool.h"
#include "property/fbxPropertyDouble.h"
#include "property/fbxPropertyFloat.h"
#include "property/fbxPropertyInteger.h"
#include "property/fbxPropertyLong.h"
#include "property/fbxPropertyShort.h"
#include "property/fbxPropertyString.h"

#include <dragengine/deEngine.h>
#include <dragengine/deObjectReference.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decStringSet.h>
#include <dragengine/systems/modules/deBaseModule.h>



// Class fbxNode
///////////////////

// Constructor, destructor
////////////////////////////

fbxNode::fbxNode() :
pNodeProperties70( NULL ){
}

fbxNode::fbxNode( decBaseFileReader &reader ) :
pNodeProperties70( NULL )
{
	pRead( reader, reader.ReadUInt() );
}

fbxNode::fbxNode( decBaseFileReader &reader, int endOffset ) :
pNodeProperties70( NULL )
{
	pRead( reader, endOffset );
}

fbxNode::~fbxNode(){
}



// Loading and Saving
///////////////////////

void fbxNode::SetName( const char *name ){
	pName = name;
}



int fbxNode::GetPropertyCount() const{
	return pProperties.GetCount();
}

fbxProperty *fbxNode::GetPropertyAt( int index ) const{
	return ( fbxProperty* )pProperties.GetAt( index );
}



bool fbxNode::GetPropertyBool( const char *name, bool &value ) const{
	const fbxNode * const node = pProp70Named( name );
	if( node ){
		value = node->GetPropertyAt( 4 )->GetValueAsBool();
		return true;
		
	}else{
		return false;
	}
}

bool fbxNode::GetPropertyInt( const char *name, int &value ) const{
	const fbxNode * const node = pProp70Named( name );
	if( node ){
		value = node->GetPropertyAt( 4 )->GetValueAsInt();
		return true;
		
	}else{
		return false;
	}
}

bool fbxNode::GetPropertyFloat( const char *name, float &value ) const{
	const fbxNode * const node = pProp70Named( name );
	if( node ){
		value = node->GetPropertyAt( 4 )->GetValueAsFloat();
		return true;
		
	}else{
		return false;
	}
}

bool fbxNode::GetPropertyDouble( const char *name, double &value ) const{
	const fbxNode * const node = pProp70Named( name );
	if( node ){
		value = node->GetPropertyAt( 4 )->GetValueAsDouble();
		return true;
		
	}else{
		return false;
	}
}

bool fbxNode::GetPropertyString( const char *name, decString &value ) const{
	const fbxNode * const node = pProp70Named( name );
	if( node ){
		value = node->GetPropertyAt( 4 )->CastString().GetValue();
		return true;
		
	}else{
		return false;
	}
}



bool fbxNode::GetPropertyBool( const char *name ) const{
	const fbxNode * const node = pProp70Named( name );
	if( ! node ){
		DETHROW_INFO( deeInvalidParam, decString( "named property not found: " ) + name );
	}
	return node->GetPropertyAt( 4 )->GetValueAsBool();
}

int fbxNode::GetPropertyInt( const char *name ) const{
	const fbxNode * const node = pProp70Named( name );
	if( ! node ){
		DETHROW_INFO( deeInvalidParam, decString( "named property not found: " ) + name );
	}
	return node->GetPropertyAt( 4 )->GetValueAsInt();
}

float fbxNode::GetPropertyFloat( const char *name ) const{
	const fbxNode * const node = pProp70Named( name );
	if( ! node ){
		DETHROW_INFO( deeInvalidParam, decString( "named property not found: " ) + name );
	}
	return node->GetPropertyAt( 4 )->GetValueAsFloat();
}

double fbxNode::GetPropertyDouble( const char *name ) const{
	const fbxNode * const node = pProp70Named( name );
	if( ! node ){
		DETHROW_INFO( deeInvalidParam, decString( "named property not found: " ) + name );
	}
	return node->GetPropertyAt( 4 )->GetValueAsDouble();
}

const decString &fbxNode::GetPropertyString( const char *name ) const{
	const fbxNode * const node = pProp70Named( name );
	if( ! node ){
		DETHROW_INFO( deeInvalidParam, decString( "named property not found: " ) + name );
	}
	return node->GetPropertyAt( 4 )->CastString().GetValue();
}



bool fbxNode::GetPropertyBool( const char *name, bool defaultValue ) const{
	const fbxNode * const node = pProp70Named( name );
	return node ? node->GetPropertyAt( 4 )->GetValueAsBool() : defaultValue;
}

int fbxNode::GetPropertyInt( const char *name, int defaultValue ) const{
	const fbxNode * const node = pProp70Named( name );
	return node ? node->GetPropertyAt( 4 )->GetValueAsInt() : defaultValue;
}

float fbxNode::GetPropertyFloat( const char *name, float defaultValue ) const{
	const fbxNode * const node = pProp70Named( name );
	return node ? node->GetPropertyAt( 4 )->GetValueAsFloat() : defaultValue;
}

double fbxNode::GetPropertyDouble( const char *name, double defaultValue ) const{
	const fbxNode * const node = pProp70Named( name );
	return node ? node->GetPropertyAt( 4 )->GetValueAsDouble() : defaultValue;
}

const decString &fbxNode::GetPropertyString( const char *name, const decString &defaultValue ) const{
	const fbxNode * const node = pProp70Named( name );
	return node ? node->GetPropertyAt( 4 )->CastString().GetValue() : defaultValue;
}



int fbxNode::GetNodeCount() const{
	return pNodes.GetCount();
}

fbxNode *fbxNode::GetNodeAt( int index ) const{
	return ( fbxNode* )pNodes.GetAt( index );
}

void fbxNode::AddNode( fbxNode *node ){
	pNodes.Add( node );
}

fbxNode *fbxNode::FirstNodeNamed( const char* name ) const{
	const int count = pNodes.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		fbxNode * const node = ( fbxNode* )pNodes.GetAt( i );
		if( node->GetName() == name ){
			return node;
		}
	}
	
	return NULL;
}

void fbxNode::FindNodesNamed( decPointerList &list, const char *name ) const{
	const int count = pNodes.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		fbxNode * const node = ( fbxNode* )pNodes.GetAt( i );
		if( node->GetName() == name ){
			list.Add( node );
		}
	}
}

void fbxNode::GetNodeNames( decStringSet &list ) const{
	const int count = pNodes.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		list.Add( ( ( fbxNode* )pNodes.GetAt( i ) )->GetName() );
	}
}



void fbxNode::Prepare( deBaseModule &module ){
	const int nodeCount = pNodes.GetCount();
	int i;
	
	for( i=0; i<nodeCount; i++ ){
		GetNodeAt( i )->Prepare( module );
	}
	
	pNodeProperties70 = FirstNodeNamed( "Properties70" );
	if( pNodeProperties70 ){
		const int count = pNodeProperties70->GetNodeCount();
		int i;
		for( i=0; i<count; i++ ){
			const fbxNode &node = *pNodeProperties70->GetNodeAt( i );
			if( node.GetName() != "P" ){
				DETHROW_INFO( deeInvalidFileFormat, "properties70: entry is not named 'P'" );
			}
			if( node.GetPropertyCount() < 4 /*5*/ ){
				DETHROW_INFO( deeInvalidFileFormat, "properties70: entry has less than 4 properties" );
			}
		}
	}
}



void fbxNode::Save(decBaseFileWriter &writer ){
}

void fbxNode::DebugPrintStructure( deBaseModule &module, const decString &prefix, bool verbose ) const{
	const int propertyCount = pProperties.GetCount();
	const decString childPrefix( prefix + "  " );
	const int nodeCount = pNodes.GetCount();
	int i;
	
	if( verbose ){
		module.LogInfoFormat( "%sNode (%s):", prefix.GetString(), pName.GetString() );
		
		for( i=0; i<propertyCount; i++ ){
			GetPropertyAt( i )->DebugPrintStructure( module, childPrefix );
		}
		
		for( i=0; i<nodeCount; i++ ){
			GetNodeAt( i )->DebugPrintStructure( module, childPrefix, true );
		}
		
	}else{
		decStringList properties;
		for( i=0; i<propertyCount; i++ ){
			properties.Add( GetPropertyAt( i )->DebugTypeName() );
		}
		module.LogInfoFormat( "%sNode (%s): %s", prefix.GetString(),
			pName.GetString(), properties.Join( ", " ).GetString() );
		
		for( i=0; i<nodeCount; i++ ){
			GetNodeAt( i )->DebugPrintStructure( module, childPrefix, false );
		}
	}
}



// Private Functions
//////////////////////

void fbxNode::pRead( decBaseFileReader &reader, int endOffset ){
	const int countProperties = reader.ReadUInt();
	const int propertyListByteCount = reader.ReadUInt();
	pName = reader.ReadString8();
	
	const int endOfPropertyList = reader.GetPosition() + propertyListByteCount;
	
	int i;
	deObjectReference property;
	for( i=0; i<countProperties; i++ ){
		property.TakeOver( fbxProperty::Read( reader ) );
		pProperties.Add( property );
	}
	
	const int position = reader.GetPosition();
	if( position != endOfPropertyList ){
		DETHROW_INFO( deeInvalidFileFormat, "reader.position > endOfPropertyList" );
	}
	
	// the presence of this list depends on the fact that there are surplus bytes in the
	// record. the count of nodes in the nested list is not stored. instead we have to
	// read until we find a zero-record. this is a record with end offset, property count,
	// property list byte count and name length all being 0 (or said differently the next
	// 13 bytes are all 0). we do this by reading the end offset and if this is 0 we
	// consider this to be the end of node list marker
	if( position > endOffset ){
		DETHROW_INFO( deeInvalidFileFormat, "reader.position > endOffset" );
	}
	if( position == endOffset ){
		return; // no node list
	}
	
	deObjectReference node;
	while( true ){
		const int checkEndOffset = reader.ReadUInt();
		if( checkEndOffset == 0 ){
			// end of list marker
			if( reader.GetPosition() + 9 != endOffset ){
				DETHROW_INFO( deeInvalidFileFormat, "reader.position + 9 > endOffset" );
			}
			
			reader.SetPosition( endOffset );
			break;
		}
		
		node.TakeOver( new fbxNode( reader, checkEndOffset ) );
		pNodes.Add( node );
	}
}


fbxNode *fbxNode::pProp70Named( const char* name ) const{
	if( ! pNodeProperties70 ){
		return NULL;
	}
	
	const int count = pNodeProperties70->GetNodeCount();
	int i;
	
	for( i=0; i<count; i++ ){
		fbxNode * const node = pNodeProperties70->GetNodeAt( i );
		if( node->GetPropertyAt( 0 )->CastString().GetValue() == name ){
			return node;
		}
	}
	
	return NULL;
}
