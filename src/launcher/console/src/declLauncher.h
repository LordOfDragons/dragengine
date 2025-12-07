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

#ifndef _DECLLAUNCHER_H_
#define _DECLLAUNCHER_H_

#include "logger/declLoggerFiltered.h"

#include <delauncher/delLauncher.h>

#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/unicode/decUnicodeArgumentList.h>
#include <dragengine/logger/deLogger.h>

#ifdef OS_BEOS
#include <delauncher/game/delGame.h>
#endif

class declConfiguration;


/**
 * Main Launcher Class.
 */
class declLauncher : public delLauncher{
private:
	decUnicodeArgumentList pArgList;
	declConfiguration *pConfiguration;
	deLogger::Ref pEngineLogger;
	deLogger::Ref pEngineLoggerDebug;
	
	decString pXMLFile;
	decUnicodeArgumentList pGameArgs;
	decString pWindowTitle;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create launcher. */
	declLauncher();
	
	/** Clean up launcher. */
	virtual ~declLauncher();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Command line arguments. */
	inline const decUnicodeArgumentList &GetArgumentList() const{ return pArgList; }
	
	/** Add argument to the command line argument list. */
	void AddArgument(const decUnicodeString &argument);
	
	/** Configuration. */
	inline declConfiguration &GetConfiguration() const{ return *pConfiguration; }
	
	/** Engine logger. */
	inline const deLogger::Ref &GetEngineLogger() const{ return pEngineLogger; }
	
	/** Engine logger debug. */
	inline const deLogger::Ref &GetEngineLoggerDebug() const{ return pEngineLoggerDebug; }
	
	/** Print syntax. */
	void PrintSyntax();
	
	/** Init. */
	void Init();
	
	/** Run. */
	int Run();
	
	/** Clean up. */
	void CleanUp();
	
	/** Read input from user. */
	decString ReadInput() const;
	
	/** Read input from user returning true if 'y' or 'Y'. */
	bool ReadInputConfirm() const;
	
	/** Read input from user converting to integer number if valid else -1. */
	int ReadInputSelection() const;
	
#ifdef OS_BEOS
	/** Running game or nullptr. */
	delGame::Ref runningGame;
#endif
	/*@}*/
	
	
	
private:
	void pInitLogger();
};

#endif
