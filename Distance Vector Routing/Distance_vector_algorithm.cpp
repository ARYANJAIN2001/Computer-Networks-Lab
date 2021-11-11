#include <iostream>
#include <bits/stdc++.h>

using namespace std;

// Maximum number of nodes
// Time complexity of the bellman-ford is n*n
#define N 1000
#define INF 1e7 //INF is used for representing node which are not in direct connection with the current node in the cost matrix.

// In distance vector routing, each node has a distance vector which has smallest distance to each node, along with the outgoing link
// SO, we'll need to have a struct to store this information for each node
typedef struct
{
    int outgoing_link[N];
    int distance[N];
} node;

vector<int> cost_matrix[N];
node nodes[N];

void clear_cost_matrix(int number_of_nodes)
{

    for (int i = 0; i < number_of_nodes; i++)
    {
        // Clearing the vector, and then resizing it according to our nodes
        cost_matrix[i].clear();
        cost_matrix[i].resize(number_of_nodes);
    }
}

void get_cost_matrix_from_user(int number_of_nodes)
{

    // Please enter the cost matrix! If there is no edge, enter -1

    for (int i = 0; i < number_of_nodes; i++)
    {
        for (int j = 0; j < number_of_nodes; j++)
        {
            int cost_i_j;
            cin >> cost_i_j;
            if (cost_i_j == -1)
                cost_matrix[i][j] = INF;
            else
                cost_matrix[i][j] = cost_i_j;
        }
    }

    // Initialise the distance vectors, for feeding to the bellman-ford algorithm
    for (int i = 0; i < number_of_nodes; i++)
    {
        cost_matrix[i][i] = 0;
        for (int j = 0; j < number_of_nodes; j++)
        {
            if(cost_matrix[i][j] == INF) nodes[i].outgoing_link[j] = -1;
            else nodes[i].outgoing_link[j] = j;
            nodes[i].distance[j] = cost_matrix[i][j];
        }
    }
}

void fill_routing_table(int number_of_nodes)
{

    int continue_flag = true;

    while (continue_flag)
    {
        continue_flag = false;
        for (int i = 0; i < number_of_nodes; i++)
        {
            for (int j = 0; j < number_of_nodes; j++)
            {
                for (int k = 0; k < number_of_nodes; k++)
                {
                    if (cost_matrix[i][k] != INF)
                    {
                        // chosing nodes which are neighbour of ith node
                        int distance_from_i_to_j_via_k = cost_matrix[i][k] + nodes[k].distance[j];
                        int current_distance_from_i_to_j = nodes[i].distance[j];
                        if (current_distance_from_i_to_j > distance_from_i_to_j_via_k)
                        {
                            continue_flag = true;
                            nodes[i].distance[j] = nodes[i].distance[k] + nodes[k].distance[j];
                            nodes[i].outgoing_link[j] = k;
                        }
                    }
                }
            }
        }
    }
}

void print_routing_table(int number_of_nodes)
{

    cout << "Printing the routing table!" << endl;

    for (int i = 0; i < number_of_nodes; i++)
    {
        cout << "For Router " << i + 1 << endl;
        for (int j = 0; j < number_of_nodes; j++)
        {
            if (nodes[i].outgoing_link[j] == -1)
                cout << "No path exists to " << j + 1 << "." << endl;
            else
                cout << "To node " << j + 1 << ". Distance is " << nodes[i].distance[j] << " from the outgoing link " << nodes[i].outgoing_link[j] + 1 << endl;
        }
    }
}

int main()
{

    freopen("test_cases/testcases.txt", "r", stdin);

    int testcases = 0;
    cin >> testcases;
    for(int i = 1; i <= testcases; i++)
    {
        int number_of_nodes;
        // Please enter the number of nodes(less than 1000):
        cin >> number_of_nodes;

        clear_cost_matrix(number_of_nodes);
        get_cost_matrix_from_user(number_of_nodes);
        fill_routing_table(number_of_nodes);
        cout<<"Testcase - "<<i<<endl;
        print_routing_table(number_of_nodes);
        cout<<"--------------------------------------------------"<<endl;
    }
}