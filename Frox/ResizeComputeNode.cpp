#include "ResizeComputeNode.h"
#include "ComputeTask.h"
#include "Frox.h"
#include "Utils.h"

#include <assert.h>
#include <algorithm>

namespace frox {

FROX_COMPUTENODE_IMPL(ResizeComputeNode)

ResizeComputeNode::ResizeComputeNode(const ComputeNodeInitializer& initializer)
	: Super(initializer)
	, _input(0)
	, _output(0)
	, _size(Size({1, 1}))
{}

ResizeComputeNode::~ResizeComputeNode()
{}

void ResizeComputeNode::AllocateDefaultPins()
{
	_input = CreateInput("input");
	_output = CreateOutput("output");
}

void ResizeComputeNode::OnPostInit()
{
	ComputeFramePtr input = GetInput(_input);
	ComputeFramePtr output = GetOutput(_output);
	if (input && !output)
	{
		// ComputeFramePtr output = FroxInstance()->CreateComputeFrame(_size, input->GetType());
		// SetOutput(_output, output);
	}
}

bool ResizeComputeNode::IsValid() const
{
	ComputeFramePtr input = GetInput(_input);
	ComputeFramePtr output = GetOutput(_output);

	return
		input != nullptr &&
		output != nullptr &&
		input->GetType() == output->GetType() &&
		_size.IsValid() &&
		input->IsValid();
}

ComputeTask* ResizeComputeNode::CreateComputeTask()
{
	ComputeFramePtr input = GetInput(_input);
	ComputeFramePtr output = GetOutput(_output);

	// Actual Size !!
	Size size = _size.GetValue();

	return ComputeTaskUtils::Make([input, output, size]() {


	});
}

void ResizeComputeNode::SetSize(Size size)
{
	_size.DefaultValue = size;
}

} // End frox