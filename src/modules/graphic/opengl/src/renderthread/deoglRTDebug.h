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
	deoglRTDebug(deoglRenderThread &renderThread);
	
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
	decBaseFileWriter *OpenFileForWriting(const decPath &path);
	
	/** Debug information list. */
	inline deoglDebugInformationList &GetDebugInformationList(){ return pDebugInformation; }
	inline const deoglDebugInformationList &GetDebugInformationList() const{ return pDebugInformation; }
	
	/** Enable hardware debug output. */
	inline bool GetEnableHwDebugOutput() const{ return pEnableHwDebugOutput; }
	
	/** Set enable hardware debug output. */
	void SetEnableHwDebugOutput(bool enable);
	
	/** Debug memory consumption. */
	inline deoglDebugMemoryConsumption &GetDebugMemoryConsumption(){ return pDebugMemoryConsumption; }
	inline const deoglDebugMemoryConsumption &GetDebugMemoryConsumption() const{ return pDebugMemoryConsumption; }
	
	/** Debug trace grouping is enabled. */
	inline bool GetEnableDebugTrace() const{ return pEnableDebugTrace; }
	
	/** Begin debug group for structuring GPU traces. */
	void BeginDebugGroup(const char *name, int id);
	
	/** End debug group for structuring GPU traces. */
	void EndDebugGroup();
	
	/** Set debug object label. */
	void SetDebugObjectLabel(GLenum type, GLuint object, const char *name);
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
