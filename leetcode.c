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
/*
503. Next Greater Element II
Given a circular array (the next element of the last element is the first element of the array), print the Next Greater Number for every element. The Next Greater Number of a number x is the first greater number to its traversing-order next in the array, which means you could search circularly to find its next greater number. If it doesn't exist, output -1 for this number.

Example 1:
Input: [1,2,1]
Output: [2,-1,2]
Explanation: The first 1's next greater number is 2; 
The number 2 can't find next greater number; 
The second 1's next greater number needs to search circularly, which is also 2.
Note: The length of given array won't exceed 10000.
*/
class Solution {
public:
    vector<int> nextGreaterElements(vector<int>& nums) {
        int n = nums.size();
        vector<int> res(n, -1);
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < i + n; ++j) {
                if (nums[j % n] > nums[i]) {
                    res[i] = nums[j % n];
                    break;
                }
            }
        }
        return res;
    }
};
/*
504. Base 7
Given an integer, return its base 7 string representation.

Example 1:
Input: 100
Output: "202"
Example 2:
Input: -7
Output: "-10"
Note: The input will be in range of [-1e7, 1e7].
*/
class Solution {
public:
    string convertToBase7(int num) {
        if (num == 0)
            return "0";
        string res;
        bool positive = (num > 0);
        while (num != 0) {
            res = to_string(abs(num) % 7) + res;
            num = num / 7;
        }
        if (positive)
            return res;
        else
            return "-" + res;
    }
};
/*
505.	The Maze II
There is a ball in a maze with empty spaces and walls. The ball can go through empty spaces by rolling up, down, left or right, but it won't stop rolling until hitting a wall. When the ball stops, it could choose the next direction.

Given the ball's start position, the destination and the maze, find the shortest distance for the ball to stop at the destination. The distance is defined by the number of empty spaces traveled by the ball from the start position (excluded) to the destination (included). If the ball cannot stop at the destination, return -1.

The maze is represented by a binary 2D array. 1 means the wall and 0 means the empty space. You may assume that the borders of the maze are all walls. The start and destination coordinates are represented by row and column indexes.

Example 1

Input 1: a maze represented by a 2D array

0 0 1 0 0
0 0 0 0 0
0 0 0 1 0
1 1 0 1 1
0 0 0 0 0

Input 2: start coordinate (rowStart, colStart) = (0, 4)
Input 3: destination coordinate (rowDest, colDest) = (4, 4)

Output: 12
Explanation: One shortest way is : left -> down -> left -> down -> right -> down -> right.
             The total distance is 1 + 1 + 3 + 1 + 2 + 2 + 2 = 12.
这道题是之前那道The Maze的拓展，那道题只让我们判断能不能在终点位置停下，而这道题让我们求出到达终点的最少步数。其实本质都是一样的，难点还是在于对于一滚到底的实现方法，唯一不同的是，这里我们用一个二位数组dists，其中dists[i][j]表示到达(i,j)这个位置时需要的最小步数，我们都初始化为整型最大值，在后在遍历的过程中不断用较小值来更新每个位置的步数值，最后我们来看终点位置的步数值，如果还是整型最大值的话，说明没法在终点处停下来，返回-1，否则就返回步数值。注意在压入栈的时候，我们对x和y进行了判断，只有当其不是终点的时候才压入栈，这样是做了优化，因为如果小球已经滚到终点了，我们就不要让它再滚了，就不把终点位置压入栈，免得它还滚，参见代码如下：
*/
class Solution {
public:
    int shortestDistance(vector<vector<int>>& maze, vector<int>& start, vector<int>& destination) {
        int m = maze.size(), n = maze[0].size();
        vector<vector<int>> dists(m, vector<int>(n, INT_MAX));
        vector<vector<int>> dirs{{0,-1},{-1,0},{0,1},{1,0}};
        queue<pair<int, int>> q;
        q.push({start[0], start[1]});
        dists[start[0]][start[1]] = 0;
        while (!q.empty()) {
            auto t = q.front(); q.pop();
            for (auto d : dirs) {
                int x = t.first, y = t.second, dist = dists[t.first][t.second];
                while (x >= 0 && x < m && y >= 0 && y < n && maze[x][y] == 0) {
                    x += d[0];
                    y += d[1];
                    ++dist;
                }
                x -= d[0];
                y -= d[1];
                --dist;
                if (dists[x][y] > dist) {
                    dists[x][y] = dist;
                    if (x != destination[0] || y != destination[1]) q.push({x, y});
                }
            }
        }
        int res = dists[destination[0]][destination[1]];
        return (res == INT_MAX) ? -1 : res;
    }
};
/*
506. Relative Ranks
Given scores of N athletes, find their relative ranks and the people with the top three highest scores, who will be awarded medals: "Gold Medal", "Silver Medal" and "Bronze Medal".

Example 1:
Input: [5, 4, 3, 2, 1]
Output: ["Gold Medal", "Silver Medal", "Bronze Medal", "4", "5"]
Explanation: The first three athletes got the top three highest scores, so they got "Gold Medal", "Silver Medal" and "Bronze Medal". 
For the left two athletes, you just need to output their relative ranks according to their scores.
Note:
N is a positive integer and won't exceed 10,000.
All the scores of athletes are guaranteed to be unique.
*/
class Solution {
public:
    vector<string> findRelativeRanks(vector<int>& nums) {
        int n = nums.size(), cnt = 1;
        vector<string> res(n, "");
        map<int, int> m;
        for (int i = 0; i < n; ++i) {
            m[nums[i]] = i;
        }
        for (auto it = m.rbegin(); it != m.rend(); ++it) {
            if (cnt == 1) res[it->second] = "Gold Medal";
            else if (cnt == 2) res[it->second] = "Silver Medal";
            else if (cnt == 3) res[it->second] = "Bronze Medal";
            else res[it->second] = to_string(cnt);
            ++cnt;
        }
        return res;
    }
};
/*
507. Perfect Number
We define the Perfect Number is a positive integer that is equal to the sum of all its positive divisors except itself.

Now, given an integer n, write a function that returns true when it is a perfect number and false when it is not.
Example:
Input: 28
Output: True
Explanation: 28 = 1 + 2 + 4 + 7 + 14
Note: The input number n will not exceed 100,000,000. (1e8)
*/
class Solution {
public:
    bool checkPerfectNumber(int num) {
        if (num == 1) return false;
        int sum = 1;
        for (int i = 2; i * i <= num; ++i) {
            if (num % i == 0) sum += (i + num / i);
            if (i * i == num) sum -= i;
            if (sum > num) return false;
        }
        return sum == num;
    }
};
/*
508. Most Frequent Subtree Sum
Given the root of a tree, you are asked to find the most frequent subtree sum. The subtree sum of a node is defined as the sum of all the node values formed by the subtree rooted at that node (including the node itself). So what is the most frequent subtree sum value? If there is a tie, return all the values with the highest frequency in any order.

Examples 1
Input:

  5
 /  \
2   -3
return [2, -3, 4], since all the values happen only once, return all of them in any order.
Examples 2
Input:

  5
 /  \
2   -5
return [2], since 2 happens twice, however -5 only occur once.
Note: You may assume the sum of values in any subtree is in the range of 32-bit signed integer.
*/
class Solution {
public:
    vector<int> findFrequentTreeSum(TreeNode* root) {
        vector<int> res;
        unordered_map<int, int> m;
        int cnt = 0;
        postorder(root, m, cnt);
        for (auto a : m) {
            if (a.second == cnt) res.push_back(a.first);
        }
        return res;
    }
    int postorder(TreeNode* node, unordered_map<int, int>& m, int& cnt) {
        if (!node) return 0;
        int left = postorder(node->left, m, cnt);
        int right = postorder(node->right, m, cnt);
        int sum = left + right + node->val;
        cnt = max(cnt, ++m[sum]);
        return sum;
    }
};
/*
513. Find Bottom Left Tree Value
Given a binary tree, find the leftmost value in the last row of the tree.
Example 1:
Input:
    2
   / \
  1   3
Output:
1

Example 2: 
Input:
        1
       / \
      2   3
     /   / \
    4   5   6
       /
      7
Output:
7
Note: You may assume the tree (i.e., the given root node) is not NULL
*/
class Solution {
public:
    int findBottomLeftValue(TreeNode* root) {
        if (!root) return 0;
        int res = 0;
        queue<TreeNode*> q;
        q.push(root);
        while (!q.empty()) {
            int n = q.size();
            for (int i = 0; i < n; ++i) {
                TreeNode *t = q.front(); q.pop();
                if (i == 0) res = t->val;
                if (t->left) q.push(t->left);
                if (t->right) q.push(t->right);
            }
        }
        return res;
    }
};
/*
514. Freedom Trail
题目来自于辐射4这个游戏。有一个圆盘，圆盘上有几个英文字母，圆盘12点方向代表当前选中的字母，通过这个圆盘拼出一个和题目一样的字符串，求出最小的步数。
圆盘可以顺时针或者逆时针旋转，每旋转一个字母代表1步，当12点方向是需要的英文字母时，按确定按钮，这个按也算一步。
*/

/*
515. Find Largest Value in Each Tree Row
You need to find the largest value in each row of a binary tree.
*/
class Solution {
public:
    vector<int> largestValues(TreeNode* root) {
        if (!root) return {};
        vector<int> res;
        helper(root, 1, res);
        return res;
    }
    void helper(TreeNode* root, int depth, vector<int>& res) {
        if (depth > res.size()) res.push_back(root->val);
        else res[depth - 1] = max(res[depth - 1], root->val);
        if (root->left) helper(root->left, depth + 1, res);
        if (root->right) helper(root->right, depth + 1, res);
    }
};
/*
516. Longest Palindromic Subsequence
设字符串为str，长度为n，p[i][j]表示第i到第j个字符间的子序列的个数（i<=j），则：
状态初始条件：dp[i][i]=1 （i=0：n-1）
状态转移方程：dp[i][j]=dp[i+1][j-1] + 2  if（str[i]==str[j]）
          dp[i][j]=max(dp[i+1][j],dp[i][j-1])  if （str[i]!=str[j]）
以下代码中的两层循环变量i,j的顺序可以改变，但必须满足i<=j的条件。
计算dp[i][j]时需要计算dp[i+1][*]或dp[*][j-1]，因此i应该从大到小，即递减；j应该从小到大，即递增。
*/
class Solution {
public:
    int longestPalindromeSubseq(string s) {
        int n = s.size();
        vector<vector<int>> dp(n, vector<int>(n));
        for (int i = n - 1; i >= 0; --i) {
            dp[i][i] = 1;
            for (int j = i + 1; j < n; ++j) {
                if (s[i] == s[j]) {
                    dp[i][j] = dp[i + 1][j - 1] + 2;
                } else {
                    dp[i][j] = max(dp[i + 1][j], dp[i][j - 1]);
                }
            }
        }
        return dp[0][n - 1];
    }
};
class Solution {
public:
    int longestPalindromeSubseq(string s) {
        int n = s.size(), res = 0;
        vector<int> dp(n, 1);
        for (int i = n - 1; i >= 0; --i) {
            int len = 0;
            for (int j = i + 1; j < n; ++j) {
                int t = dp[j];
                if (s[i] == s[j]) {
                    dp[j] = len + 2;
                } 
                len = max(len, t);
            }
        }
        for (int num : dp) res = max(res, num);
        return res;
    }
};
/*
回文子序列个数
要求：
给定字符串，求它的回文子序列个数。回文子序列反转字符顺序后仍然与原序列相同。例如字符串aba中，回文子序列为"a", "a", "aa", "b", "aba"，共5个。内容相同位置不同的子序列算不同的子序列。
思路：
动态规划思想
对于任意字符串，如果头尾字符不相等，则字符串的回文子序列个数就等于去掉头的字符串的回文子序列个数+去掉尾的字符串的回文子序列个数-去掉头尾的字符串的回文子序列个数；如果头尾字符相等，那么除了上述的子序列个数之外，还要加上首尾相等时新增的子序列个数，1+去掉头尾的字符串的回文子序列个数，1指的是加上头尾组成的回文子序列，如aa，bb等。
因此动态规划的状态转移方程为：
设字符串为str，长度为n，p[i][j]表示第i到第j个字符间的最长子序列的长度（i<=j），则：
状态初始条件：dp[i][i]=1 （i=0：n-1）
状态转移方程：dp[i][j]=dp[i+1][j] + dp[i][j-1] - dp[i+1][j-1]  if（str[i]！=str[j]）
           dp[i][j]=dp[i+1][j] + dp[i][j-1] - dp[i+1][j-1]+dp[i+1][j-1]+1=dp[i+1][j] + dp[i][j-1]+1  if （str[i]==str[j]）
*/
int NumOfPalindromeSubSequence(string str){
    int len=str.length();
    vector<vector<int> > dp(len,vector<int>(len));

    for(int j=0;j<len;j++){
        dp[j][j]=1;
        for(int i=j-1;i>=0;i--){
            dp[i][j]=dp[i+1][j]+dp[i][j-1]-dp[i+1][j-1];
            if(str[i]==str[j])
                dp[i][j]+=1+dp[i+1][j-1];
        }
    }
    return dp[0][len-1];
}
/*
517. Super Washing Machines
You have n super washing machines on a line. Initially, each washing machine has some dresses or is empty.
For each move, you could choose any m (1 ≤ m ≤ n) washing machines, and pass one dress of each washing machine to one of its adjacent washing machines at the same time .
Given an integer array representing the number of dresses in each washing machine from left to right on the line, you should find the minimum number of moves to make all the washing machines have the same number of dresses. If it is not possible to do it, return -1.
Example1
Input: [1,0,5]
Output: 3
Explanation: 
1st move:    1     0 <-- 5    =>    1     1     4
2nd move:    1 <-- 1 <-- 4    =>    2     1     3    
3rd move:    2     1 <-- 3    =>    2     2     2  
First we check the sum of dresses in all machines. if that number cannot be divided by count of machines, there is no solution.

Otherwise, we can always transfer a dress from one machine to another, one at a time until every machines reach the same number, so there must be a solution. In this way, the total actions is sum of operations on every machine.

Since we can operate several machines at the same time, the minium number of moves is the maximum number of necessary operations on every machine.

For a single machine, necessary operations is to transfer dresses from one side to another until sum of both sides and itself reaches the average number. We can calculate (required dresses) - (contained dresses) of each side as L and R:

L > 0 && R > 0: both sides lacks dresses, and we can only export one dress from current machines at a time, so result is abs(L) + abs(R)
L < 0 && R < 0: both sides contains too many dresses, and we can import dresses from both sides at the same time, so result is max(abs(L), abs(R))
L < 0 && R > 0 or L >0 && R < 0: the side with a larger absolute value will import/export its extra dresses from/to current machine or other side, so result is max(abs(L), abs(R))

For example, [1, 0, 5], average is 2
for 1, L = 0 * 2 - 0 = 0, R = 2 * 2 - 5= -1, result = 1
for 0, L = 1 * 2 - 1= 1, R = 1 * 2 - 5 = -3, result = 3
for 5, L = 2 * 2 - 1= 3, R = 0 * 2 - 0= 0, result = 3
so minium moves is 3
这个test case [0, 0, 11, 5]，最终每个洗衣机会留4件衣服，正确答案是8。
*/
public:
    int findMinMoves(vector<int>& machines) {
        int len = machines.size();
        vector<int> sum(len + 1, 0);
        for (int i = 0; i < len; ++i)
            sum[i + 1] = sum[i] + machines[i];

        if (sum[len] % len) return -1;

        int avg = sum[len] / len;
        int res = 0;
        for (int i = 0; i < len; ++i)
        {
            int l = i * avg - sum[i];
            int r = (len - i - 1) * avg - (sum[len] - sum[i] - machines[i]);

            if (l > 0 && r > 0)
                res = std::max(res, std::abs(l) + std::abs(r));
            else
                res = std::max(res, std::max(std::abs(l), std::abs(r)));
        }
        return res;
    }
};
/*
518. Coin Change 2
You are given coins of different denominations and a total amount of money. Write a function to compute the number of combinations that make up that amount. You may assume that you have infinite number of each kind of coin.
Note: You can assume that
0 <= amount <= 5000
1 <= coin <= 5000
the number of coins is less than 500
the answer is guaranteed to fit into signed 32-bit integer
Example 1:
Input: amount = 5, coins = [1, 2, 5]
Output: 4
Explanation: there are four ways to make up the amount:
5=5
5=2+2+1
5=2+1+1+1
5=1+1+1+1+1

This is a classic knapsack problem. Honestly, I'm not good at knapsack problem, it's really tough for me.
dp[i][j] : the number of combinations to make up amount j by using the first i types of coins
State transition:
not using the ith coin, only using the first i-1 coins to make up amount j, then we have dp[i-1][j] ways.
using the ith coin, since we can use unlimited same coin, we need to know how many way to make up amount j - coins[i] by using first i coins(including ith), which is dp[i][j-coins[i]]
Initialization: dp[i][0] = 1
Now we can see that dp[i][j] only rely on dp[i-1][j] and dp[i][j-coins[i]], then we can optimize the space by only using one-dimension array.

这道题是之前那道Coin Change的拓展，那道题问我们最少能用多少个硬币组成给定的钱数，而这道题问的是组成给定钱数总共有多少种不同的方法。那么我们还是要使用DP来做，首先我们来考虑最简单的情况，如果只有一个硬币的话，那么给定钱数的组成方式就最多有1种，就看此钱数能否整除该硬币值。那么当有两个硬币的话，那么组成某个钱数的方式就可能有多种，比如可能由每种硬币单独来组成，或者是两种硬币同时来组成。那么我们怎么量化呢，比如我们有两个硬币[1,2]，钱数为5，那么钱数的5的组成方法是可以看作两部分组成，一种是由硬币1单独组成，那么仅有一种情况(1+1+1+1+1)；另一种是由1和2共同组成，说明我们的组成方法中至少需要由一个2，所以此时我们先取出一个硬币2，那么我们只要拼出钱数为3即可，这个3还是可以用硬币1和2来拼，所以就相当于求由硬币[1,2]组成的钱数为3的总方法。是不是不太好理解，多想想。那么我们的需要一个二维的dp数组，其中dp[i][j]表示用前i个硬币组成钱数为j的不同组合方法，那么怎么算才不会重复，也不会漏掉呢？我们采用的方法是一个硬币一个硬币的增加，每增加一个硬币，都从1遍历到amount，对于遍历到的当前钱数j，组成方法就是不加上当前硬币的频发dp[i-1][j]，还要加上，去掉当前硬币值的钱数的组成方法，当然钱数j要大于当前硬币值，那么我们的递推公式也在上面的分析中得到了：

dp[i][j] = dp[i - 1][j] + (j >= coins[i - 1] ? dp[i][j - coins[i - 1]] : 0)

注意我们要初始化每行的第一个位置为0，参见代码如下：      
*/
class Solution {
public:
    int change(int amount, vector<int>& coins) {
        vector<vector<int>> dp(coins.size() + 1, vector<int>(amount + 1, 0));
        dp[0][0] = 1;
        for (int i = 1; i <= coins.size(); ++i) {
            dp[i][0] = 1;
            for (int j = 1; j <= amount; ++j) {
                dp[i][j] = dp[i - 1][j] + (j >= coins[i - 1] ? dp[i][j - coins[i - 1]] : 0);
            }
        }
        return dp[coins.size()][amount];
    }
};
/*
我们可以对空间进行优化，我们发现dp[i][j]仅仅依赖于dp[i - 1][j] 和 dp[i][j - coins[i - 1]] 这两项，那么我们可以使用一个一维dp数组来代替，此时的dp[i]表示组成钱数i的不同方法。其实最开始的时候，博主就想着用一维的dp数组来写，但是博主开始想的方法是把里面两个for循环调换了一个位置，结果计算的种类数要大于正确答案，所以一定要注意for循环的顺序不能搞反，参见代码如下：
*/
class Solution {
public:
    int change(int amount, vector<int>& coins) {
        vector<int> dp(amount + 1, 0);
        dp[0] = 1;
        for (int coin : coins) {
            for (int i = coin; i <= amount; ++i) {
                dp[i] += dp[i - coin];
            }
        }
        return dp[amount];
    }
};
class Solution {
public:
    int change(int amount, vector<int>& coins) {
        if (amount == 0) return 1;
        if (coins.empty()) return 0;
        map<pair<int, int>, int> memo;
        return helper(amount, coins, 0, memo);
    }
    int helper(int amount, vector<int>& coins, int idx, map<pair<int, int>, int>& memo) {
        if (amount == 0) return 1;
        else if (idx >= coins.size()) return 0;
        else if (idx == coins.size() - 1) return amount % coins[idx] == 0;
        if (memo.count({amount, idx})) return memo[{amount, idx}];
        int val = coins[idx], res = 0;
        for (int i = 0; i * val <= amount; ++i) {
            int rem = amount - i * val;
            res += helper(rem, coins, idx + 1, memo);
        }
        return memo[{amount, idx}] = res;
    }
};
