//Taymaa Nasser  1222640  Section 5.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_BUILDINGS 50 //max number of buildings
#define MAX_COURSES 50   //max number of courses

//type defining pointers of my structs for easier use
typedef struct vertex* Vptr;
typedef struct edge* Eptr;
typedef struct Queue* Qptr;
typedef Qptr MyQueue;
typedef struct MinHeapNode* Hptr;

typedef struct vertex //struct of graph vertices
{
    int inDegree;
    char Cname[50];
    int cost;
    Eptr edge;
} vertex;

typedef struct edge //struct of graph edges
{
    Eptr next;
    Eptr last;
    Vptr vertex;
    int weight;
} edge;

typedef struct Queue //struct of queue
{
    char str[50];
    Qptr next;
    Qptr last; //pointer that points to the last element in queue
} Queue;

typedef struct MinHeap //struct of array of min heap nodes
{
    int maxsize; //maximum size of the min-heap
    int size; //current size of my heap
    Vptr Heap[]; //array to store vertex nodes
} MinHeap;
MinHeap heap;

int size=0;
int BuildingCount = 0; //variable to track the number of buildings
int CourseCount = 0;   //variable to track the number of courses

//declaring all functions used in project
void displayMenu();
void loadCoursesFile(Vptr ArrayofCourses[]);
void loadBuildingsFile(Vptr ArrayofBuildings[], int MatrixofBuildings[][MAX_BUILDINGS]);
int searchBuildingIndex(Vptr ArrayofBuildings[], char key[]);
Vptr createVertex(char name[]);
Eptr createEdge(Vptr vertex, int weight);
Qptr createNode_Queue();
void makeEdgeBetween(Vptr v1, Vptr v2, int weight);
void enqueue(Qptr head, char newStr []);
Qptr dequeue(Qptr head);
void topologicalSort(Vptr ArrayofCourses[],int op);
int indexing(Vptr ArrayofCourses[],char key[]);
void MinHeapInitial(int maxsize);
int parent(int pos);
int leftChild(int pos);
int rightChild(int pos);
void insert(Vptr element);
Vptr extractMin();
int isLeaf(int pos);
void minHeapify(int pos);
void minHeap();
void swap(int fpos, int spos);
int isEmpty();
Vptr getMin();
void decreaseKey(int index, int newCost);
void Dijkstra(int src, int des, int MatrixofBuildings[][BuildingCount], Vptr ArrayofBuildings[MAX_BUILDINGS],int dijkstra_path[], int *dijkstra_length);
void printShortestRoute(Vptr ArrayofBuildings[], int MatrixofBuildings[][MAX_BUILDINGS],int src, int des, int dijkstra_path[]);;
int main()
{
    Vptr ArrayofCourses[MAX_COURSES]; //array of course vertices
    Vptr ArrayofBuildings[MAX_BUILDINGS]; //array of buildings verices
    int MatrixofBuildings[MAX_BUILDINGS][MAX_BUILDINGS]; //adjacency matrix for buildings
    for (int i= 0; i< MAX_BUILDINGS; i++)
    {
        for (int j= 0; j< MAX_BUILDINGS; j++)
        {
            MatrixofBuildings[i][j] = 0; //initializing distance between each two buildings
        }
    }


    int dijkstra_path[MAX_BUILDINGS];
    int dijkstra_length;
    char startBuilding[50], endBuilding[50],startBuilding2[50], endBuilding2[50]; //declaring variables to store user-entered data in menu
    int loadFlag=0; //initializing flag to prevent loading of txt file more than once.
    int TopFlag=0; //initializing flag to prevent printing of sorted courses before topological sorting
    int func=0; //initializing flag to enter and end menu.(when func==6, exit menu).

    while(func!=6)
    {
        displayMenu();
        scanf("%d",&func);
        getchar(); //to deal with special characters entered by user.
        switch(func)
        {
        case 1:
            if(loadFlag==0)
            {
                loadCoursesFile(ArrayofCourses);
                loadBuildingsFile(ArrayofBuildings,MatrixofBuildings);
                loadFlag=1;
            }
            else
                printf("You can not load the file more than once. Please select another operation. \n");
            break;
        case 2:
            if(loadFlag!=1) //forcing user to load file before printing it using the loadFlag.
            {
                printf("You must load file before printing it. Please select operation 1.\n");
            }
            else
            {
                printf("Please provide the name of the source building: ");
                gets(startBuilding);

                printf("Please provide the name of the destination building: ");
                gets(endBuilding);

                int startIndex = searchBuildingIndex(ArrayofBuildings,startBuilding);
                int endIndex = searchBuildingIndex(ArrayofBuildings,endBuilding);
                Dijkstra(startIndex, endIndex, MatrixofBuildings,ArrayofBuildings,dijkstra_path, &dijkstra_length);
            }
            break;
        case 3:
            if(loadFlag!=1) //forcing user to load file before printing it using the loadFlag.
            {
                printf("You must load file first. Please select operation 1.\n");
            }
            else
            {
                int startIndex2 = searchBuildingIndex(ArrayofBuildings,startBuilding);
                int endIndex2 = searchBuildingIndex(ArrayofBuildings,endBuilding);
                printf("Printing the shortest route between the two buildings...\n");
                printShortestRoute(ArrayofBuildings,MatrixofBuildings,startIndex2, endIndex2,dijkstra_path);
            }
            break;
        case 4:
            if(loadFlag!=1) //forcing user to load file before printing it using the loadFlag.
            {
                printf("You must load file first. Please select operation 1.\n");
            }
            else
            {
                printf("Sorting the courses using topological sort...\n");
                topologicalSort(ArrayofCourses,0);
                printf("Courses are successfully sorted!\n");
                TopFlag=1;
            }
            break;
        case 5:
            if(TopFlag!=1) //forcing user to sort courses before printing them.
            {
                printf("You must sort first. Please select operation 4.\n");
            }
            else
            {
                printf("Printing the sorted courses...\n");
                topologicalSort(ArrayofCourses,1);
            }
            break;
        case 6:
            printf("Thank you for using my Course Prerequisite Planner and Campus Route Finder system! Goodbye.\n"); //exiting system
            break;
        default:
            printf("No such operation! Please try again.\n"); //deals with user-entered data that is not a number from 1-6
        }
    }
    return 0;
}

void displayMenu() //function to display main menu
{
    printf("Please select one functionality to run (1-11):\n\n");
    printf("1. Load the two input files.\n");
    printf("2. Calculate the shortest distance between two buildings.\n");
    printf("3. Print the shortest route between two buildings and the total distance.\n");
    printf("4. Sort the courses using the topological sort.\n");
    printf("5. Print the sorted courses.\n");
    printf("6. Exit the application.\n");
}
void loadCoursesFile(Vptr ArrayofCourses[]) //function that loads input file input_courses.txt and saves it into adjacency list
{
    FILE *fp; //declaring a file pointer fp
    fp = fopen("input_courses.txt","r"); //using fp to open input.txt file in read mode
    if (fp == NULL)
    {
        printf("Input file failed to load!\n");
        return;
    }

    int index = 0; //variable to track the number of vertices
    int counter; //variable to keep track of tokens
    char FullString[300]; //declaring a variable to contain the tokenized string read from file
    Vptr prereq; //prerequisite vertex
    while(!feof(fp)) //read lines until end of file
    {
        fgets(FullString,sizeof(FullString),fp); //read until end of line
        if (CourseCount >= MAX_COURSES) //check if the maximum number of courses is reached
        {
            printf("Maximum number %d of courses reached!\n",MAX_COURSES);
            break;
        }
        char*token = strtok(FullString, "#\n"); //tokenise the full string after every space "#" char
        ArrayofCourses[index] = createVertex(token); //create vertex at every index of array
        index++;
        counter=0;
        token = strtok(NULL, "#\n");//tokenise the leftover string after space char.

        while(token != NULL )
        {
            counter++;
            if(counter>=1)
            {
                for(int i=0; i<index; i++)
                {
                    if(strcmp(ArrayofCourses[i]->Cname,token)==0) //to prevent creation of same vertex more than once
                    {
                        prereq =ArrayofCourses[i];
                        break;
                    }
                }
                makeEdgeBetween(prereq,ArrayofCourses[index- 1], 0); //connects 2 verices with an edge of weight 0
            }
            token = strtok(NULL, "#\n");//tokenise the leftover string after space char.
        }
    }

    CourseCount = index; //updating Coursecount dynamically
    printf("Loading Courses data input file ...\n");
    printf("Courses file Loaded!\n\n");
    fclose(fp); //closing the file
    return;
}
void loadBuildingsFile(Vptr ArrayofBuildings[], int MatrixofBuildings[][MAX_BUILDINGS]) //function that loads input file input_buildings.txt and saves it into adjacency matrix
{
    FILE *fp;
    fp = fopen("input_buildings.txt", "r");
    if (fp == NULL)
    {
        printf("Input file failed to load!\n");
        return;
    }
    char FullString[300];
    while (!feof(fp))
    {
        fgets(FullString,sizeof(FullString),fp);
        if (BuildingCount >= MAX_BUILDINGS) //check if the maximum number of buildings is reached
        {
            printf("Maximum number %d of buildings reached!\n",MAX_BUILDINGS);
            break;
        }
        char *token = strtok(FullString, "#");
        if (token != NULL)
        {
            int existingIndex = -1;
            for (int i = 0; i < BuildingCount; i++) //searching if a building already exists in ArrayofBuildings
            {
                if (strcmp(ArrayofBuildings[i]->Cname, token) == 0)
                {
                    existingIndex = i;
                    break;
                }
            }
            ArrayofBuildings[BuildingCount] = createVertex(token); //creating a new vertex if the building does not exist
            strcpy(ArrayofBuildings[BuildingCount]->Cname, token); // Assuming ArrayofBuildings is an array of Vertex structs
          //  printf("Loaded source building: %s\n", ArrayofBuildings[BuildingCount]->Cname);

            token = strtok(NULL, "#\n"); // Move to the next token
            while (token != NULL)
            {
                char destBuilding[50];
                int distance;
                strcpy(destBuilding, token);
                token = strtok(NULL, "#\n");
                if (token != NULL)
                {
                    distance = atoi(token);
               //1     printf("Loaded destination building: %s, Distance: %d\n", destBuilding, distance);

                    int destIndex = -1;  //searching for the index of the destination building
                    for (int i = 0; i < BuildingCount; i++)
                    {
                        if (strcmp(ArrayofBuildings[i]->Cname, destBuilding) == 0)
                        {
                            destIndex = i;
                            break;
                        }
                    }

                    if (destIndex != -1)  //if destination building found, update the adjacency matrix
                    {
                        Vptr sourceBuilding = ArrayofBuildings[BuildingCount];
                        Vptr destBuilding = ArrayofBuildings[destIndex];
                        makeEdgeBetween(sourceBuilding, destBuilding, distance);
                        MatrixofBuildings[BuildingCount][destIndex] = distance;
                    }
                }
                token = strtok(NULL, "#\n"); // Move to the next token
            }
            BuildingCount++; //update the building count
        }
    }
    printf("Loading buildings data input file ...\n");
    printf("Buildings file loaded!\n\n");
    fclose(fp);
}

int searchBuildingIndex(Vptr ArrayofBuildings[], char key[]) //function that returns index of key building in array of buildings
{
    for (int i = 0; i < BuildingCount; i++)
    {
        if (strcmp(ArrayofBuildings[i]->Cname, key) == 0)
        {
            return i; //building is found, return its index
        }
    }
    return -1; //building not found, return -1
}

Vptr createVertex(char name[]) //function that creates and initializes vertex
{
    Vptr head = (Vptr)malloc(sizeof(vertex));
    if (head == NULL)
    {
        printf("Memory for vertex can not be allocated.\n");
        return NULL; //return NULL pointer in case of memory allocation failure.
    }
    head->inDegree = 0;
    head->cost=0;
    strcpy(head->Cname,name);
    head->edge = NULL;
    return head;
}
Eptr createEdge(Vptr vertex, int weight) //function that creates edge
{
    Eptr head = (Eptr)malloc(sizeof(edge));
    if (head == NULL)
    {
        printf("Memory for edge can not be allocated.\n");
        return NULL; //return NULL pointer in case of memory allocation failure.
    }
    head->next = NULL;
    head->last = head;
    head->vertex = vertex;
    head->weight = weight;
    return head;
}

Qptr createNode_Queue()  //creates new Stack node for insertion.
{
    Qptr head=(Qptr)malloc(sizeof(struct Queue));
    if(head == NULL)
    {
        printf("Memory for queue can not be allocated.\n");
        return NULL; //return NULL pointer in case of memory allocation failure.
    }
    head->next= NULL;
    return head;
}
void makeEdgeBetween(Vptr v1, Vptr v2, int weight) //function that creates a new edge between two vertices given a weight
{
    Eptr newEdge = createEdge(v2, weight);

    if (v1->edge == NULL) //check if the edge list of v1 is empty
    {
        v1->edge = newEdge; //if it is empty, set the new edge as the first edge
    }
    else
    {
        v1->edge->last->next= newEdge; //if it is not empty, add the new edge to the end of the edge list
        newEdge->next=NULL;
        v1->edge->last= newEdge;
    }
    v2->inDegree++; //increment the in-degree of the second vertex
}

void enqueue(Qptr head, char newStr []) //function to enqueue elements into queue
{
    if (head!=NULL) //checks if the head node of the queue LL was created
    {
        Qptr temp=createNode_Queue(); //creates new node for enqueueing
        if (temp != NULL)
        {
            if (head->last == NULL) //if there are no elements yet in the queue
            {
                strcpy(temp->str,newStr);
                head->next = temp;
                head->last = temp;
            }
            else //if there is already other elements in queue
            {
                strcpy(temp->str,newStr);
                temp->next=head->last->next; //make next of new node point to whatever the last element was pointing to
                head->last->next=temp; //make the previously last element point to the now last element temp
                head->last=temp;//update the last pointer to be the latest node enqueued
            }
        }
        else
        {
            printf("Memory allocation failed. Cannot push.\n");
            return;
        }
    }

    else
    {
        printf("LinkedList of this queue does not exist.\n");
        return;
    }

}

Qptr dequeue(Qptr head) //function to dequeue and return the front of the queue
{
    char dequeuedWord[30]; //variable to hold word of front element
    if(head!=NULL && head->next!=NULL) //as long as the queue is not empty and has at least 1 element
    {
        Qptr temp=head->next; //assigning temp pointer to front of queue
        head->next=temp->next; //assigning next of head to be the second element in queue
        if (head->next == NULL)
        {
            head->last = NULL;
        }
        return temp; //returning temp node
    }
    else
    {
        printf("Queue is empty. There is nothing to dequeue.\n");
        return NULL;
    }
}

void topologicalSort(Vptr ArrayofCourses[],int op) //function to perform topological sort of courses
{
    MyQueue queue = createNode_Queue(); //creating a queue for BFS traversal

    int inDegree[CourseCount];  //initializing an array to store in-degrees of vertices
    for (int i = 0; i< CourseCount; i++)
    {
        inDegree[i] = ArrayofCourses[i]->inDegree;
    }
    int sortedIndex = 0;

    for (int i = 0; i< CourseCount; i++) //enqueue vertices with in-degree 0 (no prereq)
    {
        if (inDegree[i] == 0)
        {
            enqueue(queue, ArrayofCourses[i]->Cname);
        }
    }

    while (queue->next!= NULL)//do BFS
    {
        Qptr currentPrereq = dequeue(queue);
        if(op==1) //printing the sorted courses when user enters 5
        {
            printf("%s \n", currentPrereq->str);
        }
        int index = indexing(ArrayofCourses, currentPrereq->str); //calculating the index of the dequeued course
        Eptr edge = ArrayofCourses[index]->edge; //get the first edge of the current vertex
        while (edge != NULL)
        {
            Vptr adjVertex = edge->vertex; //get the adjacent vertex
            int adjIndex = indexing(ArrayofCourses, adjVertex->Cname); //get the index of the adjacent course
            inDegree[adjIndex]--; //decrement the in-degree of the adjacent vertex
            if (inDegree[adjIndex] == 0)
            {
                enqueue(queue,adjVertex->Cname); //enqueue the adjacent course if its in-degree becomes 0
            }
            edge = edge->next; // Move to the next edge
        }
    }
    while (queue->next != NULL) // freeing the memory allocated for the queue nodes
    {
        Qptr temp = dequeue(queue);
        free(temp);
    }
}

int indexing(Vptr ArrayofCourses[],char key[]) //function to find the index of a course in the array of courses
{
    int index=0;
    for(int i=0; i<CourseCount; i++)
    {
        if(strcmp(ArrayofCourses[i]->Cname,key)==0)
        {
            return index;
        }
        index++;
    }
    return -1; //return -1 if the course is not found
}

void MinHeapInitial(int maxsize) //function to initialize the heap
{
    heap.maxsize = maxsize;
    heap.size = 0;  // Initially the heap is empty
    Hptr Heap[maxsize + 1];
    heap.Heap[0] = NULL;
}

int parent(int pos) //function to return the position of the parent for the node currently at pos
{
    return pos / 2;
}

int leftChild(int pos) //function to return the position of the left child for the node currently at pos
{
    return (2 * pos);
}

int rightChild(int pos) //function to return the position of the right child for the node currently at pos
{
    return (2 * pos) + 1;
}

void insert(Vptr element) //function to insert a node into the min-heap
{
    if (size >= heap.maxsize)
    {
        printf("Heap is full. Cannot insert.\n");
        return;
    }
    size++;
    heap.Heap[size] = element;
    int current = size;

    //maintain heap property
    while (current > 1 && heap.Heap[current]->cost < heap.Heap[parent(current)]->cost)
    {
        swap(current,parent(current));
        current=parent(current);
    }
}

Vptr extractMin() //function to extract the minimum element from the heap
{
    if (size == 0)
    {
        printf("Heap is empty. Cannot extract minimum.\n");
        return NULL;
    }
    Vptr min = heap.Heap[1];
    heap.Heap[1] = heap.Heap[size--];
    heap.size--;
    minHeapify(1);
    return min;
}
int isLeaf(int pos) //function that returns true if the passed node is a leaf node
{
    if (pos >= (size / 2) && pos <= size)
    {
        return 1;
    }
    return 0;
}

void minHeapify(int pos) //function to heapify the node at pos
{
    int smallest = pos;
    int left = leftChild(pos);
    int right = rightChild(pos);

    //compare with left child
    if (left <= heap.size && heap.Heap[left]->cost < heap.Heap[smallest]->cost)
        smallest = left;

    //compare with right child
    if (right <= heap.size && heap.Heap[right]->cost < heap.Heap[smallest]->cost)
        smallest = right;

    //if smallest is not the root
    if (smallest != pos)
    {
        swap(pos, smallest);
        minHeapify(smallest);
    }
}
void minHeap()//function to build the min-heap
{
    for (int pos = (size / 2); pos >= 1; pos--)
    {
        minHeapify(pos);
    }
}

void swap(int fpos, int spos) //function to swap two nodes of the heap
{
    Vptr tmp = heap.Heap[fpos];
    heap.Heap[fpos] = heap.Heap[spos];
    heap.Heap[spos] = tmp;
}
int isEmpty()
{
    return size == 0;
}
Vptr getMin()//function to get the minimum element from the heap without extracting it
{
    if (size == 0)
        return NULL; // If the heap is empty
    return heap.Heap[1];
}

void decreaseKey(int index, int newCost) //function to update the cost of a node in the heap
{
    heap.Heap[index]->cost = newCost;
    minHeapify(index);
}
void Dijkstra(int src, int des, int MatrixofBuildings[][BuildingCount], Vptr ArrayofBuildings[MAX_BUILDINGS],int dijkstra_path[], int *dijkstra_length) //function that performs dijkstra algo
{
    //decalring visited, cost, and parent arrays
    int visited[BuildingCount]; //to store if each city is visited or not
    int cost[BuildingCount];    //to store the cost from the source city to each destination city
    int parent[BuildingCount];  //to store the parent of each visited city
    *dijkstra_length = 0;

    for (int i = 0; i < BuildingCount; i++)  //initializing visited, cost, and parent arrays
    {
        visited[i] = 0;
        parent[i] = -1;
        cost[i] = -1;
    }

    //the source is visited and has cost 0
    visited[src] = 1;
    cost[src] = 0;

    if (des == src) //if user entered same building twice
    {
        printf("Source city and destination city are the same.\n");
        return;
    }

    size = 0; //initializing the heap index

    MinHeapInitial(BuildingCount); //initializig the min-heap

    insert(ArrayofBuildings[src]);  //inserting the source node into the min-heap

    while (!isEmpty()) //while heap not empty
    {
        Vptr currentNode = extractMin();  //extract the minimum cost node from the heap

        int u = currentNode->inDegree;
        visited[u] = 1;

        if (u == des || isEmpty()) //if destination is reached or all nodes have been visited break loop
        {
            break;
        }

        for (int v = 0; v < BuildingCount; v++)  //traverse adjacent nodes of the current node
        {

            if (MatrixofBuildings[u][v] != 0 && !visited[v])  //if there is an edge from u to v and v is not visited
            {
                int altPath = cost[u] + MatrixofBuildings[u][v]; //calculate the distance from the source to v through u

                if (altPath < cost[v])  // if the new distance is LESS than the current distance, update it
                {
                    cost[v] = altPath;
                    parent[v] = u;
                    decreaseKey(v, altPath); //update the cost of node v in the min-heap
                }
            }
        }
    }

    //reconstruct the shortest path and saving the index of the buildings of the shortest path into array dijkstra_path
    int shortestPath = des; //start from the destination vertex
    while (shortestPath != -1)//continue until reaching the source vertex (going backwards)
    {
        dijkstra_path[*dijkstra_length] = shortestPath; //store the current vertex in the path array
        (*dijkstra_length)++; //incrementing the length of the path array
        shortestPath = parent[shortestPath]; //move back to the previous vertex in the shortest path
    }
}
void printShortestRoute(Vptr ArrayofBuildings[], int MatrixofBuildings[][MAX_BUILDINGS],int src, int des, int dijkstra_path[]) //function to print the shortest route between two buildings after running Dijkstra's algo
{
    int totalDis = 0; //variable to store the total distance of the shortest route
    printf("The shortest route from building %s to building %s:\n", ArrayofBuildings[src]->Cname, ArrayofBuildings[des]->Cname);
    printf("%s", ArrayofBuildings[src]->Cname);
    int current = src; //initializing the current building as the source

    while (current != des)//traversing the shortest path and printing the names of buildings crossed
    {
        int next = dijkstra_path[current]; //getting the next building in the shortest path
        printf("--> %s", ArrayofBuildings[next]->Cname);
        totalDis += ArrayofBuildings[next]->cost; //add the distance between the current and next buildings to the total distance
        current = next; //move to the next building
    }
    printf("\n");
    printf("Total distance is: %d\n", totalDis);
}
