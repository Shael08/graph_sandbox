#include "pch.h"
#include "graph.h"
#include <sstream>
#include <fstream>
#include <iostream>

graph::graph()
{
	vertices_ = 0;
	adjacency_matrix_ = nullptr;
}

graph::graph(char * file)
{
	vertices_ = get_vertices(file);
	this->adjacency_matrix_ = get_edges(file);
}

graph::graph(const int vertices, bool ** adjacency_matrix)
{
	this->vertices_ = vertices;
	this->adjacency_matrix_ = new bool*[vertices]();

	for (int i = 0; i < vertices; i++) {

		this->adjacency_matrix_[i] = new bool[vertices]();
		for(int j = 0; j < vertices; j++)
		{
			adjacency_matrix_[i][j] = adjacency_matrix[i][j];
		}
	}

}

graph::~graph()
{
	for (int i = 0; i < vertices_; i++)
	{
		delete[] adjacency_matrix_[i];
	}
	delete[] adjacency_matrix_;
}

int graph::get_vertices(char* file_name)
{
	std::ifstream file;
	file.open(file_name);

	if (file.is_open()) {
		std::string mark;

		while (file >> mark) {

			if (mark == "p") {
				std::string tmp;
				int v;
				int e;
				file >> tmp >> v >> e;
				file.close();

				return v;
			}
		}

	}

	exit(2);	
}

bool ** graph::get_edges(char* file_name) const
{
	bool** adjacency_matrix = new bool*[vertices_]();

	for (int i = 0; i < vertices_; i++) {

		adjacency_matrix[i] = new bool[vertices_]();

	}

	std::ifstream file;
	file.open(file_name);

	if (file.is_open()) {
		std::string mark;
		int i, j;

		while (file >> mark) {

			if (mark == "e") {
				file >> i >> j;
				adjacency_matrix[i - 1][j - 1] = 1;
				adjacency_matrix[j - 1][i - 1] = 1;
			}
		}
		file.close();
		return adjacency_matrix;

	}

	exit(3);
	
}

bool graph::is_any_self_loop() const
{
	for (int i = 0; i < vertices_; i++)
	{
		if (adjacency_matrix_[i][i] == 1) return true;
	}

	return false;
}

bool graph::is_symmetric() const
{
	for (int i = 0; i < vertices_; i++)
	{
		for (int j = 0; j < vertices_; j++)
		{
			if (adjacency_matrix_[i][j] != adjacency_matrix_[j][i]) return false;
		}
	}
	return true;
}

void graph::print_graph() const
{
	for (int i = 0; i < vertices_; i++)
	{
		for (int j = 0; j < vertices_; j++)
		{
			std::cout << adjacency_matrix_[i][j] << " ";

		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

bool ** graph::get_copy_of_adjacency_matrix() const
{
	bool** copy_of_adjacency_matrix = new bool*[vertices_]();
	for (int i = 0; i < vertices_; i++)
	{
		copy_of_adjacency_matrix[i] = new bool[vertices_]();
		for (int j = 0; j < vertices_; j++)
		{
			copy_of_adjacency_matrix[i][j] = adjacency_matrix_[i][j];
		}
	}

	return copy_of_adjacency_matrix;
}

int graph::get_number_of_vertices() const
{
	return vertices_;
}
