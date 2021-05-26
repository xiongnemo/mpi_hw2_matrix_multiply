# mpi_hw2_matrix_multiply# mpi_hw2_matrix_multiply

## 要求

> 编写 MPI 并行程序，完成矩阵与向量的乘法运算：A∙x。其中，A为5×5的二维矩阵，x为5×1的列向量。
>
> 调用 C++ 的 `time()` 和 `rand()` 或 `srand()` 函数，随机生成矩阵A与向量x的元素（整型数）；
>
> 共启用 5 个进程，分别计算矩阵每一行与列向量的对应元素乘法，其中第一个进程（进程 0）还负责数据的分发和结果的汇总；
>
> 数据通信时使用MPI的组通信函数（`Bcast`、`Gather`、`Scatter`）；

## 先决条件

正确配置了 MPI

## 编译运行

```bash
make run
```
