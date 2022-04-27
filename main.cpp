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

void MinHeap ::heap_update(int index, int distance)
{
    bool is_find = false;
    for (int i = 0; i < capacity; i++)
    {
        if (harr->index == index)
        {
            harr->distance = distance;
            is_find = true;
            break;
        }
    }
    if (is_find == false)
    {
        heap_insert(index, distance);
    }
}
// ------------------------------------------------------------------

int const infinity = 2147483647;

struct Node_detail
{
    int velocity;
    bool is_conected = false;
    int cost;
};

struct Output_detail
{
    int time_to_dr = 0;
    int conections_to_dr = 0;
};

class Database
{
private:
    int designated_router;
    int num_node;

public:
    Output_detail *out_table;
    int *weight_arr;
    int *predecessor_arr;

    Node_detail **velocity_table;

    Database(int Num_node, int DR);

    void get_data(int line, int node, int conection_velocity);

    void dijkstra();

    void get_path_to_dr(int starter, int msg_size);

    void make_output(int starter, int receiver, int msg_size);

};

Database ::Database(int Num_node, int DR)
{

    num_node = Num_node;
    velocity_table = new Node_detail *[Num_node];
    out_table = new Output_detail[Num_node];
    for (int i = 0; i < Num_node; i++)
    {
        velocity_table[i] = new Node_detail[Num_node];
    }

    designated_router = DR;
}

void Database ::get_data(int line, int node, int conection_velocity)
{
    int cost = (int)(1 << 20) / conection_velocity;
    velocity_table[line][node].velocity = conection_velocity;
    velocity_table[line][node].is_conected = true;
    velocity_table[line][node].cost = cost;
}

void Database ::dijkstra()
{
    int *D = new int[num_node];
    int *F = new int[num_node];
    for (int i = 0; i < num_node; i++)
    {
        D[i] = infinity;
        F[i] = -1;
    }
    D[designated_router] = 0;
    MinHeap C(num_node);
    C.heap_insert(designated_router, 0);

    for (int i = 0; i < num_node; i++)
    {
        int *input = C.heap_extract();
        int u = input[0]; // u == line
        int Du = input[1];

        for (int z = 0; z < num_node; z++)
        {
            if (velocity_table[u][z].is_conected)
            {
                int weight = velocity_table[u][z].cost;

                if (Du + weight < D[z])
                {
                    D[z] = Du + weight;
                    F[z] = u;
                    C.heap_update(z, D[z]);
                }
            }
        }
    }
    weight_arr = D;
    predecessor_arr = F;
}

void Database ::get_path_to_dr(int starter, int msg_size)
{
    int i = starter;
    int conection = 0, temp_time = 0, conections = 0;
    while (i != designated_router)
    {
        conections++;
        conection = predecessor_arr[i];
        temp_time += msg_size / velocity_table[i][conection].velocity;
        
        i = conection;
    }
    //cout <<starter << "--" <<conections << "--" << temp_time;
    out_table[starter].conections_to_dr = conections;
    out_table[starter].time_to_dr = temp_time;
}

void Database ::make_output(int starter, int receiver, int msg_size){
    if (out_table[starter].time_to_dr == 0){
        get_path_to_dr(starter, msg_size);
    }
    if (out_table[receiver].time_to_dr ==0){
        get_path_to_dr(receiver, msg_size);
    }
    
    int total_time =  out_table[starter].time_to_dr + out_table[receiver].time_to_dr;
    cout << out_table[starter].time_to_dr << "+" << out_table[receiver].time_to_dr << "=";
    int total_conections = out_table[starter].conections_to_dr + out_table[receiver].conections_to_dr;
    cout << total_conections << " " << total_time << "\n";
}

int main()
{
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

        database.dijkstra();

        cin >> M;
        cin.ignore();
        for (int z = 0; z < M; z++)
        {
            int starter, receiver, msg_size;
            cin >> starter;
            cin >> receiver;
            cin >> msg_size;
            database.make_output(starter, receiver, msg_size);
        }
    }
}
