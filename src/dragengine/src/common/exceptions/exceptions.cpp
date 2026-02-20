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


// Functions
//////////////

DE_DLL_EXPORT void DEThrowNullPointer(const char *file, int line, const char *message){
	throw deeNullPointer(file, line, message);
}

DE_DLL_EXPORT void DEThrowInvalidParam(const char *file, int line, const char *message){
	throw deeInvalidParam(file, line, message);
}



// Subclass Constructors
//////////////////////////

deeInvalidParam::deeInvalidParam(const char *file, int line, const char *description) :
deException("InvalidParam", description ? description : "Invalid Parameter specified", file, line){
}

deeOutOfMemory::deeOutOfMemory(const char *file, int line) :
deException("OutOfMemory", "There is not enough Memory left", file, line){
}

deeOutOfBoundary::deeOutOfBoundary(const char *file, int line) :
deException("OutOfBoundary", "Index is outside allowed boundaries", file, line){
}

deeStackEmpty::deeStackEmpty(const char *file, int line) :
deException("StackEmpty", "Stack is empty", file, line){
}

deeStackOverflow::deeStackOverflow(const char *file, int line) :
deException("StackOverflow", "Stack Overflow", file, line){
}

deeDivisionByZero::deeDivisionByZero(const char *file, int line) :
deException("DivisionByZero", "Division By Zero", file, line){
}

deeNullPointer::deeNullPointer(const char *file, int line, const char *description) :
deException("NullPointer", description ? description : "Null Pointer", file, line){
}

deeInvalidAction::deeInvalidAction(const char *file, int line, const char *description) :
deException("InvalidAction", description ? description : "Invalid Action (internal error)", file, line){
}

deeInvalidFormat::deeInvalidFormat(const char *file, int line, const char *description) :
deException("InvalidFormat", description ? description : "Invalid Format", file, line){
}



deeFileNotFound::deeFileNotFound(const char *file, int line, const char *path) :
deException("FileNotFound", pCreateText(path), file, line){
}

decString deeFileNotFound::pCreateText(const char *path){
	decString text("File does not exist");
	if(path){
		text += ": ";
		text += path;
	}
	return text;
}



deeFileExists::deeFileExists(const char *file, int line, const char *path) :
deException("FileExists", pCreateText(path), file, line){
}

decString deeFileExists::pCreateText(const char *path){
	decString text("File does exist already");
	if(path){
		text += ": ";
		text += path;
	}
	return text;
}



deeOpenFile::deeOpenFile(const char *file, int line, const char *path) :
deException("OpenFileFailed", pCreateText(path), file, line){
}

decString deeOpenFile::pCreateText(const char *path){
	decString text("Open File failed");
	if(path){
		text += ": ";
		text += path;
	}
	return text;
}



deeReadFile::deeReadFile(const char *file, int line, const char *path) :
deException("ReadFileFailed", pCreateText(path), file, line){
}

decString deeReadFile::pCreateText(const char *path){
	decString text("Can not read from file");
	if(path){
		text += ": ";
		text += path;
	}
	return text;
}



deeWriteFile::deeWriteFile(const char *file, int line, const char *path) :
deException("WriteFileFailed", pCreateText(path), file, line){
}

decString deeWriteFile::pCreateText(const char *path){
	decString text("Can not write to file");
	if(path){
		text += ": ";
		text += path;
	}
	return text;
}



deeInvalidFileFormat::deeInvalidFileFormat(const char *file, int line, const char *path) :
deException("InvalidFileFormat", pCreateText(path), file, line){
}

decString deeInvalidFileFormat::pCreateText(const char *path){
	decString text("Invalid File Format");
	if(path){
		text += ": ";
		text += path;
	}
	return text;
}



deeDirectoryNotFound::deeDirectoryNotFound(const char *file, int line) :
deException("DirectoryNotFound", "Directory does not exist", file, line){
}

deeDirectoryRead::deeDirectoryRead(const char *file, int line) :
deException("DirectoryReadFailed", "Directory read error", file, line){
}

deeInvalidSyntax::deeInvalidSyntax(const char *file, int line) :
deException("InvalidSyntax", "Invalid Script Syntax", file, line){
}

deeScriptError::deeScriptError(const char *file, int line) :
deException("ScriptError", "Error in Script System", file, line){
}

deeNoModuleFound::deeNoModuleFound(const char *file, int line) :
deException("NoModuleFound", "No Working Module could be found", file, line){
}

deeTestFailed::deeTestFailed(const char *file, int line, const char *description) :
deException("TestCaseFailed", description ? description : "Test Case failed", file, line){
}

deeAssertion::deeAssertion(const char *file, int line, const char *description) :
deException("AssertionException", description ? description : "Assertion exception has occurred", file, line){
}
