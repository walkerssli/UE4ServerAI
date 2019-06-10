#include "MyDelegate.h"


namespace UE4Delegates_Private
{
	uint64	GNextID = 1;
}
FDelegateHandle FDelegateHandle::INVALID_HANDLE;


uint64 FDelegateHandle::GenerateNewID()
{
	// Just increment a counter to generate an ID.
	uint64 Result = ++UE4Delegates_Private::GNextID;

	// Check for the next-to-impossible event that we wrap round to 0, because we reserve 0 for null delegates.
	if (Result == 0)
	{
		// Increment it again - it might not be zero, so don't just assign it to 1.
		Result = ++UE4Delegates_Private::GNextID;
	}

	return Result;

}
