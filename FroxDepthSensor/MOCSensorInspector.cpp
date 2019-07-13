#include "MOCSensorInspector.h"

#include <Frox.h>
#include <Log.h>
#include <Utils.h>

namespace frox {

MOCSensorInspector::MOCSensorInspector()
{}

MOCSensorInspector::~MOCSensorInspector()
{}

bool MOCSensorInspector::Start()
{
	_frame = FroxInstance()->CreateComputeFrame(Size{ 64, 64 }, EComputeFrameType::ECFT_UInt16);
	utils::Fill<uint16_t>(_frame, [](uint16_t) -> uint16_t {
		return uint16_t(std::rand() % std::numeric_limits<uint16_t>::max());
	});
	return true;
}

void MOCSensorInspector::Stop()
{
	_frame = nullptr;
}

bool MOCSensorInspector::IsValid() const
{
	return _frame != nullptr;
}

ComputeFramePtr MOCSensorInspector::ReadFrame() const
{
	return _frame;
}

} // End frox.
