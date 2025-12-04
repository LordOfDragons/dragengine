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

#ifndef _DELLLAUNCHER_H_
#define _DELLLAUNCHER_H_

#include <delauncher/delLauncher.h>

#include <dragengine/common/collection/decObjectSet.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/unicode/decUnicodeArgumentList.h>
#include <dragengine/filesystem/deVirtualFileSystem::Ref.h>
#include <dragengine/logger/deLogger::Ref.h>

#ifdef OS_BEOS
#include <delauncher/game/delGame.h>
#endif



/**
 * Live launcher.
 */
class dellLauncher{
public:
	class Launcher : public delLauncher{
	public:
		Launcher();
		virtual ~Launcher();
	};
	
#ifdef OS_UNIX
	class PreloadLibrary : public deObject{
		void * const pHandle;
	public:
		PreloadLibrary( const decPath &basePath, const char *filename );
		virtual ~PreloadLibrary();
	};
#endif
	
	
	
private:
	decUnicodeArgumentList pArguments;
	decPath pWorkingDir;
	decPath pPathConfigUser;
	decStringList pEnvParamsStore;
	
#ifdef OS_UNIX
	decObjectSet pPreloadLibraries;
#endif
	
	Launcher *pLauncher;
	
	decString pDelgaFile;
	decUnicodeArgumentList pGameArgs;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create launcher. */
	dellLauncher();
	
	/** Clean up launcher. */
	~dellLauncher();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Command line arguments. */
	inline const decUnicodeArgumentList &GetArguments() const{ return pArguments; }
	
	/** Add argument to the command line argument list. */
	void AddArgument( const decUnicodeString &argument );
	
	/** Working directory. */
	inline const decPath GetWorkingDirectory() const{ return pWorkingDir; }
	
	/** Launcher. */
	inline Launcher &GetLauncher() const{ return *pLauncher; }
	
	/** Run. */
	void Run();

#ifdef OS_BEOS
	delGame::Ref runningGame;
#endif
	/*@}*/
	
	
	
private:
	void pUpdateEnvironment();
};

#endif
