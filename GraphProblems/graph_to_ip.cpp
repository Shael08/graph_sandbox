#include "pch.h"
#include "graph_to_ip.h"
#include <vector>
#include <fstream>
#include <string>
#include <iostream>

void graph_to_ip::set_objective(const objective_type objective_type,const std::string &objective)
{
	if (objective_type_ == none)
	{
		this->objective_type_ = objective_type;
		this->objective_ = objective;
	}
}

void graph_to_ip::delete_objective()
{
	this->objective_ = "";
	this->objective_type_ = none;
}

void graph_to_ip::add_constrains(std::pair<int, std::string> constrain)
{
	constrains_.insert(constrain);
}

void graph_to_ip::delete_constrain(const int &constrain_type)
{
	const auto it = constrains_.find(constrain_type);
	constrains_.erase(it);
}

void graph_to_ip::add_binary_variables(const std::string & binary_variables)
{
	if (binary_variables_.empty()) 
	{
		binary_variables_ = binary_variables;
	}
}

void graph_to_ip::delete_binary_variables()
{
	this->binary_variables_ = "";
}

void graph_to_ip::create_ip_file(const std::string &file_name) const
{
	const std::string delimiter = "\n";

	std::vector<std::string> strings;
	std::vector<std::string> binary;
	std::string::size_type pos;
	std::string::size_type prev;

	for (auto const& constrain : constrains_)
	{
		prev = 0;
		while ((pos = constrain.second.find(delimiter, prev)) != std::string::npos)
		{
			strings.push_back(constrain.second.substr(prev, pos - prev));
			prev = pos + 1;
		}
		strings.push_back(constrain.second.substr(prev));
	}

	if (!binary_variables_.empty())
	{
		prev = 0;
		while ((pos = binary_variables_.find(delimiter, prev)) != std::string::npos)
		{
			binary.push_back(binary_variables_.substr(prev, pos - prev));
			prev = pos + 1;
		}
		binary.push_back(binary_variables_.substr(prev));
	}

	std::ofstream my_file;
	my_file.open(file_name.c_str(), std::ios::trunc);

	switch (objective_type_)
	{
	case minimize:
		my_file << "Minimize\n obj:";
		break;
	case maximize:
		my_file << "Maximize\n obj:";
		break;
	default:
		std::cout << "objective type not set";
		return;
	}

	my_file << objective_;

	my_file << "\nSubject To\n";

	int counter = 0;
	for (std::vector<std::string>::iterator	it = strings.begin(); it != strings.end(); ++it)
	{
		if ((*it).length() > 1)
		{
			my_file << "c" << patch::to_string(counter) << ": " << *it << "\n";
			counter++;
		}
		else
		{
			my_file << "\n";
		}
	}


	if(binary.size() > 1)
	{
		my_file << "\nBinary\n";

		for (std::vector<std::string>::iterator it = binary.begin(); it != binary.end(); ++it)
		{
			if ((*it).length() > 1)
			{
				my_file << *it << "\n";
			}
			else
			{
				my_file << "\n";
			}
		}
	}

	my_file << "End";
	my_file.close();

	std::cout << "file created successfully" << std::endl;

}