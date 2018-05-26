/* ====================================================================
5. Longest Palindromic Substring
Given a string s, find the longest palindromic substring in s. You may assume that the maximum length of s is 1000.

Example 1:

Input: "babad"
Output: "bab"
Note: "aba" is also a valid answer.
Example 2:

Input: "cbbd"
Output: "bb"
==================================================================== */
/* =================================================================
10. Regular Expression Matching
Given an input string (s) and a pattern (p), implement regular expression matching with support for '.' and '*'.

'.' Matches any single character.
'*' Matches zero or more of the preceding element.
The matching should cover the entire input string (not partial).

Note:

s could be empty and contains only lowercase letters a-z.
p could be empty and contains only lowercase letters a-z, and characters like . or *.
Example 1:

Input:
s = "aa"
p = "a"
Output: false
Explanation: "a" does not match the entire string "aa".
Example 2:

Input:
s = "aa"
p = "a*"
Output: true
Explanation: '*' means zero or more of the precedeng element, 'a'. Therefore, by repeating 'a' once, it becomes "aa".
Example 3:

Input:
s = "ab"
p = ".*"
Output: true
Explanation: ".*" means "zero or more (*) of any character (.)".
Example 4:

Input:
s = "aab"
p = "c*a*b"
Output: true
Explanation: c can be repeated 0 times, a can be repeated 1 time. Therefore it matches "aab".
Example 5:

Input:
s = "mississippi"
p = "mis*is*p*."
Output: false
==================================================================== */
/* =================================================================
32. Longest Valid Parentheses
Given a string containing just the characters '(' and ')', find the length of the longest valid (well-formed) parentheses substring.

Example 1:

Input: "(()"
Output: 2
Explanation: The longest valid parentheses substring is "()"
Example 2:

Input: ")()())"
Output: 4
Explanation: The longest valid parentheses substring is "()()"
==================================================================== */
/* =================================================================
44. Wildcard Matching
Given an input string (s) and a pattern (p), implement wildcard pattern matching with support for '?' and '*'.

'?' Matches any single character.
'*' Matches any sequence of characters (including the empty sequence).
The matching should cover the entire input string (not partial).

Note:

s could be empty and contains only lowercase letters a-z.
p could be empty and contains only lowercase letters a-z, and characters like ? or *.
Example 1:

Input:
s = "aa"
p = "a"
Output: false
Explanation: "a" does not match the entire string "aa".
Example 2:

Input:
s = "aa"
p = "*"
Output: true
Explanation: '*' matches any sequence.
Example 3:

Input:
s = "cb"
p = "?a"
Output: false
Explanation: '?' matches 'c', but the second letter is 'a', which does not match 'b'.
Example 4:

Input:
s = "adceb"
p = "*a*b"
Output: true
Explanation: The first '*' matches the empty sequence, while the second '*' matches the substring "dce".
Example 5:

Input:
s = "acdcb"
p = "a*c?b"
Output: false

==================================================================== */
/* =================================================================
53. Maximum Subarray
Given an integer array nums, find the contiguous subarray (containing at least one number) which has the largest sum and return its sum.

Example:

Input: [-2,1,-3,4,-1,2,1,-5,4],
Output: 6
Explanation: [4,-1,2,1] has the largest sum = 6.
Follow up:

If you have figured out the O(n) solution, try coding another solution using the divide and conquer approach, which is more subtle.
==================================================================== */
/* =================================================================
62. Unique Paths
A robot is located at the top-left corner of a m x n grid (marked 'Start' in the diagram below).

The robot can only move either down or right at any point in time. The robot is trying to reach the bottom-right corner of the grid (marked 'Finish' in the diagram below).

How many possible unique paths are there?
Above is a 7 x 3 grid. How many possible unique paths are there?

Note: m and n will be at most 100.

Example 1:

Input: m = 3, n = 2
Output: 3
Explanation:
From the top-left corner, there are a total of 3 ways to reach the bottom-right corner:
1. Right -> Right -> Down
2. Right -> Down -> Right
3. Down -> Right -> Right
Example 2:

Input: m = 7, n = 3
Output: 28
==================================================================== */
class Solution {
public:
    int uniquePaths(int m, int n) {
        vector<int> dp(n, 1);
        for (int i = 1; i < m; ++i) {
            for (int j = 1; j < n; ++j) {
                dp[j] += dp[j - 1]; 
            }
        }
        return dp[n - 1];
    }
};
/* =================================================================
63. Unique Paths II
A robot is located at the top-left corner of a m x n grid (marked 'Start' in the diagram below).
The robot can only move either down or right at any point in time. The robot is trying to reach the bottom-right corner of the grid (marked 'Finish' in the diagram below).
Now consider if some obstacles are added to the grids. How many unique paths would there be?
An obstacle and empty space is marked as 1 and 0 respectively in the grid.
Note: m and n will be at most 100.
Example 1:
Input:
[
  [0,0,0],
  [0,1,0],
  [0,0,0]
]
Output: 2
Explanation:
There is one obstacle in the middle of the 3x3 grid above.
There are two ways to reach the bottom-right corner:
1. Right -> Right -> Down -> Down
2. Down -> Down -> Right -> Right
==================================================================== */
class Solution {
public:
    int uniquePathsWithObstacles(vector<vector<int> > &obstacleGrid) {
        if (obstacleGrid.empty() || obstacleGrid[0].empty()) return 0;
        int m = obstacleGrid.size(), n = obstacleGrid[0].size();
        if (obstacleGrid[0][0] == 1) return 0;
        vector<int> dp(n, 0);
        dp[0] = 1;
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (obstacleGrid[i][j] == 1) dp[j] = 0;
                else if (j > 0) dp[j] += dp[j - 1];
            }
        }
        return dp[n - 1];
    }
};
/* =================================================================
64. Minimum Path Sum
Given a m x n grid filled with non-negative numbers, find a path from top left to bottom right which minimizes the sum of all numbers along its path.
Note: You can only move either down or right at any point in time.
Input:
[
  [1,3,1],
  [1,5,1],
  [4,2,1]
]
Output: 7
==================================================================== */
class Solution {
public:
    int minPathSum(vector<vector<int>>& grid) {
        if (grid.empty() || grid[0].empty())
            return 0;
        int m = grid.size();
        int n = grid[0].size();
        
        vector<int> dp(n, 0);
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (i == 0) {
                    dp[j] = grid[i][j] + ((j == 0) ? 0 : dp[j-1]);
                } else {
                    dp[j] = grid[i][j] + ((j == 0) ? dp[j] : min(dp[j], dp[j-1]));   
                }
            }
        }
        return dp[n-1];
    }
};
/* =================================================================
70. Climbing Stairs
You are climbing a stair case. It takes n steps to reach to the top.

Each time you can either climb 1 or 2 steps. In how many distinct ways can you climb to the top?

Note: Given n will be a positive integer.

Example 1:

Input: 2
Output: 2
Explanation: There are two ways to climb to the top.
1. 1 step + 1 step
2. 2 steps
==================================================================== */
class Solution {
public:
    int climbStairs(int n) {
        if (n <= 1) return 1;
        vector<int> dp(n);
        dp[0] = 1; dp[1] = 2;
        for (int i = 2; i < n; ++i) {
            dp[i] = dp[i - 1] + dp[i - 2];
        }
        return dp.back();
    }
};
/* =================================================================
72. Edit Distance
Given two words word1 and word2, find the minimum number of operations required to convert word1 to word2.

You have the following 3 operations permitted on a word:

    Insert a character
    Delete a character
    Replace a character

Example 1:

Input: word1 = "horse", word2 = "ros"
Output: 3
Explanation: 
horse -> rorse (replace 'h' with 'r')
rorse -> rose (remove 'r')
rose -> ros (remove 'e')
维护一个二维的数组dp，其中dp[i][j]表示从word1的前i个字符转换到word2的前j个字符所需要的步骤。那我们可以先给这个二维数组dp的第一行第一列赋值，这个很简单，因为第一行和第一列对应的总有一个字符串是空串，于是转换步骤完全是另一个字符串的长度。跟以往的DP题目类似，难点还是在于找出递推式，我们可以举个例子来看，比如word1是“bbc"，word2是”abcd“，那么我们可以得到dp数组如下：
  Ø a b c d
Ø 0 1 2 3 4
b 1 1 1 2 3
b 2 2 1 2 3
c 3 3 2 1 2
当word1[i] == word2[j]时，dp[i][j] = dp[i - 1][j - 1]，其他情况时，dp[i][j]是其左，左上，上的三个值中的最小值加1，那么可以得到递推式为：
dp[i][j] =  /    dp[i - 1][j - 1]        if word1[i - 1] == word2[j - 1]
            \    min(dp[i - 1][j - 1], min(dp[i - 1][j], dp[i][j - 1])) + 1
==================================================================== */
class Solution {
public:
    int minDistance(string word1, string word2) {
        int n1 = word1.size(), n2 = word2.size();
        int dp[n1 + 1][n2 + 1];
        for (int i = 0; i <= n1; ++i) dp[i][0] = i;
        for (int i = 0; i <= n2; ++i) dp[0][i] = i;
        for (int i = 1; i <= n1; ++i) {
            for (int j = 1; j <= n2; ++j) {
                if (word1[i - 1] == word2[j - 1]) {
                    dp[i][j] = dp[i - 1][j - 1];
                } else {
                    dp[i][j] = min(dp[i - 1][j - 1], min(dp[i - 1][j], dp[i][j - 1])) + 1;
                }
            }
        }
        return dp[n1][n2];
    }
};
/* =================================================================
84. Largest Rectangle in Histogram
Given n non-negative integers representing the histogram's bar height where the width of each bar is 1, find the area of largest rectangle in the histogram.
 Above is a histogram where width of each bar is 1, given height = [2,1,5,6,2,3].
  The largest rectangle is shown in the shaded area, which has area = 10 unit.
==================================================================== */
class Solution {
public:
    int largestRectangleArea(vector<int>& heights) {
        stack<int> stk;   // Monotone increasing stack
        heights.push_back(0);
        int res = 0;
        
        int n = heights.size();
        for (int i = 0; i < n; ++i) {
            while (!stk.empty() && heights[i] <= heights[stk.top()]) {
                int t = stk.top(); stk.pop();
                res = max(res, heights[t] * (stk.empty() ? ((i-1) - (0) + 1) : ((i-1) - (stk.top()+1) + 1)));
            }
            stk.push(i);
        }
        return res;
    }
};
/* =================================================================
85. Maximal Rectangle
Given a 2D binary matrix filled with 0's and 1's, find the largest rectangle containing only 1's and return its area.
Example:
Input:
[
  ["1","0","1","0","0"],
  ["1","0","1","1","1"],
  ["1","1","1","1","1"],
  ["1","0","0","1","0"]
]
Output: 6
==================================================================== */
class Solution {
public:
    int maximalRectangle(vector<vector<char>>& matrix) {
        if (matrix.empty() || matrix[0].empty())
            return 0;
        int m = matrix.size();
        int n = matrix[0].size();
        int rs = 0;
        vector<int> dp(n, 0);
        for (int i = 0 ; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (matrix[i][j] == '0') {
                    dp[j] = 0;
                } else {
                    dp[j] += 1;
                }
            }
            rs = max(rs, maximalSize(dp));
        }
        return rs;
    }
    int maximalSize(vector<int> heights) {
        heights.push_back(0);
        int n = heights.size();
        int rs = 0;
        stack<int> stk;
        
        for (int i = 0; i < n; ++i) {
            while (!stk.empty() && heights[i] <= heights[stk.top()]) {
                int t = stk.top(); stk.pop();
                rs = max(rs, heights[t] * (stk.empty() ? i : ((i-1) - (stk.top()+1) + 1)));
            }
            stk.push(i);
        }
        return rs;
    }
};
/* =================================================================
87. Scramble String
Given a string s1, we may represent it as a binary tree by partitioning it to two non-empty substrings recursively.
Below is one possible representation of s1 = "great":
    great
   /    \
  gr    eat
 / \    /  \
g   r  e   at
           / \
          a   t
To scramble the string, we may choose any non-leaf node and swap its two children.
For example, if we choose the node "gr" and swap its two children, it produces a scrambled string "rgeat".
    rgeat
   /    \
  rg    eat
 / \    /  \
r   g  e   at
           / \
          a   t

We say that "rgeat" is a scrambled string of "great".
Similarly, if we continue to swap the children of nodes "eat" and "at", it produces a scrambled string "rgtae".
    rgtae
   /    \
  rg    tae
 / \    /  \
r   g  ta  e
       / \
      t   a
We say that "rgtae" is a scrambled string of "great".
Given two strings s1 and s2 of the same length, determine if s2 is a scrambled string of s1.
维护量res[i][j][n]，其中i是s1的起始字符，j是s2的起始字符，而n是当前的字符串长度，res[i][j][len]表示的是以i和j分别为s1和s2起点的长度为len的字符串是不是互为scramble
有了维护量我们接下来看看递推式，也就是怎么根据历史信息来得到res[i][j][len]。判断这个是不是满足，其实我们首先是把当前s1[i...i+len-1]字符串劈一刀分成两部分，然后分两种情况：第一种是左边和s2[j...j+len-1]左边部分是不是scramble，以及右边和s2[j...j+len-1]右边部分是不是scramble；第二种情况是左边和s2[j...j+len-1]右边部分是不是scramble，以及右边和s2[j...j+len-1]左边部分是不是scramble。如果以上两种情况有一种成立，说明s1[i...i+len-1]和s2[j...j+len-1]是scramble的。而对于判断这些左右部分是不是scramble我们是有历史信息的，因为长度小于n的所有情况我们都在前面求解过了（也就是长度是最外层循环）。
上面说的是劈一刀的情况，对于s1[i...i+len-1]我们有len-1种劈法，在这些劈法中只要有一种成立，那么两个串就是scramble的。
总结起来递推式是res[i][j][len] = || (res[i][j][k]&&res[i+k][j+k][len-k] || res[i][j+len-k][k]&&res[i+k][j][len-k]) 对于所有1<=k<len，也就是对于所有len-1种劈法的结果求或运算。因为信息都是计算过的，对于每种劈法只需要常量操作即可完成，因此求解递推式是需要O(len)（因为len-1种劈法）。
如此总时间复杂度因为是三维动态规划，需要三层循环，加上每一步需要线行时间求解递推式，所以是O(n^4)。虽然已经比较高了，但是至少不是指数量级的，动态规划还是有很大优势的，空间复杂度是O(n^3)。代码如下
==================================================================== */
class Solution {
public:
    bool isScramble(string s1, string s2) {
        if (s1.size() != s2.size())
            return false;
        if (s1 == s2)
            return true;
        
        int n = s1.size();
        vector<vector<vector<bool> > > dp(n, vector<vector<bool> >(n, vector<bool>(n+1, false)));
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                dp[i][j][1] = (s1[i] == s2[j]);
            }
        }
        for (int len = 2; len <= n; ++len) {
            for (int i = 0; i <= n - len; ++i) {
                for (int j = 0; j <= n - len; ++j) {
                    for (int z = 1; z < len; ++z) {
                        if ((dp[i][j][z] && dp[i+z][j+z][len-z]) || 
                            (dp[i][j+len-z][z] && dp[i+z][j][len-z]))
                            dp[i][j][len] = true;
                    }    
                }
            }
        }
        return dp[0][0][n];
    }
};
/* =================================================================
91. Decode Ways
A message containing letters from A-Z is being encoded to numbers using the following mapping:
'A' -> 1
'B' -> 2
...
'Z' -> 26
Given a non-empty string containing only digits, determine the total number of ways to decode it.
Input: "12"
Output: 2
Explanation: It could be decoded as "AB" (1 2) or "L" (12).
跟之前那道 Climbing Stairs 爬梯子问题 非常的相似，但是还有一些其他的限制条件，比如说一位数时不能为0，两位数不能大于26，其十位上的数也不能为0，出去这些限制条件，根爬梯子基本没啥区别，也勉强算特殊的斐波那契数列，当然需要用动态规划Dynamci Programming来解。建立一位dp数组，长度比输入数组长多多2，全部初始化为1，因为斐波那契数列的前两项也为1，然后从第三个数开始更新，对应数组的第一个数。对每个数组首先判断其是否为0，若是将改为dp赋0，若不是，赋上一个dp值，此时相当如加上了dp[i - 1], 然后看数组前一位是否存在，如果存在且满足前一位不是0，且和当前为一起组成的两位数不大于26，则当前dp值加上dp[i - 2], 至此可以看出来跟斐波那契数组的递推式一样，代码如下：
==================================================================== */
class Solution {
public:
    int numDecodings(string s) {
        if (s.empty() || s.at(0) == '0')
            return 0;
        int n = s.size();
        vector<int> dp(n+1, 0);
        dp[0] = 1;
        dp[1] = 1;
        for (int i = 2; i < n+1; ++i) {
            // one step to index i
            if (s.at(i-1) > '0' && s.at(i-1) <= '9') {
                dp[i] += dp[i-1];
            }
            // two steps to index i
            if ((s.at(i-2) == '1' && s.at(i-1) >= '0' && s.at(i-1) <= '9') || 
                (s.at(i-2) == '2' && s.at(i-1) >= '0' && s.at(i-1) <= '6')) {
                dp[i] += dp[i-2];
            }
        }
        return dp[n];
    }
};
/* =================================================================
96. Unique Binary Search Trees
Given n, how many structurally unique BST's (binary search trees) that store values 1 ... n?
我们先来看当 n = 1的情况，只能形成唯一的一棵二叉搜索树，n分别为1,2,3的情况如下所示：

                    1                        n = 1

                2        1                   n = 2
               /          \
              1            2
  
   1         3     3      2      1           n = 3
    \       /     /      / \      \
     3     2     1      1   3      2
    /     /       \                 \
   2     1         2                 3
就跟斐波那契数列一样，我们把n = 0 时赋为1，因为空树也算一种二叉搜索树，那么n = 1时的情况可以看做是其左子树个数乘以右子树的个数，左右字数都是空树，所以1乘1还是1。那么n = 2时，由于1和2都可以为跟，分别算出来，再把它们加起来即可。n = 2的情况可由下面式子算出：
dp[2] =  dp[0] * dp[1]　　　(1为根的情况)
　　　　+ dp[1] * dp[0]　　  (2为根的情况)
同理可写出 n = 3 的计算方法：
dp[3] =  dp[0] * dp[2]　　　(1为根的情况)
　　　　+ dp[1] * dp[1]　　  (2为根的情况)
 　　　  + dp[2] * dp[0]　　  (3为根的情况)
==================================================================== */
class Solution {
public:
    int numTrees(int n) {
        vector<int> dp(n + 1, 0);
        dp[0] = 1;
        dp[1] = 1;
        for (int i = 2; i <= n; ++i) {
            for (int j = 0; j < i; ++j) {
                dp[i] += dp[j] * dp[i - j - 1];
            }
        }
        return dp[n];
    }
};
/* =================================================================
449. Serialize and Deserialize BST
struct TreeNode {
     int val;
     TreeNode *left;
     TreeNode *right;
     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};
[1,2,3,null,null,4,5]
    1
   / \
  2   3
     / \
    4   5
[]
    Empty tree. The root is a reference to NULL (C/C++), null (Java/C#/Javascript), None (Python), or nil (Ruby).
[1,2,3]
           1
          / \
         2   3
[1,null,2,3]
           1
            \
             2
            /
           3
[5,4,7,3,null,2,null,-1,null,9]
           5
          / \
         4   7
        /   /
       3   2
      /   /
     -1  9
[
  [1,null,3,2],
  [3,2,null,1],
  [3,1,null,null,2],
  [2,1,3],
  [1,null,2,null,3]
]
   1         3     3      2      1
    \       /     /      / \      \
     3     2     1      1   3      2
    /     /       \                 \
   2     1         2                 3
==================================================================== */
class Codec {
public:
    static constexpr char* NullNodeString      = "null";
    static constexpr char* NodeSeparator       = ",";
  
    // Encodes a tree to a single string.
    string serialize(TreeNode* root) {
        string res;
        
        deque<TreeNode*> nodes;
        nodes.push_back(root);
        while (!nodes.empty()) {
            TreeNode *p = nodes.front();
            nodes.pop_front();
            if (p) {
                if (!res.empty())
                    res += NodeSeparator;
                res += to_string(p->val);
                nodes.push_back(p->left);
                nodes.push_back(p->right);
            } else {
                if (!res.empty())
                    res += NodeSeparator;
                res += string(NullNodeString);
            }
        }
        return res;
    }
    
    // Decodes your encoded data to tree.
    //  1,null,2,null,3
    //  1,#,2,#,3
    TreeNode* deserialize(string data) {        
        vector<TreeNode*> vec;
        char * pch = NULL;
  
        pch = strtok (const_cast<char*>(data.c_str()), NodeSeparator);
        while (pch != NULL) {
            if (strncmp(NullNodeString, pch, strlen(NullNodeString)) == 0) {
                vec.push_back(NULL);
            } else {
                TreeNode *node = new TreeNode(atoi(pch));
                vec.push_back(node);
            }
            pch = strtok (NULL, NodeSeparator);
        }

        int n = vec.size();
        if (n == 0)
            return NULL;
        TreeNode *root = vec[0];
        int i = 0;
        for (int k = 1; k < n; ) {
            if (vec[i]) {
                vec[i]->left = vec[k++];
                if (k < n)
                    vec[i]->right = vec[k++];
            }
            ++i;
        }
        return root;
    }
};
/* =================================================================
95. Unique Binary Search Trees II
Given an integer n, generate all structurally unique BST's (binary search trees) that store values 1 ... n.
Input: 3
Output:
[
  [1,null,3,2],
  [3,2,null,1],
  [3,1,null,null,2],
  [2,1,3],
  [1,null,2,null,3]
]
Explanation:
The above output corresponds to the 5 unique BST's shown below:

   1         3     3      2      1
    \       /     /      / \      \
     3     2     1      1   3      2
    /     /       \                 \
   2     1         2                 3
==================================================================== */
class Solution {
public:
    
    vector<TreeNode*> generateTrees(int n) {
        if (n <= 0)
            return vector<TreeNode*>();
        return generateTrees(1, n);   
    }
    
    vector<TreeNode*> generateTrees(int start, int end) {
        vector<TreeNode*> subtree;
        if (start > end) {
            subtree.push_back(NULL);
            return subtree;
        }        
        for (int i = start; i <= end; ++i) {
            vector<TreeNode*> left = generateTrees(start, i-1);
            vector<TreeNode*> right = generateTrees(i+1, end);
            for (int m = 0; m < left.size(); ++m) {
                for(int n = 0; n < right.size(); ++n) {
                    TreeNode* root = new TreeNode(i);
                    root->left = left[m];
                    root->right = right[n];
                    subtree.push_back(root);
                }
            }
        }
        return subtree;
    }
};
class Solution {
public:
    vector<TreeNode *> generateTrees(int n) {
        if (n == 0) return {};
        return *generateTreesDFS(1, n);
    }
    vector<TreeNode*> *generateTreesDFS(int start, int end) {
        vector<TreeNode*> *subTree = new vector<TreeNode*>();
        if (start > end) subTree->push_back(NULL);
        else {
            for (int i = start; i <= end; ++i) {
                vector<TreeNode*> *leftSubTree = generateTreesDFS(start, i - 1);
                vector<TreeNode*> *rightSubTree = generateTreesDFS(i + 1, end);
                for (int j = 0; j < leftSubTree->size(); ++j) {
                    for (int k = 0; k < rightSubTree->size(); ++k) {
                        TreeNode *node = new TreeNode(i);
                        node->left = (*leftSubTree)[j];
                        node->right = (*rightSubTree)[k];
                        subTree->push_back(node);
                    }
                }
            }
        }
        return subTree;
    }
};
/* =================================================================
==================================================================== */
/* =================================================================
==================================================================== */
/* =================================================================
==================================================================== */
/* =================================================================
==================================================================== */
/* =================================================================
==================================================================== */
/* =================================================================
==================================================================== */
/* =================================================================
==================================================================== */
/* =================================================================
==================================================================== */
/* =================================================================
==================================================================== */
/* =================================================================
==================================================================== */
/* =================================================================
==================================================================== */
