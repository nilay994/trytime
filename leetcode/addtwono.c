/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
#include <stdio.h>
#include <stdlib.h>

class Solution {
public:

    void push(ListNode **list, int val) {
        // (ListNode*) malloc(sizeof(ListNode));
        ListNode *newlist = new ListNode; // use new, malloc not allowed
        newlist->val = val;
        newlist->next = *list;
        *list = newlist;
    }

    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        ListNode *list_l1 = l1;
        ListNode *list_l2 = l2;
        ListNode *list_l3 = NULL;

        int num1 = 0;
        int num2 = 0;

        for (int i = 0; i < 3; i++) {
            num1 += l1->val * pow(10, i);
            l1 = l1->next;
            num2 += l2->val * pow(10, i);
            l2 = l2->next;
        }
        // DBG: printf("%d, %d\n", num1, num2);
        int sum = num1 + num2;

        // count number of digits
        int numdigits = 0;
        int tempvar = sum;
        while(tempvar != 0) {
            numdigits = numdigits + 1;
            tempvar = tempvar / 10;
        }
        int digit = 0;
        // reverse the output list
        for (int i = (numdigits-1); i >= 0; i--) {
            int rem = (int) pow(10, i+1);
            digit = (sum % rem) / pow(10, i);
            push(&list_l3, digit);
        }
        return list_l3;
    }
};
