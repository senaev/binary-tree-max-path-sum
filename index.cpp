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
            cout << endl;
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

tuple<int, int> maxForTree(TreeNode* root, int currentMaximum) {
    const int val = root -> val;

    if (!root -> left && !root -> right) {
        return {val, val};
    }

    if (!root -> left || !root -> right) {
        TreeNode* branchNode = root -> left
            ? root -> left
            : root -> right;

        auto [branchPathMax, branchMax] = maxForTree(branchNode, currentMaximum);

        int sum = val + branchMax;
        int maxPath = max({val, sum, branchPathMax});
        int maxBranch = max({val, sum});

        return {maxPath, maxBranch};
    }

    int leftMax;
    int leftBranchMax;
    int rightMax;
    int rightBranchMax;

    if (root -> left) {
        auto [_leftMax, _leftBranchMax] = maxForTree(root -> left, currentMaximum);
        leftMax = _leftMax;
        leftBranchMax = _leftBranchMax;
    }

    if (root -> right) {
        auto [_rightMax, _rightBranchMax] = maxForTree(root -> right, currentMaximum);
        rightMax = _rightMax;
        rightBranchMax = _rightBranchMax;
    }

    int rootMaxPath = leftBranchMax + rightBranchMax + val;

    int leftToTop = leftBranchMax + val;
    int rightToTop = rightBranchMax + val;

    int maxPath = max({val, leftMax, rightMax, leftToTop, rightToTop, rootMaxPath});
    int branchMax = max({val, leftToTop, rightToTop});

    return { maxPath, branchMax };
}

int main()
{
    vector<int> treeMap { 
                      0,
          1,                    0, 
  1,            1,          0,        0, 
0, -1,        0, 0,       0, 0,      0, 0
         };
    TreeNode* root1 = constructTree(treeMap);
    auto [firstMax, zero] = maxForTree(root1, INT_MIN);
    if (firstMax != 3) {
        cout << "!!!!!!!!!!!!!!!!!!!!!first error " << firstMax << endl;
        throw 0;
    }

    vector<int> treeMap2 {
                             0,
            1,                                0,
      1,              1,           0,                   2,
   0,    -1,         0,    0,     0,    0,            0,       0,
};

    TreeNode* root2 = constructTree(treeMap2);
    auto [secondMax, zero2] = maxForTree(root2, INT_MIN);
    if (secondMax != 4) {
        cout << "!!!!!!!!!!!!!!!!!!!!!second error" << endl;
        throw 0;
    }

    vector<int> treeMap3 {1, 2, 3};

    TreeNode* root3 = constructTree(treeMap3);
    auto [thirdMax, zero3] = maxForTree(root3, INT_MIN);
    if (thirdMax != 6) {
        cout << "!!!!!!!!!!!!!!!!!!!!!third error" << endl;
        throw 0;
    }

    vector<int> treeMap4 {-2, -1};

    TreeNode* root4 = constructTree(treeMap4);
    auto [fourthMax, zero4] = maxForTree(root4, INT_MIN);
    if (fourthMax != -1) {
        cout << "!!!!!!!!!!!!!!!!!!!!!forth error" << endl;
        throw 0;
    }

    vector<int> treeMap5 {1, 2};

    TreeNode* root5 = constructTree(treeMap5);
    auto [fifthMax, zero5] = maxForTree(root5, INT_MIN);
    if (fifthMax != 3) {
        cout << "!!!!!!!!!!!!!!!!!!!!!fifth error" << endl;
        throw 0;
    }

    vector<int> treeMap6 {2, -1};

    TreeNode* root6 = constructTree(treeMap6);
    auto [sixthMax, zero6] = maxForTree(root6, INT_MIN);
    if (sixthMax != 2) {
        cout << "!!!!!!!!!!!!!!!!!!!!!sixthMax error" << endl;
        throw 0;
    }

    vector<int> treeMap7 {
                5,
         4,            8,
    11,      0,     13,     4,
   7, 2,   0,  0,  0, 0,  0,   1
   };

    TreeNode* root7 = constructTree(treeMap7);
    auto [seventhMax, zero7] = maxForTree(root7, INT_MIN);
    if (seventhMax != 48) {
        cout << "!!!!!!!!!!!!!!!!!!!!!seventhMax error " << seventhMax << endl;
        throw 0;
    }

    vector<int> treeMap8 {1,-2,3};

    TreeNode* root8 = constructTree(treeMap8);
    auto [eighthMax, zero8] = maxForTree(root8, INT_MIN);
    if (eighthMax != 4) {
        cout << "!!!!!!!!!!!!!!!!!!!!!eighthMax error " << eighthMax << endl;
        throw 0;
    }

    vector<int> treeMap9 {2,-1, -2};

    TreeNode* root9 = constructTree(treeMap9);
    auto [ninthMax, zero9] = maxForTree(root9, INT_MIN);
    if (ninthMax != 2) {
        cout << "!!!!!!!!!!!!!!!!!!!!!ninthMax error " << ninthMax << endl;
        throw 0;
    }

    vector<int> treeMap10 {
                                    -1,
                5,                                      0,
         4,              0,                         0,          0,
     0,         2,     0,     0,            0,           0,     0,   0,
0,      0,    -4
};

    TreeNode* root10 = constructTree(treeMap10);
    auto [tenthMax, zero10] = maxForTree(root10, INT_MIN);
    if (tenthMax != 11) {
        cout << "!!!!!!!!!!!!!!!!!!!!!tenthMax error " << tenthMax << endl;
        throw 0;
    }

    cout << tenthMax << endl;
}