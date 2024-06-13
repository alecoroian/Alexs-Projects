#include <iostream>
#include <fstream>
#include <sstream>
#include <bits/stdc++.h>

using namespace std;
int num_vertices;
long INF = -1;

long len(long num) // returns the length of a long number
{
    return std::to_string(num).length();
}

bool isSingleCharVertex(const string &vertex, char maxVertex)
{
    return vertex.length() == 1 && vertex[0] >= 'A' && vertex[0] <= maxVertex;
}

bool isPositiveInteger(const string &s)
{
    return !s.empty() && s.find_first_not_of("0123456789") == string::npos && s[0] != '0';
}

bool errorCheck(const string &line, int line_number)
{
    const int minLineLength = 5;
    if (line.length() < minLineLength)
    {
        cerr << "Error: Invalid edge data '" << line << "' on line " << line_number << "." << endl;
        return false;
    }

    char maxVertex = 'A' + num_vertices - 1;

    string startingVertex = line.substr(0, line.find(' '));
    string endingVertex = line.substr(line.find(' ') + 1, line.rfind(' ') - line.find(' ') - 1);
    string weight = line.substr(line.rfind(' ') + 1);

    if (!isSingleCharVertex(startingVertex, maxVertex)) // checks the vertices
    {
        cerr << "Error: Starting vertex '" << startingVertex << "' on line " << line_number << " is not among valid values A-" << maxVertex << "." << endl;
        return false;
    }

    if (!isSingleCharVertex(endingVertex, maxVertex))
    {
        cerr << "Error: Ending vertex '" << endingVertex << "' on line " << line_number << " is not among valid values A-" << maxVertex << "." << endl;
        return false;
    }

    if (!isPositiveInteger(weight)) // checks the edge weight
    {
        cerr << "Error: Invalid edge weight '" << weight << "' on line " << line_number << "." << endl;
        return false;
    }

    return true;
}

void addLine(long **matrix, const string &line) // adds the line to the matrix
{
    if (line.size() < 5 || line[1] != ' ' || line[3] != ' ') // makes sure the value is in the expected range
    {
        return;
    }

    int vertex1 = line[0] - 'A';
    int vertex2 = line[2] - 'A';

    string distance_str = line.substr(4);
    long distance = 0;
    for (char digit : distance_str)
    {
        if (isdigit(digit))
        {
            distance = distance * 10 + (digit - '0');
        }
    }

    matrix[vertex1][vertex2] = distance;
}

void floyd(long **graph, long **pathTrackers) // logic of flloyd's algorithm
{
    for (int mid = 0; mid < num_vertices; mid++)
    {
        for (int src = 0; src < num_vertices; src++)
        {
            for (int dest = 0; dest < num_vertices; dest++)
            {
                if (src != mid && dest != mid && src != dest)
                {
                    long directPath = graph[src][dest];
                    long newPath = graph[src][mid] + graph[mid][dest];

                    if (graph[src][mid] != INF && graph[mid][dest] != INF && (directPath > newPath || directPath == INF))
                    {
                        graph[src][dest] = newPath;
                        pathTrackers[src][dest] = mid;
                    }
                }
            }
        }
    }
}

void display_table(long **const matrix, const string &label, const bool use_letters = false) // displays the table
{
    cout << label << endl;
    long max_val = 0;

    for (int i = 0; i < num_vertices; i++)
    {
        for (int j = 0; j < num_vertices; j++)
        {
            long cell = matrix[i][j];

            if (cell != INF && cell > max_val)
            {
                max_val = matrix[i][j];
            }
        }
    }

    int max_cell_width = use_letters ? len(max_val) : len(max(static_cast<long>(num_vertices), max_val));
    cout << ' ';

    for (int j = 0; j < num_vertices; j++)
    {
        cout << setw(max_cell_width + 1) << static_cast<char>(j + 'A');
    }

    cout << endl;

    for (int i = 0; i < num_vertices; i++)
    {
        cout << static_cast<char>(i + 'A');

        for (int j = 0; j < num_vertices; j++)
        {
            cout << " " << setw(max_cell_width);

            if (matrix[i][j] == INF)
            {
                cout << "-";
            }

            else if (use_letters)
            {
                cout << static_cast<char>(matrix[i][j] + 'A');
            }

            else
            {
                cout << matrix[i][j];
            }
        }

        cout << endl;
    }

    cout << endl;
}

string backtrack(long **const distances, long **const paths, int start, int end, string route)
{
    if (start == end)
    {
        return route;
    }

    if (paths[start][end] == INF)
    {
        string destination = string(1, 'A' + end);

        if (route.empty())
        {
            route = destination;
        }

        else
        {
            route += " -> " + destination;
        }

        return route;
    }

    int midPoint = paths[start][end];

    // recursive calls
    string firstHalf = backtrack(distances, paths, start, midPoint, route);
    string secondHalf = backtrack(distances, paths, midPoint, end, "");

    return firstHalf + (!firstHalf.empty() && !secondHalf.empty() ? " -> " : "") + secondHalf;
}

void display_paths(long **const dist_matrix, long **const path_matrix) // displays the paths
{
    for (int from = 0; from < num_vertices; ++from)
    {
        char start_vertex = 'A' + from;

        for (int to = 0; to < num_vertices; ++to)
        {
            long path_distance = dist_matrix[from][to];
            char end_vertex = 'A' + to;

            cout << start_vertex << " -> " << end_vertex;

            if (path_distance == INF)
            {
                cout << ", distance: infinity, path: none" << endl;
            }

            else
            {
                string path = backtrack(dist_matrix, path_matrix, from, to, string(1, start_vertex));
                cout << ", distance: " << path_distance << ", path: " << path << endl;
            }
        }
    }
}

void deleteMatrix(long **matrix, long **intermediates)
{
    for (int i = 0; i < num_vertices; i++)
    {
        delete[] matrix[i];
        delete[] intermediates[i];
    }

    delete[] matrix;
    delete[] intermediates;
}

int main(int argc, const char *argv[])
{
    if (argc != 2)
    {
        cerr << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    ifstream inputFile(argv[1]);

    if (!inputFile)
    {
        cerr << "Error: Cannot open file '" << argv[1] << "'.\n";
        return 1;
    }

    inputFile.exceptions(ifstream::badbit);
    string currentLine;
    unsigned int lineCount = 1;

    try
    {
        getline(inputFile, currentLine);
        istringstream iss(currentLine);

        if (!(iss >> num_vertices) || num_vertices < 1 || num_vertices > 26)
        {
            cerr << "Error: Invalid number of vertices '" << currentLine << "' on line " << lineCount << ".\n";

            return 1;
        }

        long **distanceMatrix = new long *[num_vertices];
        long **pathMatrix = new long *[num_vertices];

        for (int i = 0; i < num_vertices; i++)
        {
            distanceMatrix[i] = new long[num_vertices];
            pathMatrix[i] = new long[num_vertices];

            for (int j = 0; j < num_vertices; j++)
            {
                distanceMatrix[i][j] = INF;
                pathMatrix[i][j] = INF;
            }

            distanceMatrix[i][i] = 0;
        }

        lineCount++;

        while (getline(inputFile, currentLine))
        {
            if (!errorCheck(currentLine, lineCount))
            {
                deleteMatrix(distanceMatrix, pathMatrix);
                return 1;
            }

            addLine(distanceMatrix, currentLine);
            lineCount++;
        }

        display_table(distanceMatrix, "Distance matrix:");
        floyd(distanceMatrix, pathMatrix);

        display_table(distanceMatrix, "Path lengths:");
        display_table(pathMatrix, "Intermediate vertices:", true);

        display_paths(distanceMatrix, pathMatrix);

        deleteMatrix(distanceMatrix, pathMatrix);
        inputFile.close();
    }

    catch (const ifstream::failure &e)
    {
        cerr << "Error: An issue occurred while processing the file '" << argv[1] << "'.\n";
        return 1;
    }

    return 0;
}
