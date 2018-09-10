# 1. linux下一个write()函数的具体过程 
```
1，insmod驱动程序。驱动程序申请设备名和主设备号，这些可以在/proc/devIEces中获得。
2，从/proc/devices中获得主设备号，并使用mknod命令建立设备节点文件。这是通过主
设备号将设备节点文件和设备驱动程序联系在一起。设备节点文件中的file属性中指明了
驱动程序中fops方法实现的函数指针。
3，用户程序使用open打开设备节点文件，这时操作系统内核知道该驱动程序工作了，就
调用fops方法中的open函数进行相应的工作。open方法一般返回的是文件标示符，实际
上并不是直接对它进行操作的，而是有操作系统的系统调用在背后工作。
4，当用户使用write函数操作设备文件时，操作系统调用sys_write函数，该函数首先通
过文件标示符得到设备节点文件对应的inode指针和flip指针。inode指针中有设备号信
息，能够告诉操作系统应该使用哪一个设备驱动程序，flip指针中有fops信息，可以告诉
操作系统相应的fops方法函数在那里可以找到。
5，然后这时sys_write才会调用驱动程序中的write方法来对设备进行写的操作。
其中1-3都是在用户空间进行的，4-5是在核心空间进行的。用户的write函数和操作系统
的write函数通过系统调用sys_write联系在了一起。
注意：
对于块设备来说，还存在写的模式的问题，这应该是由GNU C库来解决的，这里不予讨
论，因为我没有看过GNU C库的源代码。
```

# 2. 共享内存实现
```
		就是分配一块能被其他进程访问的内存。共享内存可以说是最有用的进程间通信方式，也是最快的IPC形式。首先说下在使用共享内存
	区前，必须通过系统函数将其附加到进程的地址空间或说为映射到进程空间。两个不同进程A、B共享内存的意思是，同一块物理内存被映射
	到进程A、B各自的进程地址空间。进程A可以即时看到进程B对共享内存中数据的更新，反之亦然。由于多个进程共享同一块内存区域，必然
	需要某种同步机制，互斥锁和信号量都可以。采用共享内存通信的一个显而易见的好处是效率高，因为进程可以直接读写内存，而不需要任
	何数据的拷贝。对于像管道和消息队列等通信方式，则需要在内核和用户空间进行四次的数据拷贝，而共享内存则只拷贝两次数据[1]：一次
	从输入文件到共享内存区，另一次从共享内存区到输出文件。实际上，进程之间在共享内存时，并不总是读写少量数据后就解除映射，有新的
	通信时，再重新建立共享内存区域。而是保持共享区域，直到通信完毕为止，这样，数据内容一直保存在共享内存中，并没有写回文件。共享
	内存中的内容往往是在解除映射时才写回文件的。因此，采用共享内存的通信方式效率是非常高的。
```

# 3. ELF是什么？
```
		可执行连接格式，是UNIX系统实验室（USL）作为应用程序二进制接口（Application Binary Interface，ABI）而开发和发布的。扩
	展名为elf。工具接口标准委员会(TIS)选择了正在发展中的ELF标准作为工作在32位INTEL体系上不同操作系统之间可移植的二进制文件格
	式。假定开发者定义了一个二进制接口集合，ELF标准用它来支持流线型的软件发展。应该减少不同执行接口的数量。因此可以减少重新编程
	重新编译的代码。
	ELF文件（目标文件）格式主要三种:
	1）可重定向文件：文件保存着代码和适当的数据，用来和其他的目标文件一起来创建一个可执行文件或者是一个共享目标文件。（目标文件或者
	静态库文件，即linux通常后缀为.a和.o的文件）
	2）可执行文件：文件保存着一个用来执行的程序。（例如bash，gcc等）
	3）共享目标文件：共享库。文件保存着代码和合适的数据，用来被下连接编辑器和动态链接器链接。（linux下后缀为.so的文件。）
	
BSS段

		BSS（Block Started by Symbol）通常是指用来存放程序中未初始化的全局变量和静态变量的一块内存区域。特点是:可读写的，在
	程序执行之前BSS段会自动清0。所以，未初始的全局变量在程序执行之前已经成0了。
	注意和数据段的区别，BSS存放的是未初始化的全局变量和静态变量，数据段存放的是初始化后的全局变量和静态变量。
	
内存分布

	可执行二进制程序 = 代码段(text)＋数据段(data)+BSS段
	正在运行的C程序 = 代码段+初始化数据段(data)+未初始化数据段(BSS)+堆+栈
```

	
# 4. 进程之间通讯方式
```
	进程间通信：
	管道、有名管道、消息队列、信号量、共享内存、信号、套接字。
	
	线程间通信：
	（1）原子操作符集
	（2）关键代码段（Windows）
	（3）互斥锁
	（4）信号量
	（5）条件变量（Linux）
	（6）读写锁（Linux）
```


# 5. 如何定位内存泄露？
```
		内存泄漏是指堆内存的泄漏。堆内存是指程序从堆中分配的、大小任意的（内存块的大小可以在程序运行期决定）、使用完后必须显示释
	放的内存。应用程序一般使用malloc、realloc、new等函数从堆中分配到一块内存，使用完后，程序必须负责相应的调用free或delete
	释放该内存块。否则，这块内存就不能被再次使用，我们就说这块内存泄漏了。
		C++程序缺乏相应的手段来检测内存信息，只能使用top指令观察进程的动态内存总额。而且程序退出时，我们无法获知任何内存泄漏信息
	使用Linux命令回收内存，可以使用ps、kill两个命令检测内存使用情况和进行回收。在使用超级用户权限时使用命令“ps”，它会列出所有正在运行
	的程序名称和对应的进程号（PID）。kill命令的工作原理是向Linux操作系统的内核送出一个系统操作信号和程序的进程号（PID）
```


# 6. 动态链接和静态链接的区别
```
	动态链接是指在生成可执行文件时不将所有程序用到的函数链接到一个文件，因为有许多函数在操作系统带的dll文件中，当程序运行时直接从操作系统中找。 而静态链接就是把所有用到的函数全部链接到exe文件中。
	动态链接是只建立一个引用的接口，而真正的代码和数据存放在另外的可执行模块中，在运行时再装入；而静态链接是把所有的代码和数据都复制到本模块中，运行时就不再需要库了。
```

# 7. 大端or小端字节序
```
c程序辨别系统是16位or32位
	法一：int k=~0;
	
	if((unsigned int)k >63356) cout<<"at least 32bits"<<endl;
	else cout<<"16 bits"<<endl;
	
	法二：//32为系统
	
	int i=65536;
	cout<<i<<endl;
	int j=65535;
	cout<<j<<endl;
	
大端or小端字节序

	1) Little-Endian就是低位字节排放在内存的低地址端，高位字节排放在内存的高地址端。
	2) Big-Endian就是高位字节排放在内存的低地址端，低位字节排放在内存的高地址端。
	举一个例子，比如数字0x12 34 56 78在内存中的表示形式为：
	
	1)大端模式：
	低地址 -----------------> 高地址
	0x12  |  0x34  |  0x56  |  0x78
	2)小端模式：
	低地址 ------------------> 高地址
	0x78  |  0x56  |  0x34  |  0x12
	
	32bit宽的数0x12345678在Little-endian模式以及Big-endian模式）CPU内存中的存放方式（假设从地址0x4000开始存放）为：
	内存地址	小端模式存放内容    大端模式存放内容
	0x4000		0x78		0x12
	0x4001		0x56		0x34
	0x4002		0x34		0x56
	0x4003		0x12		0x78
	```
  
  
	4)大端小端没有谁优谁劣，各自优势便是对方劣势：
	小端模式 ：强制转换数据不需要调整字节内容，1、2、4字节的存储方式一样。
	大端模式 ：符号位的判定固定为第一个字节，容易判断正负。


	BOOL IsBigEndian()  
	{  
	    int a = 0x1234;  
	    char b =  *(char *)&a;  //通过将int强制类型转换成char单字节，通过判断起始存储位置。即等于 取b等于a的低地址部分  
	    if( b == 0x12)  
	    {  
	        return TRUE;  
	    }  
	    return FALSE;  
	}

	联合体union的存放顺序是所有成员都从低地址开始存放，利用该特性可以轻松地获得了CPU对内存采用Little-endian还是Big-endian模式读写：

	BOOL IsBigEndian()  
	{  
	    union NUM  
	    {  
	        int a;  
	        char b;  
	    }num;  
	    num.a = 0x1234;  
	    if( num.b == 0x12 )  
	    {  
	        return TRUE;  
	    }  
	    return FALSE;  
	}
```

# 8. i++是否原子操作？并解释为什么
```	
	i++分为三个阶段：
	内存到寄存器
	寄存器自增
	写回内存
	这三个阶段中间都可以被中断分离开.
```

# 9. linux系统的各类同步机制
```	
	自旋锁,读写锁,原子操作,信号量,等待队列,

死锁必要条件及避免算法、

	1、互斥条件，资源不能共享，只能由一个进程使用。
	2、请求与保持条件（Hold andwait）：一个进程因请求资源而阻塞时，对已获得的资源保持不放。
	3、不剥夺条件（Nopre-emption）：进程已获得的资源，在末使用完之前，不能强行剥夺。
	4、循环等待：系统中若干进程组成环路，该环路中每个进程都在等待相邻进程正占用的资源

	处理死锁的策略：
	1.忽略该问题。例如鸵鸟算法，该算法可以应用在极少发生死锁的的情况下。为什么叫鸵鸟算法呢，因为传说中鸵鸟看到危险就把头埋在地底下，可能鸵鸟觉得看不到危险也就没危险了吧。跟掩耳盗铃有点像。
	2.检测死锁并且恢复。
	3.仔细地对资源进行动态分配，以避免死锁。
	4.通过破除死锁四个必要条件之一，来防止死锁产生。
```

# 10. exit()与_exit()的区别？
```
	_exit终止调用进程，但不关闭文件，不清除输出缓存，也不调用出口函数。
	exit函数将终止调用进程。在退出程序之前，所有文件关闭，缓冲输出内容将刷新定义，并调用所有已刷新的“出口函数”
```

# 11. linux的内存管理机制是什么？
```
		Linux虚拟内存的实现需要6种机制的支持：地址映射机制、内存分配回收机制、缓存和刷新机制、请求页机制、交换机制和内存共享机制
		内存管理程序通过映射机制把用户程序的逻辑地址映射到物理地址。当用户程序运行时，如果发现程序中要用的虚地址没有对应的物理内存，
	就发出了请求页要求。如果有空闲的内存可供分配，就请求分配内存(于是用到了内存的分配和回收)，并把正在使用的物理页记录在缓存中(使用了缓
	存机制)。如果没有足够的内存可供分配，那么就调用交换机制；腾出一部分内存。另外，在地址映射中要通过TLB(翻译后援存储器)来寻找物理页；
	交换机制中也要用到交换缓存，并且把物理页内容交换到交换文件中，也要修改页表来映射文件地址。

任务调度算法：

	1. 先来先服务
	2. 短作业优先算法
	3. 时间片轮转算法
	4. 优先级算法
```


# 12. 五种I/O 模式—
```
	【1】阻塞I/O         (Linux下的I/O操作默认是阻塞I/O，即open和socket创建的I/O都是阻塞I/O)
	【2】非阻塞 I/O      (可以通过fcntl或者open时使用O_NONBLOCK参数，将fd设置为非阻塞的I/O)
	【3】I/O 多路复用    (I/O多路复用，通常需要非阻塞I/O配合使用)
	【4】信号驱动 I/O    (SIGIO)
	【5】异步 I/O

Apache 模型（Process Per Connection，简称PPC），TPC（ThreadPer Connection）模型，以及 select 模型和 poll 模型，epoll模型

	 PPC/TPC 模型
		这两种模型思想类似，就是让每一个到来的连接一边自己做事去，别再来烦我。只是 PPC 是为它开了一个进程，而 TPC 开了一个线程。
	可是别烦我是有代价的，它要时间和空间啊，连接多了之后，那么多的进程 / 线程切换，这开销就上来了；因此这类模型能接受的最大连接数都不
	会高，一般在几百个左右。
```

# 13. select和epoll的区别
```
	1. 最大并发数限制，因为一个进程所打开的 FD （文件描述符）是有限制的，由FD_SETSIZE 设置，默认值是 1024/2048 ，因此 Select
	模型的最大并发数就被相应限制了。自己改改这个 FD_SETSIZE ？想法虽好，可是先看看下面吧 …
	2. 效率问题， select 每次调用都会线性扫描全部的 FD 集合，这样效率就会呈现线性下降，把 FD_SETSIZE 改大的后果就是，大家都慢慢来.
	3. 内核 / 用户空间内存拷贝问题，如何让内核把 FD 消息通知给用户空间呢？在这个问题上 select 采取了内存拷贝方法。

	poll 模型
	基本上效率和select 是相同的，select 缺点的 2 和 3 它都没有改掉。

	Epoll 的提升
	把其他模型逐个批判了一下，再来看看 Epoll 的改进之处吧，其实把 select 的缺点反过来那就是 Epoll 的优点了。
	3.1. Epoll 没有最大并发连接的限制，上限是最大可以打开文件的数目，这个数字一般远大于 2048, 一般来说这个数目和系统内存关系很大，
	具体数目可以 cat /proc/sys/fs/file-max 察看。
	3.2. 效率提升， Epoll 最大的优点就在于它只管你“活跃”的连接，而跟连接总数无关，因此在实际的网络环境中， Epoll 的效率就会远远
	高于 select 和 poll 。
	3.3. 内存拷贝， Epoll 在这点上使用了“共享内存 ”，这个内存拷贝也省略了。

select和epoll的区别
	
		select的本质是采用32个整数的32位，即32*32= 1024来标识，fd值为1-1024。当fd的值超过1024限制时，就必须修改FD_SETSIZE
	的大小。这个时候就可以标识32*max值范围的fd。

	对于单进程多线程，每个线程处理多个fd的情况，select是不适合的。
	1.所有的线程均是从1-32*max进行扫描，每个线程处理的均是一段fd值，这样做有点浪费
	2.1024上限问题，一个处理多个用户的进程，fd值远远大于1024
	所以这个时候应该采用poll，
	
		poll传递的是数组头指针和该数组的长度，只要数组的长度不是很长，性能还是很不错的，因为poll一次在内核中申请4K（一个页的大小
	来存放fd），尽量控制在4K以内.
		epoll还是poll的一种优化，返回后不需要对所有的fd进行遍历，在内核中维持了fd的列表。select和poll是将这个内核列表维持在用
	户态，然后传递到内核中。但是只有在2.6的内核才支持。
		epoll更适合于处理大量的fd ，且活跃fd不是很多的情况，毕竟fd较多还是一个串行的操作
	
	epoll哪些触发模式，有啥区别？（必须非常详尽的解释水平触发和边缘触发的区别，以及边缘触发在编程中要做哪些更多的确认）
		epoll可以同时支持水平触发和边缘触发（Edge Triggered，只告诉进程哪些文件描述符刚刚变为就绪状态，它只说一遍，如果我们没有
	采取行动，那么它将不会再次告知，这种方式称为边缘触发），理论上边缘触发的性能要更高一些，但是代码实现相当复杂。
		epoll同样只告知那些就绪的文件描述符，而且当我们调用epoll_wait()获得就绪文件描述符时，返回的不是实际的描述符，而是一个代表
	就绪描述符数量的值，你只需要去epoll指定的一个数组中依次取得相应数量的文件描述符即可，这里也使用了内存映射（mmap）技术，这样便彻底省掉
	了这些文件描述符在系统调用时复制的开销。
		另一个本质的改进在于epoll采用基于事件的就绪通知方式。在select/poll中，进程只有在调用一定的方法后，内核才对所有监视的文件描
	述符进行扫描，而epoll事先通过epoll_ctl()来注册一个文件描述符，一旦基于某个文件描述符就绪时，内核会采用类似callback的回调机制，迅速
	激活这个文件描述符，当进程调用epoll_wait()时便得到通知。	
```

# 14. 一个String类的实现
```cpp
	class String  
	{  
	public:  
		String(const char *str = NULL);// 普通构造函数    
		String(const String &other);// 拷贝构造函数    
		~String(void);// 析构函数    
		String & operator = (const String &other);// 赋值函数    
	private:  
		char *m_data;// 用于保存字符串    
	};  
	请编写String的上述4个函数。

	//普通构造函数    
	String::String(const char *str)  
	{  
		if (str == NULL)  
		{  
			m_data = new char[1];// 得分点：对空字符串自动申请存放结束标志'\0'的，加分点：对m_data加NULL判断    
			*m_data = '\0';  
		}  
		else  
		{  
			int length = strlen(str);  
			m_data = new char[length + 1];// 若能加 NULL 判断则更好    
			strcpy(m_data, str);  
		}  
	}  

	// String的析构函数    
	String::~String(void)  
	{  
		delete[] m_data; // 或delete m_data;    
	}  

	//拷贝构造函数    
	String::String(const String &other)// 得分点：输入参数为const型    
	{          
		int length = strlen(other.m_data);  
		m_data = new char[length + 1];//加分点：对m_data加NULL判断    
		strcpy(m_data, other.m_data);  
	}  

	//赋值函数    
	String & String::operator = (const String &other) // 得分点：输入参数为const型    
	{  
		if (this == &other)//得分点：检查自赋值    
			return *this;   
		if (m_data)  
			delete[] m_data;//得分点：释放原有的内存资源    
		int length = strlen(other.m_data);  
		m_data = new char[length + 1];//加分点：对m_data加NULL判断    
		strcpy(m_data, other.m_data);  
		return *this;//得分点：返回本对象的引用      
	}
  
#include <utility>
#include <string.h>
 
class String
{
 public:
  String()
    : data_(new char[1])
  {
    *data_ = '\0';
  }
 
  String(const char* str)
    : data_(new char[strlen(str) + 1])
  {
    strcpy(data_, str);
  }
 
  String(const String& rhs)
    : data_(new char[rhs.size() + 1])
  {
    strcpy(data_, rhs.c_str());
  }
  /* Delegate constructor in C++11
  String(const String& rhs)
    : String(rhs.data_)
  {
  }
  */
 
  ~String()
  {
    delete[] data_;
  }
 
  /* Traditional:
  String& operator=(const String& rhs)
  {
    String tmp(rhs);
    swap(tmp);
    return *this;
  }
  */
  String& operator=(String rhs) // yes, pass-by-value
  {
    swap(rhs);
    return *this;
  }
 
  // C++ 11
  String(String&& rhs)
    : data_(rhs.data_)
  {
    rhs.data_ = nullptr;
  }
 
  String& operator=(String&& rhs)
  {
    swap(rhs);
    return *this;
  }
 
  // Accessors
 
  size_t size() const
  {
    return strlen(data_);
  }
 
  const char* c_str() const
  {
    return data_;
  }
 
  void swap(String& rhs)
  {
    std::swap(data_, rhs.data_);
  }
 
 private:
  char* data_;
};  
```


# 15. 虚函数的作用和实现原理，什么是虚函数,有什么作用?
```
	C++的多态分为静态多态（编译时多态）和动态多态（运行时多态）两大类。
	静态多态通过重载、模板来实现；
	动态多态就是通过本文的主角虚函数来体现的。	
	
	虚函数实现原理:包括虚函数表、虚函数指针等 。
	如果是基类的实例，对应位置存放的是基类的函数指针；
	如果是继承类，对应位置存放的是继承类的函数指针（如果在继承类有实现）。
	所以 ，当使用基类指针调用对象方法时，也会根据具体的实例，调用到继承类的方法
	
	虚函数的作用：
		当调用一个虚函数时，被执行的代码必须和调用函数的对象的动态类型相一致。编译器需要做的就是如何高效的实现提供这种特性。
	不同编译器实现细节也不相同。大多数编译器通过vtbl（virtual table）和vptr（virtual table pointer）来实现的。
	当一个类声明了虚函数或者继承了虚函数，这个类就会有自己的vtbl。vtbl实际上就是一个函数指针数组，有的编译器用的是链表，
	不过方法都是差不多。vtbl数组中的每一个元素对应一个函数指针指向该类的一个虚函数，同时该类的每一个对象都会包含一个vptr，
	vptr指向该vtbl的地址。

	结论：
	每个声明了虚函数或者继承了虚函数的类，都会有一个自己的vtbl
	同时该类的每个对象都会包含一个vptr去指向该vtbl
	虚函数按照其声明顺序放于vtbl表中, vtbl数组中的每一个元素对应一个函数指针指向该类的虚函数
	如果子类覆盖了父类的虚函数，将被放到了虚表中原来父类虚函数的位置
	在多继承的情况下，每个父类都有自己的虚表。子类的成员函数被放到了第一个父类的表中
```

# 16. 指针和引用的区别
```
	相同点：
	1. 都是地址的概念；
	指针指向一块内存，它的内容是所指内存的地址；引用是某块内存的别名。

	区别：
	1. 指针是一个实体，而引用仅是个别名；
	2. 引用使用时无需解引用(*)，指针需要解引用；
	3. 引用只能在定义时被初始化一次，之后不可变；指针可变；
	4. 引用没有 const，指针有 const；
	5. 引用不能为空，指针可以为空；
	6. “sizeof 引用”得到的是所指向的变量(对象)的大小，而“sizeof 指针”得到的是指针本身(所指向的变量或对象的地址)的大小；
	7. 指针和引用的自增(++)运算意义不一样；
	8. 从内存分配上看：程序为指针变量分配内存区域，而引用不需要分配内存区域。
```

# 17. 多重类构造和析构的顺序
```
	先调用基类的构造函数，在调用派生类的构造函数
	先构造的后析构，后构造的先析构
```


# 18. STL各容器的实现原理
```	
	STL共有六大组件:1、容器。2、算法。3、迭代器。4、仿函数。6、适配器。

	序列式容器：
	vector - 数组，元素不够时再重新分配内存，拷贝原来数组的元素到新分配的数组中。
	list － 单链表。
	deque - 分配中央控制器map(并非map容器)，map记录着一系列的固定长度的数组的地址.记住这个map仅仅保存的是数组的地址,
	真正的数据在数组中存放着.deque先从map中央的位置(因为双向队列，前后都可以插入元素)找到一个数组地址，向该数组中放入数据，
	数组不够时继续在map中找空闲的数组来存数据。当map也不够时重新分配内存当作新的map,把原来map中的内容copy的新map中。
	所以使用deque的复杂度要大于vector，尽量使用vector。
	stack-基于deque。
	queue-基于deque。
	heap-完全二叉树，使用最大堆排序，以数组(vector)的形式存放。
	priority_queue-基于heap。
	slist-双向链表。

	关联式容器：
	set,map,multiset,multimap-基于红黑树(RB-tree)，一种加上了额外平衡条件的二叉搜索树。
	
	hash table-散列表。将待存数据的key经过映射函数变成一个数组(一般是vector)的索引，例如：数据的key%数组的大小＝数组的索引
	(一般文本通过算法也可以转换为数字)，然后将数据当作此索引的数组元素。有些数据的key经过算法的转换可能是同一个数组的索引值
	(碰撞问题，可以用线性探测，二次探测来解决)，STL是用开链的方法来解决的，每一个数组的元素维护一个list，他把相同索引值的数据
	存入一个list，这样当list比较短时执行删除，插入，搜索等算法比较快。

	hash_map,hash_set,hash_multiset,hash_multimap-基于hashtable。
```


# 20. volatile作用
```
	volatile的本意是“易变的” 因为访问寄存器要比访问内存单元快的多,所以编译器一般都会作减少存取内存的优化，但有可能会读脏数据。
	当要求使用volatile声明变量值的时候，系统总是重新从它所在的内存读取数据，即使它前面的指令刚刚从该处读取过数据。精确地说就是，
	遇到这个关键字声明的变量，编译器对访问该变量的代码就不再进行优化，从而可以提供对特殊地址的稳定访问；如果不使用volatile，
	则编译器将对所声明的语句进行优化。（简洁的说就是：volatile关键词影响编译器编译的结果，用volatile声明的变量表示该变量随时
	可能发生变化，与该变量有关的运算，不要进行编译优化，以免出错）
```

# 21. tcp与udp的区别
```
	1．基于连接与无连接 
	2．对系统资源的要求（TCP较多，UDP少） 
	3．UDP程序结构较简单 
	4．流模式与数据报模式
	5．TCP保证数据正确性，UDP可能丢包，TCP保证数据顺序，UDP不保证

	TCP---传输控制协议,提供的是面向连接、可靠的字节流服务。当客户和服务器彼此交换数据前，必须先在双方之间建立一个TCP连接，
	之后才能传输数据。TCP提供超时重发，丢弃重复数据，检验数据，流量控制等功能，保证数据能从一端传到另一端。
	UDP---用户数据报协议，是一个简单的面向数据报的运输层协议。UDP不提供可靠性，它只是把应用程序传给IP层的数据报发送出去，
	但是并不能保证它们能到达目的地。由于UDP在传输数据报前不用在客户和服务器之间建立一个连接，且没有超时重发等机制，故而传输速度很快
```

# 22. udp调用connect有什么作用？
```
	1:UDP中可以使用connect系统调用
	2:UDP中connect操作与TCP中connect操作有着本质区别.TCP中调用connect会引起三次握手,client与server建立连结.
	UDP中调用connect内核仅仅把对端ip&port记录下来.
	3:UDP中可以多次调用connect,TCP只能调用一次connect.
	UDP多次调用connect有两种用途:
	#1,指定一个新的ip&port连结.
	#2,断开和之前的ip&port的连结.指定新连结,直接设置connect第二个参数即可.断开连结,需要将connect第二个参数中的sin_family设置成 AF_UNSPEC即可. 
	4:UDP中使用connect可以提高效率.原因如下:
	普通的UDP发送两个报文内核做了如下:
	#1:建立连结
	#2:发送报文
	#3:断开连结
	#4:建立连结
	#5:发送报文
	#6:断开连结
	采用connect方式的UDP发送两个报文内核如下处理:
	#1:建立连结
	#2:发送报文
	#3:发送报文另外一点,每次发送报文内核都由可能要做路由查询.
	
	5:采用connect的UDP发送接受报文可以调用send,write和recv,read操作.当然也可以调用sendto,recvfrom.调用sendto的时候第五个参数必须是NULL,第六个参数是0.
	ssize_t sendto(int socket, const void *buffer, size_t length, int flags, const struct sockaddr *dest_addr, socklen_t dest_len);
     
	调用recvfrom,recv,read系统调用只能获取到先前connect的ip&port发送的报文. 
	ssize_t recvfrom(int socket, void *restrict buffer, size_t length, int flags, struct sockaddr *restrict address, socklen_t *restrict address_len);
     
	UDP中使用connect的好处:
	1:会提升效率.前面已经描述了.
	2:高并发服务中会增加系统稳定性.
	原因:假设client A 通过非connect的UDP与serverB,C通信.B,C提供相同服务.为了负载均衡,我们让A与B,C交替通信.
	A 与 B通信IPa:PORTa<----> IPb:PORTbA 与 C通信IPa:PORTa'<---->IPc:PORTc 
	假设PORTa 与 PORTa'相同了(在大并发情况下会发生这种情况),那么就有可能出现A等待B的报文,却收到了C的报文.导致收报错误.
	解决方法内就是采用connect的UDP通信方式.在A中创建两个udp,然后分别connect到B,C.
```

# 23. 大规模连接上来，并发模型怎么设计
```
	增加机群, Nginx负载均衡 + 自己设计的负载均衡策略
```

# 24. tcp三次握手：
```
	（1）第一次握手：Client将标志位SYN置为1，随机产生一个值seq=J，并将该数据包发送给Server，Client进入SYN_SENT状态，
	等待Server确认。
        （2）第二次握手：Server收到数据包后由标志位SYN=1知道Client请求建立连接，Server将标志位SYN和ACK都置为1，ack=J+1，
	随机产生一个值seq=K，并将该数据包发送给Client以确认连接请求，Server进入SYN_RCVD状态。
        （3）第三次握手：Client收到确认后，检查ack是否为J+1，ACK是否为1，如果正确则将标志位ACK置为1，ack=K+1，
	并将该数据包发送给Server，Server检查ack是否为K+1，ACK是否为1，如果正确则连接建立成功，Client和Server
	进入ESTABLISHED状态，完成三次握手，随后Client与Server之间可以开始传输数据了。

	SYN攻击：
                在三次握手过程中，Server发送SYN-ACK之后，收到Client的ACK之前的TCP连接称为半连接（half-open connect），
	此时Server处于SYN_RCVD状态，当收到ACK后，Server转入ESTABLISHED状态。SYN攻击就是Client在短时间内伪造大量不存在
	的IP地址，并向Server不断地发送SYN包，Server回复确认包，并等待Client的确认，由于源地址是不存在的，因此，Server需要
	不断重发直至超时，这些伪造的SYN包将产时间占用未连接队列，导致正常的SYN请求因为队列满而被丢弃，从而引起网络堵塞甚至系统
	瘫痪。SYN攻击时一种典型的DDOS攻击，检测SYN攻击的方式非常简单，即当Server上有大量半连接状态且源IP地址是随机的，则可以
	断定遭到SYN攻击了，使用如下命令可以让之现行：
                #netstat -nap | grep SYN_RECV

tcp四次挥手：
	
	（1）第一次挥手：Client发送一个FIN，用来关闭Client到Server的数据传送，Client进入FIN_WAIT_1状态。
       （2）第二次挥手：Server收到FIN后，发送一个ACK给Client，确认序号为收到序号+1（与SYN相同，一个FIN占用一个序号），
       Server进入CLOSE_WAIT状态。
       （3）第三次挥手：Server发送一个FIN，用来关闭Server到Client的数据传送，Server进入LAST_ACK状态。
       （4）第四次挥手：Client收到FIN后，Client进入TIME_WAIT状态，接着发送一个ACK给Server，确认序号为收到序号+1，
       Server进入CLOSED状态，完成四次挥手。

time_wait状态:

	TIME_WAIT：表示收到了对方的FIN报文，并发送出了ACK报文。 TIME_WAIT状态下的TCP连接会等待2*MSL（Max Segment Lifetime，
	最大分段生存期，指一个TCP报文在Internet上的最长生存时间。每个具体的TCP协议实现都必须选择一个确定的MSL值，RFC 1122建议是
	2分钟，但BSD传统实现采用了30秒，Linux可以cat /proc/sys/net/ipv4/tcp_fin_timeout看到本机的这个值），然后即可回到
	CLOSED 可用状态了。如果FIN_WAIT_1状态下，收到了对方同时带FIN标志和ACK标志的报文时，可以直接进入到TIME_WAIT状态，而无须
	经过FIN_WAIT_2状态。
	
	time_wait状态产生的原因:
	1）为实现TCP全双工连接的可靠释放
	假设发起主动关闭的一方（client）最后发送的ACK在网络中丢失，由于TCP协议的重传机制，执行被动关闭的一方（server）
	将会重发其FIN，在该FIN到达client之前，client必须维护这条连接状态，也就说这条TCP连接所对应的资源（client方的
	local_ip,local_port）不能被立即释放或重新分配，直到另一方重发的FIN达到之后，client重发ACK后，经过2MSL时间
	周期没有再收到另一方的FIN之后，该TCP连接才能恢复初始的CLOSED状态。如果主动关闭一方不维护这样一个TIME_WAIT状态，
	那么当被动关闭一方重发的FIN到达时，主动关闭一方的TCP传输层会用RST包响应对方，这会被对方认为是有错误发生，然而这
	事实上只是正常的关闭连接过程，并非异常。
	2）为使旧的数据包在网络因过期而消失
	我们先假设TCP协议中不存在TIME_WAIT状态的限制，再假设当前有一条TCP连接：(local_ip, local_port, remote_ip,
	remote_port)，因某些原因，我们先关闭，接着很快以相同的四元组建立一条新连接。本文前面介绍过，TCP连接由四元组唯一
	标识，因此，在我们假设的情况中，TCP协议栈是无法区分前后两条TCP连接的不同的，在它看来，这根本就是同一条连接，中间
	先释放再建立的过程对其来说是“感知”不到的。这样就可能发生这样的情况：前一条TCP连接由local peer发送的数据到达
	remote peer后，会被该remot peer的TCP传输层当做当前TCP连接的正常数据接收并向上传递至应用层（而事实上，在我们假
	设的场景下，这些旧数据到达remote peer前，旧连接已断开且一条由相同四元组构成的新TCP连接已建立，因此，这些旧数据是
	不应该被向上传递至应用层的），从而引起数据错乱进而导致各种无法预知的诡异现象。作为一种可靠的传输协议，TCP必须在协议
	层面考虑并避免这种情况的发生，这正是TIME_WAIT状态存在的第2个原因。
	
	具体而言，local peer主动调用close后，此时的TCP连接进入TIME_WAIT状态，处于该状态下的TCP连接不能立即以同样的四
	元组建立新连接，即发起active close的那方占用的local port在TIME_WAIT期间不能再被重新分配。由于TIME_WAIT状态
	持续时间为2MSL，这样保证了旧TCP连接双工链路中的旧数据包均因过期（超过MSL）而消失，此后，就可以用相同的四元组建立
	一条新连接而不会发生前后两次连接数据错乱的情况。
	
	time_wait状态如何避免
	首先服务器可以设置SO_REUSEADDR套接字选项来通知内核，如果端口忙，但TCP连接位于TIME_WAIT状态时可以重用端口。
	在一个非常有用的场景就是，如果你的服务器程序停止后想立即重启，而新的套接字依旧希望使用同一端口，此时SO_REUSEADDR
	选项就可以避免TIME_WAIT状态。
```

# 25. 什么是滑动窗口，超时重传，拥塞控制
```
	超时重传：
	在发送某一个数据以后就开启一个计时器，在一定时间内如果没有得到发送的数据报的ACK报文，那么就重新发送数据，直到发送
	成功为止。
	影响超时重传机制协议效率的一个关键参数是重传超时时间（RTO，Retransmission TimeOut）。RTO的值被设置过大过小都
	会对协议造成不利影响。 
	（1）RTO设长了，重发就慢，没有效率，性能差。 
	（2）RTO设短了，重发的就快，会增加网络拥塞，导致更多的超时，更多的超时导致更多的重发。

	滑动窗口：
	接收端可以根据自己的状况通告窗口大小，从而控制发送端的接收，进行流量控制。
	发送窗口只有收到对端对于本段发送窗口内字节的ACK确认，才会移动发送窗口的左边界。 

	拥塞控制：
	TCP拥塞控制4个核心算法：慢开始（slow start）、拥塞避免（Congestion Avoidance）、快速重传（fast retransmit）
	、快速回复（fast recovery） 
	拥塞窗口（cwnd，congestion window），其大小取决于网络的拥塞程度，并且动态地在变化。 
	慢开始算法的思路就是，不要一开始就发送大量的数据，先探测一下网络的拥塞程度，也就是说由小到大逐渐增加拥塞窗口的大小。
	拥塞避免算法让拥塞窗口缓慢增长，即每经过一个往返时间RTT就把发送发的拥塞窗口cwnd加1，而不是加倍。
	快速重传(Fast retransmit)要求接收方在收到一个失序的报文段后就立即发出重复确认（为的是使发送方及早知道有报文段没
	有到达对方），而不要等到自己发送数据时捎带确认。快重传算法规定，发送方只要一连收到3个重复确认就应当立即重传对方尚未
	收到的报文段，而不必继续等待设置的重传计数器时间到期。
	快速恢复(Fast Recovery) 
	（1）当发送方连续收到三个重复确认，就执行“乘法减小”算法，把慢开始门限ssthresh减半。这是为了预防网络发生拥塞。
	请注意：接下去不执行慢开始算法。 
	（2）由于发送方现在认为网络很可能没有发生拥塞，因此与慢开始不同之处是现在不执行慢开始算法（即拥塞窗口cwnd现在
	不设置为1），而是把cwnd值设置为慢开始门限ssthresh减半后的数值，然后开始执行拥塞避免算法（“加法增大”），使拥
	塞窗口缓慢地线性增大。
```

# 26. keepalive是什么东东？如何使用
```
	设置Keepalive参数，检测已中断的客户连接
	在TCP中有一个Keep-alive的机制可以检测死连接，原理很简单，TCP会在空闲了一定时间后发送数据给对方：
	1.如果主机可达，对方就会响应ACK应答，就认为是存活的。
	2.如果可达，但应用程序退出，对方就发RST应答，发送TCP撤消连接。
	3.如果可达，但应用程序崩溃，对方就发FIN消息。
	4.如果对方主机不响应ack, rst，继续发送直到超时，就撤消连接。这个时间就是默认的二个小时。

OFFSETOF(s, m)的宏定义，s是结构类型，m是s的成员，求m在s中的偏移量。

	#define OFFSETOF(s, m) ({s s1;(void*)(&s1)-(void*)(&s1->m);})/*gcc*/


100亿个数，求最大的1万个数，并说出算法的时间复杂度。
	
	建一个堆,先把最开始的1万个数放进去。以后每进一个，都把最小的赶出来。

```

# 27. c++
```
1.new、delete、malloc、free之间的关系

malloc和free都是C/C++语言的标准库函数，new/delete是C++的运算符。

new调用构造函数，delete会调用对象的析构函数，而free只会释放内存。

它们都可用于申请动态内存和释放内存。但对于非内部数据类型的对象而言，光用malloc/free无法满足动态对象的要求。对象在创建的同时要自动执行构造函数，对象在消亡之前要自动执行析构函数。由于malloc/free是库函数而不是运算符，不在编译器控制权限之内，不能够把执行构造函数和析构函数的任务强加给malloc/free。因此C++语言需要一个能完成动态内存分配和初始化工作的运算符new，以及一个能完成清理与释放内存工作的运算符delete。注意：new/delete不是库函数。

 

2.delete和delete []的区别

delete只会调用一次析构函数，而delete[]会调用每一个成员函数的析构函数。

在More Effective C++中有更为详细的解释：“当delete操作符用于数组时，它为每个数组元素调用析构函数，然后调用operator delete来释放内存。”delete与new配套，delete []与new []配套
复制代码

 1 MemTest *mTest1=new MemTest[10];
 2 
 3 MemTest *mTest2=new MemTest;
 4 
 5 Int *pInt1=new int [10];
 6 
 7 Int *pInt2=new int;
 8 
 9 delete[]pInt1; //-1-
10 
11 delete[]pInt2; //-2-
12 
13 delete[]mTest1;//-3-
14 
15 delete[]mTest2;//-4-

复制代码

在-4-处报错。

这就说明：对于内建简单数据类型，delete和delete[]功能是相同的。对于自定义的复杂数据类型，delete和delete[]不能互用。delete[]删除一个数组，delete删除一个指针。简单来说，用new分配的内存用delete删除；用new[]分配的内存用delete[]删除。delete[]会调用数组元素的析构函数。内部数据类型没有析构函数，所以问题不大。如果你在用delete时没用括号，delete就会认为指向的是单个对象，否则，它就会认为指向的是一个数组。

 

3.C++有哪些性质（面向对象特点）

封装、继承和多态

 

4.子类析构时要调用父类的析构函数吗？

析构函数调用的次序是先派生类的析构后基类的析构，也就是说在基类的的析构调用的时候,派生类的信息已经全部销毁了。定义一个对象时先调用基类的构造函数、然后调用派生类的构造函数；析构的时候恰好相反：先调用派生类的析构函数、然后调用基类的析构函数。

 

5.介绍多态、虚函数和纯虚函数。

多态：是对于不同对象接收相同消息时产生不同的动作。C++的多态性具体体现在运行和编译两个方面：在程序运行时的多态性通过继承和虚函数来体现；在程序编译时多态性体现在函数和运算符的重载上；

虚函数：在基类中冠以关键字 virtual 的成员函数。 它提供了一种接口界面。允许在派生类中对基类的虚函数重新定义。

纯虚函数的作用：在基类中为其派生类保留一个函数的名字，以便派生类根据需要对它进行定义。作为接口而存在的纯虚函数不具备函数的功能，一般不能直接被调用。

从基类继承来的纯虚函数，在派生类中仍是虚函数。如果一个类中至少有一个纯虚函数，那么这个类被称为抽象类（abstract class）。

抽象类中不仅包括纯虚函数，也可包括虚函数。抽象类是必须用作派生其他类的基类，而不能用于直接创建对象实例。但仍可使用指向抽象类的指针支持运行时多态性。

注：

定义一个函数为虚函数，不代表函数为不被实现的函数。
定义他为虚函数是为了允许用基类的指针来调用子类的这个函数。
定义一个函数为纯虚函数，才代表函数没有被实现。

 

6.求下面函数的返回值（微软）
复制代码

 1 int func(x)
 2 {
 3     int countx = 0;
 4     while(x)
 5     {
 6         countx ++;
 7         x = x&(x-1);
 8     }
 9     return countx;
10 }

复制代码

假定x = 9999。 答案：8

思路：将x转化为2进制，看含有的1的个数

 

7.什么是“引用”？申明和使用“引用”要注意哪些问题？

引用就是某个目标变量的“别名”，对应用的操作与变量直接操作效果完全相同。声明一个引用的时候，切记要对其进行初始化。引用声明完毕后，相当于目标变量名有两个名称，即该目标原名称和引用名，不能再把该引用名作为其他变量名的别名。声明一个引用，不是新定义了一个变量，它只表示该引用名是目标变量名的一个别名，它本身不是一种数据类型，因为该引用本身不占存储单元，系统也不给引用分配存储单元。不能建立数组的引用。

 

8.将“引用”作为函数参数有哪些特点？

（1）传递引用给函数与传递指针的效果是一样的。这时，被调函数的形参就成为原来主调函数中的实参变量或对象的一个别名来使用，所以在被调函数中对形参变量的操作就是对相应的目标对象（在主调函数）的操作。

（2）使用引用传递函数的参数，在内存中并没有产生实参的副本，它是直接对实参操作；而使用一般变量传递函数的参数，当发生函数调用时，需要给形参分配存储单元，形参变量是实参变量的副本；如果传递的是对象，还将调用拷贝构造函数。因此，当参数传递的数据较大时，用引用比用一般变量传递参数的效率和所占空间都好。

（3）使用指针作为函数的参数虽然也能达到与使用引用的效果，但是，在被调函数中同样要给形参分配存储单元，且需要重复使用"*指针变量名"的形式进行运算，这很容易产生错误且程序的阅读性较差；另一方面，在主调函数的调用点处，必须用变量的地址作为实参。而引用更容易使用，更清晰。

 

9.在什么时候需要使用“常引用”？

如果既要利用引用提高程序的效率，又要保护传递给函数的数据不在函数中被改变，就应使用常引用。常引用声明方式：const 类型标识符 &引用名=目标变量名；

例1

1 int a ;
2 const int &ra=a;
3 ra=1; //错误
4 a=1; //正确

例2

1 string foo( ); 2 void bar(string & s); 

那么下面的表达式将是非法的：

1 bar(foo( )); 2 bar("hello world"); 

原因在于foo( )和"hello world"串都会产生一个临时对象，而在C++中，这些临时对象都是const类型的。因此上面的表达式就是试图将一个const类型的对象转换为非const类型，这是非法的。引用型参数应该在能被定义为const的情况下，尽量定义为const 。

 

10.将“引用”作为函数返回值类型的格式、好处和需要遵守的规则?

格式：类型标识符 &函数名（形参列表及类型说明）{ //函数体 }

好处：在内存中不产生被返回值的副本；（注意：正是因为这点原因，所以返回一个局部变量的引用是不可取的。因为随着该局部变量生存期的结束，相应的引用也会失效，产生runtime error! 

注意事项：

（1）不能返回局部变量的引用。这条可以参照Effective C++[1]的Item 31。主要原因是局部变量会在函数返回后被销毁，因此被返回的引用就成为了"无所指"的引用，程序会进入未知状态。

（2）不能返回函数内部new分配的内存的引用。这条可以参照Effective C++[1]的Item 31。虽然不存在局部变量的被动销毁问题，可对于这种情况（返回函数内部new分配内存的引用），又面临其它尴尬局面。例如，被函数返回的引用只是作为一个临时变量出现，而没有被赋予一个实际的变量，那么这个引用所指向的空间（由new分配）就无法释放，造成memory leak。

（3）可以返回类成员的引用，但最好是const。这条原则可以参照Effective C++[1]的Item 30。主要原因是当对象的属性是与某种业务规则（business rule）相关联的时候，其赋值常常与某些其它属性或者对象的状态有关，因此有必要将赋值操作封装在一个业务规则当中。如果其它对象可以获得该属性的非常量引用（或指针），那么对该属性的单纯赋值就会破坏业务规则的完整性。

（4）流操作符重载返回值申明为“引用”的作用：

流操作符<<和>>，这两个操作符常常希望被连续使用，例如：cout << "hello" << endl;　因此这两个操作符的返回值应该是一个仍然支持这两个操作符的流引用。可选的其它方案包括：返回一个流对象和返回一个流对象指针。但是对于返回一个流对象，程序必须重新（拷贝）构造一个新的流对象，也就是说，连续的两个<<操作符实际上是针对不同对象的！这无法让人接受。对于返回一个流指针则不能连续使用<<操作符。因此，返回一个流对象引用是惟一选择。这个唯一选择很关键，它说明了引用的重要性以及无可替代性，也许这就是C++语言中引入引用这个概念的原因吧。 

赋值操作符=。这个操作符象流操作符一样，是可以连续使用的，例如：x = j = 10;或者(x=10)=100;赋值操作符的返回值必须是一个左值，以便可以被继续赋值。因此引用成了这个操作符的惟一返回值选择。
复制代码

 1 #include<iostream>
 2 using namespace std;
 3 int &put(int n);
 4 int vals[10];
 5 int error=-1;
 6 int main()
 7 {
 8     put(0)=10; //以put(0)函数值作为左值，等价于vals[0]=10;
 9     put(9)=20; //以put(9)函数值作为左值，等价于vals[9]=20;
10     cout<<vals[0];
11     cout<<vals[9];
12     return 0;
13 }
14 int &put(int n)
15 {
16     if (n>=0 && n<=9 ) return vals[n];
17     else
18     {
19         cout<<"subscript error";
20         return error;
21     }
22 }

复制代码

（5）在另外的一些操作符中，却千万不能返回引用：+-*/ 四则运算符。它们不能返回引用，Effective C++[1]的Item23详细的讨论了这个问题。主要原因是这四个操作符没有side effect，因此，它们必须构造一个对象作为返回值，可选的方案包括：返回一个对象、返回一个局部变量的引用，返回一个new分配的对象的引用、返回一个静态对象引用。根据前面提到的引用作为返回值的三个规则，第2、3两个方案都被否决了。静态对象的引用又因为((a+b) == (c+d))会永远为true而导致错误。所以可选的只剩下返回一个对象了。

 

11.结构与联合有何区别？

(1). 结构和联合都是由多个不同的数据类型成员组成, 但在任何同一时刻, 联合中只存放了一个被选中的成员（所有成员共用一块地址空间）, 而结构的所有成员都存在（不同成员的存放地址不同）。 

(2). 对于联合的不同成员赋值, 将会对其它成员重写, 原来成员的值就不存在了, 而对于结构的不同成员赋值是互不影响的。

 

12.试写出程序结果：
复制代码

 1 int  a=4;
 2 int  &f(int  x)
 3 {
 4     a = a + x;
 5     return  a;
 6 }
 7 int main()
 8 {
 9     int t = 5;
10     cout<<f(t)<<endl;  //a = 9
11     f(t) = 20;           //a = 20
12     cout<<f(t)<<endl;  //t = 5,a = 25
13     t = f(t);            //a = 30 t = 30
14     cout<<f(t)<<endl;  //t = 60
15     return 0;
16 }

复制代码
13.重载（overload)和重写(overried，有的书也叫做“覆盖”）的区别？

从定义上来说：

重载：是指允许存在多个同名函数，而这些函数的参数表不同（或许参数个数不同，或许参数类型不同，或许两者都不同）。

重写：是指子类重新定义父类虚函数的方法。

从实现原理上来说：

重载：编译器根据函数不同的参数表，对同名函数的名称做修饰，然后这些同名函数就成了不同的函数（至少对于编译器来说是这样的）。如，有两个同名函数：function func(p:integer):integer;和function func(p:string):integer;。那么编译器做过修饰后的函数名称可能是这样的：int_func、str_func。对于这两个函数的调用，在编译器间就已经确定了，是静态的。也就是说，它们的地址在编译期就绑定了（早绑定）。

重写：当子类重新定义了父类的虚函数后，父类指针根据赋给它的不同的子类指针，动态的调用属于子类的该函数，这样的函数调用在编译期间是无法确定的（调用的子类的虚函数的地址无法给出）。因此，这样的函数地址是在运行期绑定的（晚绑定）。

 

14.有哪几种情况只能用intialization list 而不能用assignment？

当类中含有const、reference 成员变量；基类的构造函数都需要初始化表。

 
15. C++是不是类型安全的？

不是。两个不同类型的指针之间可以强制转换（用reinterpret cast)。C#是类型安全的。

 

16.main函数执行之前，还会执行什么代码？

全局对象的构造函数会在main函数之前执行。

 

17.描述内存分配方式以及它们的区别？

1）从静态存储区域分配。内存在程序编译的时候就已经分配好，这块内存在程序的整个运行期间都存在。例如全局变量，static 变量。

2）在栈上创建。在执行函数时，函数内局部变量的存储单元都可以在栈上创建，函数执行结束时这些存储单元自动被释放。栈内存分配运算内置于处理器的指令集。

3）从堆上分配，亦称动态内存分配。程序在运行的时候用malloc 或new 申请任意多少的内存，程序员自己负责在何时用free 或delete 释放内存。动态内存的生存期由程序员决定，使用非常灵活，但问题也最多。

 

18.分别写出bool,int,float,指针类型的变量a 与“零”的比较语句。

1 bool : if(!a) or if(a)
2 int : if(a == 0)
3 float : const EXPRESSION EXP = 0.000001
4 if (a < EXP && a >-EXP)
5 pointer : if(a != NULL) or if(a == NULL)

 

19.请说出const与#define相比，有何优点？

const作用：定义常量、修饰函数参数、修饰函数返回值三个作用。被const修饰的东西都受到强制保护，可以预防意外的变动，能提高程序的健壮性。

1）const 常量有数据类型，而宏常量没有数据类型。编译器可以对前者进行类型安全检查。而对后者只进行字符替换，没有类型安全检查，并且在字符替换可能会产生意料不到的错误。

2）有些集成化的调试工具可以对const 常量进行调试，但是不能对宏常量进行调试。

 

20.简述数组与指针的区别？

数组要么在静态存储区被创建（如全局数组），要么在栈上被创建。指针可以随时指向任意类型的内存块。

（1）修改内容上的差别

1 char a[] = "hello";
2 a[0] = 'X';
3 char *p = "world"; // 注意p 指向常量字符串
4 p[0] = 'X'; // 编译器不能发现该错误，运行时错误

(2) 用运算符sizeof 可以计算出数组的容量（字节数）。sizeof(p),p 为指针得到的是一个指针变量的字节数，而不是p 所指的内存容量。C++/C 语言没有办法知道指针所指的内存容量，除非在申请内存时记住它。注意当数组作为函数的参数进行传递时，该数组自动退化为同类型的指针。
复制代码

 1 char a[] = "hello world";
 2 char *p = a;
 3 
 4 //计算数组和指针的内存容量
 5 cout<< sizeof(a) << endl; // 12 字节
 6 cout<< sizeof(p) << endl; // 4 字节
 7 
 8 //数组作为函数参数传递
 9 void Func(char a[100])
10 {
11     cout<< sizeof(a) << endl; // 4 字节而不是100 字节
12 }

复制代码

 

21.引用与指针有什么区别？

1) 引用必须被初始化，指针不必。

2) 引用初始化以后不能被改变，指针可以改变所指的对象。

3) 不存在指向空值的引用，但是存在指向空值的指针。

 

22.基类的析构函数不是虚函数，会带来什么问题？

派生类的析构函数用不上，会造成资源的泄漏。

 

23.全局变量和局部变量有什么区别？是怎么实现的？操作系统和编译器是怎么知道的？

生命周期不同：

全局变量随主程序创建和创建，随主程序销毁而销毁；局部变量在局部函数内部，甚至局部循环体等内部存在，退出就不存在；

使用方式不同：

通过声明后全局变量程序的各个部分都可以用到；局部变量只能在局部使用；分配在栈区。

内存分配位置不同：

全局变量分配在全局数据段并且在程序开始运行的时候被加载。局部变量则分配在堆栈里面 。

 

24.写出完整版的strcpy函数：

如果编写一个标准strcpy函数的总分值为10，下面给出几个不同得分的答案：   

2分 
1
2
3
4
	
void strcpy( char *strDest, char *strSrc )
{
    while( (*strDest++ = * strSrc++) != '\0' );
}

4分
1
2
3
4
5
	
void strcpy( char *strDest, const char *strSrc )
//将源字符串加const，表明其为输入参数，加2分
{
    while( (*strDest++ = * strSrc++) != '\0' );
}

7分
1
2
3
4
5
6
	
void strcpy(char *strDest, const char *strSrc)
{
    //对源地址和目的地址加非0断言，加3分
    assert( (strDest != NULL) && (strSrc != NULL) );
    while( (*strDest++ = * strSrc++) != '\0' );
}

10分
1
2
3
4
5
6
7
8
	
//为了实现链式操作，将目的地址返回，加3分！ 
char * strcpy( char *strDest, const char *strSrc )
{
    assert( (strDest != NULL) && (strSrc != NULL) );
    char *address = strDest;
    while( (*strDest++ = * strSrc++) != '\0' );
    return address;
}

 

25.为什么标准头文件都有类似以下的结构？
1
2
3
4
5
6
7
8
9
10
	
#ifndef __INCvxWorksh
#define __INCvxWorksh
#ifdef __cplusplus
extern "C" {
#endif
/*...*/
#ifdef __cplusplus
}
#endif
#endif /* __INCvxWorksh */

头文件中的编译宏   
1
2
3
	
#ifndef　__INCvxWorksh
#define　__INCvxWorksh
#endif

的作用是防止被重复引用。

作为一种面向对象的语言，C++支持函数重载，而过程式语言C则不支持。函数被C++编译后在symbol库中的名字与C语言的不同。例如，假设某个函数的原型为：    
1
	
void foo(int x, int y);

该函数被C编译器编译后在symbol库中的名字为_foo，而C++编译器则会产生像_foo_int_int之类的名字。_foo_int_int这样的名字包含了函数名和函数参数数量及类型信息，C++就是考这种机制来实现函数重载的。   

为了实现C和C++的混合编程，C++提供了C连接交换指定符号extern "C"来解决名字匹配问题，函数声明前加上extern "C"后，则编译器就会按照C语言的方式将该函数编译为_foo，这样C语言中就可以调用C++的函数了。

 

26.编写类String的构造函数、析构函数和赋值函数，已知类String的原型为：
1
2
3
4
5
6
7
8
9
10
	
class String
{
public:
    String(const char *str = NULL); // 普通构造函数
    String(const String &other); // 拷贝构造函数
    ~ String(void); // 析构函数
    String & operator =(const String &other); // 赋值函数
private:
    char *m_data; // 用于保存字符串
};
复制代码

 1 //普通构造函数
 2 String::String(const char *str)
 3 {
 4     if(str==NULL)
 5     {
 6         m_data = new char[1];   // 得分点：对空字符串自动申请存放结束标志'\0'的空
 7         *m_data = '\0';         //加分点：对m_data加NULL 判断
 8     }
 9     else
10     {
11         int length = strlen(str);
12         m_data = new char[length+1];    // 若能加 NULL 判断则更好
13         strcpy(m_data, str);
14     }
15 }
16 // String的析构函数
17 String::~String(void)
18 {
19     delete [] m_data;
20 }
21 //拷贝构造函数
22 String::String(const String &other) // 得分点：输入参数为const型
23 {
24     int length = strlen(other.m_data);
25     m_data = new char[length+1];    //加分点：对m_data加NULL 判断
26     strcpy(m_data, other.m_data);
27 }
28 //赋值函数
29 String & String::operator =(const String &other) // 得分点：输入参数为const型
30 {
31     if(this == &other)  //得分点：检查自赋值
32         return *this;
33     delete [] m_data;   //得分点：释放原有的内存资源
34     int length = strlen( other.m_data );
35     m_data = new char[length+1];    //加分点：对m_data加NULL 判断
36     strcpy( m_data, other.m_data );
37     return *this;   //得分点：返回本对象的引用
38 }

复制代码

 

27.请说出static和const关键字尽可能多的作用？

static关键字至少有下列5个作用：   
（1）函数体内static变量的作用范围为该函数体，不同于auto变量，该变量的内存只被分配一次，因此其值在下次调用时仍维持上次的值；   
（2）在模块内的static全局变量可以被模块内所用函数访问，但不能被模块外其它函数访问；   
（3）在模块内的static函数只可被这一模块内的其它函数调用，这个函数的使用范围被限制在声明它的模块内；   
（4）在类中的static成员变量属于整个类所拥有，对类的所有对象只有一份拷贝；   
（5）在类中的static成员函数属于整个类所拥有，这个函数不接收this指针，因而只能访问类的static成员变量。    

const关键字至少有下列5个作用：   
（1）欲阻止一个变量被改变，可以使用const关键字。在定义该const变量时，通常需要对它进行初始化，因为以后就没有机会再去改变它了；   
（2）对指针来说，可以指定指针本身为const，也可以指定指针所指的数据为const，或二者同时指定为const；   
（3）在一个函数声明中，const可以修饰形参，表明它是一个输入参数，在函数内部不能改变其值；   
（4）对于类的成员函数，若指定其为const类型，则表明其是一个常函数，不能修改类的 成员变量；   
（5）对于类的成员函数，有时候必须指定其返回值为const类型，以使得其返回值不为“左值”。例如：   
1
2
3
4
5
	
const classA operator*(const classA& a1,const classA& a2);  
operator*的返回结果必须是一个const对象。如果不是，这样的变态代码也不会编译出错：  
classA a, b, c;  
(a * b) = c; // 对a*b的结果赋值  
操作(a * b) = c显然不符合编程者的初衷，也没有任何意义。  

 

28.请写一个C函数，若处理器是Big_endian的，则返回0；若是Little_endian的，则返回1。
1
2
3
4
5
6
7
8
9
10
11
12
	
int checkCPU()
{
    {
        union w
        {
            int a;
            char b;
        } c;
        c.a = 1;
        return (c.b == 1);
    }
}

 

29.写一个函数返回1+2+3+…+n的值（假定结果不会超过长整型变量的范围）
1
2
3
4
	
int Sum( int n )
{
    return ( (long)1 + n) * n / 2;  //或return (1l + n) * n / 2;
}

对于这个题，只能说，也许最简单的答案就是最好的答案。
```

# 1. linux下一个write()函数的具体过程 

# 2. 介绍下page cache和buffer cache ，它们有什么区别 
```
1、page cahe和buffer cache

Page cache实际上是针对文件系统的，是文件的缓存，在文件层面上的数据会缓存到page cache。文件的逻辑层需要映射到实际的物理磁盘，这种映射关系由文件系统来完成。当page cache的数据需要刷新时，page cache中的数据交给buffer cache，但是这种处理在2.6版本的内核之后就变的很简单了，没有真正意义上的cache操作。 

Buffer cache是针对磁盘块的缓存，也就是在没有文件系统的情况下，直接对磁盘进行操作的数据会缓存到buffer cache中，例如，文件系统的元数据都会缓存到buffer cache中。 
简单说来，page cache用来缓存文件数据，buffer cache用来缓存磁盘数据。在有文件系统的情况下，对文件操作，那么数据会缓存到page cache，如果直接采用dd等工具对磁盘进行读写，那么数据会缓存到buffer cache。 

补充一点，在文件系统层每个设备都会分配一个def_blk_ops的文件操作方法，这是设备的操作方法，在每个设备的inode下面会存在一个radix tree，这个radix tree下面将会放置缓存数据的page页。这个page的数量将会在top程序的buffer一栏中显示。如果设备做了文件系统，那么会生成一个inode，这个inode会分配ext3_ops之类的操作方法，这些方法是文件系统的方法，在这个inode下面同样存在一个radix tree，这里会缓存文件的page页，缓存页的数量在top程序的cache一栏进行统计。从上面的分析可以看出，2.6内核中的buffer cache和page cache在处理上是保持一致的，但是存在概念上的差别，page cache针对文件的cache，buffer是针对磁盘块数据的cache，仅此而已。 
2、cache 和 buffer的区别

A buffer is something that has yet to be “written” to disk. A cache is something that has been “read” from the disk and stored for later use ; 对于共享内存（Shared memory），主要用于在UNIX 环境下不同进程之间共享数据，是进程间通信的一种方法，一般的应用程序不会申请使用共享内存

Cache：高速缓存，是位于CPU与主内存间的一种容量较小但速度很高的存储器。由于CPU的速度远高于主内存，CPU直接从内存中存取数据要等待一定时间周期，Cache中保存着CPU刚用过或循环使用的一部分数据，当CPU再次使用该部分数据时可从Cache中直接调用，这样就减少了CPU的等待时间，提高了系统的效率。Cache又分为一级Cache（L1 Cache）和二级Cache（L2 Cache），L1 Cache集成在CPU内部，L2 Cache早期一般是焊在主板上，现在也都集成在CPU内部，常见的容量有256KB或512KB L2 Cache

它是根据程序的局部性原理而设计的，就是cpu执行的指令和访问的数据往往在集中的某一块，所以把这块内容放入cache后，cpu就不用在访问内存了，这就提高了访问速度。当然若cache中没有cpu所需要的内容，还是要访问内存的

查看CPU的 L1、L2、L3
1
2
3
4
5
6
	
[root@AY1301180424258d59678 ~]# ll /sys/devices/system/cpu/cpu0/cache/
total 0
drwxr-xr-x 2 root root 0 Jan 26 22:49 index0 #一级cache中的data和instruction cache
drwxr-xr-x 2 root root 0 Jan 26 22:49 index1 #一级cache中的data和instruction cache
drwxr-xr-x 2 root root 0 Jan 26 22:49 index2 #二级cache，共享的
drwxr-xr-x 2 root root 0 Jan 26 22:49 index3 #三级cache，共享的 

Buffer：缓冲区，一个用于存储速度不同步的设备或优先级不同的设备之间传输数据的区域。通过缓冲区，可以使进程之间的相互等待变少，从而使从速度慢的设备读入数据时，速度快的设备的操作进程不发生间断。 
3、Free中的buffer和cache （它们都是占用内存）基于内存的

buffer ：作为buffer cache的内存，是块设备的读写缓冲区 

cache：作为page cache的内存， 文件系统的cache 

如果 cache 的值很大，说明cache住的文件数很多。如果频繁访问到的文件都能被cache住，那么磁盘的读IO 必会非常小

如何释放Cache Memory
1
2
3
4
5
6
7
8
	
To free pagecache:
echo 1 > /proc/sys/vm/drop_caches
To free dentries and inodes:
echo 2 > /proc/sys/vm/drop_caches
To free pagecache, dentries and inodes:
echo 3 > /proc/sys/vm/drop_caches
 
#注意，释放前最好sync一下，防止丢失数据，但是一般情况下没有必要手动释放内存
4、总结
cached是cpu与内存间的，buffer是内存与磁盘间的，都是为了解决速度不对等的问题

    缓存（cached）是把读取过的数据保存起来，重新读取时若命中（找到需要的数据）就不要去读硬盘了，若没有命中就读硬盘。其中的数据会根据读取频率进行组织，把最频繁读取的内容放在最容易找到的位置，把不再读的内容不断往后排，直至从中删除

    缓冲（buffers）是根据磁盘的读写设计的，把分散的写操作集中进行，减少磁盘碎片和硬盘的反复寻道，从而提高系统性能。linux有一个守护进程定期 清空缓冲内容（即写入磁盘），也可以通过sync命令手动清空缓冲。举个例子吧：我这里有一个ext2的U盘，我往里面cp一个3M的MP3，但U盘的灯 没有跳动，过了一会儿（或者手动输入sync）U盘的灯就跳动起来了。卸载设备时会清空缓冲，所以有些时候卸载一个设备时要等上几秒钟

    修改/etc/sysctl.conf中的vm.swappiness右边的数字可以在下次开机时调节swap使用策略。该数字范围是0～100，数字越大越倾向于使用swap。默认为60，可以改一下试试。–两者都是RAM中的数据

buffer是即将要被写入磁盘的，而cache是被从磁盘中读出来的
 

    buffer是由各种进程分配的，被用在如输入队列等方面。一个简单的例子如某个进程要求有多个字段读入，在所有字段被读入完整之前，进程把先前读入的字段放在buffer中保存

    cache经常被用在磁盘的I/O请求上，如果有多个进程都要访问某个文件，于是该文件便被做成cache以方便下次被访问，这样可提高系统性能

    Buffer Cachebuffer cache，又称bcache，其中文名称为缓冲器高速缓冲存储器，简称缓冲器高缓。另外，buffer cache按照其工作原理，又被称为块高缓

    作者：踏雪无痕
    出处：http://www.cnblogs.com/chenpingzhao/

最近碰到个问题，弄不清楚Linux中buffer cache和page cache的概念，然后在Quora找到了Linux Kernel Development的作者Robert Love的两个answer，顿时茅厕顿开，决定翻译一下，记录一下。有些感觉翻译不到位的在/**/中给出原文。

第一个问题，Linux Kernel: What is the major difference between the buffer cache and the page cache?
buffer cache与page cache的主要区别是什么？

为什么在早期的内核中他们是分开的？为什么后来又合并了？

Robert Love：

page cache缓存了文件页/*pages of file*/，这是为了优化文件I/O。而buffer cache缓存了磁盘块，则是为了优化块I/O。

在Linux内核2.4版本之前，这两个cache是不一样的：文件在page cache，而磁盘块在buffer cache。考虑到大部分文件都是由文件系统表示，并存储在磁盘上，文件数据将会被缓存两份，每一个cache中一份。许多的Unix系统也是采用同样的方式。

这样的方式实现起来简单，但是很明显，不优雅也不高效。从Linux内核2.4版本开始，这两个cache的内容被统一了。VM子系统现在可以驱动I/O，不仅是在page cache/*it does so out of the page cache*/。 如果被缓存的数据即是文件数据又是块数据—大部分数据都是这样—buffer cache只需要指向page cache即可；这样数据就只需要在内存中缓存一份。当你在考虑磁盘缓存的时候，其实就是page cache了：它缓存了磁盘文件数据，这样可以让后续的I/O操作更加快速。

buffer cache仍然存在，因为内核仍然需要执行块I/O。因为大多数块都是用来存储文件数据，所以大部分buffer cache都指向了page cache。但还是有一小部分块并不是文件数据，例如metadata和raw block I/O，因此还是需要单独用buffer cache来缓存。

Jeff Darcy：

很高兴Robert Love给出了一个很优秀的答案。我不打算再用相同的视角来回答这个问题，我将试着回到更早之前，那时甚至都还没有Linux。我知道那看起来是一个奇怪的问题，但Linux开发者也会跟人谈起，因此历史上的事件，围绕其他系统的情况，我觉得还是有必要提一下的/*but Linux developers do talk to others and so on matters of history the context around other systems is IMO still relevant*/。

一开始只有buffer cache。它是一个非常简单的概念：操作它就像操作磁盘，只是最后不会有真正的磁盘请求或者传输。这是一个非常强大的想法，几乎每一个点/*workload*/（除了CPU密集型的，当时还不会碰到网络密集型的问题）上面都能极大的提升性能。谁还会要求更多呢？

后来，有些人觉得将这个cache移到更高的层次会更好。当你只需要维护一个简单的文件加位移的哈希表时，为什么还要在复杂的文件系统代码上面浪费时间呢？而且在文件系统上面实现一个cache意味着可以更紧密的和内存管理子系统结合起来，内存管理子系统的开发在当时也是比较活跃的。因此，你的第一个问题的答案，他们之所以是分开的是因为他们是在不同时期为了满足不同需要而开发的，并且一开始也没有人想要把buffer cache搞乱。

再后来，大家意识到可以将两个cache统一以避免Robert提到的缓存两份的问题。我希望可以找到一个文档的链接让你看下Solaris7和8是怎样来处理这个问题的，因为他们增加的复杂度实在是有点可笑。让buffer cache指向page cache，而不是反过来，原因有两方面：

    并不是所有文件系统都是挂载在磁盘上面。有一些是在内存，有一些则是通过网络操作。将page cache拆解成buffer cache和本地处理的并不那么优雅/*Having the page cache split between stuff that points into the buffer cache and stuff that's handled natively is a bit inelegant*/。
    page cache被寄予厚望，它的寻址方法和其他语义如果成为首选会更加有意义。

buffer cache仍然存在，因为不是只有文件，但它也只是过去辉煌的一个阴影了。如今一个系统没有了buffer cache只是会变慢，这与没有了page cache会造成的严重后果是无法比拟的。


第二个问题，What is the difference between Buffers and Cached columns in /proc/meminfo output?
/proc/meminfo输出的Buffers与Cached这两列有什么区别？

cat /proc/meminfo
MemTotal:      8162388 kB
MemFree:         86004 kB
Buffers:         56432 kB
Cached:        1141924 kB
SwapCached:     800992 kB
Active:        6090024 kB
Inactive:      1857208 kB
HighTotal:           0 kB
HighFree:            0 kB
LowTotal:      8162388 kB
LowFree:         86004 kB
SwapTotal:     2096472 kB
SwapFree:      1048264 kB
...

Robert Love：

长话短说，Cached是page cache的大小，Buffers是内存中/*in-memory*/块I/O缓冲区的大小。Cached很重要，Buffers无关紧要。

短话长说，Cached等于Linux page cache的大小减去swap cache的大小，swap cache的大小是SwapCached那一列（因此全部page cache的大小就等于Cached+SwapCached）。Linux通过page cache执行所有的I/O操作。写的实现很简单，只要将page cache中相应的页标记为脏页即可；负责刷盘的线程会周期性的将脏页写回磁盘。读就是直接读取page cache的数据，如果数据还没被缓存，就先读进来。在现代的Linux系统中，Cached很容易就会达到几个G，当内存有压力时它才会缩小。只要需要系统就会清理page cache并将数据swap到磁盘以获取更多可用的内存。

Buffers是内存中块I/O的缓冲区。相对来说，它们是比较短暂的。在Linux内核2.4版本之前，page cache跟buffer cache是分开的。从2.4开始，page cache跟buffer cache统一了。Buffers就只缓存raw disk block了，这一部分不在page cache—也就是非文件数据。Buffers这个指标也就不那么重要了。大部分系统中，Buffers经常也就几十M。

```

# 单链表翻转，如果只用指针的话怎么翻转 
```
Node* reverse(Node* root) {
  Node* new_root = 0;
  while (root) {
    Node* next = root->next;
    root->next = new_root;
    new_root = root;
    root = next;
  }
  return new_root;
}
```

#  编译器对extern修饰的变量会如何处理
```
extern "C"指令非常有用，因为C和C++的近亲关系。注意：extern "C"指令中的C，表示的一种编译和连接规约，而不是一种语言。C表示符合C语言的编译和连接规约的任何语言，如Fortran、assembler等。

还有要说明的是，extern "C"指令仅指定编译和连接规约，但不影响语义。例如在函数声明中，指定了extern "C"，仍然要遵守C++的类型检测、参数转换规则。

再看下面的一个例子，为了声明一个变量而不是定义一个变量，你必须在声明时指定extern关键字，但是当你又加上了"C"，它不会改变语义，但是会改变它的编译和连接方式。

如果你有很多语言要加上extern "C"，你可以将它们放到extern "C"{ }中。

函数声明时与定义时要么都倾轧，要么都不倾轧，必须得一一对应，否则会报函数找不到的错误。倾轧是编译时进行的，而对于C++要兼容C的问题，C++就必须兼容C的语法与C库（链接库），C库只在连链接时加入，不存在让C++编译器去倾轧的问题；而C头文件中声明的C库函数在调用时会倾轧，要想使用不参加倾轧的C库函数，C++中编译时就不能倾轧C的头文件中对于库函数的声明，即C库中已经不能修改不倾轧为倾轧，则必须将头文件中的声明也设置为不倾轧，以此与库中相互对应。如果查看C的标准头文件，可以发现其中有一个extern “C”，表示不倾轧C的函数声明。如下是string.h头文件中的一部分：

```

# 函数重载时编译器是如何工作的
```
其实C++函数重载底层实现原理是C++利用name mangling(倾轧)技术，来改名函数名，区分参数不同的同名函数。
C++中，这三个函数如果在主函数中被调用选择哪一个，由编译器自身决定。那么不同编译器底下函数的命名风格又是怎样的呢？请接着往下看
总的来说：源文件通过编译后，将相同函数名，按照一定的格式，改变成可以区分的，去除了函数在调用时的二义性，从而实现函数的重载。
```
