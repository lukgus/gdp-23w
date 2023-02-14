#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <string>

namespace gdp
{

	enum EasingType
	{
		EaseIn,
		EaseOut,
		EaseInOut,
		None
	};


	struct PositionKeyFrame
	{
		PositionKeyFrame()
			: value(0.f), time(0.f), type(None) { }
		PositionKeyFrame(glm::vec3 value, float time, EasingType type = None)
			: value(value), time(time), type(type) { }

		glm::vec3 value;
		float time;
		EasingType type;
	};

	struct ScaleKeyFrame
	{
		ScaleKeyFrame()
			: value(0.f), time(0.f), type(None) { }
		ScaleKeyFrame(glm::vec3 value, float time, EasingType type = None)
			: value(value), time(time), type(type) { }

		glm::vec3 value;
		float time;
		EasingType type;
	};

	struct RotationKeyFrame
	{
		RotationKeyFrame()
			: value(1.0f, 0.f, 0.f, 0.f), time(0.f), useSlerp(true) { }
		RotationKeyFrame(glm::quat value, float time, bool useSlerp = true)
			: value(value), time(time), useSlerp(useSlerp) { }
		glm::quat value;
		float time;
		bool useSlerp;
	};

	struct AnimationData
	{
		std::vector<PositionKeyFrame> PositionKeyFrames;
		std::vector<ScaleKeyFrame> ScaleKeyFrames;
		std::vector<RotationKeyFrame> RotationKeyFrames;
		std::string Name;
		double Duration;
		double TicksPerSecond;
	};

	struct Animation
	{
		// Quadriped	4 legged animal
		// biped		Humanoid
		// BoneHierarchy
		std::string AnimationType;
		float AnimationTime;
		bool IsPlaying;
		bool IsLooping;
		float Speed;
	};
}