/* 
 * Drag[en]gine Game Engine
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
class deResourceLoaderTask : public deParallelTask{
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
