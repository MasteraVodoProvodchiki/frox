#pragma once

#include "Common.h"
#include "ComputeFrame.h"

namespace frox {

/**
 * @brief Compute flow class
 */
class FROX_API ComputeFlow
{
public:
	virtual ~ComputeFlow() {}

	// Nodes
	virtual ComputeNode* CreateNode(const char* type, const char* name = nullptr) = 0;
	virtual void DestoyNode(ComputeNode* node) = 0;

	template <typename ComputeNodeT>
	ComputeNodeT* CreateNode(const char* name = nullptr);

	// Connections
	virtual bool ConnectNodes(ComputeNode* outNode, uint32_t outPinId, ComputeNode* inNode, uint32_t inPinId) = 0;
	bool ConnectNodes(ComputeNode* outNode, ComputeNode* inNode, uint32_t inPinId = 0)
	{
		return ConnectNodes(outNode, 0, inNode, inPinId);
	}
	virtual bool DisconnectNodes(ComputeNode* outNode, uint32_t outPinId, ComputeNode* inNode, uint32_t inPinId) = 0;
	bool DisconnectNodes(ComputeNode* outNode, ComputeNode* inNode, uint32_t inPinId = 0)
	{
		return DisconnectNodes(outNode, 0, inNode, inPinId);
	}

	// Input/Output
	/**
	 * @brief 
	 * @return Entry Id
	 */
	virtual uint32_t CreateEntry(const char* name, EPinValueType type = EPinValueType::Frame) = 0;

	/**
	 * @brief
	 * @return Output Id
	 */
	virtual uint32_t CreateOutput(const char* name, EPinValueType type = EPinValueType::Frame) = 0;

	virtual int32_t FindEntryByName(const char* name) const = 0;
	virtual int32_t FindOutputByName(const char* name) const = 0;

	virtual void ConnectEntry(uint32_t entryId, ComputeNode* inNode, uint32_t inPinId = 0) = 0;
	virtual void DisconnectEntry(uint32_t entryId, ComputeNode* inNode, uint32_t inPinId = 0) = 0;

	virtual void ConnectOutput(uint32_t outputId, ComputeNode* outNode, uint32_t outPinId = 0) = 0;
	virtual void DisconnectOutput(uint32_t outputId, ComputeNode* outNode, uint32_t outPinId = 0) = 0;

	bool ConnectEntry(const char* entryName, ComputeNode* inNode, const char* inPinName);
	bool DisconnectEntry(const char* entryName, ComputeNode* inNode, const char* inPinName);

	bool ConnectOutput(const char* outputName, ComputeNode* outNode, const char* outPinName);
	bool DisconnectOutput(const char* outputName, ComputeNode* outNode, const char* outPinName);

	uint32_t FindOrCreateEntry(const char* name, EPinValueType type = EPinValueType::Frame);
	uint32_t FindOrCreateOutput(const char* name, EPinValueType type = EPinValueType::Frame);

	// Common
	void Release();
};

template <typename ComputeNodeT>
ComputeNodeT* ComputeFlow::CreateNode(const char* name)
{
	ComputeNode* node = CreateNode(ComputeNodeT::GetNodeTypeStatic(), name);
	return reinterpret_cast<ComputeNodeT*>(node);
}

} // End frox