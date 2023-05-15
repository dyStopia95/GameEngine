#pragma once
#include "Attributed.h"
#include "TypeManager.h"
#include "Factory.h"

namespace UnitTests
{
	class AttributedFoo final : public FieaGameEngine::Attributed
	{
		RTTI_DECLARATIONS(AttributedFoo, FieaGameEngine::Attributed);

	public:
		static const std::size_t ArraySize = 5;

		AttributedFoo();
		AttributedFoo(const AttributedFoo&);
		AttributedFoo(AttributedFoo&&) = default;
		AttributedFoo& operator= (const AttributedFoo&) = default;
		AttributedFoo& operator= (AttributedFoo&&) = default;
		~AttributedFoo() = default;

		[[nodiscard]] AttributedFoo* Clone() const override;

		int ExternalInteger = 0;
		float ExternalFloat = 0;
		std::string ExternalString;
		glm::vec4 ExternalVector;
		glm::mat4 ExternalMatrix;

		int ExternalIntegerArray[ArraySize];
		float ExternalFloatArray[ArraySize];
		std::string ExternalStringArray[ArraySize];
		glm::vec4 ExternalVectorArray[ArraySize];
		glm::mat4 ExternalMatrixArray[ArraySize];

		//AttributedFoo* Clone() const override;
		//bool Equals(const RTTI* rhs) const override;

		static FieaGameEngine::Vector<FieaGameEngine::Signature> Signatures();
	};

	class AttFoo2 final : public FieaGameEngine::Attributed
	{
		RTTI_DECLARATIONS(AttFoo2, FieaGameEngine::Attributed);
	public:
		static const std::size_t ArraySize = 5;

		AttFoo2();
		AttFoo2(const AttFoo2&);
		AttFoo2(AttFoo2&&) = default;
		AttFoo2& operator= (const AttFoo2&) = default;
		AttFoo2& operator= (AttFoo2&&) = default;
		~AttFoo2() = default;

		[[nodiscard]] AttFoo2* Clone() const override;

		int ExternalInteger = 0;
		int ExternalIntegerArray[ArraySize];

		static FieaGameEngine::Vector<FieaGameEngine::Signature> Signatures();
	};

	class AttFoo2Factory : public FieaGameEngine::Factory<FieaGameEngine::Scope>
	{
		std::string ClassName() override
		{
			return std::string{ "AttFoo2" };
		}

		AttFoo2* Create() override
		{
			return new AttFoo2{};
		}
	};
}