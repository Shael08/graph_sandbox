#pragma once
#include "graph.h"

class clique_ip_generator : public graph
{

	int triangle_reformulation_ = 1;

	int square_reformulation_ = 2;

	int independent_set_constrain_ = 4;

public:
	explicit clique_ip_generator(char * file) : graph(file) {};

	clique_ip_generator(const clique_ip_generator &) = delete;

	clique_ip_generator(clique_ip_generator&&) = delete;

	clique_ip_generator& operator =(clique_ip_generator const&) = delete;

	clique_ip_generator& operator=(clique_ip_generator&&) = delete;

	~clique_ip_generator() = default;

	std::string get_clique_objective() const;

	std::pair<int, std::string> independent_set_constrain() const;

	int get_type_of_independent_set_constrain() const;

	std::pair<int, std::string> triangle_reformulation() const;

	int get_type_of_triangle_reformulation() const;

	std::pair<int, std::string> square_reformulation() const;

	int get_type_of_square_reformulation() const;

	std::string get_binary_variables() const;

};

