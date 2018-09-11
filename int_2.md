### 哈希表的原理 Top K 算法详解
```
问题: 海量日志数据, 提取出某日访问百度次数最多的那个IP.

    首先是这一天, 并且是访问百度的日志中的IP取出来, 逐个写入到一个大文件中. 注意到IP是32位的, 最多有2^32个不同的IP(事实上不可能, 得扣去特殊的IP), 在重复量小的情况下, 内存放不下这些不同的IP. 同样可以采用映射的方法, 比如模1000, 把整个大文件映射为1000个小文件, 再找出每个小文件中出现频率最大的IP(可以采用hash_map进行频率统计, 然后再找出频率最大的几个)及相应的频率. 然后再在这1000个最大的IP中, 找出那个频率最大的IP, 即为所求.

具体做法如下：

    按照IP地址的Hash(IP)%1024值, 把海量IP日志分别存储到1024个小文件中.
    对于每一个小文件, 构建一个以IP为key, 出现次数为value的HashMap, 同时记录当前出现次数最多的那个IP地址;
    得到1024个小文件中的出现次数最多的IP, 再依据常规的排序算法得到总体上出现次数最多的IP.
21Hash算法以及暴雪Hash
2015年06月12日 09:08:03 阅读数：1803更多
个人分类： 算法 July文章

一：哈希表简介

    哈希表是一种查找效率极高的数据结构，理想情况下哈希表插入和查找操作的时间复杂度均为O(1)，任何一个数据项可以在一个与哈希表长度无关的时间内计算出一个哈希值（key），然后在常量时间内定位到一个桶（术语bucket，表示哈希表中的一个位置）。当然这是理想情况下，因为任何哈希表的长度都是有限的，所以一定存在不同的数据项具有相同哈希值的情况，此时不同数据项被定为到同一个桶，称为碰撞（collision）。

    哈希表的实现需要解决碰撞问题，碰撞解决大体有两种思路：

    第一种是根据某种原则将被碰撞数据定为到其它桶，例如线性探测——如果数据在插入时发生了碰撞，则顺序查找这个桶后面的桶，将其放入第一个没有被使用的桶；

    第二种策略是每个桶不是一个只能容纳单个数据项的位置，而是一个可容纳多个数据的数据结构（例如链表或红黑树），所有碰撞的数据以某种数据结构的形式组织起来。

    不论使用了哪种碰撞解决策略，都导致插入和查找操作的时间复杂度不再是O(1)。以查找为例，不能通过key定位到桶就结束，必须还要比较原始数据项是否相等，如果不相等，则要使用与插入相同的算法继续查找，直到找到匹配的值或确认数据不在哈希表中。

    使用单链表解决碰撞的哈希表，平均查找复杂度为O(L)，其中L为桶链表的平均长度；而最坏复杂度为O(N)，此时所有数据全部碰撞，哈希表退化成单链表。如下图：

 

二：暴雪的Hash算法(MPQ)

    由一个简单的问题逐步入手：有一个庞大的字符串数组，数组元素就是字符串，然后给定一个单独的字符串，让你从这个数组中查找是否有这个字符串并找到它，你会怎么做？

    有一个方法最简单，老老实实从头查到尾，一个一个比较，直到找到为止，这样做的效率极低。

    最合适的算法自然是使用HashTable（哈希表），可以把一个字符串"压缩" 成一个整数。在暴雪的HASH算法中，两个字符串计算出的Hash值相等的可能非常小，下面看看在MPQ中的Hash算法：

1：函数prepareCryptTable生成一个长度为0x500（合10进制数：1280）的cryptTable[0x500]

 

void prepareCryptTable()  

{   

         unsigned long seed = 0x00100001, index1 = 0, index2 = 0, i;  

  

         for( index1 = 0; index1 < 0x100; index1++ )  

         {   

                  for( index2 = index1, i = 0; i < 5; i++, index2 += 0x100 )  

                  {   

                          unsigned long temp1, temp2;  

  

                          seed = (seed * 125 + 3) % 0x2AAAAB;  

                          temp1 = (seed & 0xFFFF) << 0x10;  

  

                          seed = (seed * 125 + 3) % 0x2AAAAB;  

                          temp2 = (seed & 0xFFFF);  

  

                          cryptTable[index2] = ( temp1 | temp2 );   

                  }   

         }   

}   

 

2：函数HashString计算字符串lpszFileName的hash值，其中dwHashType 为hash的类型。

unsigned long HashString(const char *lpszkeyName, unsigned long dwHashType )  

{  

         unsigned char *key  = (unsigned char *)lpszkeyName;  

         unsigned long seed1 = 0x7FED7FED;  

         unsigned long seed2 = 0xEEEEEEEE;  

         int ch;  

  

         while( *key != 0 )  

         {  

                  ch = *key++;  

                  seed1 = cryptTable[(dwHashType<<8) + ch] ^ (seed1 + seed2);  

                  seed2 = ch + seed1 + seed2 + (seed2<<5) + 3;  

         }  

         return seed1;  

}  

    Blizzard的这个算法是非常高效的，被称为"One-WayHash"( 即通过HASH值反推字符串几乎是不可能的)。举个例子，字符串"unitneutralacritter.grp"通过这个算法得到的结果是0xA26067F3。

    然后是构造一个哈希表来解决问题，哈希表是一个大数组，这个数组的容量根据程序的要求来定义，例如1024。

    每一个Hash值通过取模运算 (mod) 对应到数组中的一个位置，这样，只要比较这个字符串的哈希值对应的位置有没有被占用，就可以得到最后的结果了，

#if 0

想想这是什么速度？是的，是最快的O(1)，现在仔细看看这个算法吧：

typedef struct  

{  

         int nHashA;  

         int nHashB;  

         char bExists;  

         ......  

} SOMESTRUCTRUE;  

  

3：函数GetHashTablePos在Hash表中查找是否存在目标字符串，有则返回要查找字符串的Hash值，否则，return -1.

int GetHashTablePos( char *lpszString, SOMESTRUCTURE *lpTable )   

{   

         //调用上述函数HashString，返回要查找字符串lpszString的Hash值。  

         int nHash = HashString(lpszString);      int nHashPos = nHash % nTableSize;  

   

         if ( lpTable[nHashPos].bExists  &&  !strcmp( lpTable[nHashPos].pString, lpszString ) )   

         {         

                  return nHashPos;    //返回找到的Hash值  

         }   

         else  

         {  

                  return -1;    

         }   

}  

#endif

    看到此，我想大家都在想一个很严重的问题：如果两个字符串在哈希表中对应的位置相同怎么办？毕竟一个数组容量是有限的，这种可能性很大。

    Blizzard的程序员使用精妙的方法。基本原理就是：他们在哈希表中不是用一个哈希值而是用三个哈希值来校验字符串。

 

    MPQ 使用的哈希表的格式与正常的哈希表有一些不同。它没有把实际的文件名存储在表中用于验证，实际上它根本就没有存储文件名。而是使用了3种不同的哈希：一个用于哈希表的下标，两个用于验证。这两个验证哈希替代了实际文件名。

    假如说两个不同的字符串经过一个哈希算法得到的入口点一致有可能，但用三个不同的哈希算法算出的入口点都一致，那几乎可以肯定是不可能的事了。当然了，这样仍然会出现2个不同的文件名哈希到3个同样的哈希。但是这种情况发生的概率平均是：1:18889465931478580854784，这个概率对于任何人来说应该都是足够小的。现在再回到数据结构上，Blizzard使用的哈希表没有使用链表，而采用"顺延"的方式来解决问题。

4：函数GetHashTablePos中，lpszString 为要在hash表中查找的字符串；lpTable 为存储字符串hash值的hash表；nTableSize为hash表的长度： 

 

int GetHashTablePos( char *lpszString, MPQHASHTABLE *lpTable, int nTableSize )  

{  

         const int  HASH_OFFSET = 0, HASH_A = 1, HASH_B = 2;  

   

         int  nHash = HashString( lpszString, HASH_OFFSET );  

         int  nHashA = HashString( lpszString, HASH_A );  

         int  nHashB = HashString( lpszString, HASH_B );  

         int  nHashStart = nHash % nTableSize;  

         int  nHashPos = nHashStart;  

   

         while ( lpTable[nHashPos].bExists )  

         {  

                  if (lpTable[nHashPos].nHashA == nHashA && lpTable[nHashPos].nHashB == nHashB )  

                  {  

                           return nHashPos;  

                  }  

                  else  

                  {  

                          nHashPos = (nHashPos + 1) % nTableSize;  

                  }  

   

                  if (nHashPos == nHashStart)  

                          break;  

         }  

         return -1;  

}  

   

上述程序解释：

    1计算出字符串的三个哈希值（一个用来确定位置，另外两个用来校验)

    2察看哈希表中的这个位置

    3哈希表中这个位置为空吗？如果为空，则肯定该字符串不存在，返回-1。

    4如果存在，则检查其他两个哈希值是否也匹配，如果匹配，则表示找到了该字符串，返回其Hash值。

    5移到下一个位置，如果已经移到了表的末尾，则反绕到表的开始位置起继续查询　

    6看看是不是又回到了原来的位置，如果是，则返回没找到

    7回到3。

 

三：哈希表大小

    哈希表的数组是定长的，如果太大，则浪费，如果太小，体现不出效率。合适的数组大小是哈希表的性能的关键。

    哈希表的尺寸最好是一个质数。当然，根据不同的数据量，会有不同的哈希表的大小。对于数据量时多时少的应用，最好的设计是使用动态可变尺寸的哈希表，那么如果你发现哈希表尺寸太小了，比如其中的元素是哈希表尺寸的2倍时，我们就需要扩大哈希表尺寸，一般是扩大一倍。

    下面是哈希表尺寸大小的可能取值：

    17, 37, 79, 163, 331, 673, 1361, 2729, 5471,10949, 21911, 43853, 87719, 175447, 350899, 701819, 1403641, 2807303, 5614657, 11229331,22458671, 44917381, 89834777, 179669557, 359339171, 718678369, 1437356741, 2147483647

```
