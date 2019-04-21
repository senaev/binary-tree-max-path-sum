#include <iostream>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

class Solution {
public:
    int maxPathSum(TreeNode* root) {
        return 1;
    }
};

tuple<int, int, int> getLevelParamsByIndex(int index) {
    int level = 0;
    int topNodesCount = 0;

    while (true)
    {
        int nodesOnLevel = pow(2, level);
        int nextTopNodesCount = topNodesCount + nodesOnLevel;
        int positionOnLevel = index - topNodesCount;

        if (index < nextTopNodesCount) {
            return { level, topNodesCount, positionOnLevel };
        }

        topNodesCount = nextTopNodesCount;
        level ++;
    }
}

vector<bool> getPathByIndex(int index) {
    auto [level, topNodesCount, positionOnLastLevel] = getLevelParamsByIndex(index);

    vector<bool> path;

    for (int i = level; i > 0; i--) {
        int nodesCountOnLevel = pow(2, i);
        int positionOnLevel = positionOnLastLevel % nodesCountOnLevel;

        bool isRight = positionOnLevel >= (nodesCountOnLevel / 2);
        path.push_back(isRight);
    }

    return path;
}

int getLeftChildIndex(int index) {
    auto [level, topNodesCount, positionOnLevel] = getLevelParamsByIndex(index);

    return topNodesCount + pow(2, level) + positionOnLevel * 2;
}

TreeNode* getNodeByPath(TreeNode* root, vector<bool> path) {
    TreeNode* node = root;

    for (int key = 0; key < path.size(); key++) {
        bool isRight = path[key];
        node = isRight
            ? node -> right
            : node -> left;
    }

    return node;
}

TreeNode* getNodeByIndex(TreeNode* root, int index) {
    vector<bool> nodePath = getPathByIndex(index);
    return getNodeByPath(root, nodePath);
}

TreeNode* constructTree(vector<int> treeMap) {
    TreeNode* root = new TreeNode(treeMap[0]);

    for (int key = 0; key < treeMap.size(); key ++) {
        int leftChildIndex = getLeftChildIndex(key);

        if (leftChildIndex >= treeMap.size()) {
            continue;
        }

        TreeNode* node = getNodeByIndex(root, key);

        node -> left = new TreeNode(treeMap[leftChildIndex]);

        if (leftChildIndex + 1 >= treeMap.size()) {
            continue;
        }

        node -> right = new TreeNode(treeMap[leftChildIndex + 1]);
    }

    return root;
}

void visualizeTree(TreeNode* root) {
    int i = 0;
    while (true) {
        TreeNode* node = getNodeByIndex(root, i);

        if (!node) {
            return;
        }

        cout << node -> val;

        auto [
            level,
            topNodesCount,
            positionOnLevel
        ] = getLevelParamsByIndex(i);

        bool isLastElementOnLevel = positionOnLevel + 1 == pow(2, level);

        if (isLastElementOnLevel) {
            cout << endl;
        } else {
            cout << ' ';
        }

        i++;
    }
}

int main()
{
    vector<int> treeMap { -10, 9, 20, 0, 0, 15 , 7 };

    TreeNode* root = constructTree(treeMap);

    visualizeTree(root);
}