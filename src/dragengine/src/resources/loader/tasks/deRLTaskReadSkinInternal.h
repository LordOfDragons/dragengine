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

#ifndef _DERLTASKREADSKININTERNAL_H_
#define _DERLTASKREADSKININTERNAL_H_

#include "deResourceLoaderTask.h"
#include "../../skin/deSkinReference.h"
#include "../../../common/collection/decThreadSafeObjectOrderedSet.h"

class deSkinPropertyImage;
class deSkinPropertyNodeImage;
class deSkinPropertyNodeText;


/**
 * \brief Read skin resource loader task.
 * 
 * \warning For internal use only.
 */
class deRLTaskReadSkinInternal : public deResourceLoaderTask {
public:
	/**
	 * \brief For internal use only.
	 * 
	 * Used to store tasks obtained by AddLoadRequest. This class holds a reference to
	 * the task. Do not release the reference on the task returned by AddLoadRequest after
	 * creating a cInternalTask instance since AddLoadRequest does not add a reference
	 * for the caller
	 */
	class cInternalTask : public deThreadSafeObject{
	private:
		deSkinPropertyImage *pPropertyImage;
		deSkinPropertyNodeImage *pNodeImage;
		deSkinPropertyNodeText *pNodeText;
		deResourceLoaderTask *pTask;
		
	public:
		cInternalTask( deSkinPropertyImage *propertyImage, deResourceLoaderTask *task );
		cInternalTask( deSkinPropertyNodeImage *nodeImage, deResourceLoaderTask *task );
		cInternalTask( deSkinPropertyNodeText *nodeText, deResourceLoaderTask *task );
		virtual ~cInternalTask();
		
		inline deSkinPropertyImage *GetPropertyImage() const{ return pPropertyImage; }
		inline deSkinPropertyNodeImage *GetNodeImage() const{ return pNodeImage; }
		inline deSkinPropertyNodeText *GetNodeText() const{ return pNodeText; }
		inline deResourceLoaderTask *GetTask() const{ return pTask; }
	};
	
	
	
private:
	deSkinReference pSkin;
	decThreadSafeObjectOrderedSet pInternalTasks;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create task. */
	deRLTaskReadSkinInternal( deEngine &engine, deResourceLoader &resourceLoader,
		deVirtualFileSystem *vfs, const char *path );
	
	/** \brief Clean up task. */
	virtual ~deRLTaskReadSkinInternal();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Parallel task implementation. */
	virtual void Run();
	
	/** \brief Synchronous processing of task Run() finished. */
	virtual void Finished();
	
	/** \brief Skin. */
	inline deSkin *GetSkin() const{ return pSkin; }
	/*@}*/
	
	
	
	/** \name Internal use only */
	/*@{*/
	void AddInternalTask( cInternalTask *task );
	/*@}*/
	
	
	
	/** \name Debugging */
	/*@{*/
	/** \brief Short task name for debugging. */
	virtual decString GetDebugName() const;
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pPrepare();
	void pLoadInternal();
	bool pApplyInternal();
};

#endif
