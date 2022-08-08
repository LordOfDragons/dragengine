/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEOGLDEBUGTRACEGROUP_H_
#define _DEOGLDEBUGTRACEGROUP_H_

class deoglRenderThread;


/**
 * Scoped debug tracing group.
 */
class deoglDebugTraceGroup{
private:
	const deoglRenderThread &pRenderThread;
	bool pOpen;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Begin debug tracing group. */
	deoglDebugTraceGroup( const deoglRenderThread &renderThread, const char *name, int id = 0 );
	
	/** Close other group then begin debug tracing group. */
	deoglDebugTraceGroup( deoglDebugTraceGroup &closeGroup, const char *name, int id = 0 );
	
	/** End debug tracing group if open. */
	~deoglDebugTraceGroup();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** End debug tracing group if open. */
	void Close();
	/*@}*/
};

#endif
