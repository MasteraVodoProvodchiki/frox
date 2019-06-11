#include <Frox.h>

#include <ComputeFlow.h>
#include <BasicComputeNodes.h>

#include <iostream>
#include <conio.h>

using namespace frox;

int main(int argc, char *argv[])
{
	// Init
	Frox* frox = FroxInit();

	// Create Flow
	ComputeFlow* flow = frox->CreateComputeFlow();

	auto add = flow->CreateNode<AddComputeNode>("Add");
	auto mul = flow->CreateNode<MulComputeNode>("Mul");
	flow->ConnectNodes(add, mul, 0);

	add->SetInputScalar(0, 1.f);
	add->SetInputScalar(1, 2.f);
	mul->SetInputScalar(1, 3.f);

	flow->Perform();
	flow->Fetch();

	ComputeFramePtr result = mul->GetOutput();
	if (result->IsValid())
	{
		const float* values = result->GetData<float>();
		float value = values[0];
		std::cout << "Calc: " << value << std::endl;
	}
	else
	{
		std::cout << "Calc: invalid" << std::endl;
	}

	frox->DestroyComputeFlow(flow);

	// Shutdown
	FroxShutdown(frox);

	_getch();
	return 0;
}