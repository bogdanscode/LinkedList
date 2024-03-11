#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// Template class for a linked list that can store items of any type.
template <typename ItemType>
class LinkedList {
private:
    // Internal structure for storing linked list nodes.
    struct Node {
        ItemType* data;  // Pointer to data of type ItemType.
        Node* next;  // Pointer to the next node in the list.

        // Constructor to initialize a node with data and optionally a next node.
        Node(ItemType* dataElement, Node* nextNode = nullptr) : data(new ItemType(*dataElement)), next(nextNode) {}
        // Destructor to clean up dynamically allocated data.
        ~Node() { delete data; }
    };

    Node* head;  // Pointer to the first node in the linked list.

    // Helper function to determine the node after which a new item should be inserted.
    Node* determineInsertNode(const ItemType& element) const {
        Node* curNode = head;
        Node* prevNode = nullptr;
        // Traverse the list to find the correct insertion point.
        while (curNode != nullptr && *curNode->data < element) {
            prevNode = curNode;
            curNode = curNode->next;
        }
        return prevNode;
    }

public:
    // Constructor to initialize an empty list.
    LinkedList() : head(nullptr) {}

    // Copy constructor to initialize the list with the contents of another list.
    LinkedList(const LinkedList<ItemType>& otherLinkedList) : head(nullptr) {
        Node* curNode = otherLinkedList.head;
        // Insert each element from the other list into this one.
        while (curNode != nullptr) {
            Insert(curNode->data);
            curNode = curNode->next;
        }
    }

    // Destructor to clean up all nodes in the list.
    ~LinkedList() {
        DeleteList();
    }

    // Insert a new item into the list in a sorted order.
    bool Insert(ItemType* newItem) {
        if (newItem == nullptr) return false;  // Null check.

        // Search for duplicate.
        Node* curNode = head;
        while (curNode != nullptr) {
            if (*curNode->data == *newItem) {
                return false;  // Duplicate found, do not insert.
            }
            curNode = curNode->next;
        }

        Node* index = determineInsertNode(*newItem);  // Find insertion point.

        // Insert at the beginning or in the middle/end.
        if (index == nullptr) {
            head = new Node(newItem, head);
        }
        else {
            index->next = new Node(newItem, index->next);
        }
        return true;
    }


    // Build the list from a file, inserting each item read from the file.
    bool BuildList(string fileName) {
        ifstream file(fileName);
        if (!file) return false;  // File open check.

        ItemType temp;
        while (file >> temp) {  // Read each item.
            if (!Insert(&temp)) {
                // Handle insertion failure if needed.
            }
        }
        file.close();
        return true;
    }

    // Remove an item from the list, matching by value.
    bool Remove(const ItemType& nodeToRemove, ItemType& output) {
        Node* curNode = head;
        Node* prevNode = nullptr;

        // Traverse the list to find the item to remove.
        while (curNode != nullptr) {
            if (*curNode->data == nodeToRemove) {
                // Adjust pointers to bypass the node to be removed.
                if (prevNode == nullptr) {
                    head = curNode->next;
                }
                else {
                    prevNode->next = curNode->next;
                }
                output = *curNode->data;  // Set output to the removed item.
                delete curNode;  // Free the node.
                return true;
            }
            prevNode = curNode;
            curNode = curNode->next;
        }

        return false;  // Item not found.
    }

    // Search for an item in the list, setting output to the item if found.
    bool Peek(const ItemType& nodeToShow, ItemType& output) const {
        Node* curNode = head;

        // Traverse the list to find the nodeToShow item.
        while (curNode != nullptr) {
            if (*curNode->data == nodeToShow) {
                output = *curNode->data;  // Set output to the found item.
                return true;
            }
            curNode = curNode->next;
        }

        return false;  // Item not found.
    }

    // Check if the list is empty.
    bool isEmpty() const {
        return head == nullptr;
    }

    // Delete all nodes in the list, freeing memory.
    void DeleteList() {
        Node* curNode = head;
        while (curNode != nullptr) {
            Node* next = curNode->next;
            delete curNode;  // Free the node.
            curNode = next;
        }
        head = nullptr;  // Reset head to indicate the list is empty.
    }

    // Merge another list into this one, clearing the other list.
    bool Merge(LinkedList& otherLinkedList) {
        if (this == &otherLinkedList) return false;  // Prevent merging with self.

        Node* curNode = otherLinkedList.head;
        // Insert each item from the other list into this one.
        while (curNode != nullptr) {
            ItemType* temp = new ItemType(*curNode->data);
            if (!Insert(temp)) {
                delete temp;  // Handle insertion failure.
            }
            curNode = curNode->next;
        }

        otherLinkedList.DeleteList();  // Clear the other list.
        return true;
    }

    // Output the list contents to an output stream.
    friend ostream& operator<<(ostream& outputStream, const LinkedList<ItemType>& listToPrint) {
        Node* curNode = listToPrint.head;
        // Traverse the list, printing each item.
        while (curNode != nullptr) {
            outputStream << *curNode->data;
            if (curNode->next != nullptr) outputStream << " ";
            curNode = curNode->next;
        }
        return outputStream;
    }

    // Assign the contents of another list to this one.
    LinkedList& operator=(const LinkedList& source) {
        if (this != &source) {  // Prevent self-assignment.
            DeleteList();  // Clear current list.

            // Copy each item from the source list.
            Node* curNode = source.head;
            while (curNode != nullptr) {
                Insert(new ItemType(*curNode->data));
                curNode = curNode->next;
            }
        }
        return *this;
    }

    // Compare two lists for equality.
    bool operator==(const LinkedList& source) const {
        Node* curNode1 = head;
        Node* curNode2 = source.head;

        // Traverse both lists in parallel, comparing each pair of items.
        while (curNode1 != nullptr && curNode2 != nullptr) {
            if (*curNode1->data != *curNode2->data) {
                return false;
            }
            curNode1 = curNode1->next;
            curNode2 = curNode2->next;
        }

        // Lists are equal if both have reached their ends.
        return curNode1 == nullptr && curNode2 == nullptr;
    }

    // Compare two lists for inequality.
    bool operator!=(const LinkedList& source) const {
        return !(*this == source);
    }

    // Concatenate another list onto this one, returning a new list.
    LinkedList operator+(const LinkedList& source) const {
        LinkedList output = *this;  // Start with a copy of this list.
        Node* curNode = source.head;
        // Insert each item from the source list.
        while (curNode != nullptr) {
            ItemType* itemCopy = new ItemType(*curNode->data);
            if (!output.Insert(itemCopy)) {
                delete itemCopy;  // Handle insertion failure.
            }
            curNode = curNode->next;
        }
        return output;
    }

    // Add another list onto this one.
    LinkedList& operator+=(const LinkedList& source) {
        Node* curNode = source.head;
        // Insert each item from the source list.
        while (curNode != nullptr) {
            ItemType* itemCopy = new ItemType(*curNode->data);
            if (!Insert(itemCopy)) {
                delete itemCopy;  // Handle insertion failure.
            }
            curNode = curNode->next;
        }
        return *this;
    }
};  
