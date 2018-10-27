#pragma once
#include "graph.h"
#include <map>

class graph_to_ip
{

public:
	enum objective_type
	{
		none,
		minimize,
		maximize
	};

	graph_to_ip() = default;;

	~graph_to_ip() = default;

	graph_to_ip(const graph_to_ip &) = delete;

	graph_to_ip(graph_to_ip&&) = delete;

	graph_to_ip& operator =(graph_to_ip const&) = delete;

	graph_to_ip& operator=(graph_to_ip&&) = delete;

	void set_objective(const objective_type objective_type, const std::string & objective);

	void delete_objective();

	void add_constrains(std::pair<int, std::string> constrain);

	void delete_constrain(const int &constrain_type);

	void add_binary_variables(const std::string& binary_variables);

	void delete_binary_variables();

	void create_ip_file(const std::string & file_name) const;

private:
	objective_type objective_type_ = none;

	std::string objective_;

	std::map<int, std::string> constrains_;

	std::string binary_variables_;
};
