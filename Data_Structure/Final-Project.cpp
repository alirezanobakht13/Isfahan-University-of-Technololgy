#include <bits/stdc++.h>
using namespace std;

// Max ertefaa e huffman tree mishe log(128) ya 7 vali just to be sure...
#define MAX_HEIGHT 100

class Table
{
public:
    int size;
    char c[128];
    string s[128];
    Table() : size(0) {}
    void add(char pc, string ps)
    {
        c[size] = pc;
        s[size] = ps;
        size++;
    }
    string getCode(char pc)
    {
        for (int i = 0; i < size; i++)
        {
            if (c[i] == pc)
            {
                return s[i];
            }
        }
        return string();
    }
    void printDebug()
    {
        for (int i = 0; i < size; i++)
        {
            cout << endl
                 << c[i] << ": " << s[i] << endl;
        }
    }
};

class QueueNode
{
public:
    char data;
    unsigned freq;
    QueueNode *left, *right;
};

class Queue
{
public:
    int front, rear;
    int capacity;
    QueueNode **array;
};

QueueNode *newNode(char data, unsigned freq)
{
    QueueNode *temp = new QueueNode[(sizeof(QueueNode))];
    temp->left = temp->right = NULL;
    temp->data = data;
    temp->freq = freq;
    return temp;
}

Queue *createQueue(int capacity)
{
    Queue *queue = new Queue[(sizeof(Queue))];
    queue->front = queue->rear = -1;
    queue->capacity = capacity;
    queue->array = new QueueNode *[(queue->capacity * sizeof(QueueNode *))];
    return queue;
}

int isSizeOne(Queue *queue)
{
    return queue->front == queue->rear && queue->front != -1;
}

int isEmpty(Queue *queue) // ! Could use bool
{
    return queue->front == -1;
}

int isFull(Queue *queue)
{
    return queue->rear == queue->capacity - 1;
}

void enQueue(Queue *queue, QueueNode *item)
{
    if (isFull(queue))
        return;
    queue->array[++queue->rear] = item;
    if (queue->front == -1) // Age khaali bude ghablesh alan okay esh kon...
        ++queue->front;
}

QueueNode *deQueue(Queue *queue)
{
    if (isEmpty(queue))
        return NULL;
    QueueNode *temp = queue->array[queue->front];
    if (queue->front == queue->rear) // If there is only one item in queue
        queue->front = queue->rear = -1;
    else
        ++queue->front; // chon ye seri enqueue va ye seri dequeue mimkonim kollan dg be avvalia niaz nadarim.
    return temp;
}

QueueNode *getFront(Queue *queue)
{
    if (isEmpty(queue))
        return NULL;
    return queue->array[queue->front];
}

QueueNode *findMin(Queue *firstQueue, Queue *secondQueue)
{
    if (isEmpty(firstQueue))
        return deQueue(secondQueue);

    if (isEmpty(secondQueue))
        return deQueue(firstQueue);

    if (getFront(firstQueue)->freq < getFront(secondQueue)->freq)
        return deQueue(firstQueue);

    return deQueue(secondQueue);
}

int isLeaf(QueueNode *root)
{
    return !(root->left) && !(root->right);
}

string arrToStr(int arr[], int n)
{
    string s = "";
    for (int i = 0; i < n; ++i)
    {
        s += char(arr[i] + 48);
    }
    return s;
}

QueueNode *buildHuffmanTree(char data[], int freq[], int size)
{
    QueueNode *left, *right, *top;

    Queue *firstQueue = createQueue(size);
    Queue *secondQueue = createQueue(size);

    // Input kardane barg haa dar saf e shomare 1
    for (int i = 0; i < size; ++i)
        enQueue(firstQueue, newNode(data[i], freq[i]));

    // Taa vaghti faghat 1 node tooye queue 2 moonde boro jelo
    while (!(isEmpty(firstQueue) && isSizeOne(secondQueue)))
    {
        left = findMin(firstQueue, secondQueue);
        right = findMin(firstQueue, secondQueue);

        top = newNode('~', left->freq + right->freq); // ! REVISE LATER
        top->left = left;
        top->right = right;
        enQueue(secondQueue, top);
    }

    return deQueue(secondQueue);
}

void getCodes(QueueNode *root, int arr[], int top, Table *t)
{
    // The arr will be rewritten az top e khodesh be baad in every if block.
    if (root->left)
    {
        arr[top] = 0;
        getCodes(root->left, arr, top + 1, t);
    }

    if (root->right)
    {
        arr[top] = 1;
        getCodes(root->right, arr, top + 1, t);
    }

    if (isLeaf(root))
    {
        // cout << root->data << "  " << arrToStr(arr, top) << endl;
        t->add(root->data, arrToStr(arr, top));
    }
}

void HuffmanCodes(char data[], int freq[], int size, Table &t)
{
    // Saakhtane huffman
    QueueNode *root = buildHuffmanTree(data, freq, size);

    // Adding the codes to the table
    int arr[MAX_HEIGHT], top = 0;
    getCodes(root, arr, top, &t);
}

class freq
{
private:
    char ch[128];
    int countt[128];
    int size;

public:
    freq(string text, char c[], int cou[], int &s)
    {
        size = 0;
        for (int i = 0; i < 128; i++)
        {
            ch[i] = char(0);
            countt[i] = 0;
        }

        for (int i = 0; i < 128; i++)
        {
            int t = count(text.begin(), text.end(), char(i));
            add(char(i), t);
        }

        sortIt();

        for (int i = 0; i < size; i++)
        {
            c[i] = ch[i];
            cou[i] = countt[i];
        }
        s = size;
    }
    int add(char a, int co)
    {
        if (co != 0 && a != '\n')
        {
            ch[size] = a;
            countt[size] = co;
            size++;
        }
    }

    void sortIt()
    {
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size - 1 - i; j++)
            {
                if (countt[j] > countt[j + 1])
                {
                    swap(countt[j], countt[j + 1]);
                    swap(ch[j], ch[j + 1]);
                }
            }
        }
    }
};

int main()
{

    ifstream infile("input.txt");

    // txt file to string
    string text((istreambuf_iterator<char>(infile)),
                (istreambuf_iterator<char>()));
    infile.close();

    char tc[128];
    int tf[128];
    int size;

    freq myFreq(text, tc, tf, size);

    Table t;
    // t.printDebug();

    HuffmanCodes(tc, tf, size, t);

    ofstream outfile("output.txt");

    for (int i = 0; i < text.size(); i++)
    {
        if (text[i] == '\n')
            outfile << endl;
        else
        {
            outfile << t.getCode(text[i]);
        }
    }

    outfile.close();

    return 0;
}
