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

#ifndef _DEOGLVBOATTRIBUTE_H_
#define _DEOGLVBOATTRIBUTE_H_

class deoglRenderThread;



/**
 * OpenGL VBO Attribute.
 */
class deoglVBOAttribute{
public:
	/** Data types. */
	enum eDataTypes{
		/** Float input floating point. */
		edtFloat,
		
		/** Float input normalized signed char. */
		edtByte,
		
		/** Float input normalized unsigned char. */
		edtUByte,
		
		/** Float input normalized signed short. */
		edtShort,
		
		/** Float input normalized unsigned short. */
		edtUShort,
		
		/** Float input normalized signed int. */
		edtInt,
		
		/** Float input normalized unsigned int. */
		edtUInt,
		
		/** Integer input signed char without normalization. */
		edtIByte,
		
		/** Integer input unsigned char without normalization. */
		edtIUByte,
		
		/** Integer input signed short without normalization. */
		edtIShort,
		
		/** Integer input unsigned short without normalization. */
		edtIUShort,
		
		/** Integer input signed int without normalization. */
		edtIInt,
		
		/** Integer input unsigned int without normalization. */
		edtIUInt
	};
	
	
	
private:
	int pComponentCount;
	eDataTypes pDataType;
	int pOffset;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create vbo attribute. */
	deoglVBOAttribute();
	
	/** Clean up vbo attribute. */
	~deoglVBOAttribute();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Count of components. */
	inline int GetComponentCount() const{ return pComponentCount; }
	
	/** Set count of components. */
	void SetComponentCount( int componentCount );
	
	/** Data type. */
	inline eDataTypes GetDataType() const{ return pDataType; }
	
	/** Set data type. */
	void SetDataType( eDataTypes dataType );
	
	/** Offset. */
	inline int GetOffset() const{ return pOffset; }
	
	/** Set offset. */
	void SetOffset( int offset );
	
	/**
	 * Set VAO attribute from the stored properties.
	 * 
	 * The VBO is supposed to be already selected. The attribute is enabled prior to be set.
	 */
	void SetVAOAttributeAt( deoglRenderThread &renderThread, int attribute, int stride ) const;
	
	/**
	 * Set VAO attribute from the stored properties.
	 * 
	 * The VBO is supposed to be already selected. The attribute is enabled prior to be set.
	 */
	void SetVAOAttributeAt( deoglRenderThread &renderThread, int attribute, int stride, int offset ) const;
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** Set attribute. */
	deoglVBOAttribute &operator=( const deoglVBOAttribute &attribute );
	
	/** Attribute matches another one. */
	bool operator==( const deoglVBOAttribute &attribute ) const;
	
	/** Attribute does not match another one. */
	bool operator!=( const deoglVBOAttribute &attribute ) const;
	/*@}*/
};

#endif
