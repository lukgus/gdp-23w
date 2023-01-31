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
		PositionKeyFrame(glm::vec3 value, float time, EasingType type = None)
			: value(value), time(time), type(type) { }

		glm::vec3 value;
		float time;
		EasingType type;
	};

	struct ScaleKeyFrame
	{
		ScaleKeyFrame(glm::vec3 value, float time, EasingType type = None)
			: value(value), time(time), type(type) { }

		glm::vec3 value;
		float time;
		EasingType type;
	};

	struct RotationKeyFrame
	{
		RotationKeyFrame(glm::quat value, float time, bool useSlerp)
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
		float Duration;
	};

	struct Animation
	{
		std::string AnimationType;
		float AnimationTime;
		bool IsPlaying;
		bool IsLooping;
		float Speed;
	};
}