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

#ifndef _PROJTESTRUNENGINE_H_
#define _PROJTESTRUNENGINE_H_

#include <dragengine/common/string/decStringDictionary.h>
#include <dragengine/systems/deModuleSystem.h>

class projTestRunProcess;
class deEngine;



/**
 * \brief Game engine handling for test running.
 */
class projTestRunEngine{
private:
	projTestRunProcess &pProcess;
	
	deEngine *pEngine;
	
	decString pPathConfig;
	decString pPathShare;
	decString pPathLib;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create engine. */
	projTestRunEngine(projTestRunProcess &process);
	
	/** \brief Clean up engine. */
	~projTestRunEngine();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Game engine. */
	inline deEngine *GetEngine() const{return pEngine;}
	
	/** \brief Config path. */
	inline const decString &GetPathConfig() const{return pPathConfig;}
	
	/** \brief Share path. */
	inline const decString &GetPathShare() const{return pPathShare;}
	
	/** \brief Library path. */
	inline const decString &GetPathLib() const{return pPathLib;}
	
	/** \brief Start engine. */
	void Start();
	
	/** \brief Put engine into virtual file system. */
	void PutIntoVFS();
	
	/** \brief Activate modules. */
	void ActivateModules();
	
	/** \brief Activate module. */
	void ActivateModule(deModuleSystem::eModuleTypes type, const char *name, const char *version = "");
	
	/** \brief Set data directory. */
	void SetDataDirectory();
	
	/** \brief Set run arguments. */
	void SetRunArguments();
	
	/** \brief Init virtual file system. */
	void InitVFS();
	
	/** \brief Add disk directory. */
	void VFSAddDiskDir(const char *pathRoot, const char *pathDisk, bool readOnly);
	
	/** \brief Create main window. */
	void CreateMainWindow();
	
	/** \brief Run. */
	void Run();
	
	/** \brief Stop engine. */
	void Stop();
	/*@}*/
	
	
	
private:
};

#endif
