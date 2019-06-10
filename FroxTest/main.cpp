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

	ComputeFramePtr A = frox->CreateScalar(1.f);
	ComputeFramePtr B = frox->CreateScalar(2.f);
	ComputeFramePtr C = frox->CreateScalar(3.f);

	auto add = flow->CreateNode<AddComputeNode>("Add");
	auto mul = flow->CreateNode<MulComputeNode>("Mul");
	flow->ConnectNodes(add, mul, 0);

	add->SetInput(0, A);
	add->SetInput(1, B);
	mul->SetInput(1, C);

	flow->Perform();
	flow->Fetch();

	ComputeFramePtr result = mul->GetOutput();
	if (result->IsValid())
	{
		const void* data = result->GetData();
		float value = *reinterpret_cast<float*>(&data);
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