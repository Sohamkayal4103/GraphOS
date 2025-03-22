# GraphOS - Privacy-Preserving Graph Processing using Hardware Enclaves

This repository contains my implementation of a privacy-preserving graph processing system using hardware enclaves. The system is designed to run common graph algorithms (BFS, DFS, Dijkstra) in a way that hides both data and memory access patterns from an adversary.

## Overview

This project builds on concepts from Path ORAM and oblivious data structures to protect sensitive graph information. Key features include:

- **Graph-to-Key-Value Transformation:**  
  Vertices are represented as key-value pairs (e.g., `"V1": {inDegree, outDegree}`) and edges as keys (e.g., `"Eout1_1": {neighbor, weight}`). These pairs are obliviously sorted and used to construct an AVL tree.

- **AVL-based Doubly-Oblivious Map (DOMAP):**  
  The AVL tree stores nodes that are then assigned random positions in an ORAM tree. Each node is encrypted using AES-CBC (via Crypto++) after serialization with Boost.Serialization. Nodes are fetched and updated in an enclave stash using a path-by-path eviction strategy.

- **Oblivious Eviction and Re-Mapping:**  
  On each access, the relevant ORAM path is fetched into a secure enclave, decrypted, updated (e.g., reassigning new random positions), and re-encrypted before writing back. This hides the physical location of the accessed node.

- **Oblivious Priority Queue for Dijkstra:**  
  I implemented an oblivious priority queue based on the approach described in [Path Oblivious Heap: Optimal and Practical Oblivious Priority Queue](https://doi.org/10.1109/SP40000.2020.00037) by E. Shi, which uses the subtree-min concept to securely extract minimum elements.

- **Hardware Enclave Integration:**  
  While the system is designed to run within Intel SGX enclaves, due to build issues with the SGX SDK in my environment, the final SGX wrappers were removed before submission. The design remains enclave-ready.

## Repository Structure (Main files used)

- `graphos/Graph_Processing.h/cpp`: Contains the graph algorithm implementations (BFS, DFS, Dijkstra) that work with the oblivious DOMAP.
- `OmixPP/domap.h/cpp`: Implements the Doubly-Oblivious Map (DOMAP) module, including AVL tree construction, ORAM tree management, and path-by-path eviction.
- `OmixPP/node.h/cpp`: Defines the node structure used in the AVL tree and ORAM tree.
- `OmixPP/helper.h/cpp`: Provides utility functions including oblivious sorting (bitonic sort), random number generation, etc.
- `priority_queue/Priority_queue.h/cpp`: Implements the oblivious priority queue for Dijkstra’s algorithm.
- `graphGenerator/main.cpp`: To generate random graphs of given size and edges

## How to Build

The project uses a Makefile for compilation. Ensure you have the necessary dependencies installed:
- [Boost](https://www.boost.org/) (for Boost.Serialization)
- [Crypto++](https://www.cryptopp.com/) (for AES-CBC encryption)
- A C++ compiler that supports C++17 (e.g., `g++`)

To compile the project, run:
```bash
cd graphos/
make clean
make
```
## How to Run

After building the project (using `make`), run the executable with:

```bash
./graphos_executable
```
The program provides an interactive menu:

- Select between a Single Test and a Benchmark Simulation.
- In Single Test mode, choose which algorithm (BFS, DFS, or Dijkstra) to run on a sample graph.

## GraphOS Paper

The design and certain aspects of this project are inspired by the GraphOS paper:  
> Javad Ghareh Chamani, Ioannis Demertzis, Dimitrios Papadopoulos, Charalampos Papamanthou, and Rasool Jalili. *GraphOS: Towards Oblivious Graph Processing.* Cryptology ePrint Archive, Paper 2024/642, 2024. [https://eprint.iacr.org/2024/642](https://eprint.iacr.org/2024/642)

## Acknowledgments

This project builds upon work from:
- [Path ORAM: An Extremely Simple Oblivious RAM Protocol](http://arxiv.org/abs/1202.5150) by Emil Stefanov and Elaine Shi.
- [Oblivious Data Structures](https://doi.org/10.1145/2660267.2660314) by Xiao Shaun Wang et al.
- [Path Oblivious Heap: Optimal and Practical Oblivious Priority Queue](https://doi.org/10.1109/SP40000.2020.00037) by E. Shi.

## Future Work

- **Hardware Enclave Wrappers:** Re-integrate the Intel SGX SDK wrappers once build issues are resolved.
- **Additional Graph Algorithms:** Extend the system to support further graph algorithms such as topological sort, MST (Prim's/Kruskal's), and network flow algorithms.
- **Performance Optimizations:** Further optimize oblivious data structures and the ORAM eviction strategy to reduce overhead.
