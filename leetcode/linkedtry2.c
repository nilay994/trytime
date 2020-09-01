#include <math.h>
#include <stdio.h>
#include <stdlib.h>

struct ListNode {
	int val;
	struct ListNode *next;
};

typedef struct ListNode ListNode;

void push(ListNode **list, int val) {
	ListNode *newlist = (ListNode*) malloc(sizeof(ListNode));
	newlist->val  = val;
	newlist->next = *list;
	*list = newlist;
}

ListNode* add(ListNode *l1, ListNode *l2) {

	ListNode *list_l1 = l1;
	ListNode *list_l2 = l2;
	ListNode *list_l3 = NULL; //(ListNode*) malloc(sizeof(ListNode));
	
	int num1 = 0;
	int num2 = 0;
	
	for (int i = 0; i < 3; i++) {    
		num1 += l1->val * pow(10, i);
		l1 = l1->next;
		num2 += l2->val * pow(10, i);
		l2 = l2->next;
	}
	int sum = num1 + num2;
	int digit = 0;
	for (int i = 0; i < 3; i++) {
		int rem = (int) pow(10, i+1);
		digit = (sum % rem) / pow(10, i);
		printf("digit %d: %d\n", i, digit);
		push(&list_l3, digit);
	}

	return list_l3;
}

void parse(ListNode **list) {
	ListNode *parser = *list;

	while(parser != NULL) {
		int val = parser->val;
		parser = parser->next;
		printf("val: %d\n", val);
	}
}

void main() {
	ListNode *listmain = NULL;
	// print the value of the ptr, which is the addr of listmain..
	// should give a segfault?!
	printf("addr: 0x%06x\n", listmain);

	// since the ptr to list is going to change after pushing a val, pass argument by ref
	push(&listmain, 1);

	// push did a malloc and finally gave back a valid memlocation to listmain
	printf("addr: 0x%06x\n", listmain);


	push(&listmain, 2);
	printf("addr: 0x%06x\n", listmain);
	push(&listmain, 3);
	printf("addr: 0x%06x\n", listmain);

	printf("\n-------------listmain-----------------\n");
	parse(&listmain);

	ListNode *result = NULL;
	printf("\n--------------add----------------\n");
	result = add(listmain, listmain);

	printf("\n------------result------------------\n");
	parse(&result);
}
