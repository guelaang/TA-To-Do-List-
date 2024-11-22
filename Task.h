#pragma once
#include <string>
#include <vector>

struct Task
{
	std::string description;
	bool done;
};

void SaveTaskToFile(const std::vector<Task>& tasks, const std::string& fileName);
std::vector<Task> loadTaskFromFile(const std::string& fileName);