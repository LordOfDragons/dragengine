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

#include "debnValueVector2.h"
#include "../deNetworkBasic.h"
#include "../half/half.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/resources/network/deNetworkMessage.h>
#include <dragengine/resources/network/value/deNetworkValueVector2.h>



// Class debnValueVector2
///////////////////////////

// Constructor, destructor
////////////////////////////

debnValueVector2::debnValueVector2( deNetworkValueVector2 &valueVector2 ) :
pValueVector2( valueVector2 ),
pLastValue( valueVector2.GetVector() )
{
	switch( valueVector2.GetFormat() ){
	case deNetworkValue::evfFloat16:
		SetDataType( evtVector2F16 );
		break;
		
	case deNetworkValue::evfFloat32:
		SetDataType( evtVector2F32 );
		break;
		
	case deNetworkValue::evfFloat64:
		SetDataType( evtVector2F64 );
		break;
		
	default:
		DETHROW( deeInvalidParam );
	}
}

debnValueVector2::~debnValueVector2(){
}



// Management
///////////////

bool debnValueVector2::UpdateValue( bool force ){
	if( ! force && pValueVector2.GetVector().IsEqualTo( pLastValue, ( float )pValueVector2.GetPrecision() ) ){
		return false;
	}
	
	pLastValue = pValueVector2.GetVector();
	return true;
}

void debnValueVector2::ReadValue( decBaseFileReader &reader ){
	switch( GetDataType() ){
	case evtVector2F16:
		pLastValue.x = halfToFloat( reader.ReadUShort() );
		pLastValue.y = halfToFloat( reader.ReadUShort() );
		break;
		
	case evtVector2F32:
		pLastValue.x = reader.ReadFloat();
		pLastValue.y = reader.ReadFloat();
		break;
		
	case evtVector2F64:
		pLastValue.x = ( float )reader.ReadDouble();
		pLastValue.y = ( float )reader.ReadDouble();
		break;
		
	default:
		return;
	}
	
	pValueVector2.SetVector( pLastValue );
}

void debnValueVector2::WriteValue( decBaseFileWriter &writer ){
	const decVector2 &vector = pValueVector2.GetVector();
	
	switch( GetDataType() ){
	case evtVector2F16:
		writer.WriteUShort( floatToHalf( ( float )vector.x ) );
		writer.WriteUShort( floatToHalf( ( float )vector.y ) );
		break;
		
	case evtVector2F32:
		writer.WriteFloat( ( float )vector.x );
		writer.WriteFloat( ( float )vector.y );
		break;
		
	case evtVector2F64:
		writer.WriteDouble( vector.x );
		writer.WriteDouble( vector.y );
		break;
		
	default:
		break;
	}
}
