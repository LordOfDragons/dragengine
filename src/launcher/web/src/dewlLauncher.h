/*
 * MIT License
 *
 * Copyright (C) 2025, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _DEWLLAUNCHER_H_
#define _DEWLLAUNCHER_H_

#include <string>
#include <vector>

#include <delauncher/delLauncher.h>

#include <dragengine/logger/deLogger.h>

#include <emscripten/bind.h>


/**
 * Web Launcher Class.
 */
class dewlLauncher : public delLauncher{
private:
	std::vector<std::string> pArgList;
	deLogger::Ref pEngineLogger, pEngineLoggerDebug;
	
	std::string pXMLFile;
	std::vector<std::string> pGameArgs;
	std::string pWindowTitle;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create launcher. */
	dewlLauncher();
	
	/** Clean up launcher. */
	~dewlLauncher() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Command line arguments. */
	inline const std::vector<std::string> &GetArgumentList() const{ return pArgList; }
	
	/** Add argument to the command line argument list. */
	void AddArgument(const std::string &argument);
	
	/** Engine logger. */
	inline const deLogger::Ref &GetEngineLogger() const{ return pEngineLogger; }
	
	/** Engine logger debug. */
	inline const deLogger::Ref &GetEngineLoggerDebug() const{ return pEngineLoggerDebug; }
	
	/** Init. */
	void Init();
	
	/** Run. */
	int Run();
	
	/** Clean up. */
	void CleanUp();
	/*@}*/
	
	
private:
	void pInitLogger();
};

EMSCRIPTEN_BINDINGS(dragengine_web_launcher) {
	emscripten::class_<dewlLauncher>("dewlLauncher")
		.constructor<>()
		.function("AddArgument", &dewlLauncher::AddArgument)
		;
}

#endif
