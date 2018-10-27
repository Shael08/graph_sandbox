#include "pch.h"
#include "graph.h"
#include <algorithm>
#include <iostream>

int graph::find_clique(bool **adjacency_matrix, const int vertices)
{
	int *degree = new int[vertices];
	bool *neighbors_vector = new bool[vertices];
	bool** copy_of_adjacency_matrix = new bool*[vertices]();
	for (int i = 0; i < vertices; i++)
	{
		copy_of_adjacency_matrix[i] = new bool[vertices]();
		for (int j = 0; j < vertices; j++)
		{
			copy_of_adjacency_matrix[i][j] = adjacency_matrix[i][j];
		}
	}

	std::vector<int> clique;

	for (int i = 0; i < vertices; i++)
	{
		int counter = 0;
		for (int j = 0; j < vertices; j++)
		{
			if (copy_of_adjacency_matrix[i][j] == 1) counter++;
		}
		degree[i] = counter;
	}

	for (int n = 0; ; n++)
	{
		int	max = -1;
		int highest_vertex;
		for (int i = 0; i < vertices; ++i)
		{
			if (neighbors_vector[i] && max < degree[i])
			{
				max = degree[i];
				highest_vertex = i;
			}
		}

		if (max == -1) break;

		for (int i = 0; i < vertices; i++)
		{
			if (copy_of_adjacency_matrix[i][highest_vertex] == 0) neighbors_vector[i] = 0;
		}

		clique.push_back(highest_vertex);

		for (int i = 0; i < vertices; ++i)
		{

			if (i != highest_vertex && !copy_of_adjacency_matrix[i][highest_vertex] && degree[i]) {
				degree[i] = 0;
				for (int j = 0; j < vertices; ++j)
				{

					if (copy_of_adjacency_matrix[i][j])
					{
						copy_of_adjacency_matrix[i][j] = false;
						copy_of_adjacency_matrix[j][i] = false;
						degree[j]--;
					}

				}
			}
		}

		for (int i = 0; i < vertices; i++)
		{

			if (copy_of_adjacency_matrix[i][highest_vertex])
			{
				copy_of_adjacency_matrix[i][highest_vertex] = false;
				copy_of_adjacency_matrix[highest_vertex][i] = false;
				degree[i]--;
			}


		}

		degree[highest_vertex] = 0;
	}

	sort(clique.begin(), clique.end());


	delete[] degree;
	delete[] neighbors_vector;

	for (int i = 0; i < vertices; ++i)
	{
		delete[] copy_of_adjacency_matrix[i];
	}
	delete[] copy_of_adjacency_matrix;

	return clique.size();
}

std::vector<std::vector<int>> graph::find_all_clique(bool in_complementary, unsigned minimum_clique_size) const
{
	int *degree = new int[vertices_];
	bool *neighbors_vector = new bool[vertices_];

	bool **copy_of_adjacency_matrix = get_copy_of_adjacency_matrix();

	if (in_complementary)
	{
		for (int i = 0; i < vertices_; i++)
		{
			for (int j = 0; j < vertices_; j++)
			{
				if (i != j)
				{
					copy_of_adjacency_matrix[i][j] = !copy_of_adjacency_matrix[i][j];
				}
			}
		}
	}

	std::vector<std::vector<int>> all_clique;
	for (int f = 0; ; f++)
	{
		std::vector<int> clique;
		const ::graph tmp_graph(vertices_, copy_of_adjacency_matrix);

		for (int i = 0; i < vertices_; i++)
		{
			degree[i] = 0;
			neighbors_vector[i] = true;
			int counter = 0;
			for (int j = 0; j < vertices_; j++)
			{
				if (tmp_graph.adjacency_matrix_[i][j] == 1) counter++;
			}
			degree[i] = counter;
		}

		for (int n = 0; ; n++)
		{
			int	max = -1;
			int highest_vertex;
			for (int i = 0; i < vertices_; ++i)
			{
				if (neighbors_vector[i] && max < degree[i])
				{
					max = degree[i];
					highest_vertex = i;
				}
			}

			if (max == -1) break;

			for (int i = 0; i < vertices_; i++)
			{
				if (tmp_graph.adjacency_matrix_[i][highest_vertex] == 0) neighbors_vector[i] = false;
			}

			clique.push_back(highest_vertex);

			for (int i = 0; i < vertices_; ++i)
			{

				if (i != highest_vertex && !tmp_graph.adjacency_matrix_[i][highest_vertex] && degree[i]) {
					degree[i] = 0;
					for (int j = 0; j < vertices_; ++j)
					{

						if (tmp_graph.adjacency_matrix_[i][j])
						{
							tmp_graph.adjacency_matrix_[i][j] = false;
							tmp_graph.adjacency_matrix_[j][i] = false;
							degree[j]--;
						}

					}
				}
			}

			for (int i = 0; i < vertices_; i++)
			{

				if (tmp_graph.adjacency_matrix_[i][highest_vertex])
				{
					tmp_graph.adjacency_matrix_[i][highest_vertex] = false;
					tmp_graph.adjacency_matrix_[highest_vertex][i] = false;
					degree[i]--;
				}


			}

			degree[highest_vertex] = 0;
		}

		if (clique.size() < minimum_clique_size) break;

		sort(clique.begin(), clique.end());
		all_clique.push_back(clique);

		for (std::vector<int>::iterator it = clique.begin(); it != clique.end(); ++it)
		{
			for (std::vector<int>::iterator it2 = it + 1; it2 != clique.end(); ++it2)
			{
				copy_of_adjacency_matrix[*it][*it2] = false;
				copy_of_adjacency_matrix[*it2][*it] = false;
			}
		}
	}


	delete[] degree;
	delete[] neighbors_vector;

	for (int i = 0; i < vertices_; ++i)
	{
		delete[] copy_of_adjacency_matrix[i];
	}
	delete[] copy_of_adjacency_matrix;

	return all_clique;
}

