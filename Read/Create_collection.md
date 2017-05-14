```flow 
st=>start: Start 
e=>end: End 
op1=>operation: rtnCreateCollectionCommand 入口函数，输入需要创建的collection Name (带有 collection space ) 
op2=>operation: 解析输入参数，collection space 不存在则创建之
op3=>operation: 获取 dmsStoargeData.addCollection 函数，输入参数 
op4=>operation: _findFreeSpace 让 dmsSMEMgr 预备好足够的空间，返回一个 dmsExtentID(page 位置)
op5=>operation: 从 MME._mbList 找找一个没有被使用的 MB ，其下标就是 CollectionID 和 mbid
op6=>operation: 从 MME 拿出 MB ，并將信息初始化,包括 collection name , 
collectionID(mbID) , logicalID , arrtibutes,( LogicID 来自 _dmsHeader._MBHWM)
op7=>condition: 讲 dmsExtentID 转换为指向该位置的实际指针(说事实际，在 Mmmap 操作下的指针) 
op7_1=>operation: 转换过程  
op7_2=>operation: 根据 dmsExtentID 找到是第 i 段( ossMmapFile._segments(vector < ossMmapSegment >) ) 
op7_3=>operation: ossMmapSegment 存放着该段的起始指针，再利用页面偏移*页面大小就可计算指针应该偏移多少，返回 dmsExtentID 指向 page 的实际位置 
op8=>operation: 初始化，并且会越界访问结构体后面的内容，这段内容存放着一个数组，这个数据已两个值为一单元，一个单元对应这 pages 分为固定长度的 segments 中的一个，第一个值指向 segment 的开头，第二值指向结尾
op9=>operation: getMBcontext 获得 dmsMBContext 
op10=>condition: initpage 为 0  
op10_1=>operation:allocateExtent  为 dmsMBcontext 的内容分配空间 
op10_2=>operation: 

op11=>operation: 有 index 创建 index 
op12=>operation: releaseMBContext 
op13=>operation: 有分片索引，就建立分片索引


 




```
