#pragma once

#include "Common.h"
#include "TestGenerator.h"

#include <map>

struct EachFrameType
{
	std::vector<frox::EComputeFrameType> Types;

	EachFrameType()
	{
		// type=1 - Ignore type none
		for (uint8_t typeIndex = 1; typeIndex < frox::ECFT_NumTypes; ++typeIndex)
		{
			frox::EComputeFrameType type = frox::EComputeFrameType(typeIndex);
			Types.push_back(type);
		}
	}

	EachFrameType(const std::vector<frox::EComputeFrameType>& types)
		: Types(types)
	{}

	static std::string TypeToName(frox::EComputeFrameType type, const std::string& defaultName = "none")
	{
		static std::map<frox::EComputeFrameType, std::string> names = {
			std::make_pair(frox::ECFT_Bool, "bool"),
			std::make_pair(frox::ECFT_UInt8, "uint8"),
			std::make_pair(frox::ECFT_UInt16, "uint16"),
			std::make_pair(frox::ECFT_UInt32, "uint32"),
			std::make_pair(frox::ECFT_Float, "float")
		};

		auto it = names.find(type);
		return it != names.end() ? it->second : defaultName;
	}

	EachFrameType Ignored(const std::vector<frox::EComputeFrameType>& types) const
	{
		std::vector<frox::EComputeFrameType> filteredTypes;
		for (frox::EComputeFrameType type : Types)
		{
			auto it = std::find(types.begin(), types.end(), type);
			if (it == types.end())
			{
				filteredTypes.push_back(type);
			}
		}

		return EachFrameType(filteredTypes);
	}

	template<typename FuncT, typename ContextT>
	bool operator () (const char* name, FuncT f, ContextT context)
	{
		bool result = true;
		for (frox::EComputeFrameType type : Types)
		{
			std::string newName = std::string(name) + " - " + TypeToName(type);	
			result &= context(newName.c_str(), f(type));
		}

		return result;
	}
};

struct EachFrameSize
{
	std::vector<frox::Size> Sizes;
	EachFrameSize()
	{
		Sizes.push_back(frox::Size{ 1, 1 });
		Sizes.push_back(frox::Size{ 64, 64 });
		Sizes.push_back(frox::Size{ 58, 14 });
	}

	EachFrameSize(const std::vector<frox::Size>& sizes)
		: Sizes(sizes)
	{}

	template<typename FuncT, typename ContextT>
	bool operator () (const char* name, FuncT f, ContextT context)
	{
		bool result = true;
		for (frox::Size size : Sizes)
		{
			std::string sizeText = std::to_string(size.Width) + "x" + std::to_string(size.Height);
			std::string newName = std::string(name) + " - " + sizeText;
			result &= context(newName.c_str(), f(size));
		}

		return result;
	}
};
