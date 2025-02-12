#include "Actor.h"

#include <glm/gtc/matrix_transform.hpp>

#include "AssetManager.h"
#include "Renderer.h"
#include "Scene.h"

Actor::Actor(const ::Transform& _Transform)
	: mTransform(_Transform), Velocity(0.f), Color(Colors::Black), bPendingDestroy(false)
{
}

Actor::Actor(const glm::vec3& _Location, const glm::vec3 _Size)
	: mTransform(_Location, _Size), Color(Colors::Black), bPendingDestroy(false)
{
}

void Actor::SetScene(const Scene::WeakPtr& InScene)
{
	ScenePtr = InScene;
}

Scene::SharedPtr Actor::GetScene() const
{
	return ScenePtr.lock();
}

void Actor::SetShader(const std::string& InShader)
{
	ShaderName = InShader;
}

std::string Actor::GetShader() const
{
	return ShaderName;
}

void Actor::SetTexture(const std::string& InTexture)
{
	TextureName = InTexture;
}

std::string Actor::GetTexture() const
{
	return TextureName;
}

Transform Actor::GetTransform() const
{
	return mTransform;
}

glm::vec3 Actor::GetLocation() const
{
	return mTransform.Location;
}

glm::vec3 Actor::GetSize() const
{
	return mTransform.Size;
}

void Actor::SetVelocity(const glm::vec3& InVelocity)
{
	Velocity = InVelocity;
}

glm::vec3 Actor::GetVelocity() const
{
	return Velocity;
}

void Actor::SetColor(const glm::vec4& _Color)
{
	Color = _Color;
}

glm::vec4 Actor::GetColor() const
{
	return Color;
}

void Actor::Move(const float Delta)
{
	mTransform.Location += Velocity * Delta;
}

glm::mat4 Actor::GetRenderModel() const
{
	const glm::mat4 Identity(1.f);
	glm::mat4 RenderModel = glm::translate(Identity, mTransform.Location);
	RenderModel = glm::scale(RenderModel, mTransform.Size);

	return RenderModel;
}

BoundingBox Actor::GetBoundingBox() const
{
	BoundingBox Box(mTransform);
	return Box;
}

void Actor::BindTexture() const
{
	const Texture::SharedPtr Texture = AssetManager::Get().GetTexture(TextureName);
	if (Texture != nullptr)
	{
		Texture->Bind();
	}
}

void Actor::UnBindTexture() const
{
	const Texture::SharedPtr Texture = AssetManager::Get().GetTexture(TextureName);
	if (Texture != nullptr)
	{
		Texture->UnBind();
	}
}

bool Actor::Collide(const Actor& Other) const
{
	const BoundingBox Me(GetBoundingBox());
	const BoundingBox OtherBox(Other.GetBoundingBox());

	bool bCollidedX = (Me.Right() >= OtherBox.Left()  && OtherBox.Right() >= Me.Left());
	bool bCollidedY = (Me.Bottom() <= OtherBox.Top()  && OtherBox.Bottom() <= Me.Top());

	return bCollidedX && bCollidedY;
}

void Actor::Begin()
{
}

void Actor::Update(const float Delta)
{
	Move(Delta);
}

void Actor::Input(const Window& Window, const float Delta)
{
}

void Actor::Render() const
{
	const Shader::SharedPtr Shader = AssetManager::Get().GetShader(ShaderName);
	const Texture::SharedPtr Texture = AssetManager::Get().GetTexture(TextureName);

	Renderer::Get().RenderSprite(
		Shader,
		Texture,
		GetRenderModel(),
		Color
	);
}

bool Actor::IsDestroyed() const
{
	return bPendingDestroy;
}

void Actor::Destroy()
{
	bPendingDestroy = true;
}

void Actor::SetLocation(const glm::vec3 NewLocation)
{
	mTransform.Location = NewLocation;
}
