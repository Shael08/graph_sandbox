#include "pch.h"
#include "clique_ip_generator.h"

std::string clique_ip_generator::get_clique_objective() const
{
	std::string objective;

	for (int i = 0; i < vertices_; i++) {
		objective += " + x" + patch::to_string(i);
	}

	return objective;
}

std::pair<int, std::string> clique_ip_generator::independent_set_constrain() const
{
	std::string constrain;
	std::vector<std::vector<int> > all_clique =	find_all_clique(true, 2);

	for (unsigned i = 0; i < all_clique.size(); i++)
	{
		for (unsigned j = 0; j < all_clique[i].size(); j++)
		{
			constrain += " + x" + patch::to_string(all_clique[i][j]);
		}
		constrain += " <= 1\n";
	}
	constrain += "\n";
	return std::make_pair(independent_set_constrain_, constrain);
}

int clique_ip_generator::get_type_of_independent_set_constrain() const
{
	return independent_set_constrain_;
}

std::pair<int, std::string> clique_ip_generator::triangle_reformulation() const
{
	std::string constrain;
	for (int i = 0; i < vertices_; i++)
	{

		bool** adjacency_matrix = get_copy_of_adjacency_matrix();

		for (int j = 0; j < vertices_; j++)
		{
			if (adjacency_matrix[i][j] == 1)
			{
				for (int k = 0; k < vertices_; k++)
				{
					adjacency_matrix[j][k] = false;
					adjacency_matrix[k][j] = false;
				}
			}
		}

		int k = greedy_coloring(adjacency_matrix, i);

		constrain += " +" + patch::to_string(k) + " x" + patch::to_string(i);

		for (int j = 0; j < i; j++)
		{
			if (i != j && adjacency_matrix_[i][j] == 0)
			{
				constrain += " + x" + patch::to_string(j);
			}
		}
		constrain += " <= " + patch::to_string(k) + "\n";

		for (int i = 0; i < vertices_; i++)
		{
			delete[] adjacency_matrix[i];
		}
		delete[] adjacency_matrix;
	}


	
	constrain += "\n";
	return std::make_pair(triangle_reformulation_, constrain);
}

int clique_ip_generator::get_type_of_triangle_reformulation() const
{
	return triangle_reformulation_;
}

std::pair<int, std::string> clique_ip_generator::square_reformulation() const
{
	std::string constrain;
	for (int i = 0; i < vertices_; i++)
	{

		bool** adjacency_matrix = get_copy_of_adjacency_matrix();

		for (int j = 0; j < vertices_; j++)
		{
			if (adjacency_matrix[i][j] == 1)
			{
				for (int k = 0; k < vertices_; k++)
				{
					adjacency_matrix[j][k] = false;
					adjacency_matrix[k][j] = false;
				}
			}
		}

		int k = greedy_coloring(adjacency_matrix, vertices_);

		constrain += " +" + patch::to_string(k) + " x" + patch::to_string(i);

		for (int j = 0; j < vertices_; j++)
		{
			if (i != j && adjacency_matrix_[i][j] == 0)
			{
				constrain += " + x" + patch::to_string(j);
			}
		}
		constrain += " <= " + patch::to_string(k) + "\n";

		for (int i = 0; i < vertices_; i++)
		{
			delete[] adjacency_matrix[i];
		}
		delete[] adjacency_matrix;
	}



	constrain += "\n";
	return std::make_pair(square_reformulation_, constrain);
}

int clique_ip_generator::get_type_of_square_reformulation() const
{
	return square_reformulation_;
}

std::string clique_ip_generator::get_binary_variables() const
{
	std::string constrain;

	for (int i = 0; i < vertices_; i++)
	{
		constrain += "x" + patch::to_string(i) + "\n";
	}

	constrain += "\n";
	return constrain;
}

