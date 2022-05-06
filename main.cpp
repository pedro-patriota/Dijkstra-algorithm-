#include <iostream>
using namespace std;
#include <string>

struct datastorage
{
    int index;
    int distance;
};

class MinHeap // MinHeap
{
private:
    int capacity;
    int heap_size;

public:
    datastorage *harr;
    MinHeap(int my_capacity); // Constructor

    int parent(int i) // Gets the parent
    {
        return (int)(i - 1) / 2;
    }

    void heap_insert(int index, int distance); // Inserts

    void bubble_up(int position); // Moves the value up

    void bubble_down(int position); // Moves the value down

    int *heap_extract(); // Removes the largest value

    void heap_update(int index, int distance);
};

MinHeap ::MinHeap(int my_capacity)
{
    capacity = my_capacity;
    heap_size = 0;
    harr = new datastorage[capacity];
}

void MinHeap ::heap_insert(int index, int distance)
{
    if (capacity == heap_size) // If the heap is full
    {
        cout << "";
        return;
    }
    harr[heap_size].distance = distance;
    harr[heap_size].index = index;
    bubble_up(heap_size); // Inserts the value in the end of the heap and moves it up
    heap_size++;
    return;
}

void MinHeap ::bubble_up(int position)
{
    int i = position;
    while (i > 0 && harr[i].distance <= harr[parent(i)].distance) // Minheap logic
    {
        int temp = harr[parent(i)].distance;
        int temp_index = harr[parent(i)].index; // Permutation
        harr[parent(i)] = harr[i];
        harr[i].distance = temp;
        harr[i].index = temp_index;

        i = parent(i); // The "while" will now analyze the new position of the value
    }
    return;
}

void MinHeap ::bubble_down(int position)
{
    int l, r, m;
    l = 2 * position + 1;
    r = 2 * position + 2;
    m = position;

    if (l < heap_size && harr[l].distance <= harr[m].distance) // Bubble_down's logic
    {
        m = l;
    }
    if (r < heap_size && harr[r].distance <= harr[m].distance)
    {
        m = r;
    }
    if (m != position) // If "m" changes
    {
        int temp = harr[m].distance;
        int temp_index = harr[m].index; // Permutation
        harr[m] = harr[position];
        harr[position].distance = temp;
        harr[position].index = temp_index;

        return bubble_down(m); // Recursive
    }
    return;
}

int *MinHeap ::heap_extract()
{
    int distance = harr[0].distance; // Gets the minimum value
    int ind = harr[0].index;

    int temp = harr[heap_size - 1].distance;
    int temp_index = harr[heap_size - 1].index; // Permutation
    harr[heap_size - 1] = harr[0];
    harr[0].distance = temp;
    harr[0].index = temp_index;

    heap_size = heap_size - 1;
    bubble_down(0); // Makes the leaf to move down
    int *output = new int[1];
    output[0] = ind;
    output[1] = distance;

    return output;
}

void MinHeap ::heap_update(int index, int distance) // updates the heap
{
    bool is_find = false;
    for (int i = 0; i < capacity; i++)
    {
        if (harr->index == index) // checks if the node already exists
        {
            harr->distance = distance;
            is_find = true;
            break;
        }
    }
    if (is_find == false)
    {
        heap_insert(index, distance); // if not -> insert
    }
}
// ------------------------------------------------------------------

int const infinity = 2147483647;

struct Node_detail // Node detail
{
    int velocity = 0;
    bool is_conected = false;
    int cost;
};

struct Output_detail // Output specifications
{
    int conections_to_dr = 0;
    int total_cost;
};

class Database
{
private:
    int designated_router;
    int num_node;

public:
    Output_detail *out_table;

    int *weight_arr; // used in the djkistra algorithm
    int *predecessor_arr;

    Node_detail **node_table;

    Database(int Num_node, int DR); // constructor

    void get_data(int line, int node, int conection_velocity); // gets the input

    void dijkstra(); // executes the algorithm

    void get_path_to_dr(int starter, int msg_size); // get the path to the designated router

    void make_output(int starter, int receiver, int msg_size); // creates the output
};

Database ::Database(int Num_node, int DR)
{

    num_node = Num_node;
    node_table = new Node_detail *[Num_node];
    out_table = new Output_detail[Num_node]; // creates a table
    for (int i = 0; i < Num_node; i++)
    {
        node_table[i] = new Node_detail[Num_node];
    }

    designated_router = DR;
}

void Database ::get_data(int line, int node, int conection_velocity) // inserts the data
{
    int cost = (int)(1 << 20) / conection_velocity; // cost's formula
    node_table[line][node].velocity = conection_velocity;
    node_table[line][node].is_conected = true; // since there is an edge, there is a conection
    node_table[line][node].cost = cost;
}

void Database ::dijkstra()
{
    int *D = new int[num_node]; // Distance array
    int *F = new int[num_node]; // Predecessor array
    for (int i = 0; i < num_node; i++)
    {
        D[i] = infinity; // standard values
        F[i] = -1;
    }

    D[designated_router] = 0; // distance from the DR to the DR
    MinHeap C(num_node);      // creates the min heap
    C.heap_insert(designated_router, 0);

    for (int i = 0; i < num_node; i++)
    {
        int *input = C.heap_extract(); // gets the minimum struct
        int u = input[0];              // u == line
        int Du = input[1];

        for (int z = 0; z < num_node; z++) // z == column
        {
            if (node_table[u][z].is_conected) // checks inside the node table
            {
                int weight = node_table[u][z].cost; // gets weight

                if (Du + weight < D[z]) // if the new path is smaller than the older one
                {
                    D[z] = Du + weight;
                    F[z] = u;
                    C.heap_update(z, D[z]); // updates the heap
                }
            }
        }
    }
    weight_arr = D; // assign the weight and predecessor arrays into the class
    predecessor_arr = F;
}

void Database ::get_path_to_dr(int starter, int msg_size)
{
    int i = starter;
    int predecessor = 0, temp_time = 0, conections = 0, total_cost = 0;

    while (i != designated_router) // while the path did not get to the DR
    {
        conections++;

        predecessor = predecessor_arr[i];
        //temp_time += (msg_size) / node_table[i][predecessor].velocity; // increases the time
        total_cost += node_table[i][predecessor].cost;
        i = predecessor;
    }
  
    out_table[starter].conections_to_dr = conections; // assigning values
    out_table[starter].total_cost = total_cost;
}

void Database ::make_output(int starter, int receiver, int msg_size)
{

    get_path_to_dr(starter, msg_size); // get the number of conections and partial time

    get_path_to_dr(receiver, msg_size);

    int total_time = ((out_table[starter].total_cost + out_table[receiver].total_cost)*msg_size)/(1<<20); // total time and conection
    int total_conections = out_table[starter].conections_to_dr + out_table[receiver].conections_to_dr;

    cout << total_conections << " " << total_time << "\n"; // output
}

int main()
{
    int K, N, DR, M;
    string input_n, input_m;
    cin >> K; // number of cases

    for (int i = 0; i < K; i++)
    {
        cout << "caso " << i << ":" << endl;
        cin >> N;                 // number of nodes
        cin >> DR;                // designated router
        Database database(N, DR); // creates the table

        cin.ignore();

        for (int line = 0; line < N; line++)
        {
            int qnt_nodes, node, conection_velocity;
            cin >> qnt_nodes;

            for (int i = 0; i < qnt_nodes; i++)
            {
                cin >> node;
                cin >> conection_velocity;
                database.get_data(line, node, conection_velocity); // gets the input
            }
        }

        database.dijkstra(); // gets the shortest path

        cin >> M;
        cin.ignore();
        for (int z = 0; z < M; z++)
        {
            int starter, receiver, msg_size;
            cin >> starter;
            cin >> receiver;
            cin >> msg_size;

            database.make_output(starter, receiver, msg_size); // generates the output
        }
        cout << endl;
    }
}
