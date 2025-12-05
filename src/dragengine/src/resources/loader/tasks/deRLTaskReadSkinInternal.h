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

#ifndef _DERLTASKREADSKININTERNAL_H_
#define _DERLTASKREADSKININTERNAL_H_

#include "deResourceLoaderTask.h"
#include "../../skin/deSkin.h"
#include "../../../common/collection/decThreadSafeObjectOrderedSet.h"

class deSkinPropertyImage;
class deSkinPropertyNodeImage;
class deSkinPropertyNodeText;


/**
 * \brief Read skin resource loader task.
 * 
 * \warning For internal use only.
 */
class DE_DLL_EXPORT deRLTaskReadSkinInternal : public deResourceLoaderTask {
public:
	/**
	 * \brief For internal use only.
	 * 
	 * Used to store tasks obtained by AddLoadRequest. This class holds a reference to
	 * the task. Do not release the reference on the task returned by AddLoadRequest after
	 * creating a cInternalTask instance since AddLoadRequest does not add a reference
	 * for the caller
	 */
	class DE_DLL_EXPORT cInternalTask : public deThreadSafeObject{
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
	deSkin::Ref pSkin;
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
