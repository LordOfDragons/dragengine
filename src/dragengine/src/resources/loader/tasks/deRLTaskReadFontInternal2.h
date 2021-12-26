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

#ifndef _DERLTASKREADFONTINTERNAL2_H_
#define _DERLTASKREADFONTINTERNAL2_H_

#include "deResourceLoaderTask.h"
#include "../../font/deFontReference.h"


/**
 * \brief Read font resource loader task.
 * 
 * \warning For internal use only.
 */
class DE_DLL_EXPORT deRLTaskReadFontInternal2 : public deResourceLoaderTask {
private:
	deFontReference pFont;
	bool pAlreadyLoaded;
	
	deResourceLoaderTask *pTaskImage;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create task. */
	deRLTaskReadFontInternal2( deEngine &engine, deResourceLoader &resourceLoader,
		deVirtualFileSystem *vfs, const char *path, deFont *font );
	
	/** \brief Clean up task. */
	virtual ~deRLTaskReadFontInternal2();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Parallel task implementation. */
	virtual void Run();
	
	/** \brief Synchronous processing of task Run() finished. */
	virtual void Finished();
	
	/** \brief Font has been already loaded. */
	inline bool GetAlreadyLoaded() const{ return pAlreadyLoaded; }
	/*@}*/
	
	
	
	/** \name Debugging */
	/*@{*/
	/** \brief Short task name for debugging. */
	virtual decString GetDebugName() const;
	/*@}*/
	
	
	
private:
	void pLoadFontResources();
};

#endif
