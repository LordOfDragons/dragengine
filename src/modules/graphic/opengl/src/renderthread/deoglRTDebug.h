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

#include "../deoglBasics.h"
#include "../debug/deoglDebugInformationList.h"
#include "../debug/deoglDebugMemoryConsumption.h"

#include <dragengine/common/file/decBaseFileWriter.h>

class decPath;
class deoglRenderThread;
class deoglDebugSaveTexture;
class deoglDeveloperMode;



/**
 * Render thread debug.
 */
class deoglRTDebug{
private:
	deoglRenderThread &pRenderThread;
	deoglDebugSaveTexture *pDebugSaveTexture;
	deoglDeveloperMode *pDeveloperMode;
	deoglDebugInformationList pDebugInformation;
	bool pEnableHwDebugOutput;
	deoglDebugMemoryConsumption pDebugMemoryConsumption;
	bool pEnableDebugTrace;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render thread texture related object. */
	deoglRTDebug( deoglRenderThread &renderThread );
	
	/** Clean up render thread texture related object. */
	virtual ~deoglRTDebug();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Debug save texture. */
	inline deoglDebugSaveTexture &GetDebugSaveTexture() const{ return *pDebugSaveTexture; }
	
	/** Developer mode. */
	inline deoglDeveloperMode &GetDeveloperMode() const{ return *pDeveloperMode; }
	
	/**
	 * Open file for writing safely from inside render thread.
	 * \details Creates a temporary file writer with the content written to the actual file
	 *          once the render thread synchronizes with the main thread.
	 */
	decBaseFileWriter *OpenFileForWriting( const decPath &path );
	
	/** Debug information list. */
	inline deoglDebugInformationList &GetDebugInformationList(){ return pDebugInformation; }
	inline const deoglDebugInformationList &GetDebugInformationList() const{ return pDebugInformation; }
	
	/** Enable hardware debug output. */
	inline bool GetEnableHwDebugOutput() const{ return pEnableHwDebugOutput; }
	
	/** Set enable hardware debug output. */
	void SetEnableHwDebugOutput( bool enable );
	
	/** Debug memory consumption. */
	inline deoglDebugMemoryConsumption &GetDebugMemoryConsumption(){ return pDebugMemoryConsumption; }
	inline const deoglDebugMemoryConsumption &GetDebugMemoryConsumption() const{ return pDebugMemoryConsumption; }
	
	/** Debug trace grouping is enabled. */
	inline bool GetEnableDebugTrace() const{ return pEnableDebugTrace; }
	
	/** Begin debug group for structuring GPU traces. */
	void BeginDebugGroup( const char *name, int id );
	
	/** End debug group for structuring GPU traces. */
	void EndDebugGroup();
	
	/** Set debug object label. */
	void SetDebugObjectLabel( GLenum type, GLuint object, const char *name );
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
