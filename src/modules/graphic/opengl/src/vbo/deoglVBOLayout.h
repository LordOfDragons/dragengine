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
 * OpenGL VBO Layout.
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
		eitUnsignedByte
	};
	
	
	
private:
	int pSize;
	int pStride;
	deoglVBOAttribute *pAttributes;
	int pAttributeCount;
	eIndexTypes pIndexType;
	int pIndexSize;
	int pIndexGLType;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create vbo layout. */
	deoglVBOLayout();
	
	/** Create vbo layout. */
	deoglVBOLayout( const deoglVBOLayout &layout );
	
	/** Clean up vbo layout. */
	~deoglVBOLayout();
	/*@}*/
	

	
	/** \name Management */
	/*@{*/
	/** VBO size. */
	inline int GetSize() const{ return pSize; }
	
	/** Set VBO size. */
	void SetSize( int size );
	
	/** Stride. */
	inline int GetStride() const{ return pStride; }
	
	/** Set stride. */
	void SetStride( int stride );
	
	/** Index type. */
	inline eIndexTypes GetIndexType() const{ return pIndexType; }
	
	/** Set index type. */
	void SetIndexType( eIndexTypes indexType );
	
	/** Size in bytes of indices. */
	inline int GetIndexSize() const{ return pIndexSize; }
	
	/** Opengl type of the indices. */
	inline int GetIndexGLType() const{ return pIndexGLType; }
	/*@}*/
	
	
	
	/** \name Attributes */
	/*@{*/
	/** Number of attributes. */
	inline int GetAttributeCount() const{ return pAttributeCount; }
	
	/** Set number of attributes. */
	void SetAttributeCount( int count );
	
	/** Attribute by index. */
	deoglVBOAttribute &GetAttributeAt( int index ) const;
	
	/**
	 * Set VAO attribute.
	 * 
	 * The VBO is supposed to be already selected. The attribute is enabled prior to be set.
	 */
	void SetVAOAttributeAt( deoglRenderThread &renderThread, int attribute, int target ) const;
	
	/**
	 * Set VAO attribute.
	 * 
	 * The VBO is supposed to be already selected. The attribute is enabled prior to be set.
	 */
	void SetVAOAttributeAt( deoglRenderThread &renderThread, int attribute, int target, int offset ) const;
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** Set layout. */
	deoglVBOLayout &operator=( const deoglVBOLayout &layout );
	
	/** Layout matches another one. */
	bool operator==( const deoglVBOLayout &layout ) const;
	
	/** Layout does not match another one. */
	bool operator!=( const deoglVBOLayout &layout ) const;
	/*@}*/
	
	
	
	/** \name Debugging */
	/*@{*/
	/** Print layout to the console. */
	void PrintToConsole( deoglRenderThread &renderThread, const char *name );
	/*@}*/
};

#endif
