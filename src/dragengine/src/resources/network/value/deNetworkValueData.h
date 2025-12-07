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

#ifndef _DENETWORKVALUEDATA_H_
#define _DENETWORKVALUEDATA_H_

#include <stdint.h>

#include "deNetworkValue.h"


/**
 * \brief Network Value Data Class.
 *
 * Defines a data network value. Stores in contrary to the string network
 * value a data buffer of fixed length and is not zero padded. This kind
 * of network value is useful for arbitrary data typically of files or
 * other complex data.
 */
class DE_DLL_EXPORT deNetworkValueData : public deNetworkValue{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deNetworkValueData> Ref;
	
	
private:
	uint8_t *pData;
	int pLength;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create network value object. */
	deNetworkValueData(int length);
	
	/** \brief Clean up network value object. */
	virtual ~deNetworkValueData();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Data pointer. */
	inline uint8_t *GetData() const{return pData;}
	
	/** \brief Length of the data. */
	inline int GetLength() const{return pLength;}
	
	/**
	 * \brief Set length of data.
	 * 
	 * After changing the size the data is undefined. Only values greater than 0 are allowed.
	 */
	void SetLength(int length);
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit network value. */
	virtual void Visit(deNetworkValueVisitor &visitor);
	/*@}*/
};

#endif
