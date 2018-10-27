#include "pch.h"
#include "graph.h"
#include <list>
#include <iostream>
#include <glpk.h> 
#include <limits>  
#include <cmath> 
#include <algorithm>
#include <ctime>

#pragma region greedy_coloring

void graph::init_greedy_coloring() const
{
	std::cout << "\nColoring with greedy coloring algorithm \n" << std::endl;

	std::clock_t start = std::clock();

	int result = greedy_coloring(adjacency_matrix_, vertices_);

	double duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;

	std::cout << "Running time: " << duration << '\n';

	std::cout << "The graph can be colored with: " << result << " colors \n" << std::endl;
}

int graph::greedy_coloring(bool** adjacency_matrix, const int coloring_until)
{
	if (coloring_until < 1) return 1;

	int *result = new int[coloring_until];
	bool *available_colors = new  bool[coloring_until];

	for (int i = 0; i < coloring_until; i++)
	{
		result[i] = -1;
		available_colors[i] = false;
	}

	result[0] = 0;

	for (int i = 1; i < coloring_until; i++)
	{
		for (int j = 0; j < coloring_until; j++)
		{
			if (adjacency_matrix[i][j] == 1 && result[j] != -1)
			{
				available_colors[result[j]] = true;
			}

		}

		int cr;
		for (cr = 0; cr < coloring_until; cr++)
			if (!available_colors[cr])
				break;

		result[i] = cr;


		for (int j = 0; j < coloring_until; j++)
		{
			available_colors[j] = false;
		}
	}



	int max = 0;
	for (int u = 0; u < coloring_until; u++)
	{
		if (result[u] > max) max = result[u];
	}

	max += 1;

	delete[] result;
	delete[] available_colors;

	return max;
}

#pragma endregion 

#pragma region iterated_greedy_coloring

void graph::iterated_greedy_coloring() const
{
	std::vector<std::vector<int> > color_classes;
	color_classes.resize(vertices_);
	for(int i = 0; i < vertices_; i++)
	{
		color_classes[i].push_back(i);
	}

	std::cout << "\nColoring with iterated greedy algorithm \n" << std::endl;

	std::clock_t start = std::clock();

	int min_color = recolor_color_class(color_classes);

	for (int k = 0; k < vertices_; k++)
	{

		for (int i = 0; i < 5; i++)
		{
			std::sort(color_classes.begin(), color_classes.end(), sort_decreasing_size);

			min_color = recolor_color_class(color_classes);
		}

		if(k % 5 == 0)
		{
			std::sort(color_classes.begin(), color_classes.end(), sort_increasing_size);

			min_color = recolor_color_class(color_classes);
		}
		else
		{
			std::reverse(color_classes.begin(), color_classes.end());

			min_color = recolor_color_class(color_classes);
		}

		if(k % 3 == 0)
		{
			sort_random_order(color_classes);

			min_color = recolor_color_class(color_classes);

		}

	}

	double duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;

	std::cout << "Running time: " << duration << '\n';

	std::cout << "The graph can be colored with: " << min_color << " colors \n" << std::endl;
}

int graph::recolor_color_class(std::vector<std::vector<int> > &color_classes) const
{
	int *result = new int[vertices_];
	bool *available_colors = new  bool[vertices_];

	for (int i = 0; i < vertices_; i++)
	{
		result[i] = -1;
		available_colors[i] = false;
	}


	for (unsigned i = 0; i < color_classes.size(); i++)
	{
		for (auto it = color_classes[i].begin(); it != color_classes[i].end(); ++it)
		{
			if (i == 0 && it == color_classes[0].begin()) result[color_classes[0][0]] = 0;
			for (int j = 0; j < vertices_; j++)
			{
				if (adjacency_matrix_[(*it)][j] == 1 && result[j] != -1)
				{
					available_colors[result[j]] = true;
				}

			}

			int cr;
			for (cr = 0; cr < vertices_; cr++)
				if (!available_colors[cr])
					break;

			result[*it] = cr;


			for (int j = 0; j < vertices_; j++)
			{
				available_colors[j] = false;
			}

		}

	}

	for (unsigned i = 0; i < color_classes.size(); i++)
	{
		color_classes[i].clear();
	}

	int max = 0;
	for (int u = 0; u < vertices_; u++)
	{
		if (result[u] > max) max = result[u];
	}

	max += 1;

	color_classes.resize(max);

	for (int i = 0; i < vertices_; i++)
	{
		color_classes[result[i]].push_back(i);

	}


	color_classes.resize(max);

	//for (int i = 0; i < max; i++)
	//{
	//	std::cout << i << ": ";
	//	for (auto it = color_classes[i].begin(); it != color_classes[i].end(); ++it)
	//	{
	//		std::cout << *it << " ";
	//	}
	//	std::cout << std::endl;
	//}

	delete[] result;
	delete[] available_colors;

	return max;
}

bool graph::sort_increasing_size(const std::vector<int> &color_classes_1, const std::vector<int> &color_classes_2) {
	return color_classes_1.size() < color_classes_2.size();
}

bool graph::sort_decreasing_size(const std::vector<int> &color_classes_1, const std::vector<int> &color_classes_2) {
	return color_classes_1.size() > color_classes_2.size();
}

void graph::sort_increasing_degree(std::vector<std::vector<int>>& color_classes) const
{
	int mini_pos;
	for (unsigned i = 0; i < color_classes.size(); i++)
	{
		mini_pos = i;
		for (unsigned j = i + 1; j < color_classes.size(); j++)
		{
			if (get_color_class_degree(color_classes[j]) < get_color_class_degree(color_classes[mini_pos])) 
			{
				mini_pos = j;
			}
		}

		auto temp = color_classes[mini_pos];
		color_classes[mini_pos] = color_classes[i];
		color_classes[i] = temp;
	}
}

void graph::sort_decreasing_degree(std::vector<std::vector<int>>& color_classes) const
{
	for (unsigned i = 0; i < color_classes.size(); i++)
	{
		int mini_pos = i;
		for (unsigned j = i + 1; j < color_classes.size(); j++)
		{
			if (get_color_class_degree(color_classes[j]) > get_color_class_degree(color_classes[mini_pos]))
			{
				mini_pos = j;
			}
		}

		auto temp = color_classes[mini_pos];
		color_classes[mini_pos] = color_classes[i];
		color_classes[i] = temp;
	}
}

int graph::get_color_class_degree(std::vector<int>& color_class) const
{
	int degree = 0;
	for (int i = 0; i < vertices_; i++)
	{
		for (auto it = color_class.begin(); it != color_class.end(); ++it)
		{
			if (adjacency_matrix_[i][*it]) degree++;
		}
	}
	return degree;
}

void graph::sort_random_order(std::vector<std::vector<int>>& color_classes)
{
	for (unsigned i = 0; i < color_classes.size(); i++)
	{
		int new_pos = ((i + 1) * 401) % color_classes.size();

		auto temp = color_classes[new_pos];
		color_classes[new_pos] = color_classes[i];
		color_classes[i] = temp;
	}
}

#pragma endregion 

#pragma region DSATUR

void graph::init_dsatur()
{
	std::cout << "\nColoring with DSATUR algorithm \n" << std::endl;

	std::clock_t start = std::clock();

	int result = dsatur(adjacency_matrix_, vertices_);

	double duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;

	std::cout << "Running time: " << duration << '\n';

	std::cout << "The graph can be colored with: " << result << " colors \n" << std::endl;
}

int graph::dsatur(bool** adjacency_matrix, const int vertices)
{
	int *result = new int[vertices];
	bool *available_colors = new bool[vertices];

	for (int i = 0; i < vertices; i++)
	{
		result[i] = -1;
		available_colors[i] = false;
	}

	result[0] = 0;

	for(int n = 0 ; ;n++)
	{

		int maximal_colored_neighbor = 0;
		int vertices_with_maximal_colored_neighbors = -1;

		for (int i = 0; i < vertices; i++)
		{

			int number_of_colored_neighbors = 0;
			for (int j = 0; j < vertices; j++)
			{
				if (adjacency_matrix[i][j] && result[j] != -1)
				{
					number_of_colored_neighbors++;
				}
			}

			if (result[i] == -1 && maximal_colored_neighbor < number_of_colored_neighbors)
			{
				maximal_colored_neighbor = number_of_colored_neighbors;
				vertices_with_maximal_colored_neighbors = i;
			}
		}

		if (vertices_with_maximal_colored_neighbors == -1) break;

		for (int j = 0; j < vertices; j++)
		{
			if (adjacency_matrix[vertices_with_maximal_colored_neighbors][j] == 1 && result[j] != -1)
			{
				available_colors[result[j]] = true;
			}
		}

		int cr;
		for (cr = 0; cr < vertices; cr++)
			if (!available_colors[cr])
				break;

		result[vertices_with_maximal_colored_neighbors] = cr;

		for (int j = 0; j < vertices; j++)
		{
			available_colors[j] = false;
		}

	}

	int max = 0;
	for (int u = 0; u < vertices; u++)
	{
		if (result[u] == -1) std::cout << "error!!!" << std::endl;
		if (result[u] > max) max = result[u];
	}

	max += 1;

	delete[] result;
	delete[] available_colors;

	return max;
}

#pragma endregion 

#pragma region zykov_tree

void graph::init_zykov_tree(zykov_pooling_heuristic pooling, zykov_selecting_vertices_heuristics selecting_vertices)
{
	int current_best = std::numeric_limits<int32_t>::max();

	switch (selecting_vertices)
	{
		case naive:
			selected_vertices_selecting_heuristics_ = &find_unconnected_vertex;
			break;

		case most_common_neighbors:
			selected_vertices_selecting_heuristics_ = &find_most_common_neighbors;
			break;
	}

	switch (pooling)
	{
		case none:
			selected_pooling_heuristics_ = [](bool** adjacency_matrix, int vertices) {	return -1;	};
			break;
		case clique:
			selected_pooling_heuristics_ = &find_clique;
			break;
		case lp_relaxation:
			selected_pooling_heuristics_ = &lp_relaxation_coloring;
			break;

	}

	int chromatic_number;

	std::cout << "\nColoring with zykov tree \n" << std::endl;

	std::clock_t start = std::clock();

	chromatic_number = build_zykov_tree(adjacency_matrix_, vertices_, current_best);

	double duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;

	std::cout << "Running time: " << duration << '\n';

	std::cout << "The chromatic number is: " << chromatic_number << std::endl;
}

int graph::build_zykov_tree(bool** adjacency_matrix, int vertices, int &current_best) const
{
	const std::pair<int, int> unconnected_vertices = selected_vertices_selecting_heuristics_(adjacency_matrix, vertices);

	if (unconnected_vertices.first == std::numeric_limits<int>::max())
	{
		if (vertices < current_best)
		{
			current_best = vertices;
			//std::cout << "the current chromatic number is: " << vertices << std::endl;
		}
		return vertices;
	}
	else
	{
		if (current_best == std::numeric_limits<int32_t>::max()  || current_best > selected_pooling_heuristics_(adjacency_matrix, vertices))
		{
			bool** contracted_adjacency_matrix = contract_v_into_u(adjacency_matrix, vertices, unconnected_vertices);

			int b = build_zykov_tree(contracted_adjacency_matrix, vertices - 1, current_best);
			
			for (int i = 0; i < vertices - 1; i++)
			{
				delete[]  contracted_adjacency_matrix[i];
			}
			delete[] contracted_adjacency_matrix;


			bool** added_edge_adjacency_matrix = new bool*[vertices]();
			for (int i = 0; i < vertices; i++)
			{
				added_edge_adjacency_matrix[i] = new bool[vertices]();
			}

			for (int i = 0; i < vertices; i++)
			{
				for (int j = 0; j < vertices; j++)
				{
					added_edge_adjacency_matrix[i][j] = adjacency_matrix[i][j];
					added_edge_adjacency_matrix[j][i] = adjacency_matrix[j][i];

				}
			}

			added_edge_adjacency_matrix[unconnected_vertices.first][unconnected_vertices.second] = true;
			added_edge_adjacency_matrix[unconnected_vertices.second][unconnected_vertices.first] = true;

			int a = build_zykov_tree(added_edge_adjacency_matrix, vertices, current_best);

			for (int i = 0; i < vertices; i++)
			{
				delete[]  added_edge_adjacency_matrix[i];
			}
			delete[] added_edge_adjacency_matrix;

			return (a < b ? a : b);
		}
		else
		{
			return current_best;
		}

	}

}

std::pair<int, int> graph::find_most_common_neighbors(bool** adjacency_matrix, const int vertices)
{
	int max_common_neighbors = 0;
	int u = 0;
	int v = 0;

	for (int i = 0; i < vertices; i++)
	{
		for (int j = i + 1; j < vertices; j++)
		{
			if (!adjacency_matrix[i][j])
			{
				const int tmp = get_number_of_common_neighbors(adjacency_matrix, vertices, i, j);

				if (tmp > max_common_neighbors)
				{
					max_common_neighbors = tmp;
					u = i;
					v = j;
				}
			}
		}
	}

	if (max_common_neighbors == 0)
	{
		return std::make_pair(std::numeric_limits<int>::max(),	std::numeric_limits<int>::max());;
	}

	return std::make_pair(u, v);


}

std::pair<int, int> graph::find_unconnected_vertex(bool ** adjacency_matrix, const int vertices)
{
	int u;
	int v;

	for (int i = 0; i < vertices; i++)
	{
		if (is_isolated(adjacency_matrix, i, vertices) == false)
		{
			for (int j = i + 1; j < vertices; j++)
			{
				if (i != j && adjacency_matrix[i][j] == false && is_isolated(adjacency_matrix, j, vertices) == false)
				{
					u = i;
					v = j;
					return std::make_pair(u, v);
				}
			}
		}
	}
	return std::make_pair(std::numeric_limits<int>::max(), std::numeric_limits<int>::max());;
}

int graph::get_number_of_common_neighbors(bool **adjacency_matrix, int vertices, int k, int l)
{
	int numberOfNeighbors = 0;
	for (int i = 0; i < vertices; i++)
	{
		if (adjacency_matrix[l][i] && adjacency_matrix[k][i]) numberOfNeighbors++;
	}

	return numberOfNeighbors;
}

bool graph::is_isolated(bool** adjacency_matrix, int u, int vertices)
{
	for (int i = 0; i < vertices; i++)
	{
		if (adjacency_matrix[u][i])  return false;
	}

	return true;
}

int graph::get_highest_degree_vertex(bool **adjacency_matrix, int vertices)
{
	int max_degree = -1;
	int u = 0;

	for (int i = 0; i < vertices; i++)
	{
		int degree = 0;

		for (int j = 0; j < vertices; j++)
		{
			if (adjacency_matrix[i][j]) degree++;
		}

		if (degree > max_degree)
		{
			u = i;
			max_degree = degree;
		}
	}
	if (max_degree == 0) return -1;

	return u;
}

bool** graph::contract_v_into_u(bool** adjacency_matrix,const int vertices, const std::pair<int, int> unconnected_vertices)
{
	int u = unconnected_vertices.first;
	int v = unconnected_vertices.second;

	bool** contracted_adjacency_matrix = new bool*[vertices - 1]();
	for (int i = 0; i < vertices - 1; i++)
	{
		contracted_adjacency_matrix[i] = new bool[vertices - 1]();
	}

	int x = 0;
	for (int i = 0; i < vertices; i++)
	{
		if (i != v)
		{
			int y = 0;
			for (int j = 0; j < vertices; j++)
			{
				if (j != v)
				{
					contracted_adjacency_matrix[x][y] = adjacency_matrix[i][j];
					contracted_adjacency_matrix[y][x] = adjacency_matrix[j][i];
					y++;
				}

			}
			x++;
		}
	}


	x = 0;
	for (int i = 0; i < vertices; i++)
	{
		if (i != v)
		{
			contracted_adjacency_matrix[u][x] = contracted_adjacency_matrix[x][u] = (adjacency_matrix[u][i] | adjacency_matrix[v][i]);
			x++;
		}

	}
	contracted_adjacency_matrix[u][u] = 0;

	return contracted_adjacency_matrix;
}

#pragma endregion

#pragma region lp_relaxation

int graph::lp_relaxation_coloring(bool **adjacency_matrix, const int vertices)
{
	glp_prob* prob = glp_create_prob();
	glp_set_obj_dir(prob, GLP_MIN); // minimize
	const int num_vertices = vertices;
	int max_colors = 0;
	for(int i = 0; i < vertices; i++)
	{
		int counter = 0;
		for(int j = 0; j < vertices; j++)
		{
			if(adjacency_matrix[i][j]) counter++;
		}
		if (counter > max_colors) max_colors = counter;
	}

	int y = glp_add_cols(prob, 1);
	glp_set_col_bnds(prob, y, GLP_DB, 1, max_colors); // DB = Double Bound
	glp_set_obj_coef(prob, y, 1.);
	glp_set_col_kind(prob, y, GLP_IV); // IV = Integer Variable

	std::vector<std::vector<int> > x(num_vertices, std::vector<int>(max_colors));
	for (int v = 0; v < num_vertices; ++v)
		for (int k = 0; k < max_colors; ++k)
		{
			x[v][k] = glp_add_cols(prob, 1);
			glp_set_col_kind(prob, x[v][k], GLP_BV); // BV = Binary Variable
		}

	std::vector<int> rows(1, 0);
	std::vector<int> cols(1, 0);
	std::vector<double> coeffs(1, 0.);

	// One vertex must have exactly one color:
	// for each vertex v, sum(x(v, k)) == 1
	for (int v = 0; v < num_vertices; ++v)
	{
		int row_idx = glp_add_rows(prob, 1);
		glp_set_row_bnds(prob, row_idx, GLP_FR, 1 ,1); // FX: FiXed bound
		for (int k = 0; k < max_colors; ++k)
		{
			rows.push_back(row_idx);
			coeffs.push_back(1);
			cols.push_back(x[v][k]);
		}
	}

	// We ensure we use y colors max:
	// for each vertex v and for each color c,                
	//    y >= (k + 1) * x(v, k)
	for (int v = 0; v < num_vertices; ++v)
	{
		for (int k = 0; k < max_colors; ++k)
		{
			int row_idx = glp_add_rows(prob, 1);
			glp_set_row_bnds(prob, row_idx, GLP_LO, 0, -1); // LO = LOwer bound

			rows.push_back(row_idx);
			coeffs.push_back(1);
			cols.push_back(y);

			rows.push_back(row_idx);
			coeffs.push_back(-k - 1);
			cols.push_back(x[v][k]);
		}
	}

	// Adjacent vertices cannot have the same color:        
	// for each edge (src, dst) and for each color k,                         
	//    x(src, k) + x(dst, k) <= 1                                          
	for (int src = 0; src < num_vertices; ++src)
	{
		for (int s = 0; s < num_vertices; ++s)
		{
			if (adjacency_matrix[src][s]) 
			{
				int dst = s;
				// Ensure we don't add both (u, v) and (v, u)                                    
				if (src > dst)
				{
					for (int k = 0; k < max_colors; ++k)
					{
						int row_idx = glp_add_rows(prob, 1);
						glp_set_row_bnds(prob, row_idx, GLP_UP, -1, 1); // UP = UPper bound

						rows.push_back(row_idx);
						coeffs.push_back(1);
						cols.push_back(x[src][k]);

						rows.push_back(row_idx);
						coeffs.push_back(1);
						cols.push_back(x[dst][k]);
					}
				}
			}
		}
	}

	glp_load_matrix(prob, rows.size() - 1, &rows[0], &cols[0], &coeffs[0]);
	glp_iocp parm;
	glp_init_iocp(&parm);
	//parm.msg_lev = GLP_MSG_ERR;
	parm.presolve = GLP_ON;
	glp_intopt(prob, &parm);

	double solution = glp_mip_obj_val(prob);
	std::cout << "Colors: " << solution << std::endl;
	//for (int i = 0; i < num_vertices; ++i)
	//{
	//	std::cout << i << ": ";
	//	for (int j = 0; j < max_colors; ++j)
	//		std::cout << glp_mip_col_val(prob, x[i][j]) << " ";
	//	std::cout << std::endl;
	//}

	return (int)ceil(solution);
}

#pragma endregion 

#pragma region WIP - Recursive-Largest-First

//int graph::recursive_largest_first(bool **adjacency_matrix, int vertices)
//{
//	int color_number = 0;
//	int u = 0;
//	bool** copy_of_adjacency_matrix = new bool*[vertices]();
//	for (int i = 0; i < vertices; i++)
//	{
//		copy_of_adjacency_matrix[i] = new bool[vertices]();
//		for (int j = 0; j < vertices; j++)
//		{
//			copy_of_adjacency_matrix[i][j] = adjacency_matrix[i][j];
//		}
//	}
//
//	while (vertices > 0)
//	{
//		color_number++;
//		copy_of_adjacency_matrix = swap_highest_degree_vertex_to_position_zero(copy_of_adjacency_matrix, vertices);
//
//		while (zero_vertex_is_complete(copy_of_adjacency_matrix, vertices) == false)
//		{
//			int max_common_neighbors = -1;
//			int y_degree = -1;
//			int tmp_degree_max = -1;
//			int selected_vertex;
//			int max_degree_vertex;
//
//			for (int i = 1; i < vertices; i++)
//			{
//				if (copy_of_adjacency_matrix[0][i] == 0)
//				{
//					int common_neighbors = get_number_of_common_neighbors(copy_of_adjacency_matrix, vertices, i);
//
//					int tmp_degree = get_degree(copy_of_adjacency_matrix, vertices, i);
//
//					if (tmp_degree > tmp_degree_max)
//					{
//						tmp_degree_max = tmp_degree;
//						max_degree_vertex = i;
//					}
//
//					if (common_neighbors > max_common_neighbors || (common_neighbors == max_common_neighbors && tmp_degree < y_degree))
//					{
//						selected_vertex = i;
//						y_degree = tmp_degree;
//						max_common_neighbors = common_neighbors;
//					}
//				}
//			}
//			if (max_common_neighbors == 0)
//			{
//				selected_vertex = max_degree_vertex;
//			}
//			copy_of_adjacency_matrix = contract_v_into_u(copy_of_adjacency_matrix, vertices, std::make_pair(0, selected_vertex));
//			--vertices;
//		}
//
//
//		copy_of_adjacency_matrix = delete_zero_vertex(copy_of_adjacency_matrix, vertices);
//		--vertices;
//	}
//	for (int i = 0; i < vertices; i++)
//	{
//		delete[] copy_of_adjacency_matrix[i];
//	}
//	delete[] copy_of_adjacency_matrix;
//
//	return color_number;
//}
//
//bool** graph::swap_highest_degree_vertex_to_position_zero(bool** adjacency_matrix, int vertices)
//{
//	int highest_degree_vertex = 0;
//	int max_degree = -1;
//
//
//	for (int i = 0; i < vertices; i++)
//	{
//		int degree = 0;
//		for (int j = 0; j < vertices; j++)
//		{
//			if (adjacency_matrix[i][j] == 1) degree++;
//		}
//		if (degree > max_degree)
//		{
//			highest_degree_vertex = i;
//			max_degree = degree;
//		}
//	}
//	if (highest_degree_vertex == 0)
//	{
//		return adjacency_matrix;
//	}
//
//	bool tmp;
//	for (int i = 0; i < vertices; i++)
//	{
//		tmp = adjacency_matrix[0][i];
//		adjacency_matrix[0][i] = adjacency_matrix[i][0] = adjacency_matrix[highest_degree_vertex][i];
//		adjacency_matrix[i][highest_degree_vertex] = adjacency_matrix[highest_degree_vertex][i] = tmp;
//	}
//
//
//	adjacency_matrix[0][highest_degree_vertex] = adjacency_matrix[highest_degree_vertex][0] = adjacency_matrix[0][0];
//	adjacency_matrix[0][0] = 0;
//	adjacency_matrix[0][highest_degree_vertex] = adjacency_matrix[highest_degree_vertex][0] = adjacency_matrix[highest_degree_vertex][highest_degree_vertex];
//	adjacency_matrix[highest_degree_vertex][highest_degree_vertex] = 0;
//
//	return adjacency_matrix;
//}
//
//bool graph::zero_vertex_is_complete(bool **adjacency_matrix, int vertices)
//{
//	for (int i = 1; i < vertices; i++)
//	{
//		if (adjacency_matrix[0][i] == 0) return false;
//	}
//	return true;
//}
//
//int graph::get_number_of_common_neighbors(bool **adjacency_matrix, int vertices, int k)
//{
//	int number_of_neighbors = 0;
//	for (int i = 0; i < vertices; i++)
//	{
//		if (adjacency_matrix[0][i] == 1 && adjacency_matrix[k][i] == 1) number_of_neighbors++;
//	}
//
//	return number_of_neighbors;
//}
//
//int graph::get_degree(bool **adjacency_matrix, int vertices, int k)
//{
//	int degree = 0;
//	for (int i = 0; i < vertices; i++)
//	{
//		if (adjacency_matrix[k][i] == 1) degree++;
//	}
//
//	return degree;
//}
//
//bool** graph::delete_zero_vertex(bool**adjacency_matrix, int vertices)
//{
//	bool** contracted_adjacency_matrix = new bool*[vertices - 1]();
//
//	for (int i = 0; i < vertices - 1; i++) 
//	{
//		contracted_adjacency_matrix[i] = new bool[vertices - 1]();
//	}
//
//	for (int i = 1; i < vertices - 1; i++) 
//	{
//		for (int j = 1; j < vertices - 1; j++) 
//		{
//			contracted_adjacency_matrix[i - 1][j - 1] = adjacency_matrix[i][j];
//			contracted_adjacency_matrix[j - 1][i - 1] = adjacency_matrix[i][j];
//		}
//	}
//
//	return contracted_adjacency_matrix;
//
//}

#pragma endregion