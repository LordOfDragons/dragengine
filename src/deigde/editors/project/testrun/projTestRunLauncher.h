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

#ifndef _PROJTESTRUNLAUNCHER_H_
#define _PROJTESTRUNLAUNCHER_H_

#ifdef OS_W32
#include <dragengine/app/include_windows.h>
#endif
#include <dragengine/filesystem/deVirtualFileSystem.h>

class projTestRunProcess;



/**
 * \brief Launcher handlign simulation for test runner.
 * 
 * Simulates functionality done by a true launcher.
 */
class projTestRunLauncher{
private:
	projTestRunProcess &pProcess;
	
	decString pPathConfigSystem;
	decString pPathConfigUser;
	decString pPathShares;
	
	deVirtualFileSystem::Ref pVFS;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create launcher simulation. */
	projTestRunLauncher(projTestRunProcess &process);
	
	/** \brief Clean up launcher simulation. */
	~projTestRunLauncher();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief System config path. */
	inline const decString &GetPathConfigSystem() const{ return pPathConfigSystem; }
	
	/** \brief User config path. */
	inline const decString &GetPathConfigUser() const{ return pPathConfigUser; }
	
	/** \brief Shares path. */
	inline const decString &GetPathShares() const{ return pPathShares; }
	
	/** \brief Virtual file system. */
	inline const deVirtualFileSystem::Ref &GetVFS() const{ return pVFS; }
	
	/** \brief Locate path. */
	void LocatePath();
	
	/** \brief Create virtual file system. */
	void CreateVFS();
	/*@}*/
	
	
	
private:
};

#endif
