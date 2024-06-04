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
#include <string.h>

#include "../exceptions.h"



// Subclass Constructors
//////////////////////////

deeInvalidParam::deeInvalidParam( const char *file, int line ) :
deException( "InvalidParam" ,"Invalid Parameter specified", file, line ){
}

deeOutOfMemory::deeOutOfMemory( const char *file, int line ) :
deException( "OutOfMemory" ,"There is not enough Memory left", file, line ){
}

deeOutOfBoundary::deeOutOfBoundary( const char *file, int line ) :
deException( "OutOfBoundary" ,"Index is outside allowed boundaries", file, line ){
}

deeStackEmpty::deeStackEmpty( const char *file, int line ) :
deException( "StackEmpty" ,"Stack is empty", file, line ){
}

deeStackOverflow::deeStackOverflow( const char *file, int line ) :
deException( "StackOverflow" ,"Stack Overflow", file, line ){
}

deeDivisionByZero::deeDivisionByZero( const char *file, int line ) :
deException( "DivisionByZero" ,"Division By Zero", file, line ){
}

deeNullPointer::deeNullPointer( const char *file, int line ) :
deException( "NullPointer" ,"Null Pointer", file, line ){
}

deeInvalidAction::deeInvalidAction( const char *file, int line ) :
deException( "InvalidAction" ,"Invalid Action (internal error)", file, line ){
}

deeInvalidFormat::deeInvalidFormat( const char *file, int line ) :
deException( "InvalidFormat" ,"Invalid Format", file, line ){
}

deeFileNotFound::deeFileNotFound( const char *file, int line ) :
deException( "FileNotFound" ,"File does not exist", file, line ){
}

deeFileExists::deeFileExists( const char *file, int line ) :
deException( "FileExists" ,"File does exist already", file, line ){
}

deeOpenFile::deeOpenFile( const char *file, int line ) :
deException( "OpenFileFailed" ,"Open File failed", file, line ){
}

deeReadFile::deeReadFile( const char *file, int line ) :
deException( "ReadFileFailed" ,"Can not read from file", file, line ){
}

deeWriteFile::deeWriteFile( const char *file, int line ) :
deException( "WriteFileFailed" ,"Can not write to file", file, line ){
}

deeInvalidFileFormat::deeInvalidFileFormat( const char *file, int line ) :
deException( "InvalidFileFormat" ,"Invalid File Format", file, line ){
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

deeTestFailed::deeTestFailed( const char *file, int line ) :
deException( "TestCaseFailed" ,"Test Case failed", file, line ){
}

deeAssertion::deeAssertion( const char *file, int line ) :
deException( "AssertionException" ,"Assertion exception has occurred", file, line ){
}
