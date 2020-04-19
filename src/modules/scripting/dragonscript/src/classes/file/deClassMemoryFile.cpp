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

#include "deClassMemoryFile.h"
#include "deClassFileReader.h"
#include "deClassFileWriter.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decMemoryFile.h>
#include <dragengine/common/file/decMemoryFileReader.h>
#include <dragengine/common/file/decMemoryFileWriter.h>
#include <dragengine/common/string/decString.h>
#include <libdscript/exceptions.h>



struct sMemFileNatDat{
	decMemoryFile *memoryFile;
};



// Constructors, Destructors
//////////////////////////////

// public func new( String filename )
deClassMemoryFile::nfNew::nfNew( const sInitData &init ) : dsFunction( init.clsMemFile,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsStr ); // filename
}
void deClassMemoryFile::nfNew::RunFunction( dsRunTime *rt, dsValue *myself ){
	sMemFileNatDat &nd = *( ( sMemFileNatDat* )p_GetNativeData( myself ) );
	
	// clear ( important )
	nd.memoryFile = NULL;
	
	// check arguments
	const char * const filename = rt->GetValue( 0 )->GetString();
	
	// create memory file
	nd.memoryFile = new decMemoryFile( filename );
}

// public func new( MemoryFile memoryFile )
deClassMemoryFile::nfNewCopy::nfNewCopy( const sInitData &init ) : dsFunction( init.clsMemFile,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsMemFile ); // memoryFile
}
void deClassMemoryFile::nfNewCopy::RunFunction( dsRunTime *rt, dsValue *myself ){
	sMemFileNatDat &nd = *( ( sMemFileNatDat* )p_GetNativeData( myself ) );
	const deClassMemoryFile &clsMemFile = *( ( deClassMemoryFile* )GetOwnerClass() );
	
	// clear ( important )
	nd.memoryFile = NULL;
	
	// check arguments
	const decMemoryFile * const copyMemoryFile = clsMemFile.GetMemoryFile( rt->GetValue( 0 )->GetRealObject() );
	if( ! copyMemoryFile ){
		DSTHROW( dueNullPointer );
	}
	
	// create memory file
	nd.memoryFile = new decMemoryFile( copyMemoryFile->GetFilename() );
	
	const int size = copyMemoryFile->GetLength();
	if( size > 0 ){
		nd.memoryFile->Resize( size, true );
		memcpy( nd.memoryFile->GetPointer(), copyMemoryFile->GetPointer(), size );
	}
}

// public func destructor()
deClassMemoryFile::nfDestructor::nfDestructor( const sInitData &init ) : dsFunction( init.clsMemFile,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassMemoryFile::nfDestructor::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sMemFileNatDat &nd = *( ( sMemFileNatDat* )p_GetNativeData( myself ) );
	
	if( nd.memoryFile ){
		nd.memoryFile->FreeReference();
		nd.memoryFile = NULL;
	}
}



// Management
///////////////

// public func String getFilename()
deClassMemoryFile::nfGetFilename::nfGetFilename( const sInitData &init ) : dsFunction( init.clsMemFile,
"getFilename", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsStr ){
}
void deClassMemoryFile::nfGetFilename::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decMemoryFile &memoryFile = *( ( ( const sMemFileNatDat * )p_GetNativeData( myself ) )->memoryFile );
	rt->PushString( memoryFile.GetFilename() );
}

// public func int getSize()
deClassMemoryFile::nfGetSize::nfGetSize( const sInitData &init ) : dsFunction( init.clsMemFile,
"getSize", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassMemoryFile::nfGetSize::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decMemoryFile &memoryFile = *( ( ( const sMemFileNatDat * )p_GetNativeData( myself ) )->memoryFile );
	rt->PushInt( memoryFile.GetLength() );
}

// public func void setSize( int size )
deClassMemoryFile::nfSetSize::nfSetSize( const sInitData &init ) : dsFunction( init.clsMemFile,
"setSize", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // size
}
void deClassMemoryFile::nfSetSize::RunFunction( dsRunTime *rt, dsValue *myself ){
	decMemoryFile &memoryFile = *( ( ( const sMemFileNatDat * )p_GetNativeData( myself ) )->memoryFile );
	memoryFile.Resize( rt->GetValue( 0 )->GetInt(), false );
}



// public func FileReader getReader()
deClassMemoryFile::nfGetReader::nfGetReader( const sInitData &init ) : dsFunction( init.clsMemFile,
"getReader", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFRead ){
}
void deClassMemoryFile::nfGetReader::RunFunction( dsRunTime *rt, dsValue *myself ){
	decMemoryFile * const memoryFile = ( ( const sMemFileNatDat * )p_GetNativeData( myself ) )->memoryFile;
	deScriptingDragonScript &ds = ( ( deClassMemoryFile* )GetOwnerClass() )->GetDS();
	decMemoryFileReader *reader = NULL;
	
	try{
		reader = new decMemoryFileReader( memoryFile );
		ds.GetClassFileReader()->PushFileReader( rt, reader );
		reader->FreeReference();
		
	}catch( ... ){
		if( reader ){
			reader->FreeReference();
		}
		throw;
	}
}

// public func FileWriter getWriter( bool append )
deClassMemoryFile::nfGetWriter::nfGetWriter( const sInitData &init ) : dsFunction( init.clsMemFile,
"getWriter", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFWrite ){
	p_AddParameter( init.clsBool ); // append
}
void deClassMemoryFile::nfGetWriter::RunFunction( dsRunTime *rt, dsValue *myself ){
	decMemoryFile * const memoryFile = ( ( const sMemFileNatDat * )p_GetNativeData( myself ) )->memoryFile;
	deScriptingDragonScript &ds = ( ( deClassMemoryFile* )GetOwnerClass() )->GetDS();
	decMemoryFileWriter *writer = NULL;
	
	const bool append = rt->GetValue( 0 )->GetBool();
	
	try{
		writer = new decMemoryFileWriter( memoryFile, append );
		ds.GetClassFileWriter()->PushFileWriter( rt, writer );
		writer->FreeReference();
		
	}catch( ... ){
		if( writer ){
			writer->FreeReference();
		}
		throw;
	}
}



// Class deClassMemoryFile
////////////////////////////

// Constructor
////////////////

deClassMemoryFile::deClassMemoryFile( deScriptingDragonScript &ds ) :
dsClass( "MemoryFile", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED ),
pDS( ds )
{
	GetParserInfo()->SetParent( DENS_DRAGENGINE );
	GetParserInfo()->SetBase( "Object" );
	
	p_SetNativeDataSize( sizeof( sMemFileNatDat ) );
}

deClassMemoryFile::~deClassMemoryFile(){
}



// Management
///////////////

void deClassMemoryFile::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	init.clsMemFile = this;
	
	init.clsVoid = engine->GetClassVoid();
	init.clsBool = engine->GetClassBool();
	init.clsStr = engine->GetClassString();
	init.clsObj = engine->GetClassObject();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	
	init.clsFRead = pDS.GetClassFileReader();
	init.clsFWrite = pDS.GetClassFileWriter();
	
	AddFunction( new nfNew( init ) );
	AddFunction( new nfNewCopy( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfGetFilename( init ) );
	AddFunction( new nfGetSize( init ) );
	AddFunction( new nfSetSize( init ) );
	
	AddFunction( new nfGetReader( init ) );
	AddFunction( new nfGetWriter( init ) );
	
	CalcMemberOffsets();
}

decMemoryFile *deClassMemoryFile::GetMemoryFile( dsRealObject *myself ) const{
	if( ! myself ){
		return NULL;
	}
	return ( ( const sMemFileNatDat * )p_GetNativeData( myself->GetBuffer() ) )->memoryFile;
}

void deClassMemoryFile::PushMemoryFile( dsRunTime *rt, decMemoryFile *memoryFile ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	if( ! memoryFile ){
		rt->PushObject( NULL, this );
		return;
	}
	
	rt->CreateObjectNakedOnStack( this );
	( ( sMemFileNatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) )->memoryFile = memoryFile;
	memoryFile->AddReference();
}
