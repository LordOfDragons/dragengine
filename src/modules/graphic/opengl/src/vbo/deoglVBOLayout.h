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

#ifndef _DEOGLVBOLAYOUT_H_
#define _DEOGLVBOLAYOUT_H_

class deoglVBOAttribute;
class deoglRenderThread;



/**
 * @brief OpenGL VBO Layout.
 * Stores the layout of a VBO.
 */
class deoglVBOLayout{
public:
	/** Index types. */
	enum eIndexTypes{
		/** No indices. */
		eitNone,
		/** Unsigned int indices. */
		eitUnsignedInt,
		/** Unsigned short indices. */
		eitUnsignedShort,
		/** Unsigned byte indices. */
		eitUnsignedByte,
		/** Number of index types. */
		EIT_COUNT
	};
	
public:
	int pSize;
	int pStride;
	deoglVBOAttribute *pAttributes;
	int pAttributeCount;
	eIndexTypes pIndexType;
	int pIndexSize;
	int pIndexGLType;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new opengl vbo layout. */
	deoglVBOLayout();
	/** Cleans up the opengl vbo layout. */
	~deoglVBOLayout();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the VBO size. */
	inline int GetSize() const{ return pSize; }
	/** Sets the VBO size. */
	void SetSize( int size );
	/** Retrieves the stride. */
	inline int GetStride() const{ return pStride; }
	/** Sets the stride. */
	void SetStride( int stride );
	/** Retrieves the index type. */
	inline eIndexTypes GetIndexType() const{ return pIndexType; }
	/** Sets the index type. */
	void SetIndexType( eIndexTypes indexType );
	/** Retrieves the size in bytes of indices. */
	inline int GetIndexSize() const{ return pIndexSize; }
	/** Retrieves the opengl type of the indices. */
	inline int GetIndexGLType() const{ return pIndexGLType; }
	/*@}*/
	
	/** @name Attributes */
	/*@{*/
	/** Retrieves the number of attributes. */
	inline int GetAttributeCount() const{ return pAttributeCount; }
	/** Sets the number of attributes. */
	void SetAttributeCount( int count );
	/** Retrieves the attribute by index. */
	deoglVBOAttribute &GetAttributeAt( int index ) const;
	/**
	 * Set VAO attribute for the given target attribute. The VBO is supposed to
	 * be already selected. The attribute is enabled prior to be set.
	 */
	void SetVAOAttributeAt( deoglRenderThread &renderThread, int attribute, int target ) const;
	/**
	 * Set VAO attribute for the given target attribute. The VBO is supposed to
	 * be already selected. The attribute is enabled prior to be set.
	 */
	void SetVAOAttributeAt( deoglRenderThread &renderThread, int attribute, int target, int offset ) const;
	/*@}*/
	
	/** @name Operators */
	/*@{*/
	/** Determines if this layout matches another one. */
	bool operator==( const deoglVBOLayout &layout ) const;
	/** Determines if this layout does not match another one. */
	bool operator!=( const deoglVBOLayout &layout ) const;
	/*@}*/
	
	/** @name Debugging */
	/*@{*/
	/** Prints the layout to the console. */
	void PrintToConsole( deoglRenderThread &renderThread, const char *name );
	/*@}*/
};

// end of include only once
#endif
