#include <iostream>
#include <ctime>
#include <unordered_set>
#include <string>
#include <vector>

using namespace std;

string pack(int x, int y) {
	/* Packs coordinates into a string*/
	return (to_string(x) + " " + to_string(y));
}

void unpack(string to_unpack, int &a, int &b) {
	/* unpacks coordinates into 2 integers*/
	b = stoi(to_unpack.substr(to_unpack.find(' '), to_unpack.size() - 1));
	a = stoi(to_unpack.substr(0, to_unpack.size()));
}

void cell_check(int arr[][41], int arr_width, int arr_height, int cell_x, int cell_y, int response[], int dist, int equal) {
	/* Checks the 4 cardinal directions relative to cell_x,cell_y and puts the response in response[] starting at N = 0 and moving in a clockwise direction. */
	if (0 <= cell_y - dist && arr[cell_x][cell_y - dist] == equal) response[0]++;
	if (arr_width > cell_x + dist && arr[cell_x + dist][cell_y] == equal) response[1]++;
	if (arr_height > cell_y + dist && arr[cell_x][cell_y + dist] == equal) response[2]++;
	if (0 <= cell_x - dist && arr[cell_x - dist][cell_y] == equal) response[3]++;
}

int main() {
	srand(time(NULL));
	// Initialize maze size and map_empty bool
	const int x_size = 51;
	const int y_size = 41;
	bool is_empty = false;
	// Initialize maze, queue, and set of unvisited cells.
	int maze[x_size][y_size];
	int queue[4]{ 0 };
	unordered_set <string> empty;
	vector <int> maze_heads; // Stored as x, and then y
	vector <int> neighbors;
	// Initialize pair that will be used to explore.
	maze_heads.push_back(1);
	maze_heads.push_back(1);
	maze[1][1] = 0; // This is normally done in the loop when moved, but as it is the first one it is not.

	// Fill the maze with walls
	fill_n(&maze[0][0], x_size * y_size, 1);
		
	//Explore Maze
	while (!is_empty) {
		// Get Length of mazeheads
		int vector_size;
		if (maze_heads.empty()) {
			vector_size = 0;
		}
		else {
			vector_size = maze_heads.size();
		}
		// If dead end is reached make a new maze head
		if (vector_size == 0 && !empty.empty()) {
			// Get New maze head
			int x;
			int y;
			unpack(*empty.begin(), x, y);
			empty.erase(empty.begin());
			maze_heads.push_back(x);
			maze_heads.push_back(y);

			// Get Neighboring Cells that are paths
			cell_check(maze, x_size, y_size, x, y, queue, 2, 0);

			// Add neighbors to queue
			for (int j = 0; j < 4; j++) {
				if (queue[j] > 0) {
					queue[j] = 0;
					if (j == 0) {
						neighbors.push_back(x);
						neighbors.push_back(y - 2);
					}
					else if (j == 1) {
						neighbors.push_back(x + 2);
						neighbors.push_back(y);
					}
					else if (j == 2) {
						neighbors.push_back(x);
						neighbors.push_back(y + 2);
					}
					else if (j == 3) {
						neighbors.push_back(x - 2);
						neighbors.push_back(y);
					}
				}
			}

			// Pick a random neighbor
			int index = rand() % (neighbors.size() / 2) * 2;
			int temp_x = neighbors[index];
			int temp_y = neighbors[index + 1];

			// Modify Maze to connect new path to existing paths
			maze[(x + temp_x) / 2][(y + temp_y) / 2] = 0;

			// Kill neighbors
			while (!neighbors.empty()) {
				neighbors.pop_back();
			}

		}
		// Don't loop if there's no positions left to occupy
		else if (vector_size == 0) {
			is_empty = true;
		}
		// Make the maze if there are empty positions left.
		for (int i = 0; i < vector_size && !maze_heads.empty(); i += 2) {

			// Get Neighboring Empty Cells
			cell_check(maze, x_size, y_size, maze_heads[i], maze_heads[i + 1], queue, 2, 1);
			
			// Add all neighboring cells of first cell to neighbors
			for (int j = 0; j < 4; j++) {
				if (queue[j] > 0) {
					queue[j] = 0;
					if (j == 0) {
						neighbors.push_back(maze_heads[i]);
						neighbors.push_back(maze_heads[i+1] - 2);
					} else if (j == 1) {
						neighbors.push_back(maze_heads[i] + 2);
						neighbors.push_back(maze_heads[i + 1]);
					} else if (j == 2) {
						neighbors.push_back(maze_heads[i]);
						neighbors.push_back(maze_heads[i + 1] + 2);
					} else if (j == 3) {
						neighbors.push_back(maze_heads[i] - 2);
						neighbors.push_back(maze_heads[i + 1]);
					}
				}
			}
			// Clear current position.
			maze[maze_heads[i]][maze_heads[i + 1]] = 0;

			// Move to next position, if it can't, destroy itself.
			if (!neighbors.empty()) {
				// Pick a random neighbor
				int index = rand() % (neighbors.size() / 2) * 2;
				int x = neighbors[index];
				int y = neighbors[index + 1];
				neighbors.erase(neighbors.begin() + index);
				neighbors.erase(neighbors.begin() + index);

				// Modify Maze
				maze[(maze_heads[i] + x) / 2][(maze_heads[i + 1] + y) / 2] = 0;
				maze[x][y] = 0;

				// Update Head Position
				maze_heads[i] = x;
				maze_heads[i + 1] = y;

				// Erase new position from empty
				empty.erase(pack(x, y));

				// Put rest of neighbors in queue
				while (!neighbors.empty()) {
					int y_neighbor = neighbors.back();
					neighbors.pop_back();
					empty.insert(pack(neighbors.back(), y_neighbor));
					neighbors.pop_back();
				}
			}
			else {
				// Clear maze head if it is at a dead end.
				maze_heads.clear();
			}
		}
	}
	// Place Enterance
	int random_num = rand() % 4;
	// Horizontal Left
	if (random_num == 0) maze[0][rand() % (y_size / 2) * 2 + 1] = 2;
	// Horizontal Right
	else if (random_num == 1) maze[x_size-1][rand() % (y_size / 2) * 2 + 1] = 2;
	// Vertical Top
	else if (random_num == 2) maze[rand() % (x_size / 2) * 2 + 1][0] = 2;
	// Vertical Bottom
	else maze[rand() % (x_size / 2) * 2 + 1][y_size - 1] = 2;

	// Place Exit
	// Horizontal Left
	if (random_num == 1) maze[0][rand() % (y_size / 2) * 2 + 1] = 3;
	// Horizontal Right
	else if (random_num == 0) maze[x_size - 1][rand() % (y_size / 2) * 2 + 1] = 3;
	// Vertical Top
	else if (random_num == 3) maze[rand() % (x_size / 2) * 2 + 1][0] = 3;
	// Vertical Bottom
	else maze[rand() % (x_size / 2) * 2 + 1][y_size - 1] = 3;

	// Outputs the maze in pretty colors.
	for (int y = 0; y < y_size; y++) {
		for (int x = 0; x < x_size; x++) {
			
			if (maze[x][y] == 0) cout << "\033[47;35m  \033[0m"; // Pathways
			else if (maze[x][y] == 1) cout << "\033[100;90m #\033[0m"; // Walls
			else if (maze[x][y] == 2) cout << "\033[42;32m #\033[0m"; // Enterance
			else if (maze[x][y] == 3) cout << "\033[101;91m #\033[0m"; // Exit
		}
		cout << "\033[40;30m.\033[0m" << endl; // Allows for console to be resized.
	}

	return 0;
}