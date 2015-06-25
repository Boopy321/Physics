#pragma once
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>

using glm::vec3;
using glm::vec4;
using glm::mat4;

class Camera
{
public:
	Camera();
	Camera(vec3 position);
	Camera(mat4 transform);

	//virtual void update(double dt) = 0;
	//Sets the camera postion
	void setPersepective(float FOV, float aspectRatio, float near, float far);
	void setLookAt(vec3 from, vec3 to, vec3 far);
	void setLookAt(vec3 lookAt, vec3 worldup);
	void setPosition(vec3 pos);
	void SetTransform(mat4 transform);
	void UpdateProjectionViewTransform();

	vec3 GetPosition() const{ return m_worldTransform[3].xyz();}

	mat4 getWorldTransform();                                                                                   
	const mat4 getView() const{ return m_viewTransform; }
	const mat4 getProjection()  const{ return m_projectionTransform; }
	const mat4 getProjectionView() const{ return m_projectionViewTransform; }

	bool GetPerSpectiveSet() const{ return m_bPerspectiveSet; }


	glm::vec3			screenPositionToDirection(float x, float y) const;

	// returns the point of intersection of a camera ray and a world-space plane
	// the plane has a normal of XYZ and is offset from (0,0,0) by -W in the direction of the normal
	glm::vec3			pickAgainstPlane(float x, float y, const glm::vec4& plane) const;
	~Camera();

protected:

	mat4 m_worldTransform;
	mat4 m_viewTransform;
	mat4 m_projectionTransform;
	mat4 m_projectionViewTransform;

	bool m_bPerspectiveSet;
};


