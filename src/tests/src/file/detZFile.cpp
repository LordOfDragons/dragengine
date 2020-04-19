#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "detZFile.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decMemoryFile.h>
#include <dragengine/common/file/decMemoryFileWriter.h>
#include <dragengine/common/file/decDiskFileWriter.h>
#include <dragengine/common/file/decMemoryFileReader.h>
#include <dragengine/common/file/decZFileWriter.h>
#include <dragengine/common/file/decZFileReader.h>



// Class detZFile
///////////////////

// Constructors, Destructor
/////////////////////////////

detZFile::detZFile(){
	Prepare();
}

detZFile::~detZFile(){
	CleanUp();
}



// Testing
////////////

void detZFile::Prepare(){
	pMemoryFileCompressed = NULL;
	pTestBuffer = NULL;
	
	pMemoryFileWriter = NULL;
	pZWriter = NULL;
	
	pMemoryFileReader = NULL;
	pZReader = NULL;
	
	pMemoryFileCompressed = new decMemoryFile( "compressed" );
	pTestBuffer = new char[ 16000 ];
}

void detZFile::Run(){
	pTestZFile();
}

void detZFile::CleanUp(){
	pDestroyZReader();
	pDestroyZWriter();
	
	if( pTestBuffer ){
		delete [] pTestBuffer;
		pTestBuffer = NULL;
	}
	
	if( pMemoryFileCompressed ){
		pMemoryFileCompressed->FreeReference();
		pMemoryFileCompressed = NULL;
	}
}

const char *detZFile::GetTestName(){
	return "ZFile";
}



// Private Functions
//////////////////////

static const char * const vLorumIpsum12000 =
	"Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt "
	"ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo "
	"dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit "
	"amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt "
	"ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo "
	"dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit "
	"amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt "
	"ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores "
	"et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. "
	"Duis autem vel eum iriure dolor in hendrerit in vulputate velit esse molestie consequat, vel illum "
	"dolore eu feugiat nulla facilisis at vero eros et accumsan et iusto odio dignissim qui blandit praesent "
	"luptatum zzril delenit augue duis dolore te feugait nulla facilisi. Lorem ipsum dolor sit amet, "
	"consectetuer adipiscing elit, sed diam nonummy nibh euismod tincidunt ut laoreet dolore magna aliquam "
	"erat volutpat. Ut wisi enim ad minim veniam, quis nostrud exerci tation ullamcorper suscipit lobortis "
	"nisl ut aliquip ex ea commodo consequat. Duis autem vel eum iriure dolor in hendrerit in vulputate velit "
	"esse molestie consequat, vel illum dolore eu feugiat nulla facilisis at vero eros et accumsan et iusto "
	"odio dignissim qui blandit praesent luptatum zzril delenit augue duis dolore te feugait nulla facilisi. "
	"Nam liber tempor cum soluta nobis eleifend option congue nihil imperdiet doming id quod mazim placerat "
	"facer possim assum. Lorem ipsum dolor sit amet, consectetuer adipiscing elit, sed diam nonummy nibh "
	"euismod tincidunt ut laoreet dolore magna aliquam erat volutpat. Ut wisi enim ad minim veniam, quis "
	"nostrud exerci tation ullamcorper suscipit lobortis nisl ut aliquip ex ea commodo consequat. "
	"Duis autem vel eum iriure dolor in hendrerit in vulputate velit esse molestie consequat, vel illum "
	"dolore eu feugiat nulla facilisis. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita "
	"kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, "
	"consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam "
	"erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd "
	"gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur "
	"sadipscing elitr, At accusam aliquyam diam diam dolore dolores duo eirmod eos erat, et nonumy sed "
	"tempor et et invidunt justo labore Stet clita ea et gubergren, kasd magna no rebum. sanctus sea sed "
	"takimata ut vero voluptua. est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur "
	"sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat. "
	"Consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam "
	"erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd "
	"gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur "
	"sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed "
	"diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no "
	"sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing "
	"elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. "
	"At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata "
	"sanctus. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, "
	"Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt "
	"ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo "
	"dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit "
	"amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt "
	"ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo "
	"dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit "
	"amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt "
	"ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores "
	"et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. "
	"Duis autem vel eum iriure dolor in hendrerit in vulputate velit esse molestie consequat, vel illum "
	"dolore eu feugiat nulla facilisis at vero eros et accumsan et iusto odio dignissim qui blandit praesent "
	"luptatum zzril delenit augue duis dolore te feugait nulla facilisi. Lorem ipsum dolor sit amet, "
	"consectetuer adipiscing elit, sed diam nonummy nibh euismod tincidunt ut laoreet dolore magna aliquam "
	"erat volutpat. Ut wisi enim ad minim veniam, quis nostrud exerci tation ullamcorper suscipit lobortis "
	"nisl ut aliquip ex ea commodo consequat. Duis autem vel eum iriure dolor in hendrerit in vulputate velit "
	"esse molestie consequat, vel illum dolore eu feugiat nulla facilisis at vero eros et accumsan et iusto "
	"odio dignissim qui blandit praesent luptatum zzril delenit augue duis dolore te feugait nulla facilisi. "
	"Nam liber tempor cum soluta nobis eleifend option congue nihil imperdiet doming id quod mazim placerat "
	"facer possim assum. Lorem ipsum dolor sit amet, consectetuer adipiscing elit, sed diam nonummy nibh "
	"euismod tincidunt ut laoreet dolore magna aliquam erat volutpat. Ut wisi enim ad minim veniam, quis "
	"nostrud exerci tation ullamcorper suscipit lobortis nisl ut aliquip ex ea commodo consequat. "
	"Duis autem vel eum iriure dolor in hendrerit in vulputate velit esse molestie consequat, vel illum "
	"dolore eu feugiat nulla facilisis. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita "
	"kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, "
	"consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam "
	"erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd "
	"gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur "
	"sadipscing elitr, At accusam aliquyam diam diam dolore dolores duo eirmod eos erat, et nonumy sed "
	"tempor et et invidunt justo labore Stet clita ea et gubergren, kasd magna no rebum. sanctus sea sed "
	"takimata ut vero voluptua. est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur "
	"sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat. "
	"Consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam "
	"erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd "
	"gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur "
	"sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed "
	"diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no "
	"sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing "
	"elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. "
	"At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata "
	"sanctus. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, "
	"Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt "
	"ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo "
	"dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit "
	"amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt "
	"ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo "
	"dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit "
	"amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt "
	"ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores "
	"et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. "
	"Duis autem vel eum iriure dolor in hendrerit in vulputate velit esse molestie consequat, vel illum "
	"dolore eu feugiat nulla facilisis at vero eros et accumsan et iusto odio dignissim qui blandit praesent "
	"luptatum zzril delenit augue duis dolore te feugait nulla facilisi. Lorem ipsum dolor sit amet, "
	"consectetuer adipiscing elit, sed diam nonummy nibh euismod tincidunt ut laoreet dolore magna aliquam "
	"erat volutpat. Ut wisi enim ad minim veniam, quis nostrud exerci tation ullamcorper suscipit lobortis "
	"nisl ut aliquip ex ea commodo consequat. Duis autem vel eum iriure dolor in hendrerit in vulputate velit "
	"esse molestie consequat, vel illum dolore eu feugiat nulla facilisis at vero eros et accumsan et iusto "
	"odio dignissim qui blandit praesent luptatum zzril delenit augue duis dolore te feugait nulla facilisi. "
	"Nam liber tempor cum soluta nobis eleifend option congue nihil imperdiet doming id quod mazim placerat "
	"facer possim assum. Lorem ipsum dolor sit amet, consectetuer adipiscing elit, sed diam nonummy nibh "
	"euismod tincidunt ut laoreet dolore magna aliquam erat volutpat. Ut wisi enim ad minim veniam, quis "
	"nostrud exerci tation ullamcorper suscipit lobortis nisl ut aliquip ex ea commodo consequat. "
	"Duis autem vel eum iriure dolor in hendrerit in vulputate velit esse molestie consequat, vel illum "
	"dolore eu feugiat nulla facilisis. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita "
	"kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, "
	"consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam "
	"erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd "
	"gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur "
	"sadipscing elitr, At accusam aliquyam diam diam dolore dolores duo eirmod eos erat, et nonumy sed "
	"tempor et et invidunt justo labore Stet clita ea et gubergren, kasd magna no rebum. sanctus sea sed "
	"takimata ut vero voluptua. est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur "
	"sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat. "
	"Consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam "
	"erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd "
	"gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur "
	"sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed "
	"diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no "
	"sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing "
	"elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. "
	"At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata "
	"sanctus. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, ";



void detZFile::pTestZFile(){
	// in chunks of 10 bytes
	SetSubTestNum( 0 );
	int i, count;
	
	pCreateZWriter();
	count = strlen( vLorumIpsum12000 );
	for( i=0; i<count; i+=10 ){
		pZWriter->Write( vLorumIpsum12000+i, (i+10>count) ? (count-i) : 10 );
	}
	pDestroyZWriter();
	
	pCreateZReader();
	count = strlen( vLorumIpsum12000 );
	for( i=0; i<count; i+=10 ){
		pZReader->Read( pTestBuffer+i, (i+10>count) ? (count-i) : 10 );
	}
	pDestroyZReader();
	ASSERT_EQUAL( strncmp( vLorumIpsum12000, pTestBuffer, count ), 0 );
	
	// all in once
	SetSubTestNum( 1 );
	pCreateZWriter();
	pZWriter->Write( vLorumIpsum12000, count );
	pDestroyZWriter();
	
	pCreateZReader();
	pZReader->Read( pTestBuffer, count );
	pDestroyZReader();
	ASSERT_EQUAL( strncmp( vLorumIpsum12000, pTestBuffer, count ), 0 );
	
	// seeking to read
	SetSubTestNum( 2 );
	pCreateZWriter();
	pZWriter->Write( vLorumIpsum12000, count );
	pDestroyZWriter();
	
	pCreateZReader();
	pZReader->SetPosition( 2500 );
	pZReader->Read( pTestBuffer, 100 );
	pDestroyZReader();
	ASSERT_EQUAL( strncmp( vLorumIpsum12000 + 2500, pTestBuffer, 100 ), 0 );
	
	// seeking back to read
	SetSubTestNum( 3 );
	pCreateZWriter();
	pZWriter->Write( vLorumIpsum12000, count );
	pDestroyZWriter();
	
	pCreateZReader();
	pZReader->Read( pTestBuffer, count );
	pZReader->SetPosition( 2500 );
	pZReader->Read( pTestBuffer, 100 );
	pDestroyZReader();
	ASSERT_EQUAL( strncmp( vLorumIpsum12000 + 2500, pTestBuffer, 100 ), 0 );
	
	// seek move to read
	SetSubTestNum( 4 );
	pCreateZWriter();
	pZWriter->Write( vLorumIpsum12000, count );
	pDestroyZWriter();
	
	pCreateZReader();
	pZReader->Read( pTestBuffer, 3000 );
	pZReader->MovePosition( -500 );
	pZReader->Read( pTestBuffer, 100 );
	pDestroyZReader();
	ASSERT_EQUAL( strncmp( vLorumIpsum12000 + 2500, pTestBuffer, 100 ), 0 );
	
	// seek to end
	SetSubTestNum( 5 );
	pCreateZWriter();
	pZWriter->Write( vLorumIpsum12000, count );
	pDestroyZWriter();
	
	pCreateZReader();
	pZReader->SetPositionEnd( 100 );
	pZReader->Read( pTestBuffer, 100 );
	pDestroyZReader();
	ASSERT_EQUAL( strncmp( vLorumIpsum12000 + ( count - 100 ), pTestBuffer, 100 ), 0 );
	
	// find real size
	SetSubTestNum( 6 );
	pCreateZWriter();
	pZWriter->Write( vLorumIpsum12000, count );
	pDestroyZWriter();
	
	pCreateZReader();
	ASSERT_EQUAL( pZReader->GetLength(), count );
	pDestroyZReader();
}



void detZFile::pOutputCompressedToFile(){
	decDiskFileWriter *writer = NULL;
	
	try{
		writer = new decDiskFileWriter( "detests_ztest_compressed", false );
		writer->Write( pMemoryFileCompressed->GetPointer(), pMemoryFileCompressed->GetLength() );
		writer->FreeReference();
		
	}catch( const deException & ){
		if( writer ){
			writer->FreeReference();
		}
		throw;
	}
}

void detZFile::pOutputTestBufferToFile( int size ){
	decDiskFileWriter *reader = NULL;
	
	try{
		reader = new decDiskFileWriter( "detests_ztest_uncompressed", false );
		reader->Write( pTestBuffer, size );
		reader->FreeReference();
		
	}catch( const deException & ){
		if( reader ){
			reader->FreeReference();
		}
		throw;
	}
}



void detZFile::pCreateZWriter(){
	pMemoryFileWriter = new decMemoryFileWriter( pMemoryFileCompressed, false );
	pZWriter = new decZFileWriter( pMemoryFileWriter );
	pMemoryFileWriter = NULL;
}

void detZFile::pDestroyZWriter(){
	if( pZWriter ){
		pZWriter->FreeReference();
		pZWriter = NULL;
	}
	if( pMemoryFileWriter ){
		pMemoryFileWriter->FreeReference();
		pMemoryFileWriter = NULL;
	}
}



void detZFile::pCreateZReader(){
	pMemoryFileReader = new decMemoryFileReader( pMemoryFileCompressed );
	pZReader = new decZFileReader( pMemoryFileReader );
	pMemoryFileReader = NULL;
}

void detZFile::pDestroyZReader(){
	if( pZReader ){
		pZReader->FreeReference();
		pZReader = NULL;
	}
	if( pMemoryFileReader ){
		pMemoryFileReader->FreeReference();
		pMemoryFileReader = NULL;
	}
}
