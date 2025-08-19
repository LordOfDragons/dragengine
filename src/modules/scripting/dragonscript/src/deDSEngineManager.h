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

#include <dragengine/common/string/decString.h>
#include <dragengine/common/file/decPath.h>

#include <libdscript/libdscript.h>

class deScriptingDragonScript;
class decPath;


/**
 * DragonScript Engine Manager.
 */
class deDSEngineManager : public dsDefaultEngineManager{
private:
	deScriptingDragonScript &pDS;
	const decPath pPathContrib;
	const decPath pVfsPathContrib;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create dragonscript engine manager. */
	deDSEngineManager(deScriptingDragonScript &ds, const decPath &pathContrib,
		const decPath &vfsPathContrib);
	
	/** Clean up dragonscript engine manager. */
	~deDSEngineManager() override;
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	void OutputMessage(const char *message) override;
	
	void OutputWarning(const char *message, int warnID, dsScriptSource *script,
		int line, int position) override;
	
	void OutputWarningMore(const char *message) override;
	
	void OutputError(const char *message, int errorID, dsScriptSource *script,
		int line, int position) override;
	
	void OutputErrorMore(const char *message) override;
	
	bool ContinueParsing() override;
	
	void AddPackageScriptFiles(dsPackage &package, const char *baseDirectory) override;
	/*@}*/
};

#endif
