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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debnValueData.h"
#include "../deNetworkBasic.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/resources/network/deNetworkMessage.h>
#include <dragengine/resources/network/value/deNetworkValueData.h>



// Class debnValueData
////////////////////////

// Constructor, destructor
////////////////////////////

debnValueData::debnValueData( deNetworkValueData &valueData ) :
pValueData( valueData ),
pLastValue( NULL ),
pLastValueLen( 0 )
{
	SetDataType( evtData );
	
	if( valueData.GetLength() > 0 ){
		pLastValue = new uint8_t[ valueData.GetLength() ];
		memcpy( pLastValue, valueData.GetData(), valueData.GetLength() );
		pLastValueLen = valueData.GetLength();
	}
}

debnValueData::~debnValueData(){
	if( pLastValue ){
		delete [] pLastValue;
	}
}



// Management
///////////////

bool debnValueData::LastValueEqualsNetworkValue() const{
	if( pLastValueLen != pValueData.GetLength() ){
		return false;
	}
	
	if( pLastValue ){
		return pValueData.GetData() && memcmp( pLastValue, pValueData.GetData(), pLastValueLen ) == 0;
		
	}else{
		return ! pValueData.GetData();
	}
}

void debnValueData::SetLastValueFromNetworkValue(){
	if( pValueData.GetLength() > pLastValueLen ){
		if( pLastValue ){
			delete [] pLastValue;
			pLastValue = NULL;
			pLastValueLen = 0;
		}
		pLastValue = new uint8_t[ pValueData.GetLength() ];
	}
	
	pLastValueLen = pValueData.GetLength();
	memcpy( pLastValue, pValueData.GetData(), pLastValueLen );
}

bool debnValueData::UpdateValue( bool force ){
	if( ! force && LastValueEqualsNetworkValue() ){
		return false;
	}
	
	SetLastValueFromNetworkValue();
	return true;
}

void debnValueData::ReadValue( decBaseFileReader &reader ){
	pValueData.SetLength( reader.ReadUShort() );
	reader.Read( pValueData.GetData(), pValueData.GetLength() );
	SetLastValueFromNetworkValue();
}

void debnValueData::WriteValue( decBaseFileWriter &writer ){
	writer.WriteUShort( ( uint16_t )pValueData.GetLength() );
	writer.Write( pValueData.GetData(), pValueData.GetLength() );
}
