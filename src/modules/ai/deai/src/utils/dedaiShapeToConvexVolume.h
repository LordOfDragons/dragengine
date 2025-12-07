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

#ifndef _DEDAISHAPETOCONVEXVOLUME_H_
#define _DEDAISHAPETOCONVEXVOLUME_H_

#include <dragengine/common/shape/decShapeVisitor.h>
#include <dragengine/common/math/decConvexVolumeList.h>

class decConvexVolumeFace;
class decConvexVolume;


/**
 * \brief Shape visitor generating a convex volume from visited shapes.
 * \details The resolution of the generated convex volume can be controlled
 *          using parameters. Every visited shapes is added as a new convex
 *          volume to the list allowing to process lists of shapes. To
 *          support custom convex volume lists the creation of convex volumes
 *          and faces can be overwritten.
 */
class dedaiShapeToConvexVolume : public decShapeVisitor{
private:
	decConvexVolumeList *pList;
	
	int pSphereRingCount;
	int pSphereSegmentCount;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create visitor. */
	dedaiShapeToConvexVolume();
	
	/**  \brief Clean up visitor. */
	virtual ~dedaiShapeToConvexVolume();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Cconvex volume list to add volumes to or \em NULL if not set. */
	inline decConvexVolumeList *GetList() const{return pList;}
	
	/** \brief Set convex volume list to add volumes to or \em NULL if not set. */
	void SetList(decConvexVolumeList *list);
	
	
	
	/** \brief Number of rings to generate for spheres. */
	inline int GetSphereRingCount() const{return pSphereRingCount;}
	
	/** \brief Set number of rings to generate for spheres clamped to 2 or larger. */
	void SetSphereRingCount(int ringCount);
	
	/** \brief Number of segments to generate for spheres. */
	inline int GetSphereSegmentCount() const{return pSphereSegmentCount;}
	
	/** \brief Set number of segments to generate for spheres clamped to 8 or larger. */
	void SetSphereSegmentsCount(int segmentCount);
	
	
	
	/**
	 * \brief Adds a triangle convex face.
	 * \details Calculates the normal for the new face.
	 */
	void AddTriangle(decConvexVolume &volume, int p1, int p2, int p3);
	
	/**
	 * \brief Adds a quadrilateral convex face.
	 * \details Calculates the normal for the new face.
	 */
	void AddQuad(decConvexVolume &volume, int p1, int p2, int p3, int p4);
	
	
	
	/**
	 * \brief Create convex volume.
	 * \details Overwrite to provide your own subclass instance. The default
	 *          implementation create an instance of decConvexVolume.
	 */
	virtual decConvexVolume *CreateVolume();
	
	/**
	 * \brief Create convex volume face and adds it to the volume.
	 * \details Overwrite to provide your own subclass instance. The default
	 *          implementation create an instance of decConvexVolumeFace.
	 */
	virtual decConvexVolumeFace *CreateFace();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit shape. */
	virtual void VisitShape(decShape &shape);
	
	/** \brief Visit sphere shape. */
	virtual void VisitShapeSphere(decShapeSphere &sphere);
	
	/** \brief Visit box shape. */
	virtual void VisitShapeBox(decShapeBox &box);
	
	/** \brief Visit cylinder shape. */
	virtual void VisitShapeCylinder(decShapeCylinder &cylinder);
	
	/** \brief Visit capsule shape. */
	virtual void VisitShapeCapsule(decShapeCapsule &capsule);
	
	/** \brief Visit hull shape. */
	virtual void VisitShapeHull(decShapeHull &hull);
	/*@}*/
};

#endif
