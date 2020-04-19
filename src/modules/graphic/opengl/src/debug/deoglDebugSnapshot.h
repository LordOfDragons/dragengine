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

#ifndef _DEOGLDEBUGSNAPSHOT_H_
#define _DEOGLDEBUGSNAPSHOT_H_

#include <dragengine/common/string/decString.h>

class deoglRenderThread;
class decBaseFileWriter;



/**
 * \brief Generate debug snapshots.
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
	
	int pStatesKeyWidth;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create debug snapshot. */
	deoglDebugSnapshot( deoglRenderThread &renderThread );
	
	/** \brief Clean up debug snapshot. */
	~deoglDebugSnapshot();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Snapshot name. */
	inline const decString &GetName() const{ return pName; }
	
	/**
	 * \brief Set snapshot name.
	 * \details Can contain '/' to create directories.
	 */
	void SetName( const char *name );
	
	
	
	/** \brief Enable save snapshot of opengl states. */
	inline bool GetEnableStates() const{ return pEnableStates; }
	
	/** \brief Set enable save snapshot of opengl states. */
	void SetEnableStates( bool enable );
	
	/** \brief Enable save snapshot of deferred rendering depth buffer. */
	inline bool GetEnableDepth() const{ return pEnableDepth; }
	
	/** \brief Set enable save snapshot of deferred rendering depth buffer. */
	void SetEnableDepth( bool enable );
	
	/** \brief Enable save snapshot of deferred rendering second depth buffer. */
	inline bool GetEnableDepth2() const{ return pEnableDepth2; }
	
	/** \brief Set enable save snapshot of deferred rendering second depth buffer. */
	void SetEnableDepth2( bool enable );
	
	/** \brief Enable save snapshot of deferred rendering third depth buffer. */
	inline bool GetEnableDepth3() const{ return pEnableDepth3; }
	
	/** \brief Set enable save snapshot of deferred rendering third depth buffer. */
	void SetEnableDepth3( bool enable );
	
	/** \brief Enable save snapshot of deferred rendering stencil buffer. */
	inline bool GetEnableStencil() const{ return pEnableStencil; }
	
	/** \brief Set enable save snapshot of deferred rendering stencil buffer. */
	void SetEnableStencil( bool enable );
	
	/** \brief Enable save snapshot of deferred rendering second stencil buffer. */
	inline bool GetEnableStencil2() const{ return pEnableStencil2; }
	
	/** \brief Set enable save snapshot of second deferred rendering stencil buffer. */
	void SetEnableStencil2( bool enable );
	
	/** \brief Enable save snapshot of deferred rendering color buffer. */
	inline bool GetEnableColor() const{ return pEnableColor; }
	
	/** \brief Set enable save snapshot of deferred rendering color buffer. */
	void SetEnableColor( bool enable );
	
	/** \brief Enable save snapshot of deferred rendering diffuse buffer. */
	inline bool GetEnableDiffuse() const{ return pEnableDiffuse; }
	
	/** \brief Set enable save snapshot of deferred rendering diffuse buffer. */
	void SetEnableDiffuse( bool enable );
	
	/** \brief Enable save snapshot of deferred rendering normal buffer. */
	inline bool GetEnableNormal() const{ return pEnableNormal; }
	
	/** \brief Set enable save snapshot of deferred rendering normal buffer. */
	void SetEnableNormal( bool enable );
	
	/** \brief Enable save snapshot of deferred rendering reflectivity buffer. */
	inline bool GetEnableReflectivity() const{ return pEnableReflectivity; }
	
	/** \brief Set enable save snapshot of deferred rendering reflectivity buffer. */
	void SetEnableReflectivity( bool enable );
	
	/** \brief Enable save snapshot of deferred rendering roughness buffer. */
	inline bool GetEnableRoughness() const{ return pEnableRoughness; }
	
	/** \brief Set enable save snapshot of deferred rendering roughness buffer. */
	void SetEnableRoughness( bool enable );
	
	/** \brief Enable save snapshot of deferred rendering ao solidity buffer. */
	inline bool GetEnableAOSolidity() const{ return pEnableAOSolidity; }
	
	/** \brief Set enable save snapshot of deferred rendering ao solidity buffer. */
	void SetEnableAOSolidity( bool enable );
	
	/** \brief Enable save snapshot of post processing buffer. */
	inline bool GetEnablePostProcess() const{ return pEnablePostProcess; }
	
	/** \brief Set enable save snapshot of post processing buffer. */
	void SetEnablePostProcess( bool enable );
	
	
	
	/** \brief Enable snapshot of deferred rendering depth and stencil buffers. */
	void SetEnableDepthStencil( bool enable );
	
	/** \brief Enable snapshot of second deferred rendering depth and stencil buffers. */
	void SetEnableDepthStencil2( bool enable );
	
	/**
	 * \brief Enable snapshot of deferred rendering material buffers.
	 * \details This includes diffuse, normal, reflectivity, roughness, ao and solidity buffers.
	 */
	void SetEnableMaterialBuffers( bool enable );
	
	/** \brief Enable snapshot of all deferred rendering buffers. */
	void SetEnableAllBuffers( bool enable );
	
	
	
	/** \brief Take snapshot. */
	void TakeSnapshot() const;
	/*@}*/
	
private:
	void pTakeSnapshotStates() const;
	void pWriteLine( decBaseFileWriter &writer, const char *text ) const;
	void pWriteState( decBaseFileWriter &writer, const char *key, const char *value ) const;
	decString pGLConstantName( int value ) const;
	decString pBitString( int value, int maxBits ) const;
};

#endif
