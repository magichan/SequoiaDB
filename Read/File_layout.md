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

## Collection 的分配
Collection 的开头必定在一个页面的有个 dmsMetaExtent 和 紧急其后的 ossMmap.segment 的信息

Collection 在一个页面的开头会放上 dmsExtent 的结构体
