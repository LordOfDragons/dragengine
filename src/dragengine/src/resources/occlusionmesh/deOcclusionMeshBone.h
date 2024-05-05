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

#ifndef _DEOCCLUSIONMESHBONE_H_
#define _DEOCCLUSIONMESHBONE_H_

#include "../../common/string/decString.h"
#include "../../common/math/decMath.h"


/**
 * \brief Occlusion Mesh Bone.
 */
class DE_DLL_EXPORT deOcclusionMeshBone{
private:
	decString pName;
	int pParent;
	decVector pPosition;
	decQuaternion pOrientation;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new bone. */
	deOcclusionMeshBone();
	
	/** \brief Clean up bone. */
	~deOcclusionMeshBone();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set name. */
	void SetName( const char *name );
	
	/** \brief Index of the parent bone or -1 if a top level bone. */
	inline int GetParent() const{ return pParent; }
	
	/** \brief Set index of the parent bone or -1 if a top level bone. */
	void SetParent( int bone );
	
	/** \brief Position relative to the parent or the mesh if there is no parent. */
	inline decVector GetPosition() const{ return pPosition; }
	
	/** \brief Set position relative to the parent or the mesh if there is no parent. */
	void SetPosition( const decVector &position );
	
	/** \brief Orientation relative to the parent or the mesh if there is no parent. */
	inline decQuaternion GetOrientation() const{ return pOrientation; }
	
	/** \brief Set orientation relative to the parent or the mesh if there is no parent. */
	void SetOrientation( const decQuaternion &orientation );
	/*@}*/
};

#endif
