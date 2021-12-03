#include "Transform.h"

namespace pbls
{
	void Transform::Update()
	{
		glm::mat4 mxt = glm::translate(position);
		glm::mat4 mxr = glm::eulerAngleYXZ(rotation.y, rotation.x, rotation.z);
		glm::mat4 mxs = glm::scale(scale);

		matrix = mxt * mxr * mxs;
	}

	void Transform::Update(const glm::mat4& mx)
	{
		glm::mat4 mxs = glm::scale(localScale);

		//glm::mat4 mxr;
		//mxr.Rotate(localRotation);

		glm::mat4 mxt = glm::translate(localPosition);

		matrix = mxs * mxt * mx;

		//position = matrix.GetTranslate();
		//rotation = matrix.GetRotation();
		//scale = matrix.GetScale();
	}

	bool Transform::Write(const rapidjson::Value& value) const
	{
		return false;
	}
	bool Transform::Read(const rapidjson::Value& value)
	{
		JSON_READ(value, position);
		JSON_READ(value, rotation);
		JSON_READ(value, scale);

		return true;
	}

}