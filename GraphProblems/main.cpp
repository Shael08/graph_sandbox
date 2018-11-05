#include "pch.h"
#include "graph.h"
#include "graph_to_ip.h"
#include "coloring_ip_generator.h"
#include "clique_ip_generator.h"

void coloring_ip_factory(char* file_name, std::string constrains_index)
{
	coloring_ip_generator ip_generator(file_name);
	graph_to_ip coloring_ip;

	static std::string old_name = patch::to_string(file_name);
	std::string base_name = old_name.substr(0, old_name.size() - 4);

	coloring_ip.set_objective(coloring_ip.minimize, ip_generator.get_coloring_objective());
	coloring_ip.add_constrains(ip_generator.color_every_vertex_constrain());
	coloring_ip.add_constrains(ip_generator.every_vertex_have_only_one_color_constrain());

	if (constrains_index[0] == '1')
	{
		coloring_ip.add_constrains(ip_generator.proper_vertex_coloring_with_clique_constrain());
		base_name += "_clq";
	}
	else
	{
		coloring_ip.add_constrains(ip_generator.proper_vertex_coloring_naive_constrain());
	}

	if (constrains_index[1] == '1')
	{
		coloring_ip.add_constrains(ip_generator.symmetry_breaking_constrain());
		base_name += "_sym";
	}

	if (constrains_index[2] == '1')
	{
		coloring_ip.add_constrains(ip_generator.synchronization_with_general_integer_variables_constrain());
		base_name += "_sync";
	}

	coloring_ip.add_binary_variables(ip_generator.get_binary_variables());
	base_name += ".lp";
	coloring_ip.create_ip_file(base_name);
}

void init_coloring_factory(char* file_name)
{
	coloring_ip_factory(file_name, "000");
	coloring_ip_factory(file_name, "001");
	coloring_ip_factory(file_name, "010");
	coloring_ip_factory(file_name, "011");
	coloring_ip_factory(file_name, "100");
	coloring_ip_factory(file_name, "101");
	coloring_ip_factory(file_name, "110");
	coloring_ip_factory(file_name, "111");
}

void clique_ip_factory(char* file_name, std::string constrains_index)
{
	graph_to_ip clique_ip{};
	clique_ip_generator ip_generator(file_name);

	static std::string old_name = patch::to_string(file_name);
	std::string base_name = old_name.substr(0, old_name.size() - 4);

	clique_ip.set_objective(clique_ip.maximize, ip_generator.get_clique_objective());

	if (constrains_index[0] == '1')
	{
		clique_ip.add_constrains(ip_generator.triangle_reformulation());
		base_name += "_tri";
	}

	if (constrains_index[1] == '1')
	{
		clique_ip.add_constrains(ip_generator.square_reformulation());
		base_name += "_sqr";
	}

	if (constrains_index[2] == '1')
	{
		clique_ip.add_constrains(ip_generator.independent_set_constrain());
		base_name += "_ind";
	}

	clique_ip.add_binary_variables(ip_generator.get_binary_variables());
	base_name += ".lp";
	clique_ip.create_ip_file(base_name);
}

void init_clique_factory(char* file_name)
{
	clique_ip_factory(file_name, "001");
	clique_ip_factory(file_name, "010");
	clique_ip_factory(file_name, "011");
	clique_ip_factory(file_name, "100");
	clique_ip_factory(file_name, "101");
	clique_ip_factory(file_name, "110");
	clique_ip_factory(file_name, "111");
}

int main(const int argc, char *argv[])
{
	if (argc != 2) exit(1);

	//init_clique_factory(argv[1]);

	graph g(argv[1]);

	g.init_greedy_coloring();

	g.init_dsatur();

	//g.iterated_greedy_coloring();

	//std::cout << "pooling type: clique \nvertices selecting heuristics: most common neighbors" << std::endl;
	//g.init_zykov_tree(g.lp_relaxation, g.most_common_neighbors);

	//std::cout << "pooling type: clique \nvertices selecting heuristics: naive" << std::endl;
	//g.init_zykov_tree(g.clique, g.naive);

	//std::cout << "pooling type: none \nvertices selecting heuristics: most_common_neighbors" << std::endl;
	//g.init_zykov_tree(g.none, g.most_common_neighbors);

	//std::cout << "pooling type: naive \nvertices selecting heuristics: naive" << std::endl;
	//g.init_zykov_tree(g.none, g.naive);

	return 0;
}
