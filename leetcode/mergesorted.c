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
class Solution {
public:
    void push(ListNode **old, int val) {
        ListNode* list_dest = new(ListNode);
        list_dest->val = val;
        list_dest->next = *old;
        *old = list_dest;
    }
    
    void reverse(ListNode **l) {
        ListNode* curr = *l;
        ListNode* next = NULL;
        ListNode* prev = NULL;
        while(curr!=NULL) {
            next = curr->next;
            curr->next = prev;
            prev = curr;
            curr = next;
        }
        *l = prev;
    }
    
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        ListNode *l3 = NULL;
        while(1) {
            printf("l1->val: %d, l2->val: %d\n", l1->val, l2->val);
            if (l1->val < l2->val) {
                push(&l3, l1->val);
                push(&l3, l2->val);
            } else if (l1->val == l2->val) {
                push(&l3, l1->val);
                push(&l3, l2->val);
            } else {
                push(&l3, l2->val);
                push(&l3, l1->val);
            }
            if (l1->next != NULL) {
                l1 = l1->next;
            } else {
                break;
            }
            if (l2->next != NULL) {
                l2 = l2->next;
            } else {
                break;
            }
        }
        reverse(&l3);
        return l3;
    }
};
