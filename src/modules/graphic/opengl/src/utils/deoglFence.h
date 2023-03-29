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

#ifndef _DEOGLFENCE_H_
#define _DEOGLFENCE_H_

#include "../deoglBasics.h"

#include <dragengine/deObject.h>

class deoglRenderThread;


/**
 * Fence.
 */
class deoglFence : public deObject{
public:
	typedef deTObjectReference<deoglFence> Ref;
	
	
	
private:
	deoglRenderThread &pRenderThread;
	GLsync pFence;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create fence. */
	deoglFence( deoglRenderThread &renderThread );
	
protected:
	/** Clean up fence. */
	virtual ~deoglFence();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Fence or 0 if not armed. */
	inline GLsync GetFence() const{ return pFence; }
	
	/** Resets fence then arms it. */
	void Arm();
	
	/** Test if fence has fired. If fence is not armed true is returned. */
	bool HasFired();
	
	/** Wait for fence to be fired. If fence is not armed returns immediately. */
	void Wait();
	
	/** Deletes fence if present. */
	void Reset();
	/*@}*/
};

#endif
