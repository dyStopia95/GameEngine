#include "pch.h"
#include "ParseHelper.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ParseHelper);
	
	void ParseHelper::SetStackFrame(std::shared_ptr<Vector<StackFrame>> contextStack)
	{
		_contextRef = contextStack;
	}

	void ParseHelper::Cleanup()
	{
		_contextRef.reset();
	}
}