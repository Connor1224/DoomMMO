/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/



// primary contact: MAMU - secondary contact: GUAL
#ifndef Navigation_TerrainTypeColorPalette_H
#define Navigation_TerrainTypeColorPalette_H

#include "gwnavruntime/visualsystem/visualcolor.h"

namespace Kaim
{

class NavTag;

/* Defines an abstract base interface for an object that provides access to a palette of colors based on a terrain type mask.
	This class cannot be used directly. You must use a class that derives from ITerrainTypeColorPalette and
	that implements its virtual methods. You can use the TerrainTypeColorPalette class supplied with Gameware Navigation, or
	a custom class that you write yourself.
	This class is used internally to support the visual NavData representations supplied with Gameware Navigation. 
	Use of this class in your own code is entirely optional.
	 */
class ITerrainTypeColorPalette : public NewOverrideBase<MemStat_VisualRep>
{
public:
	virtual ~ITerrainTypeColorPalette() {}

	/* Returns a color that can be used to represent the specified terrain type mask. */
	virtual VisualColor GetTerrainTypeColor(KyUInt32 terrainType) const = 0;

	virtual VisualColor GetNavTagColor(const NavTag& tag) const = 0;

	virtual bool IsLightingEnabled() const = 0;
};



/* This class is used by the visual NavData representations supplied with Gameware Navigation in order to 
	retrieve the colors that should be used to represent different terrain types.
	By default this palette contains 32 different colors, one for each byte of the terrain type mask.
	This class is used internally to support the visual NavData representations supplied with Gameware Navigation. 
	Use of this class in your own code is entirely optional.
	 */
class TerrainTypeColorPalette : public ITerrainTypeColorPalette
{
public:
	/* Constructs a new instance of this class.
		\param nonWalkableColor			Sets the value of #m_nonWalkableColor.
		\param allWalkableColor			Sets the value of #m_allWalkableColor.
		\param saturation				Sets the value of #m_saturation in range [0, 1] (0 = white, 1 = fully saturated colors).
										Use this value to make different versions of the default color palette with same color tone but different aspect.
										Aspect can be washed aspect (low saturation) to very colorful (high saturation).
										Default value is 1.0f (Colorful palette).
		\param luminance				Sets the value of #m_luminance in range [0, 1] (0 = dark, 1 = sunny).
										Use this value to make different versions of the default color palette with same color tone but different luminance.
										Default value is 1.0f (Sunny palette).
		\param alpha					The transparency of the colors. Default value is 1.0f (fully opaque). */
	TerrainTypeColorPalette(
		VisualColor nonWalkableColor = VisualColor::Red,
		VisualColor allWalkableColor = VisualColor::Blue,
		KyFloat32 saturation = 1.0f,
		KyFloat32 luminance = 1.0f,
		KyFloat32 alpha = 1.0f);

	/* @copydoc ITerrainTypeColorPalette::GetTerrainTypeColor()
		This implementation returns:
		-	A different color for each terrain type. If theterrainType is
			a mask of two or more terrain types, the colors are mixed.
		-	#m_allWalkableColor if theterrainType is 0xFFFFFFFF (#NavTerrainTypeMask_All).
		-	#m_nonWalkableColor if theterrainType is 0x00000000 (#NavTerrainType_None). */
	virtual VisualColor GetTerrainTypeColor(KyUInt32 terrainType) const;

	virtual VisualColor GetNavTagColor(const NavTag& tag) const;

	virtual bool IsLightingEnabled() const { return m_isLightingEnabled; }

	virtual ~TerrainTypeColorPalette() {}

public:
	void InitStyleForGeometry() { InitStyle(0.5f, 1.0f, 1.0f); }
	void InitStyleForGraph()    { InitStyle(1.0f, 0.6f, 1.0f); }
	void InitStyleForNavMesh()  { InitStyle(1.0f, 1.0f, 0.5f); }
	void InitStyleForVolume()   { InitStyle(1.0f, 1.0f, 0.5f); }

	void InitStyle(KyFloat32 saturation, KyFloat32 luminance, KyFloat32 alpha);

private:
	/* Fills this palette with a predefined set of colors. */
	void ComputeColors(KyFloat32 saturation, KyFloat32 luminance, KyFloat32 alpha);

public:
	bool m_isLightingEnabled;
	bool m_useDefaultColor;
	VisualColor m_defaultColor;
	VisualColor m_nonWalkableColor; /*< Stores the color that will be used for the non-walkable terrain type. */
	VisualColor m_allWalkableColor; /*< Stores the color that will be used for the default terrain type. */

private:
	KyFloat32 m_saturation;         /*< Stores the saturation of the default color palette. Value is defined from 0 (not saturated = all white) to 1.0f (saturated colors). */
	KyFloat32 m_luminance;          /*< Stores the luminance of the default color palette. Value is defined from 0 (dark) to 1.0f (sunny). */
	KyFloat32 m_alpha;
	VisualColor m_colors[32];       /*< Stores the list of colors in this palette. */
};




}

#endif
