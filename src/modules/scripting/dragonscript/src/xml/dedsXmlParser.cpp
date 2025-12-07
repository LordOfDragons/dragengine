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

// includes
#include <libdscript/libdscript.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dedsXmlParser.h"
#include <dragengine/logger/deLogger.h>
#include <libdscript/exceptions.h>



// class dedsXmlParser
////////////////////////

// constructor, destructor
////////////////////////////

dedsXmlParser::dedsXmlParser(deLogger *logger) : decXmlParser(logger){
	pLog = new char[1];
	pLog[0] = '\0';
	pLogLen = 0;
}

dedsXmlParser::~dedsXmlParser(){
	if(pLog){
		delete [] pLog;
	}
}



// management
///////////////

void dedsXmlParser::UnexpectedEOF(int line, int pos){
	const int newline = pLogLen == 0 ? 0 : 1;
	char * const newLog = new char[pLogLen + newline + 29];

	if(pLogLen > 0){
		#ifdef OS_W32_VS
			strncpy_s(newLog, pLogLen + 1, pLog, pLogLen);
		#else
			strncpy(newLog, pLog, pLogLen + 1);
		#endif
		newLog[pLogLen] = '\n';
	}

	const char * const message = "Unexpected end-of-file found";

	#ifdef OS_W32_VS
		strcpy_s(newLog + pLogLen + newline, strlen(message) + 1, message);
	#else
		strcpy(newLog + pLogLen + newline, message);
	#endif

	delete [] pLog;
	pLog = newLog;
	pLogLen += newline + 28;
}

void dedsXmlParser::UnexpectedToken(int line, int pos, const char *token){
	const int newline = pLogLen == 0 ? 0 : 1;
	const int newLen = 50 + (int)strlen(token);
	char * const newLog = new char[pLogLen + newline + newLen];

	if(pLogLen > 0){
		#ifdef OS_W32_VS
			strncpy_s(newLog, pLogLen + 1, pLog, pLogLen);
		#else
			strncpy(newLog, pLog, pLogLen + 1);
		#endif
		newLog[pLogLen] = '\n';
	}
	snprintf(newLog + pLogLen + newline, newLen, "Unexpected token '%s' found at %i:%i", token, line, pos);
	delete [] pLog;
	pLog = newLog;
	pLogLen += newline + newLen;
}
