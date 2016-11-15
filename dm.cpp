#include <iostream>
using namespace std;

#define MAXSIZE 100
typedef int Status;

#define OK  1
#define ERROR 0
#define TRUE  1
#define FALSE 0

template <typename T>
class SqList
{
public:
	T r[MAXSIZE+1];
	int length;
public:
	SqList(T * var,int len);
	SqList();
	void printList();
	void BubbleSort();
	Status swapElement(int i, int j);
	void SelectSort();
	void InsertSort();
	void ShellSort();
	
	void HeapAdjust(int s,int m);
	void HeapSort();

	void MergeSort();
	void MSort(T src[],T dst[],int s,int t);
	void Merge(T src[],T dst[],int i,int m,int n);
	void MergeSort_2();
	void MergePass(T src[], T dst[], int s, int n);

	void QuickSort();
	int Partition(int low,int high);
	void QSOrt(int low,int high);
};

template <typename T>
SqList<T>::SqList(T * var,int len)
{
	length = len/sizeof(T);
	memcpy(&(r[1]), var, len);
}

template <typename T>
SqList<T>::SqList()
{
	memset(this, 0, sizeof(SqList));
}

template <typename T>
Status SqList<T>::swapElement(int i,int j)
{
	T tmp =this->r[i];
	this->r[i] = this->r[j];
	this->r[j] = tmp;
	return OK;
}

template <typename T>
void SqList<T>::printList()
{
	for (int i = 1; i <= length; i++)
	{
		cout << this->r[i] << "\t";
	}
	cout << endl;
}
/************************************************************************/
/* 升序,将小元素排到前面                                                                     */
/************************************************************************/

/*相邻元素两两比较，如果存在反序则交换位置
**一共需要n-1趟循环，外层循环是【1...n-1】
**每趟循环能够把最小的元素冒泡到第一个元素
**第i趟循环需要比较n-i次，内层循环是【n-1...i】*/
template <typename T>
void SqList<T>::BubbleSort()
{
	int i=0, j=0;
	Status flag = TRUE;//用flag辅助循环结束的条件，优化冒泡排序算法。
	for (i = 1; i < this->length && flag;i++)//跑多少趟
	{
		flag = FALSE;//每一趟进来默认表示不用进行下一趟比较了（进行该轮比较以后即可排序完毕）
		for (j = this->length - 1; j >= i; j--)//每一趟比较无序序列的两两相邻的元素
		{
			if (this->r[j + 1] < this->r[j])//后一个元素比较小的话，交换位置
			{
				swapElement(j, j + 1);//小元素往上浮动
				flag = TRUE;//发生数据交换（表示没有排好）以后还需要进行下一趟排序
			}
		}
	}
}


/************************************************************************/
/* 固定一个位置的元素，将该元素与剩余的元素逐个比较，
**在反序时交换位置，永远保持该位置是最小元素                                                                     */
/************************************************************************/
template <typename T>
void SqList<T>::SelectSort()
{
	int i, j, min;

	for (i = 1; i < length;i++)//第几趟比较
	{
		min = i;//将当前下标定义为最小元素的下标，每一趟都会依次更新，从1开始到n-1
		for (j = i + 1; j <= length;j++)//每一趟循环找出所剩序列中最小元素的下标
		{
			if (r[min] > r[j])
				min = j;
		}
		if (min != i)//如果找到的最小元素的下标和预先默认的不一致，
					//说明无序序列最开始的元素并非最小元素，需要进行交换
			swapElement(i, min);
	}
}


/************************************************************************/
/* 直接插入排序                                                                     */
/************************************************************************/
template <typename T>
void SqList<T>::InsertSort()
{
	int i = 0, j = 0;
	for (i = 2; i <= length; i++)//外层循环表示从无序表中依次将其元素插入到有序表中
								//默认第一个元素是有序表，所以从第二个元素开始直到第n个元素结束，
								//这些元素都是无序表中的元素，需要依次把它们插入到前面的有序表中
								//一次外层循环表示处理好无序表中的一个元素
	{
		if (r[i] < r[i - 1])//第i个元素是无序表中的第一个元素
							//第i-1个元素是有序表中的最后一个元素，也是有序表中最大的元素
							//如果无序表中的第一个元素（当前元素）比有序表中的最大元素小
							//表示需要将当前元素往前插，至于插到什么位置，由内层循环决定
		{
			r[0] = r[i];//利用哨兵将当前元素的值缓存下来
			for (j = i - 1; r[j] > r[0]; j--)//内层循环将比当前元素的值（哨兵的值）大的元素后移挪出空位
			{
				r[j + 1] = r[j];//移动比哨兵大的元素
			}

			r[j + 1] = r[0];//将哨兵插入空位，该空位以前的元素要么为空要么逗比他小，否则不会结束循环
		}
	}//遍历到最后一个元素的时候排序完毕
}


/************************************************************************/
/* 希尔排序                                                                     */
/************************************************************************/
template <typename T>
void SqList<T>::ShellSort()
{
	int i, j;
	int inc = length;

	do //do-while循环主要是用来调节增量
		//每调节一次增量代表越来越趋向基本有序
	{
		inc = inc / 3 + 1;//业界常用迭代公式--增量越来越小，
							//每一个分组的元素越来越多
		
		/*分组里面进行插入排序--默认分组的第一个元素已经排好序*/
		for (i = inc + 1; i <= length;i++)//i从第inc+1开始（第一个分组的第二个元素）
										//然后依次遍历后面的元素，他们相应是每一个分组的第二个元素
		{
			if (r[i]<r[i-inc])//如果分组的第二个元素比第一个元素小--
							//代表需要移动分组里面已经排好序的元素--
							//挪出位置以便给当前元素使用
			{
				r[0] = r[i];//哨兵--缓存当前元素
				for (j = i - inc; j > 0 && r[j] > r[0]; j -= inc)//分组里面移动元素--按照增量调节下标
				{
					r[j + inc] = r[j];//向后移动
				}
				r[j + inc] = r[0];//在相应位置插入哨兵--完成插入排序
			}
		}
	} while (inc > 1);//增量序列的最后一个增量必须等于1--等于1的时候跳跃式的插入排序--希尔排序完毕
}


/************************************************************************/
/* 堆排序                                                                     */
/************************************************************************/

//调整从第s个元素到第m个元素为大顶堆
//实质就是将根节点和左右结点比较，
//确保根节点在最后是当前二叉树中最大的元素
template <typename T>
void SqList<T>::HeapAdjust(int s, int m)
{
	T tmp;
	int j = 0;//j表示关键字较大的孩子的编号
	tmp = r[s];//s是一个编号，是元素在无序表中的下标
			//r[s]是要处理的二叉树或者其子树的根节点
			//tmp缓存最原始的根节点

	for (j = 2 * s; j <= m; j *= 2)//沿关键字较大的孩子节点向下筛选，默认为左孩子（二叉树性质5）
	{
		if (j < m && r[j] < r[j + 1])//左孩子小于右孩子
		{
			++j;//更新j指向右孩子
		}

		if (tmp >= r[j])//如果根节点大于等于左右孩子中最大的直接跳出循环，
						//完成该二叉树的调节，这样小范围内逐步调节，
						//在循环多次从不同的根节点开始调节以后，最终会将整个或者剩余的无序表构造成大顶堆
		{
			break;
		}

		r[s] = r[j];//如果根节点小于孩子节点，则将根节点换为最大的孩子节点
					//注意：此时原来的根节点被覆盖，没有发生交换，
					//也就是此时的二叉树中有一个孩子节点和根节点一样大

		s = j;//更新根节点为当前最大的孩子节点，也就是开始下一次循环处理新的二叉树
			//新的二叉树的根节点是当前最大的孩子节点，如此一直往下调节，
			//直到从tmp缓存的r[s]开始的子树变成一个大顶堆
	}
	r[s] = tmp;//如果之前根节点被覆盖了，
				//代表目前的二叉树中有一个孩子节点和根节点一样
				//将之前缓存的最初根节点插入该孩子节点的位置完成交换，
				//这样就不会被覆盖，不会有元素丢失
				
				//如果没有发生覆盖操作，说明原来一开始的根节点就是最大值
				//再将一开始的根节点赋值插入到自己原来的位置即可，
				//看上去显得多余--因为实际上就是自己给自己赋值，但是这是为了程序的通用性
				//为的就是解决发生覆盖的情况，只不过这样对于没发生覆盖的情况也适用
}


template <typename T>
void SqList<T>::HeapSort()
{
	int i = 0;
	/*1.将待排序序列构建成一个大顶堆*/
	for (i = length / 2; i > 0; i--)//n/2是倒数第一个非终端结点，
									//由它开始，按照层序遍历法反向遍历所有的非终端结点
									//直到完全二叉树的根节点
	{
		HeapAdjust(i, length);//调整从第i个元素到第length个元素为大顶堆
							//实质就是将根节点和左右结点比较，
							//确保根节点在最后是当前二叉树中最大的元素
	}

	/*上面只是单纯的根据非终端结点构造大顶堆
	之后便要将根节点和末尾结点交换
	再从根节点开始到无序表中倒数第一个节点开始构造新的大顶堆
	调整i使得他始终指向无序表中倒数第一个节点
	交换根节点和无序表中倒数第一个节点，再用剩余元素构造大顶堆
	反复执行，直到无序表中的倒数第一个节点是根节点结束循环*/
	for (i = length; i > 1; i--)
	{
		/*2.将最大值（根节点）和无序表中倒数第一个节点交换*/
		swapElement(1, i);

		/*3.将剩余的i-1个节点（从根节点开始到无序表中的最后一个节点）再构造大顶堆*/
		HeapAdjust(1, i - 1);
	}
}

/************************************************************************/
/*归并排序                                                                      */
/************************************************************************/

/*为了统一接口--内部调用实际的归并排序函数*/
template <typename T>
void SqList<T>::MergeSort()
{
	MSort(r, r, 1, length);//调用实际的归并排序
}


/*归并排序的递归算法
将src[s..t]归并排序为dst[s..t]*/
template <typename T>
void SqList<T>::MSort(T src[], T dst[], int s, int t)
{
	int m = 0;
	T tmp[MAXSIZE+1];//开头是一个哨兵--辅助变量存放各阶段归并的结果
	memset(tmp, 0, sizeof(tmp));

	if (s == t)//递归结束的条件--意味着两两归并的子序列中都只有一个元素
	{
		dst[s] = src[s];//由于只有一个元素--直接存放元素到目标表
						//表示两个有序子序列，只不过每个子序列只有一个元素
						//执行完这条语句表示递归结束，该返回了
						//紧接着执行Merge函数完成对这两个子序列的归并操作
	}
	else{
		m = (s + t) / 2;//将src[s..t]评分为src[s..m]和src[m+1..t]
		
		MSort(src, tmp, s, m);//递归将src[s..m]归并为tmp[s..m]
		MSort(src, tmp, m + 1, t);//递归将src[m+1..t]归并为tmp[m+1..t]

		Merge(tmp, dst, s, m, t);//将tmp[s..m]和tmp[m+1..t]归并到dst[s..t]
								//每次递归返回的时候都会执行这条语句，
								//里面完成了两个局部有序子序列的归并操作
								//这条语句执行完以后就会得到局部的有序序列以供外层递归返回时
	}

}


/*将有序的src[i..m]和src[m+1..n]归并为dst[i..n]
默认序号小的子序列比序号大的子序列要小--升序排列
*/
template <typename T>
void SqList<T>::Merge(T src[], T dst[], int i, int m, int n)
{
	int j, k, l;//j是目标序列的下标，从i到n
				//k是第二个有序子序列的下标，从m+1开始到n
				//第一个有序子序列的下标就用i来表示，从i到m
				//l是用来处理有序子序列剩下的元素的辅助变量
				//因为最后可能子序列的个别元素会在循环结束后还没有存放到目标表/序列中
	for (j = i, k = m + 1; i <= m && k <= n;j++)
	{
		if (src[i] < src[k])
			dst[j] = src[i++];
		else
			dst[j] = src[k++];
	}

	if (i <= m)//将剩余的src[i..m]复制到dst中
	{
		for (l = 0; l <= m - i;l++)//这里循环m-i+1次，因为剩下的元素下标从i开始，
									//直到m结束，一共就是m-i+1个
		{
			dst[j + l] = src[i + l];
		}
	}

	if (k <= n)//将剩余的src[k..n]复制到dst中
	{
		for (l = 0; l <= n - k; l++)//这里循环n-k+1次，因为剩下的元素下标从k开始，
								//直到n结束，一共就是m-i+1个
		{
			dst[j + l] = src[k + l];
		}
	}
}


/************************************************************************/
/*非递归的归并排序                                                                      */
/************************************************************************/
template <typename T>
void SqList<T>::MergeSort_2()
{
	T tmp[MAXSIZE];//辅助变量存放临时归并的结果
	memset(tmp, 0, sizeof(tmp));//初始化

	int k = 1;//有序子序列的长度--从1开始，逐渐翻倍


	/*每一次循环需要进行两次归并*/
	while (k < length)//只要有序子序列长度没有达到原始序列的长度表示排序未结束
	{
		MergePass(r, tmp, k, length);//第一步从原始序列归并到辅助序列
									//每一次归并结束以后，子序列长度都会翻倍
		k *= 2;//子序列长度翻倍

		MergePass(tmp, r, k, length);//第二部又从辅助序列归并到原始序列
									//保证没一次循环结束以后归并结果存放到原始序列
		k *= 2;//子序列长度翻倍
	}
}


/*将src中相邻长度为s的子序列两两归并到dst*/
template <typename T>
void SqList<T>::MergePass(T src[], T dst[], int s, int n)
{
	int i = 1;//要归并的子序列中元素的下标
	int  j = 0;//用来处理剩余单个子序列的辅助变量

	/*归并的范围是1到n
	但是i的范围是1到（n-2*s+1）
	因为（n-2*s+1）表示的是从最后一个元素往前数，
	最后两个子序列的起始下标，比如原始序列是1-9，每个子序列的长度s为2，
	则9-2*2+1 = 6表示最后两个子序列的起始下标，6-7和8-9表示最后两个子序列
	如果在循环过程中i没有超过这个下标，表示后面还有至少两个子序列
	还可以进行一轮循环，否则表示后面不足两个子序列，结束循环
	比如只要i不大于6表示后面至少还有4个元素，还可以组成至少两个子序列，
	在循环里可以对两个相邻的子序列进行处理*/
	while (i <= n-2*s+1)
	{
		Merge(src, dst, i, i + s - 1, i + 2 * s - 1);//两两子序列归并
													//第一个子序列从i到i+s-1
													//第二个子序列从i+s到i+2*s-1
		i += 2 * s;//根据增量更新下标，这里的增量是两个子序列的总长度
	}

	if (i < n - s + 1)//如果i小于最后一个子序列的起始下标，表示剩余两个子序列，
					//如i小于8,即使i=7，后面还有7-9三个元素，而每个子序列只有俩元素
					//所以还可以将三个元素组成两个子序列，只不过有可能有一个子序列的长度为s-1
		Merge(src, dst, i, i + s - 1, n);//将最后的两个子序列进行归并
	else{//最后只剩下单个子序列，有可能剩下的这个子系列刚好有s个元素，也可能不足s个元素，
		//但最多就s个元素，然后就逐个将这些元素复制到目标序列完成一次总的归并
		for (j = i; j <= n;j++)//从i到n逐个复制，此时n-i+1<= s，（n-i+1）表示剩下的元素个数
		{
			dst[j] = src[j];
 		}
	}
}


/************************************************************************/
/*快速排序                                                                      */
/************************************************************************/

/*保持和其他排序算法接口一致*/
template <typename T>
void SqList<T>::QuickSort()
{
	QSOrt(1,length);
}

#define MAXCNT 7

/*核心的快排算法*/
template <typename T>
void SqList<T>::QSOrt(int low, int high)
{
	int pivot_Position;//记录枢轴的位置
	if (high - low > MAXCNT)//避免杀鸡用牛刀，整个序列大于8（MAXCNt+1）个用快排
	{
		while (low < high)//减少递归的深度
		{
			pivot_Position = Partition(low, high);//获取枢轴的位置
			QSOrt(low, pivot_Position - 1);//递归处理独立的两部分
											//第一次循环的时候处理前半部分
											//第二次处理后半部分
			//QSOrt(pivot_Position + 1, high);
			low = pivot_Position + 1;//更新待处理序列的起始位置
		}
	}
	else{//小于8个元素用直接插入排序
		InsertSort();
	}
}

/*交换无序表中子表的元素，使得枢轴移动到该到的位置，并返回其位置
此时前半部分全部比他小
后半部分全部比他大*/
template <typename T>
int SqList<T>::Partition(int low, int high)
{

	T pivot_Var;//记录枢轴的值
	int m = (low + high) / 2;//找到中间元素的下标

	/*三个if语句实现取出左、中、右三个元素的中间值并存放到r[low]*/
	if (r[low] > r[high])//保证r[low]比r[high]小
		swapElement(low, high);

	if (r[m] > r[high])//保证r[m]比r[high]小,至此r[high]肯定是三个元素中最大的
		swapElement(m, high);

	if (r[m] > r[low])//如果r[m]比r[low]大，说明三个元素的中间值是r[m],进行交换操作
		swapElement(low, m);

	pivot_Var = r[low];//得到比较用的枢轴
	r[0] = pivot_Var;//缓存枢轴到哨兵

	while (low < high)
	{

		/*从high开始往前进行划分得到子序列*/
		while (r[high] >= pivot_Var && low < high )
		{
			high--;
		}
		//swapElement(low, high);
		r[low] = r[high];//避免频繁的交换操作


		/*从low开始向后进行划分得到前半部分的子序列*/
		while (r[low] <= pivot_Var && low < high)
		{
			low++;
		}
		//swapElement(low, high);
		r[high] = r[low];//避免交换操作
	}

	r[low] = r[0];//此时的low就是该趟排序中，枢轴应该位于的位置，恢复枢轴
	return low;//返回数轴的位置
}
int main(void)
{
	int myList[9] = {90,10,50,80,30,70,40,60,20};
	SqList<int> list(myList,sizeof(myList));
	
	cout << "before sort:"<< endl;
	list.printList();

	list.QuickSort();
	cout << "after sort:" << endl;
	list.printList();

	cout<<"Hello!"<<endl;
	system("pause");
	return 0;
}