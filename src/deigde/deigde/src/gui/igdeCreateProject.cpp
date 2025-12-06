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

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "igdeCreateProject.h"
#include "igdeWindowMain.h"
#include "../environment/igdeEnvironmentIGDE.h"
#include "../configuration/igdeConfiguration.h"
#include "../loadsave/igdeLoadSaveSystem.h"
#include "../template/igdeTemplate.h"
#include "../template/igdeTemplateFile.h"
#include "../template/igdeTemplateReplace.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/common/file/decDiskFileWriter.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/utils/decUuid.h>
#include <dragengine/common/xmlparser/decXmlWriter.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/filesystem/dePathList.h>
#include <dragengine/filesystem/deVFSDiskDirectory.h>
#include <dragengine/filesystem/deVFSContainer.h>
#include <dragengine/filesystem/deCollectFileSearchVisitor.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/systems/deModuleSystem.h>
#include <dragengine/systems/modules/deLoadableModule.h>

#include <deigde/engine/igdeEngineController.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gameproject/igdeGameProject.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gamedefinition/igdeXMLGameDefinition.h>


// Definitions
////////////////

#define VFS_DIR_DATA "/data"
#define VFS_DIR_PROJECT "/project"
#define VFS_DIR_TEMPLATE "/template"


// Class igdeCreateProject
////////////////////////////

// Constructor, destructor
////////////////////////////

igdeCreateProject::igdeCreateProject( igdeWindowMain &windowMain ) :
pWindowMain( windowMain ),
pTemplate( NULL ),
pProject( NULL ),
pGameId( decUuid::Random().ToHexString( false ) ),
pGameDef( NULL ){
}

igdeCreateProject::~igdeCreateProject(){
	if( pGameDef ){
		pGameDef->FreeReference();
	}
	if( pProject ){
		pProject->FreeReference();
	}
}



// Management
///////////////

void igdeCreateProject::SetName( const char *name ){
	pName = name;
}

void igdeCreateProject::SetDescription( const char *description ){
	pDescription = description;
}

void igdeCreateProject::SetPathGameDefProject( const char *path ){
	pPathGameDefProject = path;
}

void igdeCreateProject::SetPathProject( const char *path ){
	pPathProject = path;
}

void igdeCreateProject::SetPathData( const char *path ){
	pPathData = path;
}

void igdeCreateProject::SetPathCache( const char *path ){
	pPathCache = path;
}

void igdeCreateProject::SetScriptModule( const char *module ){
	pScriptModule = module;
}

void igdeCreateProject::SetTemplate( const igdeTemplate *atemplate ){
	pTemplate = atemplate;
}



void igdeCreateProject::CreateProject(){
	pNativePathProject.SetFromNative( pPathProject );
	
	decPath path( pNativePathProject );
	path.AddUnixPath( pPathData );
	pNativePathData = path.GetPathNative();
	
	pProject = new igdeGameProject( pWindowMain.GetEnvironment() );
	pProject->SetName( pName );
	pProject->SetDescription( pDescription );
	pProject->SetPathProjectGameDefinition( pPathGameDefProject );
	pProject->SetPathData( pPathData );
	pProject->SetPathCache( pPathCache );
	pProject->SetScriptModule( pScriptModule );
	
	const deLoadableModule * const scriptModule =
		pWindowMain.GetEngine()->GetModuleSystem()->GetModuleNamed( pScriptModule );
	if( ! scriptModule ){
		DETHROW_INFO( deeInvalidParam, "Script module not found" );
	}
	pProject->SetScriptModuleVersion( scriptModule->GetVersion() );
	
	pCreateGameAliasId();
	pCreateDirectories();
	pCopyDefaultFiles();
	pCreateGameDefinition();
	pStoreBaseGameDefs();
	
	if( pTemplate ){
		pApplyTemplate();
	}
	
	// finish game project
	decPath pathProjectFile( pNativePathProject );
	pathProjectFile.AddComponent( pName + ".degp" );
	pProject->SetFilePath( pathProjectFile.GetPathNative() );
	
	pProject->SetProjectGameDefinition( pGameDef );
	pProject->MergeGameDefinitions();
	
	// save game project
	pWindowMain.GetLoadSaveSystem()->SaveGameProject( pProject, pProject->GetFilePath() );
	pProject->SetChanged( false );
}



// Private Functions
//////////////////////

void igdeCreateProject::pCreateGameAliasId(){
	const char *iterName = pName.GetString();
	
	while( *iterName ){
		const int c = *iterName++;
		
		if( c >= 'a' && c <= 'z' ){
			pGameAliasId.AppendCharacter( c );
			
		}else if( c >= 'A' && c <= 'Z' ){
			pGameAliasId.AppendCharacter( tolower( c ) );
			
		}else if( c >= '0' && c <= '9' ){
			pGameAliasId.AppendCharacter( c );
			
		}else if( c == '_' || c == '-' ){
			pGameAliasId.AppendCharacter( c );
		}
	}
}

void igdeCreateProject::pCreateDirectories(){
	const decPath pathDeleteMe( decPath::CreatePathUnix( "/deleteMe" ) );
	deVFSContainer::Ref diskDirectory;
	decPath path;
	
	// create data directory if absent
	path.SetFromNative( pNativePathData );
	diskDirectory.TakeOver( new deVFSDiskDirectory( path ) );
	
	if( ! diskDirectory->ExistsFile( pathDeleteMe ) ){
		diskDirectory->TouchFile( pathDeleteMe );
		diskDirectory->DeleteFile( pathDeleteMe );
	}
	
	// create cache directory if absent
	path = pNativePathProject;
	path.AddUnixPath( pPathCache );
	
	diskDirectory.TakeOver( new deVFSDiskDirectory( path ) );
	
	if( ! diskDirectory->ExistsFile( pathDeleteMe ) ){
		diskDirectory->TouchFile( pathDeleteMe );
		diskDirectory->DeleteFile( pathDeleteMe );
	}
	
	// create local directory if absent
	path = pNativePathProject;
	path.AddUnixPath( pProject->GetPathLocal() );
	
	diskDirectory.TakeOver( new deVFSDiskDirectory( path ) );
	
	if( ! diskDirectory->ExistsFile( pathDeleteMe ) ){
		diskDirectory->TouchFile( pathDeleteMe );
		diskDirectory->DeleteFile( pathDeleteMe );
	}
}

void igdeCreateProject::pCopyDefaultFiles(){
	// save ignore files for revisioning systems
	decPath path( pNativePathProject );
	path.AddComponent( ".gitignore" );
	
	decDiskFileWriter::Ref writer(decDiskFileWriter::Ref::New( new decDiskFileWriter( path.GetPathNative(), false ) ));
	writer->WriteString( pProject->GetPathLocal() + "\n" );
	writer->WriteString( pPathCache + "\n" );
	writer->WriteString( "distribute\n" );
	writer->WriteString( "testRun*.log\n" );
	writer->WriteString( "*.kdev*\n" );
	writer->WriteString( "*.blend[1-9]\n" );
	writer->WriteString( "*.delga\n" );
	
	// save lfs files for revisioning systems
	path = pNativePathProject;
	path.AddComponent( ".gitattributes" );
	
	writer.TakeOver( new decDiskFileWriter( path.GetPathNative(), false ) );
	
	const char * const extensions[] = {
		// images
		"apng", "jpg", "jpeg", "png", "png3d", "tga", "exr", "gif", "webp",
		// vector graphics
		"svgz",
		// videos
		"ogg", "ogv", "m4a", "webm",
		// audio
		"mp3", "mp4",
		// animation
		"deanim",
		// models
		"demodel", "deoccmesh", "fbx",
		// text documents
		"odg", "ods", "odt", "pdf", "doc", "docx", "docm", "xls", "xlsx", "xlsm",
		"ppt", "pps", "pptx", "pptm", "ppsx", "ppsm", "sldx", "sldm",
		// dragengine specific
		"denavspace", "depfc",
		// audio editing
		"au", "aup",
		// 3d model editing
		"blend", "blend[1-9]",
		// image editing
		"kra", "psd", "xcf", "xcfbz2",
		// archives
		"7z", "zip", "tar", "gz", "bz2",
		// programming intermetiate files
		"o", "os", "lo", "pyc",
		// dragengine specific
		"delga",
		// end of list
		nullptr };
	
	int i;
	decString line;
	for( i=0; extensions[ i ]; i++ ){
		line.Format( "*.%s filter=lfs diff=lfs merge=lfs -text\n", extensions[ i ] );
		writer->WriteString( line );
	}
}

void igdeCreateProject::pCreateGameDefinition(){
	// create project game definition from shared new game definition file. we store the
	// file content aside so we can save it as new game definition with a bit of text
	// replacing. avoids the need to implement a full save code for game definition xml
	pGameDef = new igdeGameDefinition( pWindowMain.GetEnvironment() );
	
	pLoadSharedGameDefContent();
	pSharedGameDefContentReplace();
	
	// save project game definition
	
	decPath path( pNativePathProject );
	path.AddUnixPath( pPathGameDefProject );
	
	( decDiskFileWriter::Ref::New( new decDiskFileWriter( path.GetPathNative(), false ) ) )
		->Write( pSharedGameDefContent.GetString(), pSharedGameDefContent.GetLength() );
}

void igdeCreateProject::pLoadSharedGameDefContent(){
	decPath path;
	path.SetFromNative( pWindowMain.GetConfiguration().GetPathShares() );
	path.AddComponent( "newproject.degd" );
	const decDiskFileReader::Ref reader( decDiskFileReader::Ref::New(
		new decDiskFileReader( path.GetPathNative() ) ) );
	
	const int contentLen = reader->GetLength();
	pSharedGameDefContent.Set( ' ', contentLen );
	reader->Read( ( char* )pSharedGameDefContent.GetString(), contentLen );
	reader->SetPosition( 0 );
	
	igdeXMLGameDefinition loadGameDefinition( pWindowMain.GetEnvironment(), pWindowMain.GetLogger() );
	loadGameDefinition.Load( reader, *pGameDef );
}

void igdeCreateProject::pSharedGameDefContentReplace(){
	decString string;
	
	// update values. has to be done after loading the game definition
	pGameDef->SetFilename( pPathGameDefProject );
	pGameDef->SetBasePath( pNativePathData );
	pGameDef->SetScriptModule( pScriptModule );
	
	// replace tokens with project parameters
	decStringList tokens, values;
	pCreateProjectReplacements( tokens, values );
	
	pGameDef->SetID( pReplaceTokens( pGameDef->GetID(), tokens, values ) );
	pGameDef->SetDescription( pReplaceTokens( pGameDef->GetDescription(), tokens, values ) );
	
	pXmlEscapeValues( values );
	pSharedGameDefContent = pReplaceTokens( pSharedGameDefContent, tokens, values );
}

decString igdeCreateProject::pReplaceTokens( const decString &string,
const decStringList &tokens, const decStringList &values ){
	const int count = tokens.GetCount();
	decString work( string );
	int i;
	
	for( i=0; i<count; i++ ){
		work.ReplaceString( tokens.GetAt( i ), values.GetAt( i ) );
	}
	
	return work;
}

void igdeCreateProject::pCreateProjectReplacements( decStringList &tokens, decStringList &values ){
	tokens.Add( "%{PROJECT_NAME}" );
	values.Add( pName );
	
	tokens.Add( "%{PROJECT_PATH}" );
	values.Add( pNativePathData );
	
	tokens.Add( "%{PROJECT_SCRIPT_MODULE}" );
	values.Add( pScriptModule );
}

void igdeCreateProject::pXmlEscapeValues( decStringList &values ){
	const int count = values.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		values.SetAt( i, decXmlWriter::EscapeText( values.GetAt( i ) ) );
	}
}

void igdeCreateProject::pStoreBaseGameDefs(){
	const igdeGameDefinitionList &sharedGameDefList = pWindowMain.GetSharedGameDefinitions();
	const int count = pBaseGameDefs.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const decString &id = pBaseGameDefs.GetAt( i );
		pProject->GetBaseGameDefinitionIDList().Add( id );
		pProject->GetBaseGameDefinitionList().Add( sharedGameDefList.GetWithID( id ) );
	}
}

void igdeCreateProject::pApplyTemplate(){
	// create vfs directories to work with
	pVFS.TakeOver( new deVirtualFileSystem );
	
	deVFSDiskDirectory::Ref container(deVFSDiskDirectory::Ref::New(new deVFSDiskDirectory( decPath::CreatePathUnix( VFS_DIR_DATA ),
		decPath::CreatePathNative( pNativePathData ) )));
	pVFS->AddContainer( container );
	
	container.TakeOver( new deVFSDiskDirectory( decPath::CreatePathUnix( VFS_DIR_PROJECT ),
		pNativePathProject ) );
	pVFS->AddContainer( container );
	
	container.TakeOver( new deVFSDiskDirectory( decPath::CreatePathUnix( VFS_DIR_TEMPLATE ),
		decPath::CreatePathNative( pTemplate->GetBasePath() ) ) );
	( ( deVFSDiskDirectory& )( deVFSContainer& )container ).SetReadOnly( true );
	pVFS->AddContainer( container );
	
	// create file renames
	pCreateFileRenames();
	
	// create files
	const igdeTemplateFileList &files = pTemplate->GetFiles();
	const int fileCount = files.GetCount();
	int i;
	
	for( i=0; i<fileCount; i++ ){
		pTemplateCreateFile( *files.GetAt( i ) );
	}
}

void igdeCreateProject::pTemplateCreateFile( const igdeTemplateFile &file ){
	dePathList list;
	
	// set up replacements if present
	decStringList replaceTokens, replaceValues;
	pCreateFileReplacements( file, replaceTokens, replaceValues );
	const bool hasReplacements = replaceTokens.GetCount() > 0;
	decString renamePath;
	
	// find files
	if( ! file.GetPattern().IsEmpty() ){
		deVirtualFileSystem::Ref vfs(deVirtualFileSystem::Ref::New( new deVirtualFileSystem ));
		
		deCollectFileSearchVisitor collect( file.GetPattern() );
		pVFS->SearchFiles( decPath::CreatePathUnix( VFS_DIR_TEMPLATE ), collect );
		list = collect.GetFiles();
		
	}else{
		list.Add( decPath::CreatePathUnix( file.GetPath() ) );
		
		if( ! file.GetPathRename().IsEmpty() ){
			renamePath = pReplaceTokens( file.GetPathRename(), pFileRenameTokens, pFileRenameValues );
		}
	}
	
	// process files
	const int count = list.GetCount();
	decBaseFileReader::Ref reader;
	decBaseFileWriter::Ref writer;
	int i;
	
	for( i=0; i<count; i++ ){
		// read file from template
		decPath path( list.GetAt( i ) );
		reader.TakeOver( pVFS->OpenFileForReading( decPath::CreatePathUnix( VFS_DIR_TEMPLATE ) + path ) );
		
		int contentLen = reader->GetLength();
		decString content;
		content.Set( ' ', contentLen );
		reader->Read( ( char* )content.GetString(), contentLen );
		reader = NULL;
		
		// process file content if required. works only for text files
		if( hasReplacements ){
			content = pReplaceTokens( content, replaceTokens, replaceValues );
			contentLen = content.GetLength();
		}
		
		// create project file with content
		if( ! renamePath.IsEmpty() ){
			path = decPath::CreatePathUnix( renamePath );
		}
		
		switch( file.GetDirectory() ){
		case igdeTemplateFile::edData:
			writer.TakeOver( pVFS->OpenFileForWriting( decPath::CreatePathUnix( VFS_DIR_DATA ) + path ) );
			break;
			
		case igdeTemplateFile::edProject:
			writer.TakeOver( pVFS->OpenFileForWriting( decPath::CreatePathUnix( VFS_DIR_PROJECT ) + path ) );
			break;
			
		default:
			DETHROW( deeInvalidParam );
		}
		
		writer->Write( content.GetString(), contentLen );
		writer = NULL;
	}
}

void igdeCreateProject::pCreateFileRenames(){
	pFileRenameTokens.Add( "{projectPathDirectory}" );
	pFileRenameValues.Add( pNativePathProject.GetLastComponent() );
	
	pFileRenameTokens.Add( "{projectName}" );
	pFileRenameValues.Add( pName );
	
	pFileRenameTokens.Add( "{gameId}" );
	pFileRenameValues.Add( pGameId );
	
	pFileRenameTokens.Add( "{gameAliasId}" );
	pFileRenameValues.Add( pGameAliasId );
}

void igdeCreateProject::pCreateFileReplacements( const igdeTemplateFile &file,
decStringList &tokens, decStringList &values ){
	const igdeTemplateReplaceList &list = file.GetReplacements();
	const int count = list.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const igdeTemplateReplace &replace = *list.GetAt( i );
		tokens.Add( replace.GetToken() );
		
		decString value;
		
		switch( replace.GetValue() ){
		case igdeTemplateReplace::evProjectPath:
			value = pNativePathProject.GetPathNative();
			break;
			
		case igdeTemplateReplace::evProjectPathDirectory:
			value = pNativePathProject.GetLastComponent();
			break;
			
		case igdeTemplateReplace::evDataPath:
			value = pNativePathData;
			break;
			
		case igdeTemplateReplace::evDataDirectory:
			value = pPathData;
			break;
			
		case igdeTemplateReplace::evProjectName:
			value = pName;
			break;
			
		case igdeTemplateReplace::evProjectDescription:
			value = pDescription;
			break;
			
		case igdeTemplateReplace::evGameId:
			value = pGameId;
			break;
			
		case igdeTemplateReplace::evGameAliasId:
			value = pGameAliasId;
			break;
			
		default:
			value = replace.GetToken();
		}
		
		switch( replace.GetFormat() ){
		case igdeTemplateReplace::efStringC:
			value = pEscapeStringC( value );
			break;
			
		case igdeTemplateReplace::efXml:
			value = pEscapeStringXml( value );
			break;
			
		case igdeTemplateReplace::efXmlMultiline:
			value = pEscapeStringXmlMultiline( value );
			break;
			
		case igdeTemplateReplace::efText:
		default:
			break;
		}
		
		values.Add( value );
	}
}

decString igdeCreateProject::pEscapeStringC( const decString &string ){
	const char *iter = string.GetString();
	decString escaped;
	
	while( *iter ){
		const int c = *iter++;
		
		if( c == '"' ){
			escaped += "\\\"";
			
		}else if( c == '\'' ){
			escaped += "\\\'";
			
		}else{
			escaped.AppendCharacter( c );
		}
	}
	
	return escaped;
}

decString igdeCreateProject::pEscapeStringXml( const decString &string ){
	return decXmlWriter::EscapeText( string );
}

decString igdeCreateProject::pEscapeStringXmlMultiline( const decString &string ){
	const decString escaped( pEscapeStringXml( string ) );
	const char *iter = escaped.GetString();
	decString multiline;
	
	while( *iter ){
		const int c = *iter++;
		
		if( c == '\n' ){
			multiline += "<br/>";
			
		}else{
			multiline.AppendCharacter( c );
		}
	}
	
	return multiline;
}
