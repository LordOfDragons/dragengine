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

#ifndef _DEOGLSHADERPARAMETERBLOCK_H_
#define _DEOGLSHADERPARAMETERBLOCK_H_

#include "deoglSPBParameter.h"
#include "../../deoglBasics.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/deObject.h>

class deoglRenderThread;
class deoglSPBParameter;



/**
 * \brief OpenGL shader parameter block.
 * 
 * Subclasses provide the actual implementation depending on the chosen backend.
 */
class deoglShaderParameterBlock : public deObject{
private:
	deoglRenderThread &pRenderThread;
	
	deoglSPBParameter *pParameters;
	int pParameterCount;
	bool pRowMajor;
	
	int pElementCount;
	int pElementStride;
	int pOffsetPadding;
	
	int pBufferSize;
	
	char *pMapped;
	int pElementLower;
	int pElementUpper;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create shader storage buffer object. */
	deoglShaderParameterBlock( deoglRenderThread &renderThread );
	
	/** \brief Create copy of shader storage buffer object. */
	deoglShaderParameterBlock( const deoglShaderParameterBlock &paramBlock );
	
protected:
	/** \brief Clean up shader storage buffer object. */
	virtual ~deoglShaderParameterBlock();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** \brief Number of parameters. */
	inline int GetParameterCount() const{ return pParameterCount; }
	
	/** \brief Set number of parameters. */
	void SetParameterCount( int count );
	
	/** \brief Parameter at index. */
	deoglSPBParameter &GetParameterAt( int index ) const;
	
	/** \brief Matrices are stored in row-major order. */
	inline bool GetRowMajor() const{ return pRowMajor; }
	
	/** \brief Set if matrices are stored in row-major order. */
	void SetRowMajor( bool rowMajor );
	
	/** \brief Number of elements. */
	inline int GetElementCount() const{ return pElementCount; }
	
	/** \brief Set number of elements. */
	void SetElementCount( int count );
	
	/** \brief Map the parameter block definition to a shader uniform block using std140 layout. */
	void MapToStd140();
	
	
	
	/** \brief Activate parameter block. */
	virtual void Activate() = 0;
	
	/** \brief Deactivate parameter block. */
	virtual void Deactivate() = 0;
	
	
	
	/** \brief Element stride. */
	inline int GetElementStride() const{ return pElementStride; }
	
	/** \brief Buffer offset padding required as multiple of 4. */
	inline int GetOffsetPadding() const{ return pOffsetPadding; }
	
	/** \brief Calculate buffer offset padding and adjust element stride. */
	void CalculateOffsetPadding();
	
	/** \brief Buffer size. */
	inline int GetBufferSize() const{ return pBufferSize; }
	
	/** \brief Mapped buffer pointer or \em NULL if not mapped. */
	inline char *GetMappedBuffer() const{ return pMapped; }
	
	/** \brief Buffer is mapped. */
	bool IsBufferMapped() const;
	
	/** \brief Map buffer discarding content. */
	virtual void MapBuffer() = 0;
	
	/**
	 * \brief Map buffer for specific element discarding content.
	 * 
	 * Data outside the element range is retained. Any attempt to call SetParameter* with
	 * an element index other than the one used for mapping throws an exception.
	 */
	virtual void MapBuffer( int element ) = 0;
	
	/** \brief Unmap buffer uploading data to GPU. */
	virtual void UnmapBuffer() = 0;
	
	
	
	/** \brief Set data for float type parameter. */
	void SetParameterDataFloat( int parameter, float value );
	
	void SetParameterDataVec2( int parameter, float value1, float value2 );
	void SetParameterDataVec2( int parameter, const decVector2 &vector );
	
	void SetParameterDataVec3( int parameter, float value1, float value2, float value3 );
	void SetParameterDataVec3( int parameter, const decColor &color );
	void SetParameterDataVec3( int parameter, const decVector &vector );
	void SetParameterDataVec3( int parameter, const decDVector &vector );
	
	void SetParameterDataVec4( int parameter, float value1, float value2,
		float value3, float value4 );
	void SetParameterDataVec4( int parameter, const decColor &color );
	void SetParameterDataVec4( int parameter, const decColor &color, float alpha );
	void SetParameterDataVec4( int parameter, const decVector4 &vector );
	void SetParameterDataVec4( int parameter, const decDVector4 &vector );
	void SetParameterDataVec4( int parameter, const decVector &vector, float w );
	void SetParameterDataVec4( int parameter, const decDVector4 &vector, double w );
	
	void SetParameterDataMat3x2( int parameter, const float *values );
	void SetParameterDataMat3x2( int parameter, const decMatrix &matrix );
	void SetParameterDataMat3x2( int parameter, const decDMatrix &matrix );
	void SetParameterDataMat3x2( int parameter, const decTexMatrix &matrix );
	void SetParameterDataMat3x2( int parameter, const decTexMatrix2 &matrix );
	void SetParameterDataMat3x3( int parameter, const float *values );
	void SetParameterDataMat3x3( int parameter, const decMatrix &matrix );
	void SetParameterDataMat3x3( int parameter, const decDMatrix &matrix );
	void SetParameterDataMat3x3( int parameter, const decTexMatrix &matrix );
	void SetParameterDataMat4x3( int parameter, const float *values );
	void SetParameterDataMat4x3( int parameter, const decMatrix &matrix );
	void SetParameterDataMat4x3( int parameter, const decDMatrix &matrix );
	void SetParameterDataMat4x4( int parameter, const float *values );
	void SetParameterDataMat4x4( int parameter, const decMatrix &matrix );
	void SetParameterDataMat4x4( int parameter, const decDMatrix &matrix );
	
	/** \brief Set data for int type parameter. */
	void SetParameterDataInt( int parameter, int value );
	void SetParameterDataIVec2( int parameter, int value1, int value2 );
	void SetParameterDataIVec3( int parameter, int value1, int value2, int value3 );
	void SetParameterDataIVec4( int parameter, int value1, int value2, int value3, int value4 );
	
	/** \brief Set data for bool type parameter. */
	void SetParameterDataBool( int parameter, bool value );
	void SetParameterDataBVec2( int parameter, bool value1, bool value2 );
	void SetParameterDataBVec3( int parameter, bool value1, bool value2, bool value3 );
	void SetParameterDataBVec4( int parameter, bool value1, bool value2, bool value3, bool value4 );
	
	/** \brief Set data for array float type parameter. */
	void SetParameterDataArrayFloat( int parameter, int index, float value );
	
	void SetParameterDataArrayVec2( int parameter, int index, float value1, float value2 );
	void SetParameterDataArrayVec2( int parameter, int index, const decVector2 &vector );
	void SetParameterDataArrayVec3( int parameter, int index,
		float value1, float value2, float value3 );
	
	void SetParameterDataArrayVec3( int parameter, int index, const decColor &color );
	void SetParameterDataArrayVec3( int parameter, int index, const decVector &vector );
	void SetParameterDataArrayVec3( int parameter, int index, const decDVector &vector );
	
	void SetParameterDataArrayVec4( int parameter, int index,
		float value1, float value2, float value3, float value4 );
	void SetParameterDataArrayVec4( int parameter, int index, const decColor &color );
	void SetParameterDataArrayVec4( int parameter, int index, const decColor &color, float alpha );
	void SetParameterDataArrayVec4( int parameter, int index, const decVector4 &vector );
	void SetParameterDataArrayVec4( int parameter, int index, const decDVector4 &vector );
	void SetParameterDataArrayVec4( int parameter, int index, const decVector &vector, float w );
	void SetParameterDataArrayVec4( int parameter, int index, const decDVector &vector, double w );
	
	void SetParameterDataArrayMat3x2( int parameter, int index, const float *values );
	void SetParameterDataArrayMat3x2( int parameter, int index, const decMatrix &matrix );
	void SetParameterDataArrayMat3x2( int parameter, int index, const decDMatrix &matrix );
	void SetParameterDataArrayMat3x2( int parameter, int index, const decTexMatrix &matrix );
	void SetParameterDataArrayMat3x2( int parameter, int index, const decTexMatrix2 &matrix );
	
	void SetParameterDataArrayMat3x3( int parameter, int index, const float *values );
	void SetParameterDataArrayMat3x3( int parameter, int index, const decMatrix &matrix );
	void SetParameterDataArrayMat3x3( int parameter, int index, const decDMatrix &matrix );
	void SetParameterDataArrayMat3x3( int parameter, int index, const decTexMatrix &matrix );
	
	void SetParameterDataArrayMat4x3( int parameter, int index, const float *values );
	void SetParameterDataArrayMat4x3( int parameter, int index, const decMatrix &matrix );
	void SetParameterDataArrayMat4x3( int parameter, int index, const decDMatrix &matrix );
	
	void SetParameterDataArrayMat4x4( int parameter, int index, const float *values );
	void SetParameterDataArrayMat4x4( int parameter, int index, const decMatrix &matrix );
	void SetParameterDataArrayMat4x4( int parameter, int index, const decDMatrix &matrix );
	
	void SetParameterDataArrayFloat( int parameter, float *values, int count );
	
	/** \brief Set data for an array int type parameter. */
	void SetParameterDataArrayInt( int parameter, int index, int value );
	void SetParameterDataArrayIVec2( int parameter, int index, int value1, int value2 );
	void SetParameterDataArrayIVec3( int parameter, int index,
		int value1, int value2, int value3 );
	void SetParameterDataArrayIVec4( int parameter, int index,
		int value1, int value2, int value3, int value4 );
	void SetParameterDataArrayInt( int parameter, int *values, int count );
	
	/** \brief Set data for a array bool type parameter. */
	void SetParameterDataArrayBool( int parameter, int index, bool value );
	void SetParameterDataArrayBVec2( int parameter, int index, bool value1, bool value2 );
	void SetParameterDataArrayBVec3( int parameter, int index,
		bool value1, bool value2, bool value3 );
	void SetParameterDataArrayBVec4( int parameter, int index,
		bool value1, bool value2, bool value3, bool value4 );
	void SetParameterDataArrayBool( int parameter, bool *values, int count );
	
	
	
	/** \brief Set data for float type parameter. */
	void SetParameterDataFloat( int parameter, int element, float value );
	
	void SetParameterDataVec2( int parameter, int element, float value1, float value2 );
	void SetParameterDataVec2( int parameter, int element, const decVector2 &vector );
	
	void SetParameterDataVec3( int parameter, int element,
		float value1, float value2, float value3 );
	void SetParameterDataVec3( int parameter, int element, const decColor &color );
	void SetParameterDataVec3( int parameter, int element, const decVector &vector );
	void SetParameterDataVec3( int parameter, int element, const decDVector &vector );
	
	void SetParameterDataVec4( int parameter, int element,
		float value1, float value2, float value3, float value4 );
	void SetParameterDataVec4( int parameter, int element, const decColor &color );
	void SetParameterDataVec4( int parameter, int element, const decColor &color, float alpha );
	void SetParameterDataVec4( int parameter, int element, const decVector4 &vector );
	void SetParameterDataVec4( int parameter, int element, const decDVector4 &vector );
	void SetParameterDataVec4( int parameter, int element, const decVector &vector, float w );
	void SetParameterDataVec4( int parameter, int element, const decDVector4 &vector, double w );
	
	void SetParameterDataMat3x2( int parameter, int element, const float *values );
	void SetParameterDataMat3x2( int parameter, int element, const decMatrix &matrix );
	void SetParameterDataMat3x2( int parameter, int element, const decDMatrix &matrix );
	void SetParameterDataMat3x2( int parameter, int element, const decTexMatrix &matrix );
	void SetParameterDataMat3x2( int parameter, int element, const decTexMatrix2 &matrix );
	void SetParameterDataMat3x3( int parameter, int element, const float *values );
	void SetParameterDataMat3x3( int parameter, int element, const decMatrix &matrix );
	void SetParameterDataMat3x3( int parameter, int element, const decDMatrix &matrix );
	void SetParameterDataMat3x3( int parameter, int element, const decTexMatrix &matrix );
	void SetParameterDataMat4x3( int parameter, int element, const float *values );
	void SetParameterDataMat4x3( int parameter, int element, const decMatrix &matrix );
	void SetParameterDataMat4x3( int parameter, int element, const decDMatrix &matrix );
	void SetParameterDataMat4x4( int parameter, int element, const float *values );
	void SetParameterDataMat4x4( int parameter, int element, const decMatrix &matrix );
	void SetParameterDataMat4x4( int parameter, int element, const decDMatrix &matrix );
	
	/** \brief Set data for int type parameter. */
	void SetParameterDataInt( int parameter, int element, int value );
	void SetParameterDataIVec2( int parameter, int element, int value1, int value2 );
	void SetParameterDataIVec3( int parameter, int element,
		int value1, int value2, int value3 );
	void SetParameterDataIVec4( int parameter, int element,
		int value1, int value2, int value3, int value4 );
	
	/** \brief Set data for bool type parameter. */
	void SetParameterDataBool( int parameter, int element, bool value );
	void SetParameterDataBVec2( int parameter, int element, bool value1, bool value2 );
	void SetParameterDataBVec3( int parameter, int element,
		bool value1, bool value2, bool value3 );
	void SetParameterDataBVec4( int parameter, int element,
		bool value1, bool value2, bool value3, bool value4 );
	
	/** \brief Set data for array float type parameter. */
	void SetParameterDataArrayFloat( int parameter, int element, int index, float value );
	
	void SetParameterDataArrayVec2( int parameter, int element, int index,
		float value1, float value2 );
	void SetParameterDataArrayVec2( int parameter, int element, int index,
		const decVector2 &vector );
	void SetParameterDataArrayVec3( int parameter, int element, int index,
		float value1, float value2, float value3 );
	
	void SetParameterDataArrayVec3( int parameter, int element, int index,
		const decColor &color );
	void SetParameterDataArrayVec3( int parameter, int element, int index,
		const decVector &vector );
	void SetParameterDataArrayVec3( int parameter, int element, int index,
		const decDVector &vector );
	
	void SetParameterDataArrayVec4( int parameter, int element, int index,
		float value1, float value2, float value3, float value4 );
	void SetParameterDataArrayVec4( int parameter, int element, int index,
		const decColor &color );
	void SetParameterDataArrayVec4( int parameter, int element, int index,
		const decColor &color, float alpha );
	void SetParameterDataArrayVec4( int parameter, int element, int index,
		const decVector4 &vector );
	void SetParameterDataArrayVec4( int parameter, int element, int index,
		const decDVector4 &vector );
	void SetParameterDataArrayVec4( int parameter, int element, int index,
		const decVector &vector, float w );
	void SetParameterDataArrayVec4( int parameter, int element, int index,
		const decDVector &vector, double w );
	
	void SetParameterDataArrayMat3x2( int parameter, int element, int index,
		const float *values );
	void SetParameterDataArrayMat3x2( int parameter, int element, int index,
		const decMatrix &matrix );
	void SetParameterDataArrayMat3x2( int parameter, int element, int index,
		const decDMatrix &matrix );
	void SetParameterDataArrayMat3x2( int parameter, int element, int index,
		const decTexMatrix &matrix );
	void SetParameterDataArrayMat3x2( int parameter, int element, int index,
		const decTexMatrix2 &matrix );
	
	void SetParameterDataArrayMat3x3( int parameter, int element, int index,
		const float *values );
	void SetParameterDataArrayMat3x3( int parameter, int element, int index,
		const decMatrix &matrix );
	void SetParameterDataArrayMat3x3( int parameter, int element, int index,
		const decDMatrix &matrix );
	void SetParameterDataArrayMat3x3( int parameter, int element, int index,
		const decTexMatrix &matrix );
	
	void SetParameterDataArrayMat4x3( int parameter, int element, int index,
		const float *values );
	void SetParameterDataArrayMat4x3( int parameter, int element, int index,
		const decMatrix &matrix );
	void SetParameterDataArrayMat4x3( int parameter, int element, int index,
		const decDMatrix &matrix );
	
	void SetParameterDataArrayMat4x4( int parameter, int element, int index,
		const float *values );
	void SetParameterDataArrayMat4x4( int parameter, int element, int index,
		const decMatrix &matrix );
	void SetParameterDataArrayMat4x4( int parameter, int element, int index,
		const decDMatrix &matrix );
	
	void SetParameterDataArrayFloat( int parameter, int element, float *values, int count );
	
	/** \brief Set data for an array int type parameter. */
	void SetParameterDataArrayInt( int parameter, int element, int index, int value );
	void SetParameterDataArrayIVec2( int parameter, int element, int index,
		int value1, int value2 );
	void SetParameterDataArrayIVec3( int parameter, int element, int index,
		int value1, int value2, int value3 );
	void SetParameterDataArrayIVec4( int parameter, int element, int index,
		int value1, int value2, int value3, int value4 );
	void SetParameterDataArrayInt( int parameter, int element, int *values, int count );
	
	/** \brief Set data for a array bool type parameter. */
	void SetParameterDataArrayBool( int parameter, int element, int index, bool value );
	void SetParameterDataArrayBVec2( int parameter, int element, int index,
		bool value1, bool value2 );
	void SetParameterDataArrayBVec3( int parameter, int element, int index,
		bool value1, bool value2, bool value3 );
	void SetParameterDataArrayBVec4( int parameter, int element, int index,
		bool value1, bool value2, bool value3, bool value4 );
	void SetParameterDataArrayBool( int parameter, int element, bool *values, int count );
	/*@}*/
	
	
	
protected:
	void pSetMapped( char *data );
	void pSetMapped( char *data, int element );
	void pClearMapped();
	void pSetElementStride( int stride );
	
	virtual void pUpdateBufferSize();
	
	inline int pGetElementLower() const{ return pElementLower; }
	inline int pGetElementUpper() const{ return pElementUpper; }
	
	
	
private:
	const deoglSPBParameter &pParamFloat( int index, int componentCount,
		int vectorCount = 1, int arrayIndex = 0 ) const;
	const deoglSPBParameter &pParamInt( int index, int componentCount,
		int vectorCount = 1, int arrayIndex = 0 ) const;
	const deoglSPBParameter &pParamBool( int index, int componentCount,
		int vectorCount = 1, int arrayIndex = 0 ) const;
	const deoglSPBParameter &pParam( int index, deoglSPBParameter::eValueTypes valueType,
		int componentCount, int vectorCount, int arrayIndex ) const;
	
	GLfloat *pDataFloat( const deoglSPBParameter &parameter, int element,
		int arrayIndex = 0 ) const;
	GLint *pDataInt( const deoglSPBParameter &parameter, int element,
		int arrayIndex = 0 ) const;
	GLuint *pDataUInt( const deoglSPBParameter &parameter, int element,
		int arrayIndex = 0 ) const;
	char *pData( const deoglSPBParameter &parameter, int element, int arrayIndex ) const;
	
	void pDataMatrix2( const deoglSPBParameter &parameter, int element,
		GLfloat **data, int arrayIndex = 0 ) const;
	void pDataMatrix3( const deoglSPBParameter &parameter, int element,
		GLfloat **data, int arrayIndex = 0 ) const;
	void pDataMatrix4( const deoglSPBParameter &parameter, int element,
		GLfloat **data, int arrayIndex = 0 ) const;
};

#endif
