## 文件布局
```
dmsStorageUnitHeader + dmsSpaceManagementExtent +  dmsMetadataManagementExtent
DMS_HEADER_SZ                DMS_SME_SZ                DMS_MME_SZ= DMS_MME_SLOTS*DMS_MB_SIZE 
```
其中 dmsMetadataManagementExtent 由　dmsMetadataBlock  _mbList [ DMS_MME_SLOTS ] 　组成　
DMS_MB_SIZE = sizeof(dmsMetadataBlock)



## 底层控制方式 
#define DMS_MAX_PG             (128*1024*1024) 
定义了可以访问控制的的页面数字，利用 
dmsSpaceManagementExtent.smeMask [ DMS_SME_LEN ] 标记一个页面是否被使用

## Collection 的总控制
dmsMetadat
aManagementExtent 下属的 dmsMetadataBlock 控制

## Collection 的分配
Collection 的开头必定在一个页面的有个 dmsMetaExtent 和 紧急其后的 dmsExtentID _array[1] 的信息  这些信息组成成 _metadataBlockEx 

> array[1] 是会被越界访问的数组，两个成以组成，描述一个 segment 中最使用的 extentID 的范围，
> 第一个是 描述在这个 segment 中最前面被使用的页面，第二个描述了　最后面被使用的页面，他们之间通过该 dmsExtent 的 preEvent 和 lastEvent 链接起来

> dmsMMetaExtent 跟踪了所有装载页面链表的头部和尾部

Collection 在一个页面的开头会放上 dmsExtent 的结构体



