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

#ifndef _DEOALICOSPHERE_H_
#define _DEOALICOSPHERE_H_

#include <dragengine/common/math/decMath.h>

class deoalAudioThread;


/**
 * \brief Ico sphere builder.
 * 
 * Calculates different levels of ico sphere using an subdivision approach. The calculated
 * levels are guaranteed to compose of the same directions of previous levels with new data
 * applied to it. The vertices can be used directly as directions for ray casting.
 */
class deoalIcoSphere{
public:
	struct sFace{
		unsigned short vertices[3];
		unsigned short edges[3];
		void Set(int vertex1, int vertex2, int vertex3, int edge1, int edge2, int edge3);
	};
	
	struct sEdge{
		unsigned short vertices[2];
		void Set(int vertex1, int vertex2);
	};
	
	
	
private:
	decVector *pVertices;
	int pVertexCount;
	
	sEdge *pEdges;
	int pEdgeCount;
	
	sFace *pFaces;
	int pFaceCount;
	
	float pOpeningAngle;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty ico sphere. */
	deoalIcoSphere();
	
	/** \brief Create copy of ico sphere. */
	deoalIcoSphere(const deoalIcoSphere &icosphere);
	
	/** \brief Clean up ico sphere. */
	~deoalIcoSphere();
	
	/** \brief Base level ico sphere. */
	static deoalIcoSphere BaseLevel();
	/*@}*/
	
	
	
public:
	/** \name Manegement */
	/*@{*/
	/** \brief Vertices. */
	inline const decVector *GetVertices() const{return pVertices;}
	
	/** \brief Number of vertices. */
	inline int GetVertexCount() const{return pVertexCount;}
	
	/** \brief Edges. */
	inline const sEdge *GetEdges() const{return pEdges;}
	
	/** \brief Edge count. */
	inline int GetEdgeCount() const{return pEdgeCount;}
	
	/** \brief Opening angle. */
	inline float GetOpeningAngle() const{return pOpeningAngle;}
	
	/** \brief Subdivide ico sphere. */
	deoalIcoSphere Subdivide() const;
	
	/** \brief Debug print (blender output). */
	void DebugPrintBlender(deoalAudioThread &audioThread) const;
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy ico sphere. */
	deoalIcoSphere &operator=(const deoalIcoSphere &ico);
	/*@}*/
	
	
	
protected:
	void pCalculateOpeningAngle();
};

#endif
