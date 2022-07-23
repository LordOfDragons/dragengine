/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEOGLDEBUGMEMORYCONSUMPTION_H_
#define _DEOGLDEBUGMEMORYCONSUMPTION_H_

class deoglRenderThread;
class deoglMemoryConsumptionGPU;
class deoglMemoryConsumptionTexture;
class deoglMemoryConsumptionRenderBuffer;
class deoglMemoryConsumptionSkin;
class deoglMemoryConsumptionBufferObject;
class deoglMemoryConsumptionDeferredRendering;
class deoglMemoryConsumptionShadow;


/**
 * Debug memory consumption. Logs memory consumption whenever grant total changes.
 */
class deoglDebugMemoryConsumption{
private:
	deoglRenderThread &pRenderThread;
	
	unsigned long long pLastTotal;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create debug. */
	deoglDebugMemoryConsumption( deoglRenderThread &renderThread );
	
	/** Clean up debug. */
	~deoglDebugMemoryConsumption();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Update. */
	void Update();
	
	/** Log memory consumption to logger. */
	void LogConsumption();
	/*@}*/
	
	
	
private:
	void pLog( const deoglMemoryConsumptionGPU &consumption );
	void pLog( const deoglMemoryConsumptionTexture &consumption );
	void pLog( const deoglMemoryConsumptionRenderBuffer &consumption );
	void pLog( const deoglMemoryConsumptionSkin &consumption );
	void pLog( const deoglMemoryConsumptionBufferObject &consumption );
	void pLog( const deoglMemoryConsumptionDeferredRendering &consumption );
	void pLog( const deoglMemoryConsumptionShadow &consumption );
};

#endif
