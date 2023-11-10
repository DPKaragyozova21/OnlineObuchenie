#include "Machine.h"

namespace fl
{

void Machine::UpdateIO()
{
	for (int i = 0; i < 4; i++)
	{
		output[i] = nullptr;
		input[i] = nullptr;
	}

	if (!input[(int)Side::LEFT] && canInput[(int)Side::LEFT] && (machineMap.find(pos - 1) != machineMap.end()))
	{
		if (!machineMap[pos - 1])
		{
			if (machineMap[pos - 1]->canOutput[(int)Side::RIGHT] && !machineMap[pos - 1]->output[(int)Side::RIGHT])
			{
				input[(int)Side::LEFT] = machineMap[pos - 1];
				machineMap[pos - 1]->output[(int)Side::RIGHT] = this;
			}
		}
	}
	if (!input[(int)Side::RIGHT] && canInput[(int)Side::RIGHT] && (machineMap.find(pos + 1) != machineMap.end()))
	{
		if (!machineMap[pos + 1])
		{
			if (machineMap[pos + 1]->canOutput[(int)Side::LEFT] && !machineMap[pos + 1]->output[(int)Side::LEFT])
			{
				input[(int)Side::RIGHT] = machineMap[pos + 1];
				machineMap[pos + 1]->output[(int)Side::LEFT] = this;
			}
		}
	}
	if (!input[(int)Side::UP] && canInput[(int)Side::UP] && (machineMap.find(pos - 10000) != machineMap.end()))
	{
		if (!machineMap[pos - 10000])
		{
			if (machineMap[pos - 10000]->canOutput[(int)Side::DOWN] && !machineMap[pos - 10000]->output[(int)Side::DOWN])
			{
				input[(int)Side::UP] = machineMap[pos - 10000];
				machineMap[pos - 10000]->output[(int)Side::DOWN] = this;
			}
		}
	}
	else if (!input[(int)Side::DOWN] && canInput[(int)Side::DOWN] && (machineMap.find(pos + 10000) != machineMap.end()))
	{
		if (!machineMap[pos + 10000])
		{
			if (machineMap[pos + 10000]->canOutput[(int)Side::UP] && !machineMap[pos + 10000]->output[(int)Side::UP])
			{
				input[(int)Side::DOWN] = machineMap[pos + 10000];
				machineMap[pos + 10000]->output[(int)Side::UP] = this;
			}
		}
	}

	if (!output[(int)Side::LEFT] && canInput[(int)Side::LEFT] && (machineMap.find(pos - 1) != machineMap.end()))
	{
		if (!machineMap[pos - 1])
		{
			if (machineMap[pos - 1]->canOutput[(int)Side::RIGHT] && !machineMap[pos - 1]->output[(int)Side::RIGHT])
			{
				output[(int)Side::LEFT] = machineMap[pos - 1];
				machineMap[pos - 1]->output[(int)Side::RIGHT] = this;
			}
		}
	}
	if (!output[(int)Side::RIGHT] && canInput[(int)Side::RIGHT] && (machineMap.find(pos + 1) != machineMap.end()))
	{
		if (!machineMap[pos + 1])
		{
			if (machineMap[pos + 1]->canOutput[(int)Side::LEFT] && !machineMap[pos + 1]->output[(int)Side::LEFT])
			{
				output[(int)Side::RIGHT] = machineMap[pos + 1];
				machineMap[pos + 1]->output[(int)Side::LEFT] = this;
			}
		}
	}
	if (!output[(int)Side::UP] && canInput[(int)Side::UP] && (machineMap.find(pos - 10000) != machineMap.end()))
	{
		if (!machineMap[pos - 10000])
		{
			if (machineMap[pos - 10000]->canOutput[(int)Side::DOWN] && !machineMap[pos - 10000]->output[(int)Side::DOWN])
			{
				output[(int)Side::UP] = machineMap[pos - 10000];
				machineMap[pos - 10000]->output[(int)Side::DOWN] = this;
			}
		}
	}
	else if (!output[(int)Side::DOWN] && canInput[(int)Side::DOWN] && (machineMap.find(pos + 10000) != machineMap.end()))
	{
		if (!machineMap[pos + 10000])
		{
			if (machineMap[pos + 10000]->canOutput[(int)Side::UP] && !machineMap[pos + 10000]->output[(int)Side::UP])
			{
				output[(int)Side::DOWN] = machineMap[pos + 10000];
				machineMap[pos + 10000]->output[(int)Side::UP] = this;
			}
		}
	}
}

void Machine::RemoveMachineFromIO(Machine* machine)
{
	for (int i = 0; i < 4; i++)
	{
		if (input[i] == machine) input[i] = nullptr;
		if (output[i] == machine) output[i] = nullptr;
	}
}

void Machine::TransferItems()
{
	for (int* i : transferStorage)
	{
		storage.push_back(i);
	}
	
	transferStorage.clear();
}

void Machine::Rotate()
{
	static bool tempIn, tempOut;
	tempIn = canInput[0];
	tempOut = canOutput[0];

	for (int i = 0; i < 3; i++)
	{
		canInput[i] = canInput[i + 1];
		canOutput[i] = canOutput[i + 1];
	}

	canInput[3] = tempIn;
	canOutput[3] = tempOut;

	if (rotation == 3) rotation = 0;
	else rotation++;
}

}