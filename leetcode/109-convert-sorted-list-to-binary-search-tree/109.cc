#include <iostream>

struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(NULL) {}
};

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

class Solution {
public:
    TreeNode* sortedListToBST(ListNode* head) {
        if (!head) {
            return nullptr;
        }

        ListNode *prev = nullptr;
        ListNode *slow = head;
        for (ListNode *fast = head;
                fast && fast->next;
                fast = fast->next->next) {
            prev = slow;
            slow = slow->next;
        }

        TreeNode *root = new TreeNode(slow->val);

        if (prev) {
            prev->next = nullptr;
            root->left = sortedListToBST(head);
        }
        if (slow->next) {
            root->right = sortedListToBST(slow->next);
        }

        return root;
    }
};

ListNode *make_list(int num_nodes)
{
    if (num_nodes <= 0) {
        return nullptr;
    }
    ListNode *head = new ListNode(0);
    for (ListNode *tail = head; num_nodes > 1; num_nodes--) {
        ListNode *node = new ListNode(tail->val + 1);
        tail->next = node;
        tail = node;
    }
    return head;
}

void print_tree(TreeNode *node)
{
    if (!node) {
        std::cout << "()";
    } else if (node->left || node->right) {
        std::cout << '(';
        print_tree(node->left);
        std::cout << ' ' << node->val << ' ';
        print_tree(node->right);
        std::cout << ')';
    } else {
        std::cout << node->val;
    }
}

bool is_full_tree(TreeNode *node, int value, int diff)
{
    if (!node)
        return false;
    if (node->val != value)
        return false;
    if (diff) {
        if (!is_full_tree(node->left, value - diff, diff / 2))
            return false;
        if (!is_full_tree(node->right, value + diff, diff / 2))
            return false;
    }
    return true;
}

int main()
{
    Solution solution;

    for (int num_nodes = 0; num_nodes <= 32; num_nodes++) {
        print_tree(solution.sortedListToBST(make_list(num_nodes)));
        std::cout << std::endl;
    }

    print_tree(solution.sortedListToBST(make_list(64)));
    std::cout << std::endl;

    for (int num_nodes = 3, diff = 1;
            num_nodes < 512;
            num_nodes = num_nodes * 2 + 1, diff *= 2) {
        TreeNode *root = solution.sortedListToBST(make_list(num_nodes));
        if (!is_full_tree(root, num_nodes / 2, diff)) {
            std::cout << "incorrect at: " << num_nodes << '\n';
            print_tree(root);
            std::cout << std::endl;
            break;
        }
    }

    return 0;
}
