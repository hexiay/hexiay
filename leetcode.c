/* 
一只青蛙一次可以跳上1级台阶，也可以跳上2级……它也可以跳上n级。求该青蛙跳上一个n级的台阶总共有多少种跳法。
链接：https://www.nowcoder.com/questionTerminal/22243d016f6b47f2a6928b4313c85387
（1）假定第一次跳的是一阶，那么剩下的是n-1个台阶，跳法是f(n-1)；假定第一次跳的是2阶，那么剩下的是n-2个台阶，跳法是f(n-2)；假定第一次跳的是3阶，那么剩下的是n-3个台阶，跳法是f(n-3)......假定第一次跳的是n-1阶，那么剩下的是1个台阶，跳法是f(1)； 假定第一次跳的是n阶，那么剩下的是0个台阶，跳法是1种；
（2）总跳法为: f(n) = 1+f(n-1) + f(n-2)+....+f(1)  （第一个1是跳n阶只有一种方法）
（3）根据（2）可以得出有一阶的时候 f(1) = 1 ；有两阶的时候可以有 f(2) = 1+f(1)=2；有三阶的时候可以有 f(3) = 1+f(2)+f(1)=4...依次内推，有n阶时f(n)=2^(n-1)。
为了加快运算速度，可以通过向左移移位来完成乘以2的工作：
*/
int jumpFloorII(int number) {
        //通过移位计算2的次方
        return 1<<(number-1);        
}

/*
501. Find Mode in Binary Search Tree

Given a binary search tree (BST) with duplicates, find all the mode(s) (the most frequently occurred element) in the given BST.

Assume a BST is defined as follows:

The left subtree of a node contains only nodes with keys less than or equal to the node's key.
The right subtree of a node contains only nodes with keys greater than or equal to the node's key.
Both the left and right subtrees must also be binary search trees.
 
For example:
Given BST [1,null,2,2],

   1
    \
     2
    /
   2
 
return [2].

Note: If a tree has more than one mode, you can return them in any order.

Follow up: Could you do that without using any extra space? (Assume that the implicit stack space incurred due to recursion does not count). 

这道题让我们求二分搜索树中的众数，这里定义的二分搜索树中左根右结点之间的关系是小于等于的，有些题目中是严格小于的，所以一定要看清题目要求。所谓的众数就是出现最多次的数字，可以有多个，那么这道题比较直接点思路就是利用一个哈希表来记录数字和其出现次数之前的映射，然后维护一个变量mx来记录当前最多的次数值，这样在遍历完树之后，根据这个mx值就能把对应的元素找出来。那么用这种方法的话就不需要用到二分搜索树的性质了，随意一种遍历方式都可以，下面我们来看递归的中序遍历的解法如下：
*/

class Solution {
public:
    vector<int> findMode(TreeNode* root) {
        vector<int> res;
        int mx = 0;
        unordered_map<int, int> m; 
        inorder(root, m, mx);
        for (auto a : m) {
            if (a.second == mx) {
                res.push_back(a.first);
            }
        }
        return res;
    }
    void inorder(TreeNode* node, unordered_map<int, int>& m, int& mx) {
        if (!node) return;
        inorder(node->left, m, mx);
        mx = max(mx, ++m[node->val]);
        inorder(node->right, m, mx);
    }
};
/*
题目中的follow up说了让我们不用除了递归中的隐含栈之外的额外空间，那么我们就不能用哈希表了，不过这也不难，由于是二分搜索树，那么我们中序遍历出来的结果就是有序的，这样我们只要比较前后两个元素是否相等，就等统计出现某个元素出现的次数，因为相同的元素肯定是都在一起的。我们需要一个结点变量pre来记录上一个遍历到的结点，然后mx还是记录最大的次数，cnt来计数当前元素出现的个数，我们在中序遍历的时候，如果pre不为空，说明当前不是第一个结点，我们和之前一个结点值比较，如果相等，cnt自增1，如果不等，cnt重置1。如果此时cnt大于了mx，那么我们清空结果res，并把当前结点值加入结果res，如果cnt等于mx，那我们直接将当前结点值加入结果res，然后mx赋值为cnt。最后我们要把pre更新为当前结点，参见代码如下：
*/
class Solution {
public:
    vector<int> findMode(TreeNode* root) {
        vector<int> res;
        int mx = 0, cnt = 1;
        TreeNode *pre = NULL;
        inorder(root, pre, cnt, mx, res);
        return res;
    }
    void inorder(TreeNode* node, TreeNode*& pre, int& cnt, int& mx, vector<int>& res) {
        if (!node) return;
        inorder(node->left, pre, cnt, mx, res);
        if (pre) {
            cnt = (node->val == pre->val) ? cnt + 1 : 1;
        }
        if (cnt >= mx) {
            if (cnt > mx) res.clear();
            res.push_back(node->val);
            mx = cnt;
        } 
        pre = node;
        inorder(node->right, pre, cnt, mx, res);
    }
};
/*
502. IPO
Suppose LeetCode will start its IPO soon. In order to sell a good price of its shares to Venture Capital, LeetCode would like to work on some projects to increase its capital before the IPO. Since it has limited resources, it can only finish at most k distinct projects before the IPO. Help LeetCode design the best way to maximize its total capital after finishing at most k distinct projects.

You are given several projects. For each project i, it has a pure profit Pi and a minimum capital of Ci is needed to start the corresponding project. Initially, you have W capital. When you finish a project, you will obtain its pure profit and the profit will be added to your total capital.

To sum up, pick a list of at most k distinct projects from given projects to maximize your final capital, and output your final maximized capital.

Example 1:
Input: k=2, W=0, Profits=[1,2,3], Capital=[0,1,1].

Output: 4

Explanation: Since your initial capital is 0, you can only start the project indexed 0.
             After finishing it you will obtain profit 1 and your capital becomes 1.
             With capital 1, you can either start the project indexed 1 or the project indexed 2.
             Since you can choose at most 2 projects, you need to finish the project indexed 2 to get the maximum capital.
             Therefore, output the final maximized capital, which is 0 + 1 + 3 = 4.
             Note:
You may assume all numbers in the input are non-negative integers.
The length of Profits array and Capital array will not exceed 50,000.
The answer is guaranteed to fit in a 32-bit signed integer.
The idea is each time we find a project with max profit and within current capital capability.
Algorithm:

Create (capital, profit) pairs and put them into PriorityQueue pqCap. This PriorityQueue sort by capital increasingly.
Keep polling pairs from pqCap until the project out of current capital capability. Put them into
PriorityQueue pqPro which sort by profit decreasingly.
Poll one from pqPro, it's guaranteed to be the project with max profit and within current capital capability. Add the profit to capital W.
Repeat step 2 and 3 till finish k steps or no suitable project (pqPro.isEmpty()).
Time Complexity: For worst case, each project will be inserted and polled from both PriorityQueues once, so the overall runtime complexity should be O(NlgN), N is number of projects.
*/
class Solution {
public:
    int findMaximizedCapital(int k, int W, vector<int>& Profits, vector<int>& Capital) {
        vector<pair<int, int>> v;
        for (int i = 0; i < Capital.size(); ++i) {
            v.push_back({Capital[i], Profits[i]});
        }
        sort(v.begin(), v.end());
        for (int i = 0; i < k; ++i) {
            int left = 0, right = v.size(), mx = 0, idx = 0;
            while (left < right) {
                int mid = left + (right - left) / 2;
                if (v[mid].first <= W) left = mid + 1;
                else right = mid;
            }
            for (int j = right - 1; j >= 0; --j) {
                if (mx < v[j].second) {
                    mx = v[j].second;
                    idx = j;
                }
            }
            W += mx;
            v.erase(v.begin() + idx);
        }
        return W;
    }
};
