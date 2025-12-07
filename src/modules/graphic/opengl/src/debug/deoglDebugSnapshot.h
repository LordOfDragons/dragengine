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

#ifndef _DEOGLDEBUGSNAPSHOT_H_
#define _DEOGLDEBUGSNAPSHOT_H_

#include <dragengine/common/string/decString.h>

class deoglRenderThread;
class decBaseFileWriter;



/**
 * Generate debug snapshots.
 * 
 * Snapshots contain of writing to file current opengl states and optionally
 * saving current deferred rendering buffers to file. All saved files are
 * store in one directory per snapshot.
 */
class deoglDebugSnapshot{
private:
	deoglRenderThread &pRenderThread;
	decString pName;
	bool pEnableStates;
	bool pEnableDepth;
	bool pEnableDepth2;
	bool pEnableDepth3;
	bool pEnableStencil;
	bool pEnableStencil2;
	bool pEnableColor;
	bool pEnableDiffuse;
	bool pEnableNormal;
	bool pEnableReflectivity;
	bool pEnableRoughness;
	bool pEnableAOSolidity;
	bool pEnablePostProcess;
	bool pEnableTemporary1;
	bool pEnableTemporary2;
	bool pEnableTemporary3;
	
	int pStatesKeyWidth;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create debug snapshot. */
	deoglDebugSnapshot(deoglRenderThread &renderThread);
	
	/** Clean up debug snapshot. */
	~deoglDebugSnapshot();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Snapshot name. */
	inline const decString &GetName() const{return pName;}
	
	/**
	 * Set snapshot name.
	 * \details Can contain '/' to create directories.
	 */
	void SetName(const char *name);
	
	
	
	/** Enable save snapshot of opengl states. */
	inline bool GetEnableStates() const{return pEnableStates;}
	
	/** Set enable save snapshot of opengl states. */
	void SetEnableStates(bool enable);
	
	/** Enable save snapshot of deferred rendering depth buffer. */
	inline bool GetEnableDepth() const{return pEnableDepth;}
	
	/** Set enable save snapshot of deferred rendering depth buffer. */
	void SetEnableDepth(bool enable);
	
	/** Enable save snapshot of deferred rendering second depth buffer. */
	inline bool GetEnableDepth2() const{return pEnableDepth2;}
	
	/** Set enable save snapshot of deferred rendering second depth buffer. */
	void SetEnableDepth2(bool enable);
	
	/** Enable save snapshot of deferred rendering third depth buffer. */
	inline bool GetEnableDepth3() const{return pEnableDepth3;}
	
	/** Set enable save snapshot of deferred rendering third depth buffer. */
	void SetEnableDepth3(bool enable);
	
	/** Enable save snapshot of deferred rendering stencil buffer. */
	inline bool GetEnableStencil() const{return pEnableStencil;}
	
	/** Set enable save snapshot of deferred rendering stencil buffer. */
	void SetEnableStencil(bool enable);
	
	/** Enable save snapshot of deferred rendering second stencil buffer. */
	inline bool GetEnableStencil2() const{return pEnableStencil2;}
	
	/** Set enable save snapshot of second deferred rendering stencil buffer. */
	void SetEnableStencil2(bool enable);
	
	/** Enable save snapshot of deferred rendering color buffer. */
	inline bool GetEnableColor() const{return pEnableColor;}
	
	/** Set enable save snapshot of deferred rendering color buffer. */
	void SetEnableColor(bool enable);
	
	/** Enable save snapshot of deferred rendering diffuse buffer. */
	inline bool GetEnableDiffuse() const{return pEnableDiffuse;}
	
	/** Set enable save snapshot of deferred rendering diffuse buffer. */
	void SetEnableDiffuse(bool enable);
	
	/** Enable save snapshot of deferred rendering normal buffer. */
	inline bool GetEnableNormal() const{return pEnableNormal;}
	
	/** Set enable save snapshot of deferred rendering normal buffer. */
	void SetEnableNormal(bool enable);
	
	/** Enable save snapshot of deferred rendering reflectivity buffer. */
	inline bool GetEnableReflectivity() const{return pEnableReflectivity;}
	
	/** Set enable save snapshot of deferred rendering reflectivity buffer. */
	void SetEnableReflectivity(bool enable);
	
	/** Enable save snapshot of deferred rendering roughness buffer. */
	inline bool GetEnableRoughness() const{return pEnableRoughness;}
	
	/** Set enable save snapshot of deferred rendering roughness buffer. */
	void SetEnableRoughness(bool enable);
	
	/** Enable save snapshot of deferred rendering ao solidity buffer. */
	inline bool GetEnableAOSolidity() const{return pEnableAOSolidity;}
	
	/** Set enable save snapshot of deferred rendering ao solidity buffer. */
	void SetEnableAOSolidity(bool enable);
	
	/** Enable save snapshot of post processing buffer. */
	inline bool GetEnablePostProcess() const{return pEnablePostProcess;}
	
	/** Set enable save snapshot of post processing buffer. */
	void SetEnablePostProcess(bool enable);
	
	/** Enable save snapshot of deferred rendering temporary 1 buffer. */
	inline bool GetEnableTemportary1() const{return pEnableTemporary1;}
	
	/** Set enable save snapshot of deferred rendering temporary 1 buffer. */
	void SetEnableTemporary1(bool enable);
	
	/** Enable save snapshot of deferred rendering temporary 2 buffer. */
	inline bool GetEnableTemportary2() const{return pEnableTemporary2;}
	
	/** Set enable save snapshot of deferred rendering temporary 2 buffer. */
	void SetEnableTemporary2(bool enable);
	
	/** Enable save snapshot of deferred rendering temporary 3 buffer. */
	inline bool GetEnableTemportary3() const{return pEnableTemporary3;}
	
	/** Set enable save snapshot of deferred rendering temporary 3 buffer. */
	void SetEnableTemporary3(bool enable);
	
	
	
	/** Enable snapshot of deferred rendering depth and stencil buffers. */
	void SetEnableDepthStencil(bool enable);
	
	/** Enable snapshot of second deferred rendering depth and stencil buffers. */
	void SetEnableDepthStencil2(bool enable);
	
	/**
	 * Enable snapshot of deferred rendering material buffers.
	 * \details This includes diffuse, normal, reflectivity, roughness, ao and solidity buffers.
	 */
	void SetEnableMaterialBuffers(bool enable);
	
	/** Enable snapshot of all deferred rendering buffers. */
	void SetEnableAllBuffers(bool enable);
	
	
	
	/** Take snapshot. */
	void TakeSnapshot() const;
	/*@}*/
	
private:
	void pTakeSnapshotStates() const;
	void pWriteLine(decBaseFileWriter &writer, const char *text) const;
	void pWriteState(decBaseFileWriter &writer, const char *key, const char *value) const;
	decString pGLConstantName(int value) const;
	decString pBitString(int value, int maxBits) const;
};

#endif
