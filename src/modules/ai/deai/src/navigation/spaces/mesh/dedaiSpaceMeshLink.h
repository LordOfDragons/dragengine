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

#ifndef _DEDAISPACEMESHLINK_H_
#define _DEDAISPACEMESHLINK_H_

#include <dragengine/common/math/decMath.h>

class dedaiSpaceMesh;



/**
 * \brief Space mesh link.
 */
class dedaiSpaceMeshLink{
private:
	dedaiSpaceMesh *pMesh;
	unsigned short pFace;
	unsigned short pCorner;
	decMatrix pTransform;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create space mesh link. */
	dedaiSpaceMeshLink();
	
	/** \brief Clean up space mesh link. */
	~dedaiSpaceMeshLink();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Space mesh. */
	inline dedaiSpaceMesh *GetMesh() const{ return pMesh; }
	
	/** \brief Set space mesh. */
	void SetMesh(dedaiSpaceMesh *mesh);
	
	/** \brief Face index. */
	inline unsigned short GetFace() const{ return pFace; }
	
	/** \brief Set face index. */
	void SetFace(unsigned short face);
	
	/** \brief Face corner index. */
	inline unsigned short GetCorner() const{ return pCorner; }
	
	/** \brief Set face corner index. */
	void SetCorner(unsigned short corner);
	
	/** \brief Transformation matrix from this navigation mesh to the linked one. */
	inline const decMatrix &GetTransform() const{ return pTransform; }
	
	/** \brief Set transformation matrix from this navigation mesh to the linked one. */
	void SetTransform(const decMatrix &transform);
	
	
	
	/** \brief Link matches. */
	bool Matches(dedaiSpaceMesh *mesh, unsigned short face, unsigned short corner) const;
	
	/** \brief Foreign link from parameters. */
	dedaiSpaceMeshLink &GetForeignLink() const;
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brier Copy corner. */
	dedaiSpaceMeshLink &operator=(const dedaiSpaceMeshLink &link);
	/*@}*/
};

#endif
