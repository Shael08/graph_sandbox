#pragma once
#include <string>
#include <sstream>
#include <vector>


namespace patch
{
	template < typename T > std::string to_string(const T& n)
	{
		std::ostringstream stm;
		stm << n;
		return stm.str();
	}
}

class graph
{
private:

	static int get_vertices(char* file_name);

	bool** get_edges(char* file_name) const;

#pragma region zykov_tree

	std::pair<int, int>(*selected_vertices_selecting_heuristics_)(bool**, const int);

	int(*selected_pooling_heuristics_)(bool**, const int);

	int build_zykov_tree(bool** adjacency_matrix, int vertices, int &current_best) const;

	static std::pair<int, int> find_most_common_neighbors(bool** adjacency_matrix, int vertices);

	static std::pair<int, int> find_unconnected_vertex(bool** adjacency_matrix, int vertices);

	static int get_number_of_common_neighbors(bool ** adjacency_matrix, int vertices, int k, int l);

	static bool is_isolated(bool ** adjacency_matrix, int u, int vertices);

	static int get_highest_degree_vertex(bool ** adjacency_matrix, int vertices);

	static bool ** contract_v_into_u(bool ** adjacency_matrix, int size, std::pair<int, int> unconnected_vertices);

#pragma endregion 

#pragma region clique_finding

	static int find_clique(bool** adjacency_matrix, const int vertices);

#pragma endregion 

#pragma region lp_relaxation

	static int lp_relaxation_coloring(bool ** adjacency_matrix, const int vertices);

#pragma endregion 

#pragma region recursive_largest_first

	//static int recursive_largest_first(bool ** adjacency_matrix, int vertices);

	//static bool ** swap_highest_degree_vertex_to_position_zero(bool ** adjacency_matrix, int vertices);

	//static bool zero_vertex_is_complete(bool ** adjacency_matrix, int vertices);

	//static int get_number_of_common_neighbors(bool ** adjacency_matrix, int vertices, int k);

	//static int get_degree(bool ** adjacency_matrix, int vertices, int k);

	//static bool ** delete_zero_vertex(bool ** adjacency_matrix, int vertices);

#pragma endregion 

#pragma region iterated_greedy_coloring

	int recolor_color_class(std::vector<std::vector<int> > &color_classes) const;

	static bool sort_increasing_size(const std::vector<int> &color_classes_1, const std::vector<int> &color_classes_2);

	static bool sort_decreasing_size(const std::vector<int> &color_classes_1, const std::vector<int> &color_classes_2);

	void sort_increasing_degree(std::vector<std::vector<int> > &color_classes) const;

	void sort_decreasing_degree(std::vector<std::vector<int> > &color_classes) const;

	static void sort_random_order(std::vector<std::vector<int>>& color_classes);

	int get_color_class_degree( std::vector<int> &color_class) const;

#pragma endregion 

	static int dsatur(bool ** adjacency_matrix, const int vertices);

	static int find_vertex_with_maximal_color_degree(bool ** adjacency_matrix, const int vertices, std::vector<std::vector<int> > color_class, const int* result);

	std::vector<int> sort_graph(bool ** adjacency_matrix, const int vertices);



protected:

	int vertices_;

	bool** adjacency_matrix_;

	bool is_any_self_loop() const;

	bool is_symmetric() const;

	static int greedy_coloring(bool** adjacency_matrix, int coloring_until);

public:

	enum zykov_pooling_heuristic
	{
		none = 0,
		clique = 1,
		lp_relaxation = 2
	};

	enum zykov_selecting_vertices_heuristics
	{
		naive = 0,
		most_common_neighbors = 1
	};

	graph();
	graph(int vertices,bool** adjacency_matrix);
	explicit graph(char * file);
	virtual ~graph();

	graph(const graph &) = delete;
	graph(graph&&) = delete;
	graph& operator =(graph const&) = delete;
	graph& operator=(graph&&) = delete;

	bool** get_copy_of_adjacency_matrix() const;
	int get_number_of_vertices() const;
	void print_graph() const;

	std::vector<std::vector<int>> find_all_clique(bool in_complementary, unsigned minimum_clique_size) const;

	void init_greedy_coloring() const;

	void iterated_greedy_coloring() const;

	void init_zykov_tree(zykov_pooling_heuristic pooling, zykov_selecting_vertices_heuristics choosing_vertices);

	void init_dsatur();

};
