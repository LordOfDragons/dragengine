/* 
 * Drag[en]gine Game Engine
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

#include "../exceptions.h"



// Subclass Constructors
//////////////////////////

deeInvalidParam::deeInvalidParam( const char *file, int line, const char *description ) :
deException( "InvalidParam", description ? description : "Invalid Parameter specified", file, line ){
}

deeOutOfMemory::deeOutOfMemory( const char *file, int line ) :
deException( "OutOfMemory", "There is not enough Memory left", file, line ){
}

deeOutOfBoundary::deeOutOfBoundary( const char *file, int line ) :
deException( "OutOfBoundary", "Index is outside allowed boundaries", file, line ){
}

deeStackEmpty::deeStackEmpty( const char *file, int line ) :
deException( "StackEmpty", "Stack is empty", file, line ){
}

deeStackOverflow::deeStackOverflow( const char *file, int line ) :
deException( "StackOverflow", "Stack Overflow", file, line ){
}

deeDivisionByZero::deeDivisionByZero( const char *file, int line ) :
deException( "DivisionByZero", "Division By Zero", file, line ){
}

deeNullPointer::deeNullPointer( const char *file, int line, const char *description ) :
deException( "NullPointer", description ? description : "Null Pointer", file, line ){
}

deeInvalidAction::deeInvalidAction( const char *file, int line, const char *description ) :
deException( "InvalidAction", description ? description : "Invalid Action (internal error)", file, line ){
}

deeInvalidFormat::deeInvalidFormat( const char *file, int line ) :
deException( "InvalidFormat", "Invalid Format", file, line ){
}



deeFileNotFound::deeFileNotFound( const char *file, int line, const char *path ) :
deException( "FileNotFound", pCreateText( path ), file, line ){
}

decString deeFileNotFound::pCreateText( const char *path ){
	decString text( "File does not exit" );
	if( path ){
		text += ": ";
		text += path;
	}
	return text;
}



deeFileExists::deeFileExists( const char *file, int line, const char *path ) :
deException( "FileExists", pCreateText( path ), file, line ){
}

decString deeFileExists::pCreateText( const char *path ){
	decString text( "File does exist already" );
	if( path ){
		text += ": ";
		text += path;
	}
	return text;
}



deeOpenFile::deeOpenFile( const char *file, int line, const char *path ) :
deException( "OpenFileFailed", pCreateText( path ), file, line ){
}

decString deeOpenFile::pCreateText( const char *path ){
	decString text( "Open File failed" );
	if( path ){
		text += ": ";
		text += path;
	}
	return text;
}



deeReadFile::deeReadFile( const char *file, int line, const char *path ) :
deException( "ReadFileFailed", pCreateText( path ), file, line ){
}

decString deeReadFile::pCreateText( const char *path ){
	decString text( "Can not read from file" );
	if( path ){
		text += ": ";
		text += path;
	}
	return text;
}



deeWriteFile::deeWriteFile( const char *file, int line, const char *path ) :
deException( "WriteFileFailed", pCreateText( path ), file, line ){
}

decString deeWriteFile::pCreateText( const char *path ){
	decString text( "Can not write to file" );
	if( path ){
		text += ": ";
		text += path;
	}
	return text;
}



deeInvalidFileFormat::deeInvalidFileFormat( const char *file, int line, const char *path ) :
deException( "InvalidFileFormat", pCreateText( path ), file, line ){
}

decString deeInvalidFileFormat::pCreateText( const char *path ){
	decString text( "Invalid File Format" );
	if( path ){
		text += ": ";
		text += path;
	}
	return text;
}



deeDirectoryNotFound::deeDirectoryNotFound( const char *file, int line ) :
deException( "DirectoryNotFound", "Directory does not exist", file, line ){
}

deeDirectoryRead::deeDirectoryRead( const char *file, int line ) :
deException( "DirectoryReadFailed", "Directory read error", file, line ){
}

deeInvalidSyntax::deeInvalidSyntax( const char *file, int line ) :
deException( "InvalidSyntax", "Invalid Script Syntax", file, line ){
}

deeScriptError::deeScriptError( const char *file, int line ) :
deException( "ScriptError", "Error in Script System", file, line ){
}

deeNoModuleFound::deeNoModuleFound( const char *file, int line ) :
deException( "NoModuleFound", "No Working Module could be found", file, line ){
}

deeTestFailed::deeTestFailed( const char *file, int line, const char *description ) :
deException( "TestCaseFailed", description ? description : "Test Case failed", file, line ){
}

deeAssertion::deeAssertion( const char *file, int line, const char *description ) :
deException( "AssertionException", description ? description : "Assertion exception has occured", file, line ){
}
