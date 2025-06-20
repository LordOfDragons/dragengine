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

#ifndef _MEWORLDGUIPARAMETERS_H_
#define _MEWORLDGUIPARAMETERS_H_

#include "../filter/meFilterObjectsByClass.h"

#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/decStringSet.h>
#include <deigde/gamedefinition/igdeTagManager.h>

class meWorld;



/**
 * \brief World gui parameters.
 */
class meWorldGuiParameters{
public:
	/** \brief Element modes. */
	enum eElementModes{
		/** \brief Object mode. */
		eemObject,
		
		/** \brief Decal mode. */
		eemDecal,
		
		/** \brief Navigation Space mode. */
		eemNavSpace,
		
		/** \brief Object shape mode. */
		eemObjectShape,
	};
	
	/** \brief Work modes. */
	enum eWorkModes{
		/** \brief Select elements mode. */
		ewmSelect,
		
		/** \brief Move elements mode. */
		ewmMove,
		
		/** \brief Scale elements mode. */
		ewmScale,
		
		/** \brief Rotate elements mode. */
		ewmRotate,
		
		/** \brief Add element mode. */
		ewmAddNew,
		
		/** \brief Height paint mode. */
		ewmHeightPaint,
		
		/** \brief Mask paint mode. */
		ewmMaskPaint,
		
		/** \brief Visibility paint mode. */
		ewmVisibilityPaint,
		
		/** \brief Navigation space edit mode. */
		ewmNavSpaceEdit,
		
		/** \brief Place 3D cursor mode. */
		ewm3DCursor,
	};
	
	/** \brief Height paint draw modes. */
	enum eHPDrawModes{
		/** \brief Raise. */
		ehpdmRaise,
		
		/** \brief Lower. */
		ehpdmLower,
		
		/** \brief Level. */
		ehpdmLevel,
		
		/** \brief Smooth. */
		ehpdmSmooth,
	};
	
	/** \brief Mask paint draw modes. */
	enum eMPDrawModes{
		/** \brief Draw. */
		empdmDraw,
		
		/** \brief Erase. */
		empdmErase,
	};
	
	/** \brief Hole paint draw modes. */
	enum eVPDrawModes{
		/** \brief Visible. */
		evpdmVisible,
		
		/** \brief Invisible. */
		evpdmInvisible,
	};
	
	/** \brief Navigation space paint modes. */
	enum eNSPDrawModes{
		/** \brief Draw. */
		enspdmDraw,
		
		/** \brief Erase. */
		enspdmErase,
	};
	
	/** \brief Rotation Pivot Center. */
	enum eRotationPivotCenters{
		/** \brief Position of active element. */
		erpcActive,
		
		/** \brief Average position of all selected elements. */
		erpcSelected,
		
		/** \brief Individual element positions. */
		erpcIndividual
	};
	
	
	
private:
	meWorld &pWorld;
	
	eElementModes pElementMode;
	eWorkModes pWorkMode;
	bool pLockAxisX;
	bool pLockAxisY;
	bool pLockAxisZ;
	bool pUseLocal;
	bool pSnapToSnapPoints;
	bool pShowOcclusionMeshes;
	bool pShowOcclusionMeshesSelected;
	bool pShowNavigationSpaces;
	bool pShowNavigationSpacesSelected;
	bool pShowShapes;
	bool pShowShapesSelected;
	
	igdeTagManager pTagsHideClass;
	igdeTagManager pTagsPartialHideClass;
	
	eHPDrawModes pHPDrawMode;
	float pHPRadius;
	float pHPDrawStrength;
	float pHPLevelStrength;
	float pHPSmoothStrength;
	
	eMPDrawModes pMPDrawMode;
	float pMPRadius;
	
	eVPDrawModes pVPDrawMode;
	float pVPRadius;
	
	eRotationPivotCenters pRotationPivotCenter;
	
	bool pAutoUpdateVegetation;
	
	decString pBrowseClass;
	decString pBrowseSkin;
	
	decStringSet pAddFilterObjectSet;
	bool pAddFilterObjectInclusive;
	
	float pRectSelDistance;
	float pRectSelDistanceStep;
	int pRectSelDragThreshold;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create gui parameters. */
	meWorldGuiParameters( meWorld &world );
	
	/** \brief Clean up gui parameters. */
	~meWorldGuiParameters();
	/*@}*/
	
	
	
	/** \name Editing */
	/*@{*/
	/** \brief Element mode. */
	inline eElementModes GetElementMode() const{ return pElementMode; }
	
	/** \brief Set element mode. */
	void SetElementMode( eElementModes mode );
	
	/** \brief Work mode. */
	inline eWorkModes GetWorkMode() const{ return pWorkMode; }
	
	/** \brief Set work mode. */
	void SetWorkMode( eWorkModes mode );
	
	/** \brief Rotation pivot center. */
	inline eRotationPivotCenters GetRotationPivotCenter() const{ return pRotationPivotCenter; }
	
	/** \brief Set rotation pivot center. */
	void SetRotationPivotCenter( eRotationPivotCenters pivotCenter );
	
	
	
	/** \brief X axis is locked during editing. */
	inline bool GetLockAxisX() const{ return pLockAxisX; }
	
	/** \brief Set if x axis is locked during editing. */
	void SetLockAxisX( bool lock );
	
	/** \brief Y axis is locked during editing. */
	inline bool GetLockAxisY() const{ return pLockAxisY; }
	
	/** \brief Set if y axis is locked during editing. */
	void SetLockAxisY( bool lock );
	
	/** \brief Z axis is locked during editing. */
	inline bool GetLockAxisZ() const{ return pLockAxisZ; }
	
	/** \brief Set if z axis is locked during editing. */
	void SetLockAxisZ( bool lock );
	
	/** \brief Object local coordinates are used during editing. */
	inline bool GetUseLocal() const{ return pUseLocal; }
	
	/** \brief Set if object local coordinates are used during editing. */
	void SetUseLocal( bool useLocal );
	
	/** \brief Snap to snap points. */
	inline bool GetSnapToSnapPoints() const{ return pSnapToSnapPoints; }
	
	/** \brief Set snap to snap points. */
	void SetSnapToSnapPoints( bool snapToSnapPoints );
	
	
	
	/** \brief Recangular selection distance. */
	inline float GetRectSelDistance() const{ return pRectSelDistance; }
	
	/** \brief Set rectangular selection distance. */
	void SetRectSelDistance( float distance );
	
	/** \brief Rectangular selection distance step. */
	inline float GetRectSelDistanceStep() const{ return pRectSelDistanceStep; }
	
	/** \brief Set rectangular selection distance step. */
	void SetRectSelDistanceStep( float step );
	
	/** \brief Drag threshold in pixels for rectangular selection. */
	inline int GetRectSelDragThreshold() const{ return pRectSelDragThreshold; }
	
	/** \brief Set drag threshold in pixels for rectangular selection. */
	void SetRectSelDragThreshold( int threshold );
	
	
	
	/** \brief Occlusion meshes are shown. */
	inline bool GetShowOcclusionMeshes() const{ return pShowOcclusionMeshes; }
	
	/** \brief Set if occlusion meshes are shown. */
	void SetShowOcclusionMeshes( bool show );
	
	/** \brief Occlusion meshes of selected objects are shown. */
	inline bool GetShowOcclusionMeshesSelected() const{ return pShowOcclusionMeshesSelected; }
	
	/** \brief Set if occlusion meshes of selected objects are shown. */
	void SetShowOcclusionMeshesSelected( bool show );
	
	/** \brief Show navigation spaces. */
	inline bool GetShowNavigationSpaces() const{ return pShowNavigationSpaces; }
	
	/** \brief Set if navigation spaces are shown. */
	void SetShowNavigationSpaces( bool show );
	
	/** \brief Show navigation spaces of selected objects. */
	inline bool GetShowNavigationSpacesSelected() const{ return pShowNavigationSpacesSelected; }
	
	/** \brief Set if navigation spaces of selected objects are shown. */
	void SetShowNavigationSpacesSelected( bool show );
	
	/** \brief Shapes are shown. */
	inline bool GetShowShapes() const{ return pShowShapes; }
	
	/** \brief Set if shapes are shown. */
	void SetShowShapes(bool show);
	
	/** \brief Shapes of selected objects are shown. */
	inline bool GetShowShapesSelected() const{ return pShowShapesSelected; }
	
	/** \brief Set if shapes of selected objects are shown. */
	void SetShowShapesSelected(bool show);
	
	
	
	
	/** \brief List of set class hide tags. */
	inline igdeTagManager &GetTagsHideClass(){ return pTagsHideClass; }
	inline const igdeTagManager &GetTagsHideClass() const{ return pTagsHideClass; }
	
	/** \brief List of set class partial hide tags. */
	inline igdeTagManager &GetTagsPartialHideClass(){ return pTagsPartialHideClass; }
	inline const igdeTagManager &GetTagsPartialHideClass() const{ return pTagsPartialHideClass; }
	/*@}*/
	
	
	
	/** \name Painting */
	/*@{*/
	/** \brief Height paint draw mode. */
	inline eHPDrawModes GetHPDrawMode() const{ return pHPDrawMode; }
	
	/** \brief Set height paint draw mode. */
	void SetHPDrawMode( eHPDrawModes mode );
	
	/** \brief Height paint radius. */
	inline float GetHPRadius() const{ return pHPRadius; }
	
	/** \brief Set height paint radius. */
	void SetHPRadius( float radius );
	
	/** \brief Height paint draw strength. */
	inline float GetHPDrawStrength() const{ return pHPDrawStrength; }
	
	/** \brief Set height paint draw strength. */
	void SetHPDrawStrength( float strength );
	
	/** \brief Height paint level strength. */
	inline float GetHPLevelStrength() const{ return pHPLevelStrength; }
	
	/** \brief Set height paint level strength. */
	void SetHPLevelStrength( float strength );
	
	/** \brief Height paint smooth strength. */
	inline float GetHPSmoothStrength() const{ return pHPSmoothStrength; }
	
	/** \brief Set height paint smooth strength. */
	void SetHPSmoothStrength( float strength );
	
	
	
	/** \brief Mask paint draw mode. */
	inline eMPDrawModes GetMPDrawMode() const{ return pMPDrawMode; }
	
	/** \brief Set mask paint draw mode. */
	void SetMPDrawMode( eMPDrawModes mode );
	
	/** \brief Mask paint radius. */
	inline float GetMPRadius() const{ return pMPRadius; }
	
	/** \brief Set mask paint radius. */
	void SetMPRadius( float radius );
	
	
	
	/** \brief Visibility paint draw mode. */
	inline eVPDrawModes GetVPDrawMode() const{ return pVPDrawMode; }
	
	/** \brief Set visibility paint draw mode. */
	void SetVPDrawMode( eVPDrawModes mode );
	
	/** \brief Visibility paint radius. */
	inline float GetVPRadius() const{ return pVPRadius; }
	
	/** \brief Set visibility paint radius. */
	void SetVPRadius( float radius );
	/*@}*/
	
	
	
	/** \name Vegetation */
	/*@{*/
	/** \brief Vhe vegetation system is updated automatically. */
	inline bool GetAutoUpdateVegetation() const{ return pAutoUpdateVegetation; }
	
	/** \brief Set if vegetation system is updated automatically. */
	void SetAutoUpdateVegetation( bool autoUpdate );
	/*@}*/
	
	
	
	/** \name Browsing */
	/*@{*/
	/** \brief Browse object class. */
	inline const decString &GetBrowseClass() const{ return pBrowseClass; }
	
	/** \brief Set browse object class. */
	void SetBrowseClass( const char *name );
	
	/** \brief Browse skin. */
	inline const decString &GetBrowseSkin() const{ return pBrowseSkin; }
	
	/** \brief Set browse skin. */
	void SetBrowseSkin( const char *name );
	/*@}*/
	
	
	
	/** \name Adding */
	/*@{*/
	/** \brief Add object filter class names set. */
	inline const decStringSet &GetAddFilterObjectSet() const{ return pAddFilterObjectSet; }
	
	/** \brief Set add object filter class names set. */
	void SetAddFilterObjectSet( const decStringSet &set );
	
	/** \brief Add object filter is inclusive. */
	inline bool GetAddFilterObjectInclusive() const{ return pAddFilterObjectInclusive; }
	
	/** \brief Set if add object filter is inclusive. */
	void SetAddFilterObjectInclusive( bool inclusive );
	/*@}*/
};

#endif
