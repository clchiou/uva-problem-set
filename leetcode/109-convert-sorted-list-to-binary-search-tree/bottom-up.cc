#include <iostream>
#include <cassert>
#include <stack>

//
// NOTE: This constructs a BST from bottom up, but it does not produce a
// balanced BST (it is a cool practice anyway).
//

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

struct IntermediateState {
    IntermediateState(int index_, TreeNode *node_) :
        index(index_), node(node_) {}
    int index;
    TreeNode *node;
};

class Solution {
public:
    TreeNode* sortedListToBST(ListNode* head) {
        if (!head) {
            return nullptr;
        }

        TreeNode *root = new TreeNode(head->val);
        int root_index = 0;
        head = head->next;

        std::stack<IntermediateState> stack;

        for (int index = 1; head; head = head->next, index++) {
            TreeNode *new_node = new TreeNode(head->val);
            if (stack.empty()) {
                // new_node is the new root.
                new_node->left = root;
                root = new_node;
                root_index = index;
                stack.emplace(root_index, root);
            } else if (index & 0x1) {
                // new_node is not a leaf.
                TreeNode *parent = stack.top().node;
                assert(parent->right);
                new_node->left = parent->right;
                parent->right = new_node;
                stack.emplace(index, new_node);
            } else {
                // new_node is a leaf.
                stack.top().node->right = new_node;
                int parent_index;
                if (index % 4 == 0) {
                    // new_node would be a left child in a full tree.
                    parent_index = index + 1;
                } else {
                    // new_node would be a right child in a full tree.
                    parent_index = index - 1;
                }
                for (int n = 1; !stack.empty(); n++) {
                    if (stack.top().index == parent_index) {
                        stack.pop();
                    } else {
                        break;
                    }
                    if ((parent_index >> n) % 4 == 2) {
                        // The former top would be right child in a full
                        // tree; let's examine whether the current top
                        // would also be its parent in a full tree.
                        parent_index -= (1 << n);
                    } else {
                        break;
                    }
                }
            }
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
