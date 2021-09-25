#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>

//GRADING NOTES:
//
// Program meets all expectations
// Simple test cases work successfully
// Evidence that memory is re-used after free()
// Churn test case works for size 1 and 1000
// Churn2 test case works for size 1 and 1000
//
// Evidence that memory is re-used efficiently for churn, but seems to be
// signifcantly less efficient for churn2... not sure why, but no points off
//
// Score: 100/100

// define linked list
struct node{
    int size;
    int free;
    void* start;
    struct node* next;
};

// declare first node of linked list
// set it to NULL so we don't accidentally use it
struct node* head = NULL;

// iterates through linked list & checks for available memory location
// returns pointer to location if exists
struct node* check_for_free(size_t request) {
    struct node *current = head;
    while (current != NULL) {
        if (current->size >= request && current->free == 1) {
            return current;
        }
        current = current->next; 
    }
    return NULL;
}

// splits node if there is more memory than requested in that node
// we don't always necessarily have neough space to actually split
// 
void split_and_allocate(struct node* currNode, int request){
    struct node* nextNode;
    
    nextNode = currNode->start + request;
    nextNode->size = currNode->size - request - sizeof(struct node);
    nextNode->start = nextNode + 1;
    nextNode->next = currNode->next;
    nextNode->free = 1;

    currNode->size = request;
    currNode->free = 0;
    currNode->next = nextNode;
}

// returns the node that corresponds to given allocation ptr
struct node* find_node(void* ptr){
    // walk through linked list
    struct node* current = head;
    while(current != NULL) { 
        // if given address corresponds to the start of a node in the list, return that node
        if (current->start == ptr) {
                return current;
        }
        current = current->next;
    }
    return NULL;
}

// allocates new space of given size
void *malloc(size_t request){
    int remainder = request % 8;
    if (remainder) {
        request += remainder;
    }
    
    int needed_size = request + sizeof(struct node);
    int page_size = sysconf(_SC_PAGESIZE);
    int pages_needed = (needed_size/page_size) + 1;
    int total_request = pages_needed*page_size;

    // 1) if head is null, initialize head
    if (head == NULL) {
        head = sbrk(total_request);

        //Check for error
        if (head == NULL) {
            errno = ENOMEM;
            return NULL;
        }

        head->free = 1;
        head->size = total_request - sizeof(struct node);
        head->start = head + 1;
        head->next = NULL;
    }

    // 2) walk over the linked list- if an existing allocation is big enough to fulfill request, use that
    struct node *check = check_for_free(request);

    //Check for error
    if (check != NULL) {
        // allocation must be greater than request + sizeof(node) + 1
        // allocation must have at least one byte
        // TODO: we might not want to split
        if (check->size > (request + sizeof(struct node) + 1)) {
            split_and_allocate(check, request);
        }
        else {
            check->free = 0;
        }
        // otherwise return the node w/out splitting
      
        return check->start;
    }

    // 3) no existing allocation- call sbrk to add new memory, add to list, then return allocation to user
    struct node* newNode = sbrk(total_request);
    if (newNode == NULL) {
        errno = ENOMEM;
        return NULL;
    }

    // add node to end of node list
    int numNodes = 0;
    struct node *current = head;
    while (current != NULL) {
        if (current->next == NULL){
            current->next = newNode;
            break;
        }
        numNodes++;
        current = current->next;
    }

    // c pointer arithmetic- newNode + 1*sizeof(node)
    // we are looking for the start of memory space (points just past end of newNode)
    newNode->start = newNode + 1;
    newNode->size = total_request - sizeof(struct node);
    newNode->free = 1;
    newNode->next = NULL;

    if(newNode->size > (request + sizeof(struct node) + 1)){
        split_and_allocate(newNode, request);
    }
    else {
        newNode->free = 0;
    }
    return newNode->start;
}

// mark a given node as free
void free(void* ptr){
    // returns if free is called with NULL pointer
    if (ptr != NULL) {
        struct node* existing = find_node(ptr);
        // returns if free called w/ pointer not allocated by malloc
        if (existing != NULL) {
            existing->free = 1;
        }
    }
}

void* realloc(void* ptr, size_t request){
    struct node* existing = find_node(ptr);
    int existing_size = existing->size;

    if (request == existing_size){
        return ptr;
    }
    if (request < existing_size){
        void* new = malloc(request);
        memcpy(new, existing->start, request);
        free(ptr);
        return new;
    }
    if (request > existing_size) {
        void* new = malloc(request);
        memcpy(new, existing->start, existing_size);
        free(ptr);
        return new;
    }
}

// allocated some number of chunks of space of a given size, sets it all to 0
void *calloc(size_t nmemb, size_t size) {
    struct node *someNode;
    size_t request = nmemb * size;
    someNode = malloc(request);
    memset(someNode, 0, request);
}
