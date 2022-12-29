#pragma once
#include <type_traits>

template<typename EnumType>
using Enable_If_Enum = std::enable_if_t<std::is_enum_v<EnumType>>;

template<typename EnumType, typename = Enable_If_Enum<EnumType>>
bool operator==(EnumType e1, EnumType e2)
{ 
	return static_cast<std::underlying_type_t<EnumType>>(e1) == static_cast<std::underlying_type_t<EnumType>>(e2);
}

template<typename EnumType, typename = Enable_If_Enum<EnumType>>
bool operator!=(EnumType e1, EnumType e2)
{
	return !(e1 == e2);
}

template<typename EnumType, typename = Enable_If_Enum<EnumType>>
EnumType& operator++(EnumType& e)
{
	auto eAsType{ ++static_cast<std::underlying_type_t<EnumType>>(e) };
	return e = static_cast<EnumType>(eAsType);
}

template<typename EnumType, typename = Enable_If_Enum<EnumType>>
EnumType& operator--(EnumType& e)
{
	auto eAsType{ --static_cast<std::underlying_type_t<EnumType>>(e) };
	return e = static_cast<EnumType>(eAsType);
}

template<typename EnumType, typename = Enable_If_Enum<EnumType>>
EnumType& operator|=(EnumType& e1, EnumType e2)
{
	return e1 = static_cast<EnumType>(static_cast<std::underlying_type_t<EnumType>>(e1) | static_cast<std::underlying_type_t<EnumType>>(e2));
}

template<typename EnumType, typename = Enable_If_Enum<EnumType>>
EnumType& operator&=(EnumType& e1, EnumType e2)
{
	return e1 = static_cast<EnumType>(static_cast<std::underlying_type_t<EnumType>>(e1) & static_cast<std::underlying_type_t<EnumType>>(e2));
}

template<typename EnumType, typename = Enable_If_Enum<EnumType>>
EnumType& operator^=(EnumType& e1, EnumType e2)
{
	return e1 = static_cast<EnumType>(static_cast<std::underlying_type_t<EnumType>>(e1) ^ static_cast<std::underlying_type_t<EnumType>>(e2));
}

template<typename EnumType, typename = Enable_If_Enum<EnumType>>
EnumType operator|(EnumType e1, EnumType e2)
{
	EnumType eResult{ e1 };
	return eResult |= e2;
}

template<typename EnumType, typename = Enable_If_Enum<EnumType>>
EnumType operator&(EnumType e1, EnumType e2)
{
	EnumType eResult{ e1 };
	return eResult &= e2;
}

template<typename EnumType, typename = Enable_If_Enum<EnumType>>
EnumType operator^(EnumType e1, EnumType e2)
{
	EnumType eResult{ e1 };
	return eResult ^= e2;
}

template<typename EnumType, typename = Enable_If_Enum<EnumType>>
bool operator~(EnumType e)
{
	return static_cast<EnumType>(~static_cast<std::underlying_type_t<EnumType>>(e1));
}