/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEOGLRLOADERTHREADTASK_H_
#define _DEOGLRLOADERTHREADTASK_H_

#include <dragengine/deObject.h>
#include <dragengine/threading/deThreadSafeObject.h>
#include <dragengine/threading/deThreadSafeObjectReference.h>


/**
 * Loader thread task.
 */
class deoglLoaderThreadTask : public deThreadSafeObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deoglLoaderThreadTask> Ref;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create loader thread task. */
	deoglLoaderThreadTask();
	
protected:
	/** Clean up loader thread task. */
	virtual ~deoglLoaderThreadTask();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Run task. */
	virtual void Run() = 0;
	/*@}*/
};

#endif
