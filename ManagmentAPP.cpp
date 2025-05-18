#include <iostream>
#include <string>

using namespace std;

const int MAX = 100;

struct BSTNode{

    int Uid;
    string Name;
    BSTNode* Left;
    BSTNode* Right;

    BSTNode() {}

    BSTNode(int uid, string name):Uid(uid),Name(name),Left(NULL),Right(NULL){}

};

class BST{

public:

    BSTNode* root;

    BST() {root = NULL;}

    BSTNode* insert(BSTNode* node, int uid, string name){

        if (node == NULL) 
           return new BSTNode(uid, name);

        if (uid < node->Uid)
            node->Left = insert(node->Left, uid, name);

        else
            node->Right = insert(node->Right, uid, name);

        return node;
    }

    BSTNode* search(BSTNode* node, int uid){

        if (node == NULL)
            return NULL;

        if (node->Uid == uid)
            return node;

        if (uid < node->Uid)
			return search(node->Left, uid);
        else
        return search(node->Right, uid);
    }

    BSTNode* deleteNode(BSTNode* node, int uid){

        if (node == NULL) 
            return NULL;
        //peida kardan n
        if (uid < node->Uid)
            node->Left = deleteNode(node->Left, uid);

        else if (uid > node->Uid)
            node->Right = deleteNode(node->Right, uid);

        else {
      
            if (node->Left == NULL) {
                BSTNode* temp = node->Right;
                delete node;
                return temp;
            } else if (node->Right == NULL) {
                BSTNode* temp = node->Left;
                delete node;
                return temp;
			}
            //hardo farzand ro dare
            BSTNode* temp = node->Right;
            while (temp && temp->Left != NULL) {
                temp = temp->Left;
            }
            node->Uid = temp->Uid;
            node->Name = temp->Name;
			node->Right = deleteNode(node->Right, temp->Uid);

        }
        return node;
    }

    void preOrder(BSTNode* node){

        if (!node) 
            return;

        cout << node->Uid << " - " << node->Name << endl;
        preOrder(node->Left);
        preOrder(node->Right);
    }

    int countNodes(BSTNode* node){

        if (!node) 
            return 0;

        return 1 + countNodes(node->Left) + countNodes(node->Right);
    }

    bool isEmpty(){
        return root == NULL;
    }
};

struct HeapNode{

    int Uid;
    int Priority;

    HeapNode() {}

    HeapNode(int uid, int priority) : Uid(uid),Priority(priority){}

};

class MaxHeap {

public:
    //آرایه ای از نود های هیپ
    HeapNode heap[MAX];

    int count=0;

    MaxHeap(){count = 0;}

    int findIndex(int id){

        for (int i = 0; i < count; i++) {
            if (heap[i].Uid == id)
                return i;
        }
        return -1;
    }

    void swapNodes(int i, int j) {
        HeapNode temp = heap[i];
        heap[i] = heap[j];
        heap[j] = temp;
    }

    void insert(int id, int priority){
        //اضافه کردن به انتهای آرایه
        heap[count] = HeapNode(id, priority);
        int i = count;
        count++;
        //اجرای هیپیفای
        while (i > 0 && heap[i].Priority > heap[(i - 1) / 2].Priority) {
            swapNodes(i, (i - 1) / 2);
            i = (i - 1) / 2;
        }
    }

    void maxHeapify(int i){
        int largest = i;
        int l = 2 * i + 1;
        int r = 2 * i + 2;

        if (l < count && heap[l].Priority > heap[largest].Priority)
            largest = l;
        if (r < count && heap[r].Priority > heap[largest].Priority)
            largest = r;

        if (largest != i) {
            swapNodes(i, largest);
            maxHeapify(largest);
        }
    }

    void deleteMax() {
        if (count == 0) return;
        heap[0] = heap[count - 1];
        count--;
        maxHeapify(0);
    }

    void processHighestPriority(BST& bst) {
        if (count == 0) {
            cout << "Heap is empty\n";
            return;
        }
        int id = heap[0].Uid;
        cout << "Processing request with ID: " << id << endl;
        deleteMax();
        bst.root = bst.deleteNode(bst.root, id);
    }

    void printLevelOrder() {
        for (int i = 0; i < count; ++i) {
            cout << heap[i].Uid << " - Priority: " << heap[i].Priority << endl;
        }
    }

    void increasePriority(int id, int newPriority){

        int i = findIndex(id);
        if (i == -1) return;
        heap[i].Priority = newPriority;
        while (i > 0 && heap[i].Priority > heap[(i - 1) / 2].Priority) {
            swapNodes(i, (i - 1) / 2);
            i = (i - 1) / 2;
        }
    }

    bool isEmpty() {
        return count == 0;
    }

    int size() {
        return count;
    }
};

int main() {
    BST bst;
    MaxHeap heap;

    int choice;

    while (true) {
        cout << "\n-------- Request Management Menu --------\n";
        cout << "1. Insert new request\n";
        cout << "2. Search request in BST\n";
        cout << "3. Delete request\n";
        cout << "4. Process request with highest priority\n";
        cout << "5. Increase request priority\n";
        cout << "6. Display BST (Preorder)\n";
        cout << "7. Display MaxHeap (Level Order)\n";
        cout << "0. Exit\n";
        cout << "Your choice: ";
        cin >> choice;

        if (choice == 0) break;

        int id, priority;
        string name;

        switch (choice) {
        case 1:
            cout << "Enter request ID: ";
            cin >> id;
            cout << "Enter request name: ";
            cin.ignore();
            getline(cin, name);
            cout << "Enter request priority (integer): ";
            cin >> priority;

            bst.root = bst.insert(bst.root, id, name);
            heap.insert(id, priority);

            cout << "Request inserted successfully.\n";
            break;

        case 2:
            cout << "Enter ID to search: ";
            cin >> id;
            {
                BSTNode* result = bst.search(bst.root, id);
                if (result)
                    cout << "Request found: " << result->Uid << " - " << result->Name << endl;
                else
                    cout << "No request found with this ID.\n";
            }
            break;

        case 3:
            cout << "Enter ID to delete: ";
            cin >> id;
            bst.root = bst.deleteNode(bst.root, id);
            {
                int index = heap.findIndex(id);
                if (index != -1) {
                    heap.heap[index] = heap.heap[heap.count - 1];
                    heap.count--;
                    heap.maxHeapify(index);
                }
            }
            cout << "Request deleted.\n";
            break;

        case 4:
            heap.processHighestPriority(bst);
            break;

        case 5:
            cout << "Enter request ID: ";
            cin >> id;
            cout << "Enter new priority: ";
            cin >> priority;
            heap.increasePriority(id, priority);
            cout << "Priority updated.\n";
            break;

        case 6:
            cout << "BST (Preorder):\n";
            bst.preOrder(bst.root);
            break;

        case 7:
            cout << "MaxHeap contents:\n";
            heap.printLevelOrder();
            break;

        default:
            cout << "Invalid option. Please try again.\n";
        }
    }

    cout << "Exiting program.\n";
    return 0;
}
