#pragma once
#include <iostream>
#include <string>

// Template class declaration for a linked list.
template <typename ItemType>
class LinkedList {
private:
    // Private nested struct for linked list nodes.
    struct Node {
        ItemType* data;  // Pointer to dynamically allocated data of type ItemType.
        Node* next;  // Pointer to the next node in the list.

        // Constructor to initialize a node with data and optionally a next node.
        Node(ItemType* dataElement, Node* nextNode = nullptr) : data(new ItemType(*dataElement)), next(nextNode) {}

        // Destructor to clean up the dynamically allocated data.
        ~Node() { delete data; }
    };

    Node* head;  // Pointer to the first node in the linked list.

    // Helper function to determine where to insert a new node in the list.
    Node* determineInsertNode(const ItemType& element) const;

public:
    // Default constructor to initialize an empty linked list.
    LinkedList();

    // Copy constructor to create a deep copy of another linked list.
    LinkedList(const LinkedList& otherLinkedList);

    // Destructor to clean up all nodes in the list.
    ~LinkedList();

    // Insert a new item into the list.
    bool Insert(ItemType* newItem);

    // Remove an item from the list, based on its value.
    bool Remove(const ItemType& nodeToRemove, ItemType& output);

    // Peek at an item in the list without removing it.
    bool Peek(const ItemType& nodeToShow, ItemType& output) const;

    // Check if the list is empty.
    bool isEmpty() const;

    // Delete all nodes in the list, effectively clearing it.
    void DeleteList();

    // Merge another linked list into this one.
    bool Merge(LinkedList& otherLinkedList);

    // Overload the assignment operator for deep copying another list into this one.
    LinkedList& operator=(const LinkedList& source);

    // Overload the equality operator to check if two lists are identical.
    bool operator==(const LinkedList& source) const;

    // Overload the inequality operator to check if two lists are not identical.
    bool operator!=(const LinkedList& source) const;

    // Overload the addition operator to concatenate another list onto this one.
    LinkedList operator+(const LinkedList& source) const;

    // Overload the compound addition (+=) operator to append another list onto this one.
    LinkedList& operator+=(const LinkedList& source);

    // Overload the stream insertion operator to output the list contents.
    template <typename T>
    friend std::ostream& operator<<(std::ostream& outputStream, const LinkedList<T>& listToPrint);
};
