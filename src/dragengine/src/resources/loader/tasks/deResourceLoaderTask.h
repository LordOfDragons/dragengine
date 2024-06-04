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

#ifndef _DERESOURCELOADERTASK_X_H_
#define _DERESOURCELOADERTASK_X_H_

#include "../deResourceLoader.h"
#include "../../deResourceReference.h"
#include "../../../filesystem/deVirtualFileSystemReference.h"
#include "../../../parallel/deParallelTask.h"
#include "../../../common/utils/decTimer.h"
#include "../../../common/string/decString.h"

class deEngine;
class deFileResource;


/**
 * \brief Resource loader task.
 */
class DE_DLL_EXPORT deResourceLoaderTask : public deParallelTask{
public:
	/** \brief States. */
	enum eStates{
		/** \brief Pending. */
		esPending,
		
		/** \brief Succeeded. */
		esSucceeded,
		
		/** \brief Failed. */
		esFailed
	};
	
	/** \brief Types. */
	enum eTypes{
		/** \brief Read. */
		etRead,
		
		/** \brief Write. */
		etWrite,
	};
	
	
	
private:
	// thread safe
	deEngine &pEngine;
	deResourceLoader &pResourceLoader;
	
	deVirtualFileSystemReference pVFS;
	decString pPath;
	deResourceLoader::eResourceType pResourceType;
	
	deResourceReference pResource;
	eStates pState;
	eTypes pType;
	
	decTimer pDebugTimer;
	
	
	
protected:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create task. */
	deResourceLoaderTask( deEngine &engine, deResourceLoader &resourceLoader,
		deVirtualFileSystem *vfs, const char *path,
		deResourceLoader::eResourceType resourceType );
	
	/** \brief Clean up task. */
	virtual ~deResourceLoaderTask();
	/*@}*/
	
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Virtual file system. */
	inline deVirtualFileSystem *GetVFS() const{ return pVFS; }
	
	/** \brief Path. */
	inline const decString &GetPath() const{ return pPath; }
	
	/** \brief Resource type. */
	inline deResourceLoader::eResourceType GetResourceType() const{ return pResourceType; }
	
	/** \brief Task matches the given path/resourceType. */
	bool Matches( deVirtualFileSystem *vfs, const char *path,
		deResourceLoader::eResourceType resourceType ) const;
	
	
	
	/** \brief Resource or NULL. */
	inline deFileResource *GetResource() const{ return ( deFileResource* )( deResource* )pResource; }
	
	/** \brief State. */
	inline eStates GetState() const{ return pState; }
	
	/** \brief Type. */
	inline eTypes GetType() const{ return pType; }
	/*@}*/
	
	
	
	/** \name Debugging */
	/*@{*/
	/** \brief Short task name for debugging. */
	virtual decString GetDebugName() const;
	
	/** \brief Task details for debugging. */
	virtual decString GetDebugDetails() const;
	/*@}*/
	
	
	
	
protected:
	inline deEngine &GetEngine(){ return pEngine; }
	inline deResourceLoader &GetResourceLoader(){ return pResourceLoader; }
	void SetResource( deFileResource *resource );
	void SetState( eStates state );
	void SetType( eTypes type );
	void LogCreateEnter();
	void LogCreateExit();
	void LogRunEnter();
	void LogRunExit();
	void LogFinishedEnter();
	void LogFinishedExit();
	void LogFinished();
};

#endif
