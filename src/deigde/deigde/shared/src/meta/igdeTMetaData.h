/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _IGDETMETADATA_H_
#define _IGDETMETADATA_H_

#include <dragengine/deObject.h>


/**
 * \brief Meta data template.
 * 
 * Allows to store by design immutable data by properties to detect changes in large data.
 */
template <typename T>
class igdeTMetaData : public deObject{
public:
	/** \brief Reference type. */
	using Ref = deTObjectReference<igdeTMetaData<T>>;
	
	
private:
	T pData;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create meta data. */
	igdeTMetaData() = default;
	
	/** \brief Create copy of meta data. */
	igdeTMetaData(const igdeTMetaData &other) : pData(other.pData){}
	
	/** \brief Move meta data. */
	igdeTMetaData(igdeTMetaData &&other) : pData(std::move(other.pData)){}
	
	/** \brief Create meta data with initial value. */
	explicit igdeTMetaData(const T &data) : pData(data) {}
	
protected:
	/** \brief Clean up meta data. */
	~igdeTMetaData() override = default;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Data. */
	inline T &GetData(){ return pData; }
	inline const T &GetData() const{ return pData; }
	
	/** \brief Automatic cast. */
	inline operator T &(){ return pData; }
	inline operator const T &() const{ return pData; }
	
	/** \brief Copy operator. */
	igdeTMetaData &operator=(const igdeTMetaData &other){
		if(this != &other){
			pData = other.pData;
		}
		return *this;
	}
	
	/** \brief Move operator. */
	igdeTMetaData &operator=(igdeTMetaData &&other) noexcept{
		if(this != &other){
			pData = std::move(other.pData);
		}
		return *this;
	}
	/*@}*/
};

#endif
