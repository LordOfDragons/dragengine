/* 
 * Drag[en]gine Basic Network Module
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

// includes
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "debnVisitorValueType.h"
#include "../deNetworkBasic.h"

#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/network/deNetworkMessage.h>
#include <dragengine/resources/network/deNetworkMessageWriter.h>
#include <dragengine/resources/network/value/deNetworkValue.h>
#include <dragengine/resources/network/value/deNetworkValueInteger.h>
#include <dragengine/resources/network/value/deNetworkValueFloat.h>
#include <dragengine/resources/network/value/deNetworkValueString.h>
#include <dragengine/resources/network/value/deNetworkValueData.h>
#include <dragengine/resources/network/value/deNetworkValuePoint2.h>
#include <dragengine/resources/network/value/deNetworkValuePoint3.h>
#include <dragengine/resources/network/value/deNetworkValueVector2.h>
#include <dragengine/resources/network/value/deNetworkValueVector3.h>
#include <dragengine/resources/network/value/deNetworkValueQuaternion.h>



// Class debnVisitorValueType
///////////////////////////////

// Constructor, destructor
////////////////////////////

debnVisitorValueType::debnVisitorValueType(){
	pType = 0;
	pMessage.TakeOver( new deNetworkMessage() );
	pWriter.TakeOver( new deNetworkMessageWriter( pMessage, false ) );
}

debnVisitorValueType::~debnVisitorValueType(){
}



// Management
///////////////



// Visiting
/////////////

void debnVisitorValueType::VisitValue( deNetworkValue *value ){
	DETHROW( deeInvalidParam );
}

void debnVisitorValueType::VisitInteger( deNetworkValueInteger *value ){
	//int absInt = abs( value->GetInt() );
	
	pWriter->SetPosition( 0 );
	pMessage->Clear();
	
	/*if( absInt < 128 ){
		pType = evtIntB8;
		pWriter->WriteChar( ( signed char )value->GetInt() );
		
	}else if( absInt < 32768 ){
		pType = evtIntB16;
		pWriter->WriteShort( ( signed short )value->GetInt() );
		
	}else{*/
		pType = evtSInt32;
		pWriter->WriteInt( ( int32_t )value->GetInt() );
	//}
}

void debnVisitorValueType::VisitFloat( deNetworkValueFloat *value ){
	pType = evtFloat32;
	
	pWriter->SetPosition( 0 );
	pMessage->Clear();
	
	pWriter->WriteFloat( ( float )value->GetFloat() );
}

void debnVisitorValueType::VisitString( deNetworkValueString *value ){
	pType = evtString;
	
	pWriter->SetPosition( 0 );
	pMessage->Clear();
	
	pWriter->WriteString16( value->GetString() );
}

void debnVisitorValueType::VisitData( deNetworkValueData *value ){
	int length = value->GetLength();
	
	pWriter->SetPosition( 0 );
	pMessage->Clear();
	
	pType = evtData;
	pWriter->WriteUInt( ( uint32_t )length );
	pWriter->Write( value->GetData(), length );
}

void debnVisitorValueType::VisitPoint2( deNetworkValuePoint2 *value ){
	const decPoint &point = value->GetPoint();
	//int absX = abs( point.x );
	//int absY = abs( point.y );
	
	pWriter->SetPosition( 0 );
	pMessage->Clear();
	
	/*if( absX < 128 && absY < 128 ){
		pType = evtPoint2B8;
		pWriter->WriteChar( ( signed char )point.x );
		pWriter->WriteChar( ( signed char )point.y );
		
	}else if( absX < 32768 && absY < 32768 ){
		pType = evtPoint2B16;
		pWriter->WriteShort( ( signed short )point.x );
		pWriter->WriteShort( ( signed short )point.y );
		
	}else{*/
		pType = evtPoint2S32;
		pWriter->WriteInt( ( int32_t )point.x );
		pWriter->WriteInt( ( int32_t )point.y );
	//}
}

void debnVisitorValueType::VisitPoint3( deNetworkValuePoint3 *value ){
	const decPoint3 &point = value->GetPoint();
	//int absX = abs( point.x );
	//int absY = abs( point.y );
	//int absZ = abs( point.z );
	
	pWriter->SetPosition( 0 );
	pMessage->Clear();
	
	/*if( absX < 128 && absY < 128 && absZ < 128 ){
		pType = evtPoint3B8;
		pWriter->WriteChar( ( signed char )point.x );
		pWriter->WriteChar( ( signed char )point.y );
		pWriter->WriteChar( ( signed char )point.z );
		
	}else if( absX < 32768 && absY < 32768 && absZ < 32768 ){
		pType = evtPoint3B16;
		pWriter->WriteShort( ( signed short )point.x );
		pWriter->WriteShort( ( signed short )point.y );
		pWriter->WriteShort( ( signed short )point.z );
		
	}else{*/
		pType = evtPoint3S32;
		pWriter->WriteInt( ( int32_t )point.x );
		pWriter->WriteInt( ( int32_t )point.y );
		pWriter->WriteInt( ( int32_t )point.z );
	//}
}

void debnVisitorValueType::VisitVector2( deNetworkValueVector2 *value ){
	const decVector2 &vector = value->GetVector();
	
	pWriter->SetPosition( 0 );
	pMessage->Clear();
	
	pType = evtVector2F32;
	pWriter->WriteFloat( vector.x );
	pWriter->WriteFloat( vector.y );
}

void debnVisitorValueType::VisitVector3( deNetworkValueVector3 *value ){
	const decDVector &vector = value->GetVector();
	
	pWriter->SetPosition( 0 );
	pMessage->Clear();
	
	pType = evtVector3F64;
	pWriter->WriteDouble( vector.x );
	pWriter->WriteDouble( vector.y );
	pWriter->WriteDouble( vector.z );
}

void debnVisitorValueType::VisitQuaternion( deNetworkValueQuaternion *value ){
	const decQuaternion &quaternion = value->GetQuaternion();
	
	pWriter->SetPosition( 0 );
	pMessage->Clear();
	
	pType = evtQuaternionF32;
	pWriter->WriteFloat( quaternion.x );
	pWriter->WriteFloat( quaternion.y );
	pWriter->WriteFloat( quaternion.z );
	pWriter->WriteFloat( quaternion.w );
}
