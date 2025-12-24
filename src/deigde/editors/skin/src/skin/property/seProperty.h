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

#ifndef _SEPROPERTY_H_
#define _SEPROPERTY_H_

#include "node/sePropertyNodeSelection.h"
#include "node/sePropertyNodeGroup.h"
#include "../mapped/seMapped.h"

#include <deigde/engine/textureProperties/igdeTextureProperty.h>

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decTOrderedSet.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/video/deVideo.h>
#include <dragengine/resources/skin/property/deSkinPropertyMapped.h>

class seTexture;

class deEngine;
class deSkinPropertyNodeGroup;



/**
 * \brief Texture property.
 */
class seProperty : public deObject{
public:
	typedef deTObjectReference<seProperty> Ref;
	
	/** \brief List type. */
	typedef decTObjectOrderedSet<seProperty> List;
	
	
	/** \brief Value types. */
	enum eValueTypes{
		/** \brief Property is defined by a single value. */
		evtValue,
		
		/** \brief Property is defined by a 4-component color. */
		evtColor,
		
		/** \brief Property is defined by an image. */
		evtImage,
		
		/** \brief Property is defined by a video. */
		evtVideo,
		
		/** \brief Property is defined by a construction node group. */
		evtConstructed,
		
		/** \brief Property is defined by mapping. */
		evtMapped
	};
	
	
	
private:
	deEngine *pEngine;
	
	seTexture *pTexture;
	
	decString pName;
	eValueTypes pValueType;
	decString pRenderableName;
	decString pBoneName;
	
	float pValue;
	
	decColor pColor;
	
	decString pPathImage;
	deImage::Ref pEngImage;
	
	decString pPathVideo;
	deVideo::Ref pEngVideo;
	bool pVideoSharedTime;
	
	seMapped::Ref pMappedComponents[4];
	
	sePropertyNodeGroup::Ref pNodeGroup;
	deSkinPropertyNodeGroup *pEngNodeGroup;
	sePropertyNodeGroup::Ref pActiveNodeGroup;
	sePropertyNodeSelection pNodeSelection;
	int pActiveNodeLayer;
	decColor pNodeColor;
	bool pNodeTileX;
	bool pNodeTileY;
	int pNodeBitCount;
	
	bool pSelected;
	bool pActive;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create property. */
	seProperty(deEngine *engine, const char *name = "color");
	
	/** \brief Create copy of property. */
	seProperty(const seProperty &property);
	
	/** \brief Clean up property. */
	virtual ~seProperty();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Engine. */
	inline deEngine *GetEngine() const{ return pEngine; }
	
	
	
	/** \brief Parent texture or \em nullptr if not set. */
	inline seTexture *GetTexture() const{ return pTexture; }
	
	/** \brief Set parent texture or \em nullptr if not set. */
	void SetTexture(seTexture *texture);
	
	
	
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set name. */
	void SetName(const char *name);
	
	/** \brief Value type. */
	inline eValueTypes GetValueType() const{ return pValueType; }
	
	/** \brief Set value type. */
	void SetValueType(eValueTypes type);
	
	/** \brief Name of renderable. */
	inline const decString &GetRenderableName() const{ return pRenderableName; }
	
	/** \brief Set name of renderable. */
	void SetRenderableName(const char *name);
	
	/** \brief Name of bone. */
	inline const decString &GetBoneName() const{ return pBoneName; }
	
	/** \brief Set name of bone. */
	void SetBoneName(const char *name);
	
	
	
	/** \brief Value. */
	inline float GetValue() const{ return pValue; }
	
	/** \brief Set value. */
	void SetValue(float value);
	
	
	
	/** \brief Static color. */
	inline const decColor &GetColor() const{ return pColor; }
	
	/** \brief Set static color. */
	void SetColor(const decColor &color);
	
	
	
	/** \brief Image path. */
	inline const decString &GetImagePath() const{ return pPathImage; }
	
	/** \brief Set image path. */
	void SetImagePath(const char *imagePath);
	
	/** \brief Image or \em nullptr if not set. */
	inline const deImage::Ref &GetEngineImage() const{ return pEngImage; }
	
	/** \brief Update image. */
	void UpdateImage();
	
	
	
	/** \brief Video path. */
	inline const decString &GetVideoPath() const{ return pPathVideo; }
	
	/** \brief Set video path. */
	void SetVideoPath(const char *videoPath);
	
	/** \brief Video or \em nullptr if not set. */
	inline const deVideo::Ref &GetEngineVideo() const{ return pEngVideo; }
	
	/** \brief Update video. */
	void UpdateVideo();
	
	/** \brief Video playback time is shared across instances. */
	inline bool GetVideoSharedTime() const{ return pVideoSharedTime; }
	
	/** \brief Set if video playback time is shared across instances. */
	void SetVideoSharedTime(bool shareTime);
	
	
	
	/** \brief Mapped component. */
	const seMapped::Ref &GetMappedComponent(int index) const;
	
	/** \brief Set mapped component. */
	void SetMappedComponent(int index, seMapped *mapped);
	
	
	
	/** \brief Node group. */
	inline const sePropertyNodeGroup::Ref &GetNodeGroup() const{ return pNodeGroup; }
	
	/** \brief Set node group. */
	void SetNodeGroup(sePropertyNodeGroup *nodeGroup);
	
	/** \brief Engine node group or \em nullptr if not set. */
	inline deSkinPropertyNodeGroup *GetEngineNodeGroup() const{ return pEngNodeGroup; }
	
	/** \brief Update engine node group. */
	void UpdateEngineNodeGroup();
	
	/** \brief Node selection. */
	sePropertyNodeSelection &GetNodeSelection(){return pNodeSelection;}
	const sePropertyNodeSelection &GetNodeSelection() const{ return pNodeSelection; }
	
	/** \brief Active node group or \em nullptr. */
	inline const sePropertyNodeGroup::Ref &GetActiveNodeGroup() const{ return pActiveNodeGroup; }
	
	/** \brief Set active node group or \em nullptr. */
	void SetActiveNodeGroup(sePropertyNodeGroup *node);
	
	/** \brief Active node layer or \em nullptr. */
	inline int GetActiveNodeLayer() const{ return pActiveNodeLayer; }
	
	/** \brief Set active node layer or \em nullptr. */
	void SetActiveNodeLayer(int layer);
	
	/** \brief Node color. */
	inline const decColor &GetNodeColor() const{ return pNodeColor; }
	
	/** \brief Set node color. */
	void SetNodeColor(const decColor &color);
	
	/** \brief Nodes are tiled along x axis. */
	inline bool GetNodeTileX() const{ return pNodeTileX; }
	
	/** \brief Set if nodes are tiled along x axis. */
	void SetNodeTileX(bool tileX);
	
	/** \brief Nodes are tiled along y axis. */
	inline bool GetNodeTileY() const{ return pNodeTileY; }
	
	/** \brief Set if nodes are tiled along y axis. */
	void SetNodeTileY(bool tileY);
	
	/** \brief Node bit count. */
	inline int GetNodeBitCount() const{ return pNodeBitCount; }
	
	/** \brief Set node bit count. */
	void SetNodeBitCount(int bitCount);
	
	
	
	/** \brief Property is the active one. */
	inline bool GetActive() const{ return pActive; }
	
	/** \brief Set if property is the active one. */
	void SetActive(bool active);
	
	/** \brief Property is selected. */
	inline bool GetSelected() const{ return pSelected; }
	
	/** \brief Set if property is selected. */
	void SetSelected(bool selected);
	
	/** \brief Notify listeners property changed. */
	void NotifyChanged();
	
	/** \brief Update resources. */
	void UpdateResources();
	
	/** \brief Init from default property parameters if property name is known. */
	void InitDefaults(const igdeTextureProperty::List &knownPropertyList);
	/*@}*/
};

#endif
