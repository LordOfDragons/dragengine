/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
