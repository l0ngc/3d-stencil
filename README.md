# 3d-stencil
 3d stencil project for eda284

EDA284的实验项目

需要使用GEM5 simulator，同时使用ARM SVE的向量化指令（基本是Lab1的实验内容），来实现对现在已经有的serial code来进行向量化编写，来实现最佳性能。
我们具有以下任务：

1. Perform a study for cache sizes for your algorithm.
2. Transform a serial implementation of the problem you have selected using various types of optimizations and report the achieved results.
3. Do a design space exploration of future vector architectures by evaluating the trade-offs in performance, area, and energy requirements.

翻译下来，每件事儿就是
1. 研究一下各个感觉cache size的算法表现的影响。
2. 通过一系列算法优化，来观察能达到的性能提升。
3. 对一个向量结构的优化空间，来找到性能、面积、能源要求的取舍。

同事，我们需要使用roofline模型，来对模型进行衡量。
我们需要使用Roofline模型，来评估该算法在目标机型上的最高性能。

计算Roofline，需要知道以下三个要素：
1. simulator machine的各个内存的内存带宽，从而获得roofline前边的斜线，也就是在memory bound的情况下，我能达到的算力。
2. simulator machine的最高计算性能，从而获得roofline后边的屋顶，也就是compute bound的情况下，最高算力（如果使用向量化指令，可能会提高）
3. 算法AI，计算出来一共设计了多少个operation以及内存access，来得到AI，从而判断出来在roofline中的哪一个点。

