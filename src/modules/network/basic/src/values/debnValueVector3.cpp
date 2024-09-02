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

#include <stdio.h>
#include <stdlib.h>

#include "debnValueVector3.h"
#include "../deNetworkBasic.h"
#include "../half/half.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/resources/network/deNetworkMessage.h>
#include <dragengine/resources/network/value/deNetworkValueVector3.h>



// Class debnValueVector3
///////////////////////////

// Constructor, destructor
////////////////////////////

debnValueVector3::debnValueVector3( deNetworkValueVector3 &valueVector3 ) :
pValueVector3( valueVector3 ),
pLastValue( valueVector3.GetVector() )
{
	switch( valueVector3.GetFormat() ){
	case deNetworkValue::evfFloat16:
		SetDataType( evtVector3F16 );
		break;
		
	case deNetworkValue::evfFloat32:
		SetDataType( evtVector3F32 );
		break;
		
	case deNetworkValue::evfFloat64:
		SetDataType( evtVector3F64 );
		break;
		
	default:
		DETHROW( deeInvalidParam );
	}
}

debnValueVector3::~debnValueVector3(){
}



// Management
///////////////

bool debnValueVector3::UpdateValue( bool force ){
	if( ! force && pValueVector3.GetVector().IsEqualTo( pLastValue, pValueVector3.GetPrecision() ) ){
		return false;
	}
	
	pLastValue = pValueVector3.GetVector();
	return true;
}

void debnValueVector3::ReadValue( decBaseFileReader &reader ){
	switch( GetDataType() ){
	case evtVector3F16:
		pLastValue.x = halfToFloat( reader.ReadUShort() );
		pLastValue.y = halfToFloat( reader.ReadUShort() );
		pLastValue.z = halfToFloat( reader.ReadUShort() );
		break;
		
	case evtVector3F32:
		pLastValue.x = reader.ReadFloat();
		pLastValue.y = reader.ReadFloat();
		pLastValue.z = reader.ReadFloat();
		break;
		
	case evtVector3F64:
		pLastValue.x = reader.ReadDouble();
		pLastValue.y = reader.ReadDouble();
		pLastValue.z = reader.ReadDouble();
		break;
		
	default:
		return;
	}
	
	pValueVector3.SetVector( pLastValue );
}

void debnValueVector3::WriteValue( decBaseFileWriter &writer ){
	const decDVector &vector = pValueVector3.GetVector();
	
	switch( GetDataType() ){
	case evtVector3F16:
		writer.WriteUShort( floatToHalf( ( float )vector.x ) );
		writer.WriteUShort( floatToHalf( ( float )vector.y ) );
		writer.WriteUShort( floatToHalf( ( float )vector.z ) );
		break;
		
	case evtVector3F32:
		writer.WriteFloat( ( float )vector.x );
		writer.WriteFloat( ( float )vector.y );
		writer.WriteFloat( ( float )vector.z );
		break;
		
	case evtVector3F64:
		writer.WriteDouble( vector.x );
		writer.WriteDouble( vector.y );
		writer.WriteDouble( vector.z );
		break;
		
	default:
		break;
	}
}
