/* 
 * Drag[en]gine Game Engine
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

#ifndef _DESKYLAYER_H_
#define _DESKYLAYER_H_

#include "deSkyControllerTarget.h"
#include "../skin/deSkinReference.h"
#include "../../common/math/decMath.h"

class deSkyLayerVisitor;
class deSkyLayerBody;


/**
 * \brief Sky Layer.
 *
 * Sky layers define the content of the sky using one or more layers drawn
 * ontop of each other. Each layer can be rotated individually. This way
 * rich skies can be modeled using a simple system. The color parameter can
 * be used to tint the content of a layer. The type of the sky is defined
 * by the assigned skin. This way sky rendering can be enhanced without
 * modifying the engine just the graphic module. The following types are
 * recognized ( more can be defined by specific modules in the future ).
 *
 * \par Offset
 * 
 * Layers are render like a sphere centered around the camera. For many sky layers this
 * is the best choice. Some layers types though look strange in with a pure spherical
 * calculation. For example clouds would look like diving down into the horizon if
 * represented spherical. To get a more realistic look the clouds should get flatter
 * towards the horizon. This is due to the viewer residing on the earth surface sees
 * the cloud layer sphere from a different location than the center of the sphere is.
 * Using offsets this situation can be simulated resulting in better cloud effects.
 * The offset is relative to the sphere thus measured in the range from -1 to 1. For
 * clouds try a value of -0.9 to shift the cloud sphere down into the ground by roughly
 * 90% of the sphere radius. The rotation is always applied before the offset.
 * 
 * \par Rotation
 * 
 * The etRotation* rotation is applied ontop of the layer orientation. This allows to
 * define planetary movement with the layer orientation representing the planet orbit
 * coordinate frame and etRotationX for example rotating the planet along the x axis
 * easily.
 * 
 * \par Sky Box
 * The sky layer renders as a sky box. A sky box composes of 6 images for
 * each side of an imaginary cube onto the inside the sky is projected.
 * A sky box layer complete covers the entire sky and is therefore suitable
 * for the first layer in a sky. You can also use this layer ontop of other
 * layers if you use images with transparency or using the alpha value of the
 * blending color. The skin assigned to the layer contains the sky box images.
 * There have to be six textures inside the skin named "sky.box.left",
 * "sky.box.right", "sky.box.top", "sky.box.bottom", "sky.box.back" and
 * "sky.box.front" representing the respective image. The names are defined
 * as constants for the graphic modules.
 *
 * \par Sky Sphere
 * The sky layer renders as a sky sphere. Sky spheres are often called sky
 * domes especially if they cover only 180 degrees of the sky. The sky sphere
 * can cover optionally 180 or 360 of the sky. A sky sphere composes of 2
 * images one for the top half of the sphere and the other for the bottom half.
 * The top image is mandatory but the bottom one is not required if the player
 * can never see the lower half of the sphere. A sky sphere layer can completely
 * cover the entire sky and is therefore suitable for the first layer in a sky.
 * You can also use this layer ontop of other layers if you use images with
 * transparency or using the alpha value of the blending color. The images for
 * the individual sphere halves have to be in a sphere mapping compatible form.
 * These sky textures usually are of rectangular form with a width to height
 * ratio of 2 to 1. Light probe sky textures are not supported by this layer
 * type. The skin assigned to the layer contains the sky sphere images. There
 * has to be at least the top image and optionall the bottom image. The textures
 * have to be named "sky.sphere.top" and "sky.sphere.bottom" representing the
 * respective image. The names are defined as constants for the graphic modules.
 *
 * \par Sky Bodies
 * The sky layer is rendered as a collection of individual celestrical bodies.
 * All contained bodies can be placed individually but are moved all together
 * with the layer. This kind of layer is useful for either a single celestrical
 * body like the sun or the moon or a collection of individual bodies like clouds
 * moving together. Especially with clouds using 2 or 3 layers can be used to
 * simulate clouds at different altitude moving due to different wind influence.
 * Since this layer type covers only partially the sky it is not suited as the
 * first layer but well suited for drawing bodies ontop of existing layers. The
 * skin assigned to the layer is used as the source of images for the individual
 * bodies. Each body specifies the index of the texture to use from the skin.
 * Therefor all images for all bodies inside the same layer have to be stored in
 * the same skin.
 */
class DE_DLL_EXPORT deSkyLayer{
public:
	/** \brief Targets. */
	enum eTargets{
		etOffsetX,
		etOffsetY,
		etOffsetZ,
		etOrientationX,
		etOrientationY,
		etOrientationZ,
		etRotationX,
		etRotationY,
		etRotationZ,
		etColorR,
		etColorG,
		etColorB,
		etIntensity,
		etTransparency,
		etLightColorR,
		etLightColorG,
		etLightColorB,
		etLightIntensity,
		etAmbientIntensity
	};
	
	
	
private:
	decVector pOffset;
	decVector pOrientation;
	decColor pColor;
	float pIntensity;
	float pTransparency;
	deSkinReference pSkin;
	
	decQuaternion pLightOrientation;
	decColor pLightColor;
	float pLightIntensity;
	float pAmbientIntensity;
	
	deSkyLayerBody *pBodies;
	int pBodyCount;
	
	bool pMulBySkyLight;
	bool pMulBySkyColor;
	
	deSkyControllerTarget pTargets[ etAmbientIntensity + 1 ];
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create sky layer. */
	deSkyLayer();
	
	/** \brief Clean up sky layer. */
	~deSkyLayer();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Relative offset of layer center. */
	inline const decVector &GetOffset() const{ return pOffset; }
	
	/** \brief Set relative offset of layer center. */
	void SetOffset( const decVector &offset );
	
	/** \brief Orientation. */
	inline const decVector &GetOrientation() const{ return pOrientation; }
	
	/** \brief Set orientation. */
	void SetOrientation( const decVector &orientation );
	
	/** \brief Tint color. */
	inline const decColor &GetColor() const{ return pColor; }
	
	/** \brief Set tint color. */
	void SetColor( const decColor &color );
	
	/** \brief Intensity. */
	inline float GetIntensity() const{ return pIntensity; }
	
	/** \brief Set intensity. */
	void SetIntensity( float intensity );
	
	/** \brief Transparency. */
	inline float GetTransparency() const{ return pTransparency; }
	
	/** \brief Set transparency. */
	void SetTransparency( float transparency );
	
	/** \brief Skin or NULL to use background color only. */
	inline deSkin *GetSkin() const{ return pSkin; }
	
	/** \brief Set skin or NULL to use background color only. */
	void SetSkin( deSkin *skin );
	
	
	
	/** \brief Light orientation. */
	inline const decQuaternion &GetLightOrientation() const{ return pLightOrientation; }
	
	/** \brief Set light orientation. */
	void SetLightOrientation( const decQuaternion &orientation );
	
	/** \brief Light color. */
	inline const decColor &GetLightColor() const{ return pLightColor; }
	
	/** \brief Set light color. */
	void SetLightColor( const decColor &color );
	
	/** \brief Direct light intensity. */
	inline float GetLightIntensity() const{ return pLightIntensity; }
	
	/** \brief Set direct light intensity. */
	void SetLightIntensity( float intensity );
	
	/** \brief Diffuse ambient light intensity. */
	inline float GetAmbientIntensity() const{ return pAmbientIntensity; }
	
	/** \brief Set diffuse ambient light intensity. */
	void SetAmbientIntensity( float intensity );
	
	
	
	/** \brief Number of bodies. */
	inline int GetBodyCount() const{ return pBodyCount; }
	
	/**
	 * \brief Set number of bodies.
	 * 
	 * Sets all bodies to default vaules.
	 */
	void SetBodyCount( int count );
	
	/** \brief Body at index. */
	deSkyLayerBody &GetBodyAt( int index ) const;
	
	
	
	/** \brief Controller target. */
	const deSkyControllerTarget &GetTarget( eTargets target ) const;
	deSkyControllerTarget &GetTarget( eTargets target );
	
	
	
	/** \brief Layer intensity is multiplied by total sky light intensity. */
	inline bool GetMuliplyBySkyLight() const{ return pMulBySkyLight; }
	
	/** \brief Set if layer intensity is multiplied by total sky light intensity. */
	void SetMultiplyBySkyLight( bool multiply );
	
	/** \brief Layer color is multiplied by total sky light color. */
	inline bool GetMuliplyBySkyColor() const{ return pMulBySkyColor; }
	
	/** \brief Set if layer color is multiplied by total sky light color. */
	void SetMultiplyBySkyColor( bool multiply );
	/*@}*/
};

#endif
