EDU 的结构

pmdEDUMgr 作为管理类，操作 pmdEDUCB 的队列

msgMessage.cpp: 用来做建立和抽取用于传递信息的内容

在函数命名上 selector 是查询的返回对象，　matcher 则是构造了查询条件

Lob largic object 大数据变量

存储模型

其具体实现也是经典的文件--数据段---数据页结构
文件可以跨页（废话，页的大小最大64k），但是跨不了块。
不知道是copy mongoDB还是处于系统简化的考虑，底层实际上还是mmap，也就是说读和换页什么的还是靠系统自己去搞得，相对于mongoDB的改进在于有后台任务刷脏页到磁盘。

查找 json 存储引擎

dms 的 storage 的 hpp 结构 
dmsStorageBase.hpp -> dmsStorageData.hpp -> dmsStorageUnit.hpp
