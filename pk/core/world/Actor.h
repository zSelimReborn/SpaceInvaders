#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <vector>

#include "../utils/Common.h"

class Scene;
class Window;
class Component;
class InputHandler;

class Actor : public std::enable_shared_from_this<Actor>
{
public:
	typedef std::shared_ptr<Actor> SharedPtr;
	typedef std::shared_ptr<Scene> SceneSharedPtr;
	typedef std::weak_ptr<Scene> SceneWeakPtr;
	typedef std::shared_ptr<Component> ComponentSharedPtr;
	typedef std::vector<ComponentSharedPtr> ComponentList;

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

	bool Collide(const Actor& Other, CollisionResult& OutResult) const;

	virtual void LoadConfig();
	virtual void Begin();
	virtual void Update(const float Delta);
	virtual void Input(const InputHandler& Handler, const float Delta);
	virtual void Render() const;

	ComponentList GetComponents() const;

	template<class T>
	std::shared_ptr<T> GetComponent() const;

	bool IsDestroyed() const;
	virtual void Destroy();
	void CancelDestroy();

	bool IsInViewport() const;

	template<class Dest>
	bool IsA()
	{
		const Dest* ObjDest = dynamic_cast<Dest*>(this);
		return ObjDest != nullptr;
	}

	virtual ~Actor() = default;

protected:
	void Move(const float Delta);
	void UpdateLifeSpan(const float Delta);
	void AddComponent(const ComponentSharedPtr& InComponent);
	void BeginComponents() const;
	void UpdateComponents(const float Delta) const;

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

	ComponentList Components;

	friend class Scene;
};

template <class T>
std::shared_ptr<T> Actor::GetComponent() const
{
	for (const ComponentSharedPtr& CurrentComponent : Components)
	{
		std::shared_ptr<T> FoundComponent = std::dynamic_pointer_cast<T>(CurrentComponent);
		if (FoundComponent != nullptr)
		{
			return FoundComponent;
		}
	}

	return nullptr;
}