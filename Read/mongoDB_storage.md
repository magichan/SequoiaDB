
## Memory-Mapped Files 
a segment of virtual memory that has been assignd a direct 
byte-for-byte correlation with some portion of a file or file-like 
resource . 

This resource is typically a file that is physically present on 
disk, but can also be a device., shared memory object , or 
other resource that the operating system can reference through 
a file descriptor . 

**Aim**: 
this correlation between the file and the memory space permits 
app to treat the mapped portion as if it were primary memory

## MongoDB 
MongoDB 使用内存映射文件

Advantage: 
简化了修改数据的逻辑

逻辑结构:

* 磁盘上的文件是由 extent 构成的，分配集合空间的时候以 extent 为
单位进行分配
* 一个集合有多个 extent
* ns 文件的命名空间指向那个集合的第个extent 

### 数据文件和空间分配　
在创建数据库的时候，MongoDB　会在磁盘分配一组数据文件，所有集合
索引，数据库　和　其他元数据都保存在这些文件里
