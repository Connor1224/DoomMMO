/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/

// primary contact: LAPA - secondary contact: NOBODY
#ifndef Navigation_AvoidanceComputerConfig_H
#define Navigation_AvoidanceComputerConfig_H


#include "gwnavruntime/base/memory.h"


namespace Kaim
{


/// Set of parameters used to configure AvoidanceComputer.
/// This class is aggregated into PathFollowerConfig; you don't need to directly instantiate
/// one for your Bot.
class AvoidanceComputerConfig
{
	KY_DEFINE_NEW_DELETE_OPERATORS(MemStat_Bot)
public:
	AvoidanceComputerConfig() { SetDefaults(); }

	/// Sets all members to their default value.
	void SetDefaults()
	{
		m_colliderCollectorRadius = 15.0f;
		m_colliderCollectorHalfHeight = 5.0f;
		m_colliderCollectorFramesBetweenUpdates = 30;

		m_enableSlowingDown = true;
		m_enableStop        = true;
		m_enableBackMove    = true;
	}


	// ---------------------------------- Potential Collider Collection Parameters ----------------------------------

	/// Defines the radius around the Bot in which potential colliders will be collected.
	KyFloat32 m_colliderCollectorRadius;

	/// Defines the height above and below the Bot altitude in which potential colliders will be collected.
	KyFloat32 m_colliderCollectorHalfHeight;

	/// Defines the number of frames between two consecutive collection of potential colliders.
	KyUInt32 m_colliderCollectorFramesBetweenUpdates;


	// ---------------------------------- Velocity Obstacle Solver Parameters ----------------------------------

	/// If set to true, velocity candidates with reduced speed will be considered; else only velocity candidates
	/// with Bot::GetDesiredSpeed() length will be considered.
	/// \defaultvalue true
	bool m_enableSlowingDown;

	/// If set to true, null velocity candidate will be considered; else the VelocityObstacleSolver will
	/// never consider stopping the Bot resulting in sometimes making it force its way through colliders.
	/// \defaultvalue true
	bool m_enableStop;

	/// If set to true, velocity candidates moving the Bot in opposite direction than its target will be considered
	/// making it some times moving backwards; else only velocity candidates in the forward half plane are considered.
	/// \defaultvalue true
	bool m_enableBackMove;
};

} // namespace Kaim

#endif // Navigation_AvoidanceComputerConfig_H
