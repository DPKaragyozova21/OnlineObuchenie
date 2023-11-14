#pragma once
#include "Machine.h"

namespace fl
{

class Hub
{
public:

	Hub();

public:

	Machine* parts[16];

	void TransferItemsToHub();

private:

	std::unordered_map<std::string, int> hubStorage;
};

}
