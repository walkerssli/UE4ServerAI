#pragma once


namespace EBlackboardKeyOperation
{
	enum Type
	{
		Basic,
		Arithmetic,
		Text,
	};
}

namespace EBasicKeyOperation
{
	enum Type
	{
		Set		= 0,
		NotSet,
	};
}

namespace EArithmeticKeyOperation
{
	enum Type
	{
		Equal = 0,
		NotEqual,
		Less,
		LessOrEqual,
		Greater	,
		GreaterOrEqual,
	};
}

namespace ETextKeyOperation
{
	enum Type
	{
		Equal=0,
		NotEqual,
		Contain,
		NotContain,
	};
}
