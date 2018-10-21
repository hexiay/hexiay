最近经常能听到同学在讨论在大数据流中的随机抽样问题，即：当内存无法加载全部数据时，如何从包含未知大小的数据流中随机选取k个数据，并且要保证每个数据被抽取到的概率相等。
k=1

首先考虑简单的情况，当k=1时，如何制定策略：

假设数据流含有N个数，我们知道如果要保证所有的数被抽到的概率相等，那么每个数抽到的概率应该为 \frac{1}{N} 。

那我们可以这样做：

    遇到第1个数 n_1 的时候，我们保留它， p(n_1)=1 遇到第2个数 n_2 的时候，我们以 \frac{1}{2} 的概率保留它，那么 p(n_1)=1\times \frac{1}{2}=\frac{1}{2} ，p(n_2)=\frac{1}{2} 遇到第3个数 n_3 的时候，我们以 \frac{1}{3} 的概率保留它，那么 p(n_1)=p(n_2)=\frac{1}{2}\times(1-\frac{1}{3})=\frac{1}{3} ， p(n_3)=\frac{1}{3} ……遇到第i个数 n_i 的时候，我们以 \frac{1}{i} 的概率保留它，那么 p(n_1)=p(n_2)=p(n_3)=\dots=p(n_{i-1})=\frac{1}{i-1}\times(1-\frac{1}{i})=\frac{1}{i} ， p(n_i)=\frac{1}{i}

这样就可以看出，对于k=1的情况，我们可以制定这样简单的抽样策略：

数据流中第i个数被保留的概率为 \frac{1}{i} 。只要采取这种策略，只需要遍历一遍数据流就可以得到采样值，并且保证所有数被选取的概率均为 \frac{1}{N} 。
k>1

对于k>1的情况，我们可以采用类似的思考策略：

仍然假设数据流中含有N个数，那么要保证所有的数被抽到的概率相等，每个数被选取的概率必然为 \frac{k}{N} 。

    对于前k个数 n_1,n_2,\dots,n_k ，我们保留下来，则 p(n_1)=p(n_2)=\dots=p(n_k)=1 （下面连等采用 p(n_{1:k}) 的形式）对于第k+1个数 n_{k+1} ，我们以 \frac{k}{k+1} 的概率保留它（这里只是指本次被保留下来），那么前k个数中的 n_r(r\in{1:k}) 被保留的概率可以这样表示： p(n_r被保留)=p(上一轮n_r被保留)\times(p(n_{k+1}被丢弃)+p(n_{k+1}被保留)\times p(n_r未被替换)) ，即 p_{1:k}=\frac{1}{k+1}+\frac{k}{k+1}\times \frac{k-1}{k}=\frac{k}{k+1} 对于第k+2个数 n_{k+2} ，我们以 \frac{k}{k+2} 的概率保留它（这里只是指本次被保留下来），那么前k个被保留下来的数中的 n_r(r\in{1:k}) 被保留的概率为 p_{1:k}=\frac{k}{k+1}\times(\frac{2}{k+2}+\frac{k}{k+2}\times \frac{k-1}{k})=\frac{k}{k+1}\times\frac{k+1}{k+2}=\frac{k}{k+2} ……对于第i（i>k）个数 n_i ，我们以 \frac{k}{i} 的概率保留它，前i-1个数中的 n_r(r\in{1:i-1}) 被保留的概率为 p_{1:k}=\frac{k}{i-1}\times (\frac{i-k}{i}+\frac{k}{i}\times \frac{k-1}{k})=\frac{k}{i-1}\times\frac{i-1}{i}=\frac{k}{i} 

这样，我们可以制订策略：

对于前k个数，我们全部保留，对于第i（i>k）个数，我们以 \frac{k}{i} 的概率保留第i个数，并以 \frac{1}{k} 的概率与前面已选择的k个数中的任意一个替换。

下面是简单的水塘抽样的C++实现

vector<int> ReservoirSampling(vector<int>& results, vector<int>& nums, int k)
{
    // results.size(): k
    // nums.size(): N
    int N = nums.size();

    for (int i=0; i<k; ++i) {
        results[i] = nums[i];
    }

    for (int i=k; i<N; ++i) {
        int random = rand()%i;
        if (random<k) {
            results[random] = nums[i];
        }
    }

    return results;
}
