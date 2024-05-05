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

#ifndef _DEDSENGINEMANAGER_H_
#define _DEDSENGINEMANAGER_H_

#include <libdscript/libdscript.h>

class deScriptingDragonScript;



/**
 * @brief DragonScript Engine Manager.
 */
class deDSEngineManager : public dsBaseEngineManager{
private:
	deScriptingDragonScript *pDS;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new dragonscript engine manager. */
	deDSEngineManager( deScriptingDragonScript *ds );
	/** Cleans up the dragonscript engine manager. */
	virtual ~deDSEngineManager();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	// message output
	void OutputMessage(const char *Message);
	void OutputWarning(const char *Message, int WarnID, dsScriptSource *Script, int Line, int Position);
	void OutputWarningMore(const char *Message);
	void OutputError(const char *Message, int ErrorID, dsScriptSource *Script, int Line, int Position);
	void OutputErrorMore(const char *Message);
	// parser management
	bool ContinueParsing();
	/*@}*/
};

// end of include only once
#endif
