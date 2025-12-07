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

#ifndef _DEOGLSPBPARAMETER_H_
#define _DEOGLSPBPARAMETER_H_



/**
 * Shader parameter block parameter.
 */
class deoglSPBParameter{
public:
	/** Value types. */
	enum eValueTypes{
		/** Float values. */
		evtFloat,
		
		/** Integer values. */
		evtInt,
		
		/** Boolean values. */
		evtBool
	};
	
	
	
private:
	eValueTypes pValueType;
	int pComponentCount;
	int pVectorCount;
	int pArrayCount;
	
	int pOffset;
	int pStride;
	int pArrayStride;
	int pDataSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create shader parameter. */
	deoglSPBParameter();
	
	/** Clean up shader parameter. */
	~deoglSPBParameter();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Value type. */
	inline eValueTypes GetValueType() const{return pValueType;}
	
	/** Set value type. */
	void SetValueType(eValueTypes valueType);
	
	/** Number of components in vector. */
	inline int GetComponentCount() const{return pComponentCount;}
	
	/** Set number of components in vector. */
	void SetComponentCount(int componentCount);
	
	/** Number of vectors. */
	inline int GetVectorCount() const{return pVectorCount;}
	
	/** Set number of vectors. */
	void SetVectorCount(int vectorCount);
	
	/** Array count. */
	inline int GetArrayCount() const{return pArrayCount;}
	
	/** Set array count. */
	void SetArrayCount(int arrayCount);
	
	/** Set all parameters. */
	void SetAll(eValueTypes valueType, int componentCount,
		int vectorCount, int arrayCount);
	
	/** Offset to first value component in data block. */
	inline int GetOffset() const{return pOffset;}
	
	/** Set offset to first value component in data block. */
	void SetOffset(int offset);
	
	/** Vector stride. */
	inline int GetStride() const{return pStride;}
	
	/** Set vector stride. */
	void SetStride(int stride);
	
	/** Array stride. */
	inline int GetArrayStride() const{return pArrayStride;}
	
	/** Set array stride. */
	void SetArrayStride(int arrayStride);
	
	/** Size of data in block. */
	inline int GetDataSize() const{return pDataSize;}
	
	/** Set size of data in block. */
	void SetDataSize(int size);
	/*@}*/
};

#endif
