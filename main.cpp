#include <iostream>
using namespace std;
#include <string>

struct datastorage
{
    int index;
    int distance;
};

class MinHeap // MaxHeap
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

    void heap_insert(int distance, int index); // Inserts

    void bubble_up(int position); // Moves the value up

    void bubble_down(int position); // Moves the value down

    int *heap_extract(); // Removes the largest value
};

MinHeap ::MinHeap(int my_capacity)
{
    capacity = my_capacity;
    heap_size = 0;
    harr = new datastorage[capacity];
}

void MinHeap ::heap_insert(int distance, int index)
{
    if (capacity == heap_size) // If the heap is full
    {
        cout << "full\n";
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
    while (i > 0 && harr[i].distance <= harr[parent(i)].distance) // Maxheap logic
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
    int val = harr[0].distance; // Gets the largest value
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
    output[1] = val;

    return output;
}
// ------------------------------------------------------------------

int const infinity = 2147483647;

struct Node_detail
{
    int distance;
    bool is_conected = false;
    int cost;
};

class Database
{
private:
    int designated_router;
    int num_node;

public:
    Node_detail **velocity_table;

    Database(int Num_node, int DR);

    void get_data(int line, int node, int conection_velocity);

    void dijkstra();
};

Database ::Database(int Num_node, int DR)
{

    num_node = Num_node;
    velocity_table = new Node_detail *[Num_node];
    for (int i = 0; i < Num_node; i++)
    {
        velocity_table[i] = new Node_detail[Num_node];
    }

    designated_router = DR;
}

void Database ::get_data(int line, int node, int conection_velocity)
{
    int cost = (int)(1 << 20) / conection_velocity;
    velocity_table[line][node].distance = conection_velocity;
    velocity_table[line][node].is_conected = true;
    velocity_table[line][node].cost = cost;
}

void Database ::dijkstra()
{
    int D[num_node];
    int F[num_node];
    for (int i = 0; i < num_node; i++)
    {
        D[i] = infinity;
        F[i] = -1;
    }
    D[designated_router] = 0;
    MinHeap C(num_node);
}

int main()
{
    cout << infinity;
    int K, N, DR, M;
    string input_n, input_m;
    cin >> K;

    for (int i = 0; i < K; i++)
    {
        cin >> N;
        cin >> DR;
        Database database(N, DR);

        cin.ignore();

        for (int line = 0; line < N; line++)
        {
            int qnt_nodes, node, conection_velocity;
            cin >> qnt_nodes;

            for (int i = 0; i < qnt_nodes; i++)
            {
                cin >> node;
                cin >> conection_velocity;
                database.get_data(line, node, conection_velocity);
            }
        }
        cout << database.velocity_table[0][1].distance << endl;

        cin >> M;
        cin.ignore();
        for (int z = 0; z < M; z++)
        {
            getline(cin, input_m);
        }
    }
}
