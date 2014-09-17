/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/

// primary contact: LAPA - secondary contact: NOBODY
#ifndef Navigation_PathFollowerConfig_H
#define Navigation_PathFollowerConfig_H


#include "gwnavruntime/base/memory.h"
#include "gwnavruntime/kernel/SF_RefCount.h"
#include "gwnavruntime/pathfollower/targetonpathconfig.h"
#include "gwnavruntime/pathfollower/avoidancecomputerconfig.h"

namespace Kaim
{

/// Class that aggregates all parameters used in PathFollower class.
///
/// Generic parameters are directly aggregated into this class; specific ones are aggregated through
/// config classes dedicated to each PathFollower components as is:
/// - TargetOnPathComputer parameters are in #m_targetOnPathConfig,
/// - AvoidanceComputer parameters are in #m_avoidanceComputerConfig.
///
/// Each World owns an instance of this class that can be retrieved by World::GetDefaultPathFollowerConfig()
/// and set to BotInitConfig to share the parameters values. If you prefer each Bot have its own parameter
/// values, you should provide a new instance of this class at Bot::Init() through BotInitConfig or call
/// Bot::SetPathFollowerConfig() once your Bot is initialized. You can also have a set of PathFollowerConfig
/// instances each dedicated to a sub-set of your Bots to implement categories (pedestrians, pets, cops...),
/// each category having a specific set of values, but each member of the same category sharing these ones.
class PathFollowerConfig: public RefCountBase<PathFollowerConfig, MemStat_PathFollowing>
{
	KY_DEFINE_NEW_DELETE_OPERATORS(MemStat_PathFollowing)

public:
	// ---------------------------------- Public Member Functions ----------------------------------

	PathFollowerConfig() { SetDefaults(); }

	/// Sets all members to their default value.
	void SetDefaults()
	{
		m_targetOnPathConfig.SetDefaults();
		m_avoidanceComputerConfig.SetDefaults();
		m_pathValidationDistanceBackward = 30.0f;
		m_pathValidationDistanceForward = 50.0f;
		m_checkPointRadius = 0.2f;
		m_shortcutForceTOPForwardDistance = 0.05f;
		m_trajectoryMinimalMoveDistance = 0.01f;
	}


	// ---------------------------------- Public Data Members ----------------------------------

	TargetOnPathConfig m_targetOnPathConfig; ///< The parameter set dedicated to TargetOnPathComputer configuration.
	AvoidanceComputerConfig m_avoidanceComputerConfig; ///< The parameter set dedicated to AvoidanceComputer configuration.

	/// The distance the Path is checked backward each time a new Path is
	/// computed. The Path is never checked backward farther than this distance
	/// from the current target on path.
	KyFloat32 m_pathValidationDistanceBackward;

	/// The distance the Path is checked forward each time a new Path is
	/// computed. The Path validation is then done each frame progressing for
	/// this distance until the whole Path is validated or the validation
	/// fails somewhere forward.
	KyFloat32 m_pathValidationDistanceForward;

	/// The radius around check points that trigger their validation.
	KyFloat32 m_checkPointRadius;

	/// Advanced parameter: when the bot is closer to its target on path (TOP) than this distance, the
	/// TargetOnPathComputer forces the TOP to move forward once (stopping at any PathEvent
	/// or Path node), without CanGo validation.
	/// \pre When using our default Trajectory, this value must be strictly higher than #m_trajectoryMinimalMoveDistance.
	KyFloat32 m_shortcutForceTOPForwardDistance;

	/// Advanced parameter: when using our default Trajectory class, if the distance from Bot to
	/// Trajectory::m_target is below this value, Bot will be stopped.
	/// \pre This value must be strictly lower than #m_shortcutForceTOPForwardDistance.
	KyFloat32 m_trajectoryMinimalMoveDistance;
};


} // namespace Kaim

#endif // Navigation_PathFollowerConfig_H
