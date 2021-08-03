/* 
 * Drag[en]gine OpenGL Graphic Module
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

#ifndef _DEOGLRRTDEBUG_H_
#define _DEOGLRRTDEBUG_H_

#include "../debug/deoglDebugInformationList.h"
#include "../debug/deoglDebugMemoryConsumption.h"

#include <dragengine/common/file/decBaseFileWriter.h>

class decPath;
class deoglRenderThread;
class deoglDebugSaveTexture;
class deoglDeveloperMode;



/**
 * \brief Render thread debug.
 */
class deoglRTDebug{
private:
	deoglRenderThread &pRenderThread;
	deoglDebugSaveTexture *pDebugSaveTexture;
	deoglDeveloperMode *pDeveloperMode;
	deoglDebugInformationList pDebugInformation;
	bool pEnableHwDebugOutput;
	deoglDebugMemoryConsumption pDebugMemoryConsumption;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create render thread texture related object. */
	deoglRTDebug( deoglRenderThread &renderThread );
	
	/** \brief Clean up render thread texture related object. */
	virtual ~deoglRTDebug();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Debug save texture. */
	inline deoglDebugSaveTexture &GetDebugSaveTexture() const{ return *pDebugSaveTexture; }
	
	/** \brief Developer mode. */
	inline deoglDeveloperMode &GetDeveloperMode() const{ return *pDeveloperMode; }
	
	/**
	 * \brief Open file for writing safely from inside render thread.
	 * \details Creates a temporary file writer with the content written to the actual file
	 *          once the render thread synchronizes with the main thread.
	 */
	decBaseFileWriter *OpenFileForWriting( const decPath &path );
	
	/** \brief Debug information list. */
	inline deoglDebugInformationList &GetDebugInformationList(){ return pDebugInformation; }
	inline const deoglDebugInformationList &GetDebugInformationList() const{ return pDebugInformation; }
	
	/** \brief Enable hardware debug output. */
	inline bool GetEnableHwDebugOutput() const{ return pEnableHwDebugOutput; }
	
	/** \brief Set enable hardware debug output. */
	void SetEnableHwDebugOutput( bool enable );
	
	/** \brief Debug memory consumption. */
	inline deoglDebugMemoryConsumption &GetDebugMemoryConsumption(){ return pDebugMemoryConsumption; }
	inline const deoglDebugMemoryConsumption &GetDebugMemoryConsumption() const{ return pDebugMemoryConsumption; }
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
