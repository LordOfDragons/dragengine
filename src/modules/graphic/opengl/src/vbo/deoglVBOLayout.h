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
 * \brief OpenGL VBO Layout.
 */
class deoglVBOLayout{
public:
	/** \brief Index types. */
	enum eIndexTypes{
		/** \brief No indices. */
		eitNone,
		
		/** \brief Unsigned int indices. */
		eitUnsignedInt,
		
		/** \brief Unsigned short indices. */
		eitUnsignedShort,
		
		/** \brief Unsigned byte indices. */
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
	/** \brief Create vbo layout. */
	deoglVBOLayout();
	
	/** \brief Create vbo layout. */
	deoglVBOLayout( const deoglVBOLayout &layout );
	
	/** \brief Clean up vbo layout. */
	~deoglVBOLayout();
	/*@}*/
	

	
	/** \name Management */
	/*@{*/
	/** \brief VBO size. */
	inline int GetSize() const{ return pSize; }
	
	/** \brief Set VBO size. */
	void SetSize( int size );
	
	/** \brief Stride. */
	inline int GetStride() const{ return pStride; }
	
	/** \brief Set stride. */
	void SetStride( int stride );
	
	/** \brief Index type. */
	inline eIndexTypes GetIndexType() const{ return pIndexType; }
	
	/** \brief Set index type. */
	void SetIndexType( eIndexTypes indexType );
	
	/** \brief Size in bytes of indices. */
	inline int GetIndexSize() const{ return pIndexSize; }
	
	/** \brief Opengl type of the indices. */
	inline int GetIndexGLType() const{ return pIndexGLType; }
	/*@}*/
	
	
	
	/** \name Attributes */
	/*@{*/
	/** \brief Number of attributes. */
	inline int GetAttributeCount() const{ return pAttributeCount; }
	
	/** \brief Set number of attributes. */
	void SetAttributeCount( int count );
	
	/** \brief Attribute by index. */
	deoglVBOAttribute &GetAttributeAt( int index ) const;
	
	/**
	 * \brief Set VAO attribute.
	 * 
	 * The VBO is supposed to be already selected. The attribute is enabled prior to be set.
	 */
	void SetVAOAttributeAt( deoglRenderThread &renderThread, int attribute, int target ) const;
	
	/**
	 * \brief Set VAO attribute.
	 * 
	 * The VBO is supposed to be already selected. The attribute is enabled prior to be set.
	 */
	void SetVAOAttributeAt( deoglRenderThread &renderThread, int attribute, int target, int offset ) const;
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Set layout. */
	deoglVBOLayout &operator=( const deoglVBOLayout &layout );
	
	/** \brief Layout matches another one. */
	bool operator==( const deoglVBOLayout &layout ) const;
	
	/** \brief Layout does not match another one. */
	bool operator!=( const deoglVBOLayout &layout ) const;
	/*@}*/
	
	
	
	/** \name Debugging */
	/*@{*/
	/** \brief Print layout to the console. */
	void PrintToConsole( deoglRenderThread &renderThread, const char *name );
	/*@}*/
};

#endif
