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
#include <stdint.h>
#include <string.h>

#include "deClassEasyXML.h"
#include "deClassEasyXMLElement.h"
#include "../file/deClassFileWriter.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"
#include "../../xml/dedsXmlDocument.h"
#include "../../xml/dedsXmlParser.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileReaderReference.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/file/decBaseFileWriterReference.h>
#include <dragengine/common/utils/decTimer.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/visitors/decXmlVisitorWriter.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>

#include <libdscript/libdscript.h>
#include <libdscript/exceptions.h>


// Native structure
struct sXMLNatDat{
	dedsXmlDocument *document;
};



// Native functions
/////////////////////

// public func new()
deClassEasyXML::nfNew::nfNew( const sInitData &init ) :
dsFunction( init.clsXmlDocument, DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassEasyXML::nfNew::RunFunction( dsRunTime *rt, dsValue *myself ){
	sXMLNatDat &nd = *( ( sXMLNatDat* )p_GetNativeData( myself ) );
	
	// prepare
	nd.document = NULL;
	
	// create document
	decXmlElementTag *root = NULL;
	
	try{
		nd.document = new dedsXmlDocument( "" );
		
		root = new decXmlElementTag( "root" );
		nd.document->AddElement( root );
		root->FreeReference();
		
	}catch( const deException &e ){
		( ( deClassEasyXML* )GetOwnerClass() )->GetDS().LogException( e );
		if( root ){
			root->FreeReference();
		}
		if( nd.document ){
			nd.document->FreeReference();
			nd.document = NULL;
		}
		throw;
		
	}catch( ... ){
		if( root ){
			root->FreeReference();
		}
		if( nd.document ){
			nd.document->FreeReference();
			nd.document = NULL;
		}
		throw;
	}
}

// public func new( String filename )
deClassEasyXML::nfNewFile::nfNewFile( const sInitData &init ) :
dsFunction( init.clsXmlDocument, DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsString ); // filename
}
void deClassEasyXML::nfNewFile::RunFunction( dsRunTime *rt, dsValue *myself ){
	sXMLNatDat &nd = *( ( sXMLNatDat* )p_GetNativeData( myself ) );
	deScriptingDragonScript &ds = ( ( deClassEasyXML* )GetOwnerClass() )->GetDS();
	
	// prepare
	nd.document = NULL;
	
	// check arguments
	const char * const filename = rt->GetValue( 0 )->GetString();
	if( ! filename ){
		DSTHROW( dueNullPointer );
	}
	
	// load xml
	deVirtualFileSystem &vfs = *ds.GetGameEngine()->GetVirtualFileSystem();
	dedsXmlParser parser( ds.GetGameEngine()->GetLogger() );
	
	try{
		decBaseFileReaderReference reader;
		reader.TakeOver( vfs.OpenFileForReading( decPath::CreatePathUnix( filename ) ) );
		
		nd.document = new dedsXmlDocument( filename );
		
		if( parser.ParseXml( reader, nd.document ) ){
			nd.document->StripComments();
			nd.document->CleanCharData();
			
		}else{
			nd.document->SetParseFailed( true );
			nd.document->SetParseLog( parser.GetParseLog() );
		}
		
	}catch( const deException &e ){
		( ( deClassEasyXML* )GetOwnerClass() )->GetDS().LogException( e );
		if( nd.document ){
			nd.document->FreeReference();
			nd.document = NULL;
		}
		throw;
		
	}catch( ... ){
		if( nd.document ){
			nd.document->FreeReference();
			nd.document = NULL;
		}
		throw;
	}
}

// public func destructor()
deClassEasyXML::nfDestructor::nfDestructor( const sInitData &init ) :
dsFunction( init.clsXmlDocument, DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassEasyXML::nfDestructor::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sXMLNatDat &nd = *( ( sXMLNatDat* )p_GetNativeData( myself ) );
	
	if( nd.document ){
		nd.document->FreeReference();
		nd.document = NULL;
	}
}



// public func String getFilename()
deClassEasyXML::nfGetFilename::nfGetFilename( const sInitData &init ) :
dsFunction( init.clsXmlDocument, "getFilename", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsString ){
}
void deClassEasyXML::nfGetFilename::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsXmlDocument &document = *( ( ( sXMLNatDat* )p_GetNativeData( myself ) )->document );
	rt->PushString( document.GetFilename() );
}

// public func bool hasParseFailed()
deClassEasyXML::nfHasParseFailed::nfHasParseFailed( const sInitData &init ) :
dsFunction( init.clsXmlDocument, "hasParseFailed", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
}
void deClassEasyXML::nfHasParseFailed::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsXmlDocument &document = *( ( ( sXMLNatDat* )p_GetNativeData( myself ) )->document );
	rt->PushBool( document.GetParseFailed() );
}

// public func String getParseLog()
deClassEasyXML::nfGetParseLog::nfGetParseLog( const sInitData &init ) :
dsFunction( init.clsXmlDocument, "getParseLog", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsString ){
}
void deClassEasyXML::nfGetParseLog::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsXmlDocument &document = *( ( ( sXMLNatDat* )p_GetNativeData( myself ) )->document );
	rt->PushString( document.GetParseLog() );
}

// public func EasyXMLElement getRootElement()
deClassEasyXML::nfGetRootElement::nfGetRootElement( const sInitData &init ) :
dsFunction( init.clsXmlDocument, "getRootElement", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsXmlElement ){
}
void deClassEasyXML::nfGetRootElement::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsXmlDocument &document = *( ( ( sXMLNatDat* )p_GetNativeData( myself ) )->document );
	deScriptingDragonScript &ds = ( ( deClassEasyXML* )GetOwnerClass() )->GetDS();
	
	ds.GetClassEasyXMLElement()->PushElement( rt, document.GetRoot() );
}

// public func void setRootElement( EasyXMLElement element )
deClassEasyXML::nfSetRootElement::nfSetRootElement( const sInitData &init ) :
dsFunction( init.clsXmlDocument, "setRootElement", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsXmlElement ); // element
}
void deClassEasyXML::nfSetRootElement::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsXmlDocument &document = *( ( ( sXMLNatDat* )p_GetNativeData( myself ) )->document );
	deScriptingDragonScript &ds = ( ( deClassEasyXML* )GetOwnerClass() )->GetDS();
	
	decXmlElement * const element = ds.GetClassEasyXMLElement()->GetElement(
		rt->GetValue( 0 )->GetRealObject() );
	if( ! element ){
		DSTHROW( dueNullPointer );
	}
	
	document.RemoveAllElements();
	document.AddElement( element );
}

// public func void writeToFile( FileWriter fileWriter, bool compact )
deClassEasyXML::nfWriteToFile::nfWriteToFile( const sInitData &init ) :
dsFunction( init.clsXmlDocument, "writeToFile", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFileWriter ); // fileWriter
	p_AddParameter( init.clsBool ); // compact
}
void deClassEasyXML::nfWriteToFile::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsXmlDocument &document = *( ( ( sXMLNatDat* )p_GetNativeData( myself ) )->document );
	deScriptingDragonScript &ds = ( ( deClassEasyXML* )GetOwnerClass() )->GetDS();
	
	decBaseFileWriter * const fileWriter = ds.GetClassFileWriter()->GetFileWriter(
		rt->GetValue( 0 )->GetRealObject() );
	const bool compact = rt->GetValue( 1 )->GetBool();
	
	if( ! fileWriter ){
		DSTHROW( dueNullPointer );
	}
	
	decXmlVisitorWriter visitor;
	visitor.SetCompact( compact );
	visitor.WriteDocument( fileWriter, document );
}



// public func int hashCode()
deClassEasyXML::nfHashCode::nfHashCode( const sInitData &init ) :
dsFunction( init.clsXmlDocument, "hashCode", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger ){
}

void deClassEasyXML::nfHashCode::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsXmlDocument * const document = ( ( sXMLNatDat* )p_GetNativeData( myself ) )->document;
	rt->PushInt( ( int )( intptr_t )document );
}

// public func bool equals( Object obj )
deClassEasyXML::nfEquals::nfEquals( const sInitData &init ) :
dsFunction( init.clsXmlDocument, "equals", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsObject ); // obj
}
void deClassEasyXML::nfEquals::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsXmlDocument * const document = ( ( sXMLNatDat* )p_GetNativeData( myself ) )->document;
	deClassEasyXML * const clsXML = ( deClassEasyXML* )GetOwnerClass();
	
	dsValue * const obj = rt->GetValue( 0 );
	
	if( ! p_IsObjOfType( obj, clsXML ) ){
		rt->PushBool( false );
		
	}else{
		decXmlDocument * const other = ( ( sXMLNatDat* )p_GetNativeData( obj ) )->document;
		rt->PushBool( document == other );
	}
}



// Class deClassEasyXML
/////////////////////////

// Constructor
////////////////

deClassEasyXML::deClassEasyXML( deScriptingDragonScript &ds ) :
dsClass( "EasyXML", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED ),
pDS( ds )
{
	GetParserInfo()->SetParent( DENS_XML );
	GetParserInfo()->SetBase( "Object" );
	
	p_SetNativeDataSize( sizeof( sXMLNatDat ) );
}

deClassEasyXML::~deClassEasyXML(){
}



// Management
///////////////

void deClassEasyXML::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	init.clsXmlDocument = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsBool = engine->GetClassBool();
	init.clsInteger = engine->GetClassInt();
	init.clsString = engine->GetClassString();
	init.clsObject = engine->GetClassObject();
	init.clsXmlElement = pDS.GetClassEasyXMLElement();
	init.clsFileWriter = pDS.GetClassFileWriter();
	
	AddFunction( new nfNew( init ) );
	AddFunction( new nfNewFile( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfGetFilename( init ) );
	AddFunction( new nfHasParseFailed( init ) );
	AddFunction( new nfGetParseLog( init ) );
	AddFunction( new nfGetRootElement( init ) );
	
	AddFunction( new nfSetRootElement( init ) );
	AddFunction( new nfWriteToFile( init ) );
	
	AddFunction( new nfEquals( init ) );
	AddFunction( new nfHashCode( init ) );
	
	CalcMemberOffsets();
}

dedsXmlDocument *deClassEasyXML::GetDocument( dsRealObject *myself ) const{
	if( ! myself ){
		return NULL;
	}
	return ( ( sXMLNatDat* )p_GetNativeData( myself->GetBuffer() ) )->document;
}

void deClassEasyXML::PushDocument( dsRunTime *rt, dedsXmlDocument *document ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	if( ! document ){
		rt->PushObject( NULL, this );
		return;
	}
	
	rt->CreateObjectNakedOnStack( this );
	( ( sXMLNatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) )->document = document;
	document->AddReference();
}
