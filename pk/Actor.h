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

	Actor();
	Actor(const Transform& InTransform);
	Actor(const glm::vec3& InLocation, const glm::vec3 InSize);

	int GetId() const;

	void SetScene(const SceneWeakPtr& InScene);
	SceneSharedPtr GetScene() const;

	void SetShader(const std::string& InShader);
	std::string GetShader() const;

	void SetTexture(const std::string& InTexture);
	std::string GetTexture() const;

	Transform GetTransform() const;

	void SetLocation(const glm::vec3& InLocation);
	glm::vec3 GetLocation() const;

	void SetSize(const glm::vec3& InSize);
	glm::vec3 GetSize() const;

	void SetVelocity(const glm::vec3& InVelocity);
	glm::vec3 GetVelocity() const;

	void SetColor(const glm::vec4& InColor);
	glm::vec4 GetColor() const;

	float GetInitialLifeSpan() const;
	void SetInitialLifeSpan(float InLifeSpan);
	float GetLifeSpan() const;

	void SetConfig(const std::string& InConfigFile);
	std::string GetConfigFile() const;

	void HasCollision(bool bInCollision);
	bool HasCollision() const;

	glm::mat4 GetRenderModel() const;
	BoundingBox GetBoundingBox() const;
	void BindTexture() const;
	void UnBindTexture() const;

	bool Collide(const Actor& Other) const;

	virtual void LoadConfig();
	virtual void Begin();
	virtual void Update(const float Delta);
	virtual void Input(const Window& Window, const float Delta);
	virtual void Render() const;

	bool IsDestroyed() const;
	virtual void Destroy();
	void CancelDestroy();

	bool IsInViewport() const;

	virtual ~Actor() = default;

protected:
	void Move(const float Delta);
	void UpdateLifeSpan(const float Delta);

private:
	int Id;
	float InitialLifeSpan;
	float LifeSpan;

	Transform mTransform;
	glm::vec3 Velocity;

	std::string ShaderName;
	std::string TextureName;
	glm::vec4 Color;

	bool bPendingDestroy;
	bool bHasCollision;

	std::string ConfigFile;

	SceneWeakPtr ScenePtr;

	friend class Scene;
};

