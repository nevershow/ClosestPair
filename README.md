# ClosestPair
Find the closest point pair in 60,000 points of 784 dimensions

**输入:** 60000个784维的点(60000张28*28的手写字图片)

**算法1:**
使用Medrank算法(B+树实现), 输入一个查询点, 找出与查询点最近的点

**算法2:**
使用随机算法, 找出60000个点中的近似最近点对
