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

// include only once
#ifndef _DEOGLCOMMANDEXECUTER_H_
#define _DEOGLCOMMANDEXECUTER_H_

// includes

// definitions

// predefintions
class deGraphicOpenGl;
class decUnicodeArgumentList;
class decUnicodeString;



// opengl command executer
class deoglCommandExecuter{
private:
	deGraphicOpenGl &pOgl;
public:
	// constructor, destructor
	deoglCommandExecuter(deGraphicOpenGl &ogl);
	~deoglCommandExecuter();
	// management
	void ExecuteCommand(const decUnicodeArgumentList &command, decUnicodeString &answer);
private:
	void pHelp(const decUnicodeArgumentList &command, decUnicodeString &answer);
	void pExtensions(const decUnicodeArgumentList &command, decUnicodeString &answer);
	void pRenderWindow(const decUnicodeArgumentList &command, decUnicodeString &answer);
	void pVisual(const decUnicodeArgumentList &command, decUnicodeString &answer);
	void pFBOInfos(const decUnicodeArgumentList &command, decUnicodeString &answer);
	void pFixNaN(const decUnicodeArgumentList &command, decUnicodeString &answer);
	
	void pAnswerBoolValue(decUnicodeString &answer, const char *name, bool value);
	void pAnswerIntValue(decUnicodeString &answer, const char *name, int value);
	void pAnswerStringValue(decUnicodeString &answer, const char *name, const char *value);
	void pAnswerPointerValue(decUnicodeString &answer, const char *name, void *value);
};

// end of include only once
#endif
