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
