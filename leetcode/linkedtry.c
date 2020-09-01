/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, PHP, Ruby, 
C#, VB, Perl, Swift, Prolog, Javascript, Pascal, HTML, CSS, JS
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>

struct node {
    int key;
    struct node* next;
};

void reverse(struct node** head_ref) {
    struct node* curr = *head_ref;
    struct node* next = NULL;
    struct node* prev = NULL;
    while (curr != NULL) {
        printf("1) addr: prev: %p, curr: %p, next:%p\n", prev, curr, next);
        
        // can't access, seg fault - accessing NULLed out locations
        // printf("%d, %d, %d\n", prev->key, curr->key, next->key); 
        // printf("addr next: prev: %p, curr: %p, next:%p\n", prev->next, curr->next, next->next);
        next = curr->next;
        
        printf("2) addr: prev: %p, curr: %p, next:%p\n", prev, curr, next);
        // printf("%d, %d, %d\n", prev->key, curr->key, next->key);
        // printf("addr next: prev: %p, curr: %p, next:%p\n", prev->next, curr->next, next->next);
        curr->next = prev;
        
        printf("3) addr: prev: %p, curr: %p, next:%p\n", prev, curr, next);
        //printf("%d, %d, %d\n", prev->key, curr->key, next->key);
        //printf("addr next: prev: %p, curr: %p, next:%p\n", prev->next, curr->next, next->next);
        prev = curr;
        
        printf("4) addr: prev: %p, curr: %p, next:%p\n", prev, curr, next);
        printf("%d, %d, %d\n", prev->key, curr->key, next->key);
        printf("addr next: prev: %p, curr: %p, next:%p\n", prev->next, curr->next, next->next);
        curr = next;  // this is just copying the memory location and not the entire struct
        
        printf("addr: prev: %p, curr: %p, next:%p\n", prev, curr, next);
        printf("%d, %d, %d\n", prev->key, curr->key, next->key);
        printf("addr next: prev: %p, curr: %p, next:%p\n", prev->next, curr->next, next->next);
    }
    *head_ref = prev;
    

}

void push(struct node** head_ref, int key) {
    // malloc for the new head
    struct node* newhead = (struct node*) malloc(sizeof(struct node));

    // populate
    newhead->key = key;
    newhead->next = *head_ref;
    
    // update the head now
    *head_ref = newhead;
}

void printlist(struct node **head_ref) {
    printf("Bigger numbers are older lists\n");
    struct node *parser = *head_ref;
    int i = 0;
    while(parser != NULL) {
        printf("list%d: key:%d\n", i, parser->key);
        parser = parser->next;
        i++;
    }
}

int main()
{
    struct node *head = NULL;
    push(&head, 10);
    push(&head, 20);
    push(&head, 30);
    // printlist(&head);
    
    reverse(&head);
    printlist(&head);
    return 0;
}

