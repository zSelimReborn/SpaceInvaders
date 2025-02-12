#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <string>

#include "Texture.h"
#include "Common.h"

class Scene;
class Window;

class Actor
{
public:
	typedef std::shared_ptr<Actor> SharedPtr;
	typedef std::shared_ptr<Scene> SceneSharedPtr;
	typedef std::weak_ptr<Scene> SceneWeakPtr;

	Actor(const Transform& _Transform);
	Actor(const glm::vec3& _Location, const glm::vec3 _Size);

	void SetScene(const SceneWeakPtr& InScene);
	SceneSharedPtr GetScene() const;

	void SetShader(const std::string& InShader);
	std::string GetShader() const;

	void SetTexture(const std::string& InTexture);
	std::string GetTexture() const;

	Transform GetTransform() const;

	void SetLocation(const glm::vec3 NewLocation);
	glm::vec3 GetLocation() const;

	glm::vec3 GetSize() const;

	void SetVelocity(const glm::vec3& InVelocity);
	glm::vec3 GetVelocity() const;

	void SetColor(const glm::vec4& _Color);
	glm::vec4 GetColor() const;

	glm::mat4 GetRenderModel() const;
	BoundingBox GetBoundingBox() const;
	void BindTexture() const;
	void UnBindTexture() const;

	bool Collide(const Actor& Other) const;

	virtual void Begin();
	virtual void Update(const float Delta);
	virtual void Input(const Window& Window, const float Delta);
	virtual void Render() const;

	bool IsDestroyed() const;
	void Destroy();

	virtual ~Actor() = default;

protected:
	void Move(const float Delta);

private:
	Transform mTransform;
	glm::vec3 Velocity;

	std::string ShaderName;
	std::string TextureName;
	glm::vec4 Color;

	bool bPendingDestroy;

	SceneWeakPtr ScenePtr;
};

