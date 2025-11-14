#pragma once
#include "CoreMinimal.h"

// THE PURE, CORRECT, One-Parameter DELEGATE.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSafehouseMatchStateChanged, FName, NewState);
