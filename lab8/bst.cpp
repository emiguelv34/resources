#include "bst.h"

#include <stack>
#include <queue>
#include <set>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <climits>

using namespace std;

int rangeSum(Node *root, int L, int R) {
	
	//Base case - called on a null node -> return 0!	
	
	//Otherwise get the value of the left and right trees under you (recursive call)
	//Sum em
	//Should add node->key to the sum?
	//Return the sum!
	if(!root){
		return 0;
	}

	if (root->key < L) {
        return rangeSum(root->right, L, R);
    }

	if (root->key > R) {
        return rangeSum(root->left, L, R);
    }
	return root->key + rangeSum(root->left, L, R) + rangeSum(root->right, L, R);
}

int checkHeight(Node* root) {
    if (!root){
		return 0;
	}

    int leftHeight = checkHeight(root->left);
    if (leftHeight == -1){
		return -1;
	}

    int rightHeight = checkHeight(root->right);
    if (rightHeight == -1){
		return -1;
	}

    if (abs(leftHeight - rightHeight) > 1){
		return -1;
	}

    return max(leftHeight, rightHeight) + 1;
}

bool isBalanced(Node *root) {
	//Get the heights of the left and right subtrees - helper func?
	//Determine if this node is balanced! If not ret false!

	//Check if there are subtrees under us
	//Are they balanced?

	//If all nodes are balanced return true!
	return checkHeight(root) != -1;
}

/*

Given a binary tree, return the level order traversal of its nodes' values. (i.e. from left to right, level by level).


If you're given this binary tree as input:

    3
   / \
  9  20
    /  \
   15   7

return its level order traversal as:
[
  [3],
  [9,20],
  [15,7]
]

*/
vector<vector<int>> levelOrder(Node* root) {
	vector<vector<int>> results;

    if (!root) {
		return results;
	}
	queue<Node*> q;
    q.push(root);

    while (!q.empty()) {
        int levelSize = q.size();
        vector<int> level;
        for (int i = 0; i < levelSize; i++) {
            Node* node = q.front();
            q.pop();
            level.push_back(node->key);
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
        results.push_back(level);
    }
    return results;
}

