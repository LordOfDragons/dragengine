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

// include only once
#ifndef _MELSPFCACHETYPELIST_H_
#define _MELSPFCACHETYPELIST_H_

// includes

// predefinitions



/**
 * @brief Load/Save Prop Field Type List
 *
 * Helper class for the prop field cache load/save to store a list of types.
 */
class meLSPFCacheTypeList{
public:
	struct sType{
		int vlayer;
		int variation;
	};
	
private:
	sType *pTypes;
	int pTypeCount;
	int pTypeSize;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a type list. */
	meLSPFCacheTypeList();
	/** Cleans up the type list. */
	~meLSPFCacheTypeList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the number of types. */
	inline int GetTypeCount() const{return pTypeCount;}
	/** Retrieves the type at the given index. */
	sType &GetTypeAt(int index) const;
	/** Retrieves the index of the type with the given vlayer and variation or -1 if not found. */
	int IndexOfTypeWith(int vlayer, int variation) const;
	/** Adds the type if not existing already. */
	void AddType(int vlayer, int variation);
	/** Removes all types. */
	void RemoveAllTypes();
	/*@}*/
};

// end of include only once
#endif
