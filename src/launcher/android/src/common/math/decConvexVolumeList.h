/* 
 * Drag[en]gine Android Launcher
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

#ifndef _DECCONVEXVOLUMELIST_H_
#define _DECCONVEXVOLUMELIST_H_

#include "decMath.h"

class decConvexVolume;
class decConvexVolumeFace;



/**
 * \brief Convex volume list.
 *
 * Sometimes it is useful to have a list of convex volumes that
 * describe some object or multiple objects. The Convex Volume
 * List takes on this task storing a list of convex volumes.
 * The Convex Volume List is also able to send split operations
 * down to the stored convex volumes. The newly generated convex
 * volumes are appended to the list. This way a single Convex
 * Volume List can be used to cut an original convex volume into
 * smaller pieces tied together in one handy place.
 *
 * The Convex Volume List does only provide storing convex volumes
 * and does not use any kind of spatial optimiziation like octrees
 * for example. If you need spatial optimization or if you are
 * using a large number of convex volumes writing your own class
 * is better than using this one which is designed for handling
 * convex volumes in an easy way but not necessary a fast way.
 *
 * You can use though this class as a temporary storage to split
 * your convex volumes and moving the result afterwards into a
 * better place. The extraction functions allow you to remove
 * convex volumes without deleting them.
 */
class decConvexVolumeList{
private:
	decConvexVolume **pVolumes;
	int pVolumeCount;
	int pVolumeSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty list. */
	decConvexVolumeList();
	
	/** \brief Clean up list. */
	virtual ~decConvexVolumeList();
	/*@}*/
	
	
	
	/** \name Volume Management */
	/*@{*/
	/** \brief Number of volumes. */
	inline int GetVolumeCount() const{ return pVolumeCount; }
	
	/** \brief Volume at index. */
	decConvexVolume *GetVolumeAt( int index ) const;
	
	/** \brief Volume is present. */
	bool HasVolume( decConvexVolume *volume ) const;
	
	/** \brief Index of volume or -1 if absent. */
	int IndexOfVolume( decConvexVolume *volume ) const;
	
	/** \brief Add volume. */
	void AddVolume( decConvexVolume *volume );
	
	/** \brief Remove volume. */
	void RemoveVolume( decConvexVolume *volume );
	
	/** \brief Remove volume at index. */
	void RemoveVolumeAt( int index );
	
	/** \brief Remove volume without deleting itt. */
	void ExtractVolume( decConvexVolume *volume );
	
	/** \brief Remove volume at index without deleting it. */
	void ExtractVolumeAt( int index );
	
	/** \brief Remove all volumes. */
	void RemoveAllVolumes();
	/*@}*/
	
	
	
	/** \name Operations */
	/*@{*/
	/** \brief Remove all volumes and add a new one set to a cube. */
	void SetToCube( const decVector &halfSize );
	
	/** \brief Move all volumes. */
	void Move( const decVector &direction );
	
	/**
	 * \brief Split volumes using split plane.
	 * 
	 * All newly created volumes are added to the list. The cutFaceInfo is an
	 * optional face used only to init the new cut faces and does not have to
	 * be part of any volume.
	 */
	void SplitByPlane( const decVector &splitNormal, const decVector &splitPosition,
		bool deleteBackVolume, decConvexVolumeFace *cutFaceInit );
	
	/**
	 * \brief Split volumes using convex face.
	 * 
	 * All newly created volumes are added to the list. The face has not to be
	 * part of the convex volume list.
	 */
	void SplitByFace( const decConvexVolume &volume, int face );
	
	/**
	 * \brief Split volumes using convex volume.
	 * 
	 * All newly created volumes are added to the list. The volume has not to be
	 * part of the convex volume list.
	 */
	void SplitByVolume( const decConvexVolume &volume );
	/*@}*/
	
	
	
	/** \name Subclassing */
	/*@{*/
	/**
	 * \brief Create convex volume.
	 * 
	 * Overwrite this function to create convex volumes of specialized subclasses.
	 * If the volume is not NULL it indicates the volume this new volume is based
	 * upon. The front flag indicates of the volume to create is in front or back
	 * of a clip plane and is only used if a volume has been specified. The
	 * default implementation creates a convex volume of type decConvexVolume.
	 */
	virtual decConvexVolume *CreateVolume( decConvexVolume *volume, bool front );
	
	/**
	 * \brief Create convex volume face similar to another face.
	 * 
	 * It is possible that the face is NULL in which case a default face has to
	 * be created. NULL faces typically occure if a split is done using a split
	 * plane instead of a split volume. Overwrite this function to create convex
	 * volume face of specialized subclasses.
	 * 
	 * The default implementation creates a convex volume face of type decConvexVolumeFace.
	 */
	virtual decConvexVolumeFace *CreateVolumeFace( decConvexVolumeFace *face );
	/*@}*/
	
	
	
private:
	int pTestByVolume( int volume, const decConvexVolume &splitVolume ) const;
	bool pTestByFace( int volume, const decConvexVolume &splitVolume,
		const decConvexVolumeFace &splitFace ) const;
	int pTestByPlane( int volume, const decVector &splitNormal, float splitDot ) const;
	int pSplitByPlane( int volume, const decVector &splitNormal, float splitDot,
		bool deleteBackVolume, decConvexVolumeFace *face );
	int pSplitByVolume( int volume, const decConvexVolume &splitVolume );
};

#endif
