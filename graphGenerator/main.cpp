#include <iostream>
#include <fstream>
#include <random>
#include <set>
#include <string>
#include <vector>

// Function to generate a random graph with n nodes and m edges
void generateRandomGraph(int numNodes, int numEdges, const std::string &filename, int minWeight = 1, int maxWeight = 100)
{
  // Check if the requested number of edges is possible
  int maxPossibleEdges = numNodes * (numNodes - 1);
  if (numEdges > maxPossibleEdges)
  {
    std::cerr << "Error: Too many edges requested. Maximum possible is " << maxPossibleEdges << " for " << numNodes << " nodes." << std::endl;
    return;
  }

  // Initialize random number generators
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> nodeDist(0, numNodes - 1);
  std::uniform_int_distribution<> weightDist(minWeight, maxWeight);

  // Set to keep track of edges to avoid duplicates
  std::set<std::pair<int, int>> edgeSet;

  // Open the output file
  std::ofstream outFile(filename);
  if (!outFile.is_open())
  {
    std::cerr << "Error: Could not open file " << filename << " for writing." << std::endl;
    return;
  }

  // Generate random edges
  while (edgeSet.size() < numEdges)
  {
    int u = nodeDist(gen);
    int v = nodeDist(gen);

    // Avoid self-loops
    if (u == v)
    {
      continue;
    }

    // Check if this edge already exists
    auto edge = std::make_pair(u, v);
    if (edgeSet.find(edge) != edgeSet.end())
    {
      continue;
    }

    // Generate random weight
    int w = weightDist(gen);

    // Add edge to the set and write to file
    edgeSet.insert(edge);
    outFile << u << "," << v << "," << w << std::endl;
  }

  outFile.close();
  std::cout << "Successfully generated random graph with " << numNodes << " nodes and " << numEdges << " edges." << std::endl;
  std::cout << "Output saved to " << filename << std::endl;
}

int main()
{
  int numNodes, numEdges, minWeight, maxWeight;
  std::string filename;

  // Get user input
  std::cout << "Enter the number of nodes: ";
  std::cin >> numNodes;

  std::cout << "Enter the number of edges: ";
  std::cin >> numEdges;

  std::cout << "Enter minimum edge weight: ";
  std::cin >> minWeight;

  std::cout << "Enter maximum edge weight: ";
  std::cin >> maxWeight;

  std::cout << "Enter output filename: ";
  std::cin >> filename;

  // Generate the graph
  generateRandomGraph(numNodes, numEdges, filename, minWeight, maxWeight);

  return 0;
}