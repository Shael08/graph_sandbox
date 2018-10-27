#include "pch.h"
#include "coloring_ip_generator.h"


int coloring_ip_generator::get_upper_bound() const
{
	if (upper_bound_ != -1) return  upper_bound_;

	int highest_degree_node = 0;

	for (int i = 0; i < vertices_; i++)
	{
		int counter = 0;
		for (int j = 0; j < vertices_; j++)
		{
			if (adjacency_matrix_[i][j] == 1) counter++;
		}

		if (highest_degree_node < counter) highest_degree_node = counter;
	}

	return highest_degree_node + 1;
}

std::string coloring_ip_generator::get_coloring_objective() const
{
	const int upper_bound = get_upper_bound();

	std::string objective;

	for (int i = 0; i < upper_bound; i++) {
		objective += " + y_" + patch::to_string(i);
	}

	return objective;
}

std::pair<int, std::string> coloring_ip_generator::color_every_vertex_constrain() const
{
	std::string constrain;

	const int upper_bound = get_upper_bound();

	for (int i = 0; i < vertices_; i++)
	{
		for (int color = 0; color < upper_bound; color++)
		{
			constrain += "+ x" + patch::to_string(i) + "_" + patch::to_string(color);
		}
		constrain += " = 1\n";
	}

	constrain += "\n";
	return std::make_pair(color_every_vertex_constrain_, constrain);
}

int coloring_ip_generator::get_type_of_color_every_vertex_constrain() const
{
	return color_every_vertex_constrain_;
}

std::pair<int, std::string> coloring_ip_generator::proper_vertex_coloring_naive_constrain() const
{
	std::string constrain;

	const int upper_bound = get_upper_bound();
	for (int color = 0; color < upper_bound; color++)
	{
		for (int i = 0; i < vertices_; i++)
		{
			for (int j = i + 1; j < vertices_; j++)
			{
				if (adjacency_matrix_[i][j] == 1)
				{
					constrain += "+ x" + patch::to_string(i) + "_" + patch::to_string(color) + " + x" + patch::to_string(j) + "_" + patch::to_string(color) + " <= 1\n";
				}

			}

		}
	}

	constrain += "\n";
	return std::make_pair(proper_vertex_coloring_naive_constrain_, constrain);
}

int coloring_ip_generator::get_type_of_proper_vertex_coloring_naive_constrain() const
{
	return proper_vertex_coloring_naive_constrain_;
}

std::pair<int, std::string> coloring_ip_generator::proper_vertex_coloring_with_clique_constrain() const
{
	std::string constrain;

	const int upper_bound = get_upper_bound();

	std::vector<std::vector<int> > all_clique = find_all_clique(false, 2);

	for (int color = 0; color < upper_bound; color++) 
	{
		for (unsigned i = 0; i < all_clique.size(); i++)
		{
			for (unsigned j = 0; j < all_clique[i].size(); j++)
			{
				constrain += " + x" + patch::to_string(all_clique[i][j]) + "_" + patch::to_string(color);
			}
			constrain += " - y_" + patch::to_string(color) + " <= 0\n";
		}
		constrain += "\n";
	}

	constrain += "\n";
	return std::make_pair(proper_vertex_coloring_with_clique_constrain_, constrain);
}

int coloring_ip_generator::get_type_of_proper_vertex_coloring_with_clique_constrain() const
{
	return proper_vertex_coloring_with_clique_constrain_;
}

std::pair<int, std::string> coloring_ip_generator::every_vertex_have_only_one_color_constrain() const
{
	std::string constrain;

	const int upper_bound = get_upper_bound();

	for (int i = 0; i < vertices_; i++)
	{
		for (int j = 0; j < upper_bound; j++)
		{
			constrain += "+ x" + patch::to_string(i) + "_" + patch::to_string(j) + " - y_" + patch::to_string(j) + " <= 0\n";

		}
	}

	constrain += "\n";
	return std::make_pair(every_vertex_have_only_one_color_constrain_, constrain);
}

int coloring_ip_generator::get_type_of_every_vertex_have_only_one_color_constrain() const
{
	return every_vertex_have_only_one_color_constrain_;
}

std::pair<int, std::string> coloring_ip_generator::synchronization_with_general_integer_variables_constrain() const
{
	std::string constrain;

	const int upper_bound = get_upper_bound();

	for (int i = 0; i < vertices_; i++)
	{
		constrain += "- x" + patch::to_string(i);
		for (int color = 0; color < upper_bound; color++)
		{
			constrain += " + x" + patch::to_string(i) + "_" + patch::to_string(color);
		}
		constrain += " = 0 \n";
	}

	constrain += "\n";
	return std::make_pair(synchronization_with_general_integer_variables_constrain_, constrain);
}

int coloring_ip_generator::get_type_of_synchronization_with_general_integer_variables_constrain() const
{
	return synchronization_with_general_integer_variables_constrain_;
}

std::pair<int, std::string> coloring_ip_generator::symmetry_breaking_constrain() const
{
	std::string constrain;

	const int upper_bound = get_upper_bound();

	for (int color = 0; color < upper_bound - 1; color++)
	{
		for (int i = 0; i < vertices_; i++)
		{
			constrain += " + x" + patch::to_string(i) + "_" + patch::to_string(color);
		}

		for (int i = 0; i < vertices_; i++)
		{
			if (i == 0)
			{
				constrain += " - (+ x" + patch::to_string(i) + "_" + patch::to_string(color + 1);
			}
			else
			{
				constrain += " + x" + patch::to_string(i) + "_" + patch::to_string(color + 1);
			}
		}

		constrain += ") >= 0\n";
	}

	constrain += "\n";
	return std::make_pair(symmetry_breaking_constrain_, constrain);
}

int coloring_ip_generator::get_type_of_symmetry_breaking_constrain() const
{
	return symmetry_breaking_constrain_;
}

std::string coloring_ip_generator::get_binary_variables() const
{
	std::string constrain;

	const int upper_bound = get_upper_bound();

	for (int color = 0; color < upper_bound; color++)
	{
		constrain += "y_" + patch::to_string(color) + "\n";

		for (int i = 0; i < vertices_; i++)
		{
			constrain += "x" + patch::to_string(i) + "_" + patch::to_string(color) + "\n";
		}
		constrain += "\n";

	}

	return constrain;
}
