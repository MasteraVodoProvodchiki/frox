#include <Frox.h>

#include <ComputeFlow.h>

#include <BasicComputeNodes.h>
#include <AvgComputeNode.h>
#include <MakeFrameComputeNode.h>

#include <iostream>
#include <conio.h>
#include <assert.h>
#include <vector>

using namespace frox;

void simpleTest(Frox* frox)
{
	assert(frox != nullptr);

	// Create Flow
	ComputeFlow* flow = frox->CreateComputeFlow();

	// Create nodes
	auto add = flow->CreateNode<AddComputeNode>("Add");
	auto mul = flow->CreateNode<MulComputeNode>("Mul");
	flow->ConnectNodes(add, mul, 0);

	// Set inputs
	add->SetInputScalar(0, 1.f);
	add->SetInputScalar(1, 2.f);
	mul->SetInputScalar(1, 3.f);

	// Invoke
	flow->Perform();
	flow->Fetch();

	// Show result
	ComputeFramePtr result = mul->GetOutput();
	if (result && result->IsValid())
	{
		float value = result->At<float>(0, 0);
		std::cout << "Simple Calc: " << value << " expected 9"<< std::endl;
	}
	else
	{
		std::cout << "Simple Calc: invalid" << std::endl;
	}

	// Destroy Flow
	frox->DestroyComputeFlow(flow);
}

void multiTest(Frox* frox, uint32_t width = 64, uint32_t height = 64)
{
	assert(frox != nullptr);

	// Create Flow
	ComputeFlow* flow = frox->CreateComputeFlow();

	// Create nodes
	auto make = flow->CreateNode<MakeFrameComputeNode>("Make");
	make->SetWidth(width);
	make->SetHeight(height);
	make->SetType(EComputeFrameType::ECFT_Float);
	make->SetValue(1.f);

	auto add = flow->CreateNode<AddComputeNode>("Add");
	auto avg = flow->CreateNode<AvgComputeNode>("Avg");
	flow->ConnectNodes(make, add, 0);
	flow->ConnectNodes(make, add, 1);
	flow->ConnectNodes(add, avg, 0);

	// Set inputs
	/*
	std::vector<float> values(width * height);
	std::fill(values.begin(), values.end(), 1.f);

	auto A = frox->CreateComputeFrame(Size{ width, height }, EComputeFrameType::ECFT_Float, values.data());
	auto B = frox->CreateComputeFrame(Size{ width, height }, EComputeFrameType::ECFT_Float, values.data());

	add->SetInput(0, A);
	add->SetInput(1, B);
	*/
	// Invoke
	flow->Perform();
	flow->Fetch();

	// Show result
	ComputeFramePtr result = avg->GetOutput();
	if (result && result->IsValid())
	{
		float value = result->At<float>(0, 0);
		std::cout << "Multi Calc: " << value << " expected 2" << std::endl;
	}
	else
	{
		std::cout << "Multi Calc: invalid" << std::endl;
	}

	// Destroy Flow
	frox->DestroyComputeFlow(flow);
}

int main(int argc, char *argv[])
{
	// Init
	Frox* frox = FroxInit();

	// Test
	simpleTest(frox);
	multiTest(frox);

	// Shutdown
	FroxShutdown(frox);

	// Wait
	_getch();
	return 0;
}