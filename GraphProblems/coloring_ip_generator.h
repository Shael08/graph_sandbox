#pragma once
#include "graph.h"
class coloring_ip_generator : public graph
{
private:

	int upper_bound_ = -1;

	int get_upper_bound() const;

	int color_every_vertex_constrain_ = 1;

	int every_vertex_have_only_one_color_constrain_ = 2;

	int proper_vertex_coloring_naive_constrain_ = 4;

	int proper_vertex_coloring_with_clique_constrain_ = 4;  //forbidden with proper_vertex_coloring_naive_constrain_

	int synchronization_with_general_integer_variables_constrain_ = 8;

	int symmetry_breaking_constrain_ = 16;
	
public:
	explicit coloring_ip_generator(char * file) : graph(file) {};

	coloring_ip_generator(const coloring_ip_generator &) = delete;

	coloring_ip_generator(coloring_ip_generator&&) = delete;

	coloring_ip_generator& operator =(coloring_ip_generator const&) = delete;

	coloring_ip_generator& operator=(coloring_ip_generator&&) = delete;

	~coloring_ip_generator() = default;

	std::string get_coloring_objective() const;

	std::pair<int, std::string> color_every_vertex_constrain() const;

	int get_type_of_color_every_vertex_constrain() const;

	std::pair<int, std::string> proper_vertex_coloring_naive_constrain() const;

	int get_type_of_proper_vertex_coloring_naive_constrain() const;

	std::pair<int, std::string> proper_vertex_coloring_with_clique_constrain() const;

	int get_type_of_proper_vertex_coloring_with_clique_constrain() const;

	std::pair<int, std::string> every_vertex_have_only_one_color_constrain() const;

	int get_type_of_every_vertex_have_only_one_color_constrain() const;

	std::pair<int, std::string> synchronization_with_general_integer_variables_constrain() const;

	int get_type_of_synchronization_with_general_integer_variables_constrain() const;

	std::pair<int, std::string> symmetry_breaking_constrain() const;

	int get_type_of_symmetry_breaking_constrain() const;

	std::string get_binary_variables() const;
};

