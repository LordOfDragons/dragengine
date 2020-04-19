// includes
#include <stdio.h>
#include <string.h>
#include "detRunner.h"
#include "detCase.h"
#include "curve/detCurve2D.h"
#include "curve/detCurveBezier3D.h"
#include "string/detString.h"
#include "string/detStringList.h"
#include "string/detStringDictionary.h"
#include "string/detUnicodeString.h"
#include "string/detUnicodeStringList.h"
#include "string/detStringSet.h"
#include "string/detUnicodeStringSet.h"
#include "string/detUnicodeStringDictionary.h"
#include "path/detPath.h"
#include "math/detMath.h"
#include "math/detColorMatrix.h"
#include "math/detConvexVolume.h"
#include "math/detTexMatrix2.h"
#include "utils/detUniqueID.h"
#include "utils/detPRNG.h"
#include "utils/detUuid.h"
#include "threading/detThreading.h"
#include "file/detZFile.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLoggerConsoleColor.h>
#include <dragengine/common/xmlparser/decXmlParser.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlDocumentReference.h>
#include <dragengine/common/xmlparser/decXmlComment.h>
#include <dragengine/common/xmlparser/decXmlPI.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlEntityReference.h>
#include <dragengine/common/xmlparser/decXmlCharReference.h>
#include <dragengine/common/xmlparser/decXmlCDSect.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlNamespace.h>
#include <dragengine/common/xmlparser/decXmlVisitor.h>
#include <dragengine/common/file/decDiskFileReader.h>


// xml stuff
//////////////
class PrintXmlVisitor : public decXmlVisitor{
private:
	int pIndent;
public:
	
	// constructor
	PrintXmlVisitor(){
		pIndent = 0;
	}
	
	// helping functions
	void PrintIndent(){
		for( int i=0; i<pIndent; i++ ) printf( "  " );
	}
	
	// visiting
	void VisitDocument( decXmlDocument &doc ){
		PrintIndent();
		printf( "%c Document: Encoding(%s) DocType(%s) SysLit(%s) PubLit(%s) Standalone(%c)\n",
			doc.IsEmpty() ? '-' : '+',
			doc.GetEncoding().GetString(), doc.GetDocType().GetString(),
			doc.GetSystemLiteral().GetString(), doc.GetPublicLiteral().GetString(),
			doc.GetStandalone() ? 'T' : 'F' );
		pIndent++;
		doc.VisitElements( *this );
		pIndent--;
	}
	
	void VisitComment( decXmlComment &comment ){
		char commentBuf[54];
		PrintIndent();
		strncpy( (char*)&commentBuf, comment.GetComment(), 50 );
		strcpy( &commentBuf[50], "..." );
		for( int i=0; i<50; i++ ){
			if( commentBuf[ i ] == '\n' ) commentBuf[ i ] = ' ';
		}
		printf( "- Comment: \"%s\"\n", commentBuf );
	}
	
	void VisitPI( decXmlPI &pi ){
		PrintIndent();
		printf( "- Process Instrucion: Target(%s) Command(%s)\n",
			pi.GetTarget().GetString(), pi.GetCommand().GetString() );
	}
	
	void VisitElementTag( decXmlElementTag &tag ){
		PrintIndent();
		printf( "%c Element Tag: Name(%s) Namespace(%s) LocalName(%s)\n",
			tag.IsEmpty() ? '-' : '+', tag.GetName().GetString(),
			tag.GetNamespace().GetString(), tag.GetLocalName().GetString() );
		pIndent++;
		tag.VisitElements( *this );
		pIndent--;
	}
	
	void VisitCharacterData( decXmlCharacterData &data ){
		char dataBuf[54];
		PrintIndent();
		strncpy( (char*)&dataBuf, data.GetData(), 50 );
		strcpy( &dataBuf[50], "..." );
		printf( "- Character Data: \"%s\"\n", dataBuf );
	}
	
	void VisitEntityReference( decXmlEntityReference &ref ){
		char dataBuf[54];
		PrintIndent();
		strncpy( (char*)&dataBuf, ref.GetName(), 50 );
		strcpy( &dataBuf[50], "..." );
		printf( "- Entity Reference: \"%s\"\n", dataBuf );
	}
	
	void VisitCharReference( decXmlCharReference &ref ){
		char dataBuf[54];
		PrintIndent();
		strncpy( (char*)&dataBuf, ref.GetData(), 50 );
		strcpy( &dataBuf[50], "..." );
		if( ref.IsDecimal() ){
			printf( "- Char Reference Decimal: \"%s\"\n", dataBuf );
		}else if( ref.IsHexadecimal() ){
			printf( "- Char Reference Hexa-Decimal: \"%s\"\n", dataBuf );
		}
	}
	
	void VisitCDSect( decXmlCDSect &cdsect ){
		char dataBuf[54];
		PrintIndent();
		strncpy( (char*)&dataBuf, cdsect.GetData(), 50 );
		strcpy( &dataBuf[50], "..." );
		printf( "- CDSect: \"%s\"\n", dataBuf );
	}
	
	void VisitAttValue( decXmlAttValue &value ){
		PrintIndent();
		printf( "- Attribute Value: Name(%s) Namespace(%s) LocalName(%s) Value(\"%s\")\n",
			value.GetName().GetString(), value.GetNamespace().GetString(),
			value.GetLocalName().GetString(), value.GetValue().GetString() );
	}
	
	void VisitNamespace( decXmlNamespace &ns ){
		PrintIndent();
		printf( "- Namespace: Name(%s) URL(\"%s\")\n",
			ns.GetName().GetString(), ns.GetURL().GetString() );
	}
};



// entry point
////////////////
int main(int argc, char **args){
	detRunner runner;
	runner.Run();
	return 0;
}



// class detRunner
////////////////////
#ifndef DETESTS_SPECIAL_OFF
void special();
#endif

detRunner::detRunner(){
	#ifndef DETESTS_SPECIAL_OFF
	special();
	#endif
	
	pCount = 0;
	pCases = NULL;
	pAddTest( new detString );
	pAddTest( new detStringList );
	pAddTest( new detStringSet );
	pAddTest( new detStringDictionary );
	pAddTest( new detUnicodeString );
	pAddTest( new detUnicodeStringList );
	pAddTest( new detUnicodeStringSet );
	pAddTest( new detUnicodeStringDictionary );
	pAddTest( new detPath );
	pAddTest( new detZFile );
	pAddTest( new detMath );
	pAddTest( new detCurve2D );
	pAddTest( new detCurveBezier3D );
	pAddTest( new detConvexVolume );
	pAddTest( new detColorMatrix );
	pAddTest( new detTexMatrix2 );
	pAddTest( new detUniqueID );
	pAddTest( new detPRNG );
	pAddTest( new detUuid );
	pAddTest( new detThreading );
}
detRunner::~detRunner(){
	if(pCases){
		for(int i=0; i<pCount; i++) delete pCases[i];
		delete [] pCases;
	}
}
void detRunner::Run(){
	int i, errorCount = 0;
	detCase *curTest;
	
	setvbuf( stdout, NULL, _IONBF, 0 );
	
	printf( "Ready to start engine tests. Some of the tests can take quite\n" );
	printf( "some time so do not panic if the progress counters gets stuck\n" );
	printf( "for a longer period of time.\n\n" );
	printf( "*** Start Testing ( %i Tests ) ***", pCount);
	for( i=0; i<pCount; i++ ){
		curTest = pCases[i];
		printf("\n- Test %s: P", curTest->GetTestName());
		try{
			curTest->Prepare();
			printf(",R");
			curTest->Run();
			curTest->CleanUp();
		}catch( const deException &e ){
			curTest->CleanUp();
			e.PrintError();
			errorCount++;
		}
	}
	
	if( errorCount > 0 ){
		printf( "*** %i tests failed ***\n", errorCount );
	}else{
		printf( "\n*** All tests passed successfully ***\n" );
	}
}

// private functions
void detRunner::pAddTest(detCase *testCase){
	detCase **newArray = new detCase*[pCount+1];
	int i;
	if(!newArray) DETHROW(deeOutOfMemory);
	if(pCases){
		for(i=0; i<pCount; i++) newArray[i] = pCases[i];
		delete [] pCases;
	}
	pCases = newArray;
	pCases[pCount++] = testCase;
}
