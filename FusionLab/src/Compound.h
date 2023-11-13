#pragma once

#include <string>

namespace fl
{

struct Compound
{
	std::string formula;

	Compound(const std::string& form) : formula(form) {};
};

}

