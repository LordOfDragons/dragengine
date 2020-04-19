/* 
 * Drag[en]gine JPEG Image Module
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

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileWriter.h>

// include last to avoid conflicts with INT32 typedef and windows
#include "deJpegEncoder.h"



// Class deJpegEncoder
////////////////////////

// Constructor, destructor
////////////////////////////

deJpegEncoder::deJpegEncoder( deJpegModule *module ){
	if( ! module ){
		DETHROW( deeInvalidParam );
	}
	
	pModule = module;
	
	pDataBuffer = NULL;
	pWriter = NULL;
	
	memset( &pErrorMgr, '\0', sizeof( pErrorMgr ) );
	memset( &pDestMgr, '\0', sizeof( pDestMgr ) );
	memset( &pEncode, '\0', sizeof( pEncode ) );
	pEncode.client_data = this;
	pEncode.err = &pErrorMgr;
}

deJpegEncoder::~deJpegEncoder(){
	if( pDataBuffer ){
		delete [] pDataBuffer;
	}
}



// Data Buffer
////////////////

void deJpegEncoder::InitWrite( decBaseFileWriter *writer ){
	if( ! writer ){
		DETHROW( deeInvalidParam );
	}
	
	pWriter = writer;
	
	if( ! pDataBuffer ){
		pDataBuffer = new JOCTET[ 1024 ];
	}
	
	pEncode.dest = &pDestMgr;
}

void deJpegEncoder::ResetBuffer(){
	pDestMgr.next_output_byte = pDataBuffer;
	pDestMgr.free_in_buffer = 1024;
}

void deJpegEncoder::WriteEntireBuffer(){
	pWriter->Write( pDataBuffer, 1024 );
	ResetBuffer();
}

void deJpegEncoder::WriteRemaining(){
	const int bytes = 1024 - ( int )pDestMgr.free_in_buffer;
	
	if( bytes > 0 ){
		pWriter->Write( pDataBuffer, bytes );
	}
	
	pDestMgr.next_output_byte = NULL;
	pDestMgr.free_in_buffer = 0;
}

void deJpegEncoder::CloseReader(){
	if( pDataBuffer ){
		delete [] pDataBuffer;
		pDataBuffer = NULL;
	}
	pWriter = NULL;
}
