```flow

st=>start: Start 
e=>end: End
op0=>operation:DMS控制函数初试化 _SDB_DMSB::init()
op1=>operation:rtnLoadCollectionSpaces(载入数据库文件路径) 
op2=>operation:遍览文件夹下的文件，一个数据库文件对应一个 collection ,并且初始化出其对应的 dmsStorageUnit 变量 
op2_1=>operation:dmsStorageUnit的初始化工作有，初始化 stroageInfo( pageSzie suName,等信息 ) 
op2_2=>operation:new出一个  Unit　所下属的 dmsStorageData 
op3=>operation:dmsStorageUnit 利用 open　函数装载对应的数据库内容
op4=>operation: Open 函数分别启动 data , index, lob (class sdmStorageDate) 的 openStoarge 函数 
op4_1=>condition: 获取 pPath, createNew ,打开文件后，根据文件的大小和 createNew 判断是否需要新建立文件
op4_1_yes=>operation: 需要新键文件 
op4_1_yes_1=>operation: 初始化 dmsStorageUnitHeader 并写入文件( 负责 Unit 的配置信息)
op4_1_yes_2=>operation: 初始化 dmsSpaceManagementExtent 并写入文件( 一个bit位对应一个页面是否被使用) 
op4_1_yes_3=>operation: 调用 dmsStroageDate 集成 Base 的 _onCreate 函数 
op4_1_yes_4=>operation: _onCreate函数 初始化 dmsMetadataManagementExtent(负责Extent的管理) ,并写入文件,注意他写入的是 MB 数据

op4_1_no_1=>operation: 利用 mmap 函数装载 dms_header ,并且把装载后的内存信息放入 OssMmap 函数中的 sooMmapSegment 类变量中,压入 ossMmap 变量的 _segments (vector<ossMmapSegment>) 
op4_1_no_2=>operation: 映射 dmsSpaceManagementExtent(SME) 变量 
op4_1_no_3=>operation: 将 SME 扔到 _semMgr() 的 init 函数中，将 _semMgr 初始化 
op4_1_no_3_1=>operation: _semMgr 是 dmsSMEMgr 变量,以page为单位管理文件，利用 SME 的 mask 识别 page 的使用情况，dmsSegmentSpace 存储
op4_1_no_3_2=>operation: _onMapMeta() 从文件中装载 dmsMetadataMangementExtent(MME) 变量，并且在内存中初始化
op4_1_no_3_2_1=>operation: 初始化的过程: 依次读取 MB 
op4_1_no_3_2_2=>operation: MB 被使用，则将他的 collectionname 和 mbID(在 MME._mblist 的第几个) 插入到 Data._collectionNameMap 中 
op4_1_no_3_2_3=>operation: 将 MB 的信息(eg，totalRecords, totalDtatpages, totalDataFreeSpace) 复制到 Data._mbStatInfo[i] 中　
op4_1_no_4=>operation: 根据 header 里的 sizes 信息，依次从文件中将这些内容 以 _getSegmentSize() 的大小划分成一个个 segment
op4_1_no_5=>operation: 每次形成的 segment 都会被放到 ossMmapFile 的 _segment 中，并且 Base._dataSegId 和 Base._maxSegID 指明了这些segment在 _segment(vector)的位置范围
op4_1_no_6=>operation: 初始化 回写链 _dirtyList 
op4_2=>operation: 开启 index 和 lob 的 Data 

op5=>operation: dmsCB->addCollectionSpace: 根据 csName 将数据在 storageUnit 中初始化


st->op0->op1->op2->op3->op4->op4->e

```
