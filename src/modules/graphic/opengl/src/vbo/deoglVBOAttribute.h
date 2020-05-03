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
 * \brief OpenGL VBO Attribute.
 */
class deoglVBOAttribute{
public:
	/** \brief Data types. */
	enum eDataTypes{
		/** \brief Float input floating point. */
		edtFloat,
		
		/** \brief Float input normalized signed char. */
		edtByte,
		
		/** \brief Float input normalized unsigned char. */
		edtUByte,
		
		/** \brief Float input normalized signed short. */
		edtShort,
		
		/** \brief Float input normalized unsigned short. */
		edtUShort,
		
		/** \brief Float input normalized signed int. */
		edtInt,
		
		/** \brief Float input normalized unsigned int. */
		edtUInt,
		
		/** \brief Integer input signed char without normalization. */
		edtIByte,
		
		/** \brief Integer input unsigned char without normalization. */
		edtIUByte,
		
		/** \brief Integer input signed short without normalization. */
		edtIShort,
		
		/** \brief Integer input unsigned short without normalization. */
		edtIUShort,
		
		/** \brief Integer input signed int without normalization. */
		edtIInt,
		
		/** \brief Integer input unsigned int without normalization. */
		edtIUInt
	};
	
	
	
private:
	int pComponentCount;
	eDataTypes pDataType;
	int pOffset;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create vbo attribute. */
	deoglVBOAttribute();
	
	/** \brief Clean up vbo attribute. */
	~deoglVBOAttribute();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Count of components. */
	inline int GetComponentCount() const{ return pComponentCount; }
	
	/** \brief Set count of components. */
	void SetComponentCount( int componentCount );
	
	/** \brief Data type. */
	inline eDataTypes GetDataType() const{ return pDataType; }
	
	/** \brief Set data type. */
	void SetDataType( eDataTypes dataType );
	
	/** \brief Offset. */
	inline int GetOffset() const{ return pOffset; }
	
	/** \brief Set offset. */
	void SetOffset( int offset );
	
	/**
	 * \brief Set VAO attribute from the stored properties.
	 * 
	 * The VBO is supposed to be already selected. The attribute is enabled prior to be set.
	 */
	void SetVAOAttributeAt( deoglRenderThread &renderThread, int attribute, int stride ) const;
	
	/**
	 * \brief Set VAO attribute from the stored properties.
	 * 
	 * The VBO is supposed to be already selected. The attribute is enabled prior to be set.
	 */
	void SetVAOAttributeAt( deoglRenderThread &renderThread, int attribute, int stride, int offset ) const;
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Set attribute. */
	deoglVBOAttribute &operator=( const deoglVBOAttribute &attribute );
	
	/** \brief Attribute matches another one. */
	bool operator==( const deoglVBOAttribute &attribute ) const;
	
	/** \brief Attribute does not match another one. */
	bool operator!=( const deoglVBOAttribute &attribute ) const;
	/*@}*/
};

#endif
