/* 
 * Drag[en]gine DragonScript Script Module
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
#include <stdint.h>
#include <string.h>

#include "deClassFileWriter.h"
#include "deClassFileReader.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/string/decString.h>
#include <libdscript/exceptions.h>



struct sFileWriterNatDat{
	decBaseFileWriter *fileWriter;
	int streamVersion;
};



// Constructors, Destructors
//////////////////////////////

// public func new( String filename )
deClassFileWriter::nfNew::nfNew( const sInitData &init ) : dsFunction( init.clsFileWriter,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsString ); // filename
}
void deClassFileWriter::nfNew::RunFunction( dsRunTime *rt, dsValue *myself ){
	sFileWriterNatDat &nd = *( ( sFileWriterNatDat* )p_GetNativeData( myself ) );
	const deClassFileWriter &clsFileWriter = *( ( deClassFileWriter* )GetOwnerClass() );
	deVirtualFileSystem &vfs = *clsFileWriter.GetDS()->GetGameEngine()->GetVirtualFileSystem();
	decPath path;
	
	// clear ( important )
	nd.fileWriter = NULL;
	nd.streamVersion = clsFileWriter.GetDS()->GetStreamingVersion();
	
	// create file writer
	path.SetFromUnix( rt->GetValue( 0 )->GetString() );
	nd.fileWriter = vfs.OpenFileForWriting( path );
}

// public func destructor()
deClassFileWriter::nfDestructor::nfDestructor( const sInitData &init ) : dsFunction( init.clsFileWriter,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassFileWriter::nfDestructor::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sFileWriterNatDat &nd = *( ( sFileWriterNatDat* )p_GetNativeData( myself ) );
	
	if( nd.fileWriter ){
		nd.fileWriter->FreeReference();
		nd.fileWriter = NULL;
	}
}



// Management
///////////////

// public func String getFilename()
deClassFileWriter::nfGetFilename::nfGetFilename( const sInitData &init ) : dsFunction( init.clsFileWriter,
"getFilename", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsString ){
}
void deClassFileWriter::nfGetFilename::RunFunction( dsRunTime *rt, dsValue *myself ){
	decBaseFileWriter &fileWriter = *( ( ( const sFileWriterNatDat * )p_GetNativeData( myself ) )->fileWriter );
	
	rt->PushString( fileWriter.GetFilename() );
}



// public func int getStreamVersion()
deClassFileWriter::nfGetStreamVersion::nfGetStreamVersion( const sInitData &init ) : dsFunction( init.clsFileWriter,
"getStreamVersion", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassFileWriter::nfGetStreamVersion::RunFunction( dsRunTime *rt, dsValue *myself ){
	rt->PushInt( ( ( const sFileWriterNatDat * )p_GetNativeData( myself ) )->streamVersion );
}

// public func void setStreamVersion( int version )
deClassFileWriter::nfSetStreamVersion::nfSetStreamVersion( const sInitData &init ) : dsFunction( init.clsFileWriter,
"setStreamVersion", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // version
}
void deClassFileWriter::nfSetStreamVersion::RunFunction( dsRunTime *rt, dsValue *myself ){
	( ( sFileWriterNatDat* )p_GetNativeData( myself ) )->streamVersion = rt->GetValue( 0 )->GetInt();
}




// public func void writeChar( int value )
deClassFileWriter::nfWriteChar::nfWriteChar( const sInitData &init ) : dsFunction( init.clsFileWriter,
"writeChar", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // value
}
void deClassFileWriter::nfWriteChar::RunFunction( dsRunTime *rt, dsValue *myself ){
	decBaseFileWriter &fileWriter = *( ( ( const sFileWriterNatDat * )p_GetNativeData( myself ) )->fileWriter );
	
	const int value = rt->GetValue( 0 )->GetInt();
	if( value < -128 || value > 127 ){
		DSTHROW( dueInvalidParam );
	}
	
	fileWriter.WriteChar( ( int8_t )value );
}

// public func void writeByte( int value )
deClassFileWriter::nfWriteByte::nfWriteByte( const sInitData &init ) : dsFunction( init.clsFileWriter,
"writeByte", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // value
}
void deClassFileWriter::nfWriteByte::RunFunction( dsRunTime *rt, dsValue *myself ){
	decBaseFileWriter &fileWriter = *( ( ( const sFileWriterNatDat * )p_GetNativeData( myself ) )->fileWriter );
	
	const int value = rt->GetValue( 0 )->GetInt();
	if( value < 0 || value > 255 ){
		DSTHROW( dueInvalidParam );
	}
	
	fileWriter.WriteByte( ( uint8_t )value );
}

// public func void writeShort( int value )
deClassFileWriter::nfWriteShort::nfWriteShort( const sInitData &init ) : dsFunction( init.clsFileWriter,
"writeShort", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // value
}
void deClassFileWriter::nfWriteShort::RunFunction( dsRunTime *rt, dsValue *myself ){
	decBaseFileWriter &fileWriter = *( ( ( const sFileWriterNatDat * )p_GetNativeData( myself ) )->fileWriter );
	
	const int value = rt->GetValue( 0 )->GetInt();
	if( value < -32768 || value > 32767 ){
		DSTHROW( dueInvalidParam );
	}
	
	fileWriter.WriteShort( ( int16_t )value );
}

// public func void writeUShort( int value )
deClassFileWriter::nfWriteUShort::nfWriteUShort( const sInitData &init ) : dsFunction( init.clsFileWriter,
"writeUShort", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // value
}
void deClassFileWriter::nfWriteUShort::RunFunction( dsRunTime *rt, dsValue *myself ){
	decBaseFileWriter &fileWriter = *( ( ( const sFileWriterNatDat * )p_GetNativeData( myself ) )->fileWriter );
	
	const int value = rt->GetValue( 0 )->GetInt();
	if( value < 0 || value > 65535 ){
		DSTHROW( dueInvalidParam );
	}
	
	fileWriter.WriteUShort( ( uint16_t )value );
}

// public func void writeInt( int value )
deClassFileWriter::nfWriteInt::nfWriteInt( const sInitData &init ) : dsFunction( init.clsFileWriter,
"writeInt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // value
}
void deClassFileWriter::nfWriteInt::RunFunction( dsRunTime *rt, dsValue *myself ){
	decBaseFileWriter &fileWriter = *( ( ( const sFileWriterNatDat * )p_GetNativeData( myself ) )->fileWriter );
	
	fileWriter.WriteInt( ( int32_t )rt->GetValue( 0 )->GetInt() );
}

// public func void writeUInt( int value )
deClassFileWriter::nfWriteUInt::nfWriteUInt( const sInitData &init ) : dsFunction( init.clsFileWriter,
"writeUInt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // value
}
void deClassFileWriter::nfWriteUInt::RunFunction( dsRunTime *rt, dsValue *myself ){
	decBaseFileWriter &fileWriter = *( ( ( const sFileWriterNatDat * )p_GetNativeData( myself ) )->fileWriter );
	
	fileWriter.WriteUInt( ( uint32_t )rt->GetValue( 0 )->GetInt() );
}

// public func void writeFloat( float value )
deClassFileWriter::nfWriteFloat::nfWriteFloat( const sInitData &init ) : dsFunction( init.clsFileWriter,
"writeFloat", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // value
}
void deClassFileWriter::nfWriteFloat::RunFunction( dsRunTime *rt, dsValue *myself ){
	decBaseFileWriter &fileWriter = *( ( ( const sFileWriterNatDat * )p_GetNativeData( myself ) )->fileWriter );
	
	fileWriter.WriteFloat( rt->GetValue( 0 )->GetFloat() );
}

// public func void writeString8( String data )
deClassFileWriter::nfWriteString8::nfWriteString8( const sInitData &init ) : dsFunction( init.clsFileWriter,
"writeString8", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsString ); // data
}
void deClassFileWriter::nfWriteString8::RunFunction( dsRunTime *rt, dsValue *myself ){
	decBaseFileWriter &fileWriter = *( ( ( const sFileWriterNatDat * )p_GetNativeData( myself ) )->fileWriter );
	const char *data = rt->GetValue( 0 )->GetString();
	int size = strlen( data );
	
	if( size > 255 ){
		DSTHROW_INFO( dueInvalidParam, "String too long" );
	}
	
	fileWriter.WriteByte( ( unsigned char )size );
	if( size > 0 ){
		fileWriter.Write( data, size );
	}
}

// public func void writeString16( String data )
deClassFileWriter::nfWriteString16::nfWriteString16( const sInitData &init ) : dsFunction( init.clsFileWriter,
"writeString16", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsString ); // data
}
void deClassFileWriter::nfWriteString16::RunFunction( dsRunTime *rt, dsValue *myself ){
	decBaseFileWriter &fileWriter = *( ( ( const sFileWriterNatDat * )p_GetNativeData( myself ) )->fileWriter );
	const char *data = rt->GetValue( 0 )->GetString();
	int size = strlen( data );
	
	if( size > 65535 ){
		DSTHROW_INFO( dueInvalidParam, "String too long" );
	}
	
	fileWriter.WriteUShort( ( unsigned short )size );
	if( size > 0 ){
		fileWriter.Write( data, size );
	}
}

// public func void writeString( String data )
deClassFileWriter::nfWriteString::nfWriteString( const sInitData &init ) : dsFunction( init.clsFileWriter,
"writeString", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsString ); // data
}
void deClassFileWriter::nfWriteString::RunFunction( dsRunTime *rt, dsValue *myself ){
	decBaseFileWriter &fileWriter = *( ( ( const sFileWriterNatDat * )p_GetNativeData( myself ) )->fileWriter );
	const char *data = rt->GetValue( 0 )->GetString();
	
	fileWriter.Write( data, strlen( data ) );
}

// public func void writeData( FileReader reader )
deClassFileWriter::nfWriteData::nfWriteData( const sInitData &init ) : dsFunction( init.clsFileWriter,
"writeData", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFileReader ); // reader
}
void deClassFileWriter::nfWriteData::RunFunction( dsRunTime *rt, dsValue *myself ){
	decBaseFileWriter &fileWriter = *( ( ( const sFileWriterNatDat * )p_GetNativeData( myself ) )->fileWriter );
	deScriptingDragonScript &ds = *( ( deClassFileWriter* )GetOwnerClass() )->GetDS();
	
	decBaseFileReader * const fileReader = ds.GetClassFileReader()->GetFileReader( rt->GetValue( 0 )->GetRealObject() );
	if( ! fileReader ){
		DSTHROW( dueNullPointer );
	}
	
	int size = fileReader->GetLength();
	if( size == 0 ){
		return;
	}
	
	const int bufferSize = 1024;
	char *buffer = NULL;
	
	try{
		buffer = new char[ bufferSize ];
		
		while( size > 0 ){
			const int readBytes = decMath::min( bufferSize, size );
			fileReader->Read( buffer, readBytes );
			fileWriter.Write( buffer, readBytes );
			size -= readBytes;
		}
		
		delete [] buffer;
		
	}catch( ... ){
		if( buffer ){
			delete [] buffer;
		}
		throw;
	}
}

// public func void writeData( FileReader reader, int size )
deClassFileWriter::nfWriteData2::nfWriteData2( const sInitData &init ) : dsFunction( init.clsFileWriter,
"writeData", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFileReader ); // reader
	p_AddParameter( init.clsInt ); // size
}
void deClassFileWriter::nfWriteData2::RunFunction( dsRunTime *rt, dsValue *myself ){
	decBaseFileWriter &fileWriter = *( ( ( const sFileWriterNatDat * )p_GetNativeData( myself ) )->fileWriter );
	deScriptingDragonScript &ds = *( ( deClassFileWriter* )GetOwnerClass() )->GetDS();
	
	int size = rt->GetValue( 1 )->GetInt();
	if( size < 0 ){
		DSTHROW( dueInvalidParam );
	}
	if( size == 0 ){
		return;
	}
	
	decBaseFileReader * const fileReader = ds.GetClassFileReader()->GetFileReader( rt->GetValue( 0 )->GetRealObject() );
	if( ! fileReader ){
		DSTHROW( dueNullPointer );
	}
	
	const int bufferSize = 1024;
	char *buffer = NULL;
	
	try{
		buffer = new char[ bufferSize ];
		
		while( size > 0 ){
			const int readBytes = decMath::min( bufferSize, size );
			fileReader->Read( buffer, readBytes );
			fileWriter.Write( buffer, readBytes );
			size -= readBytes;
		}
		
		delete [] buffer;
		
	}catch( ... ){
		if( buffer ){
			delete [] buffer;
		}
		throw;
	}
}



// Class deClassFileWriter
////////////////////////////

// Constructor
////////////////

deClassFileWriter::deClassFileWriter( deScriptingDragonScript *ds ) :
dsClass( "FileWriter", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED ){
	if( ! ds ){
		DSTHROW( dueInvalidParam );
	}
	
	pDS = ds;
	
	GetParserInfo()->SetParent( DENS_DRAGENGINE );
	GetParserInfo()->SetBase( "Object" );
	
	p_SetNativeDataSize( sizeof( sFileWriterNatDat ) );
}

deClassFileWriter::~deClassFileWriter(){
}



// Management
///////////////

void deClassFileWriter::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	init.clsFileWriter = this;
	
	init.clsVoid = engine->GetClassVoid();
	init.clsBool = engine->GetClassBool();
	init.clsString = engine->GetClassString();
	init.clsObject = engine->GetClassObject();
	init.clsInt = engine->GetClassInt();
	init.clsFloat = engine->GetClassFloat();
	
	init.clsFileReader = pDS->GetClassFileReader();
	
	AddFunction( new nfNew( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfGetFilename( init ) );
	
	AddFunction( new nfGetStreamVersion( init ) );
	AddFunction( new nfSetStreamVersion( init ) );
	
	AddFunction( new nfWriteChar( init ) );
	AddFunction( new nfWriteByte( init ) );
	AddFunction( new nfWriteShort( init ) );
	AddFunction( new nfWriteUShort( init ) );
	AddFunction( new nfWriteInt( init ) );
	AddFunction( new nfWriteUInt( init ) );
	AddFunction( new nfWriteFloat( init ) );
	AddFunction( new nfWriteString8( init ) );
	AddFunction( new nfWriteString16( init ) );
	AddFunction( new nfWriteString( init ) );
	AddFunction( new nfWriteData( init ) );
	AddFunction( new nfWriteData2( init ) );
	
	CalcMemberOffsets();
}

decBaseFileWriter *deClassFileWriter::GetFileWriter( dsRealObject *myself ) const{
	if( ! myself ){
		return NULL;
	}
	
	return ( ( const sFileWriterNatDat * )p_GetNativeData( myself->GetBuffer() ) )->fileWriter;
}

int deClassFileWriter::GetStreamVersion( dsRealObject *myself ) const{
	if( ! myself ){
		DSTHROW( dueInvalidParam );
	}
	
	return ( ( const sFileWriterNatDat * )p_GetNativeData( myself->GetBuffer() ) )->streamVersion;
}

void deClassFileWriter::PushFileWriter( dsRunTime *rt, decBaseFileWriter *fileWriter ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	if( ! fileWriter ){
		rt->PushObject( NULL, this );
		return;
	}
	
	rt->CreateObjectNakedOnStack( this );
	( ( sFileWriterNatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) )->fileWriter = fileWriter;
	fileWriter->AddReference();
}
