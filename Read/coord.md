Coord 承担代理的角色，作为集群对外的接头人，他转发
消息给其他节点

客户端链接到 coord ，coord 便会启动一个线程，为该链接服务

流程:
监听 client　的链接，启动一个 EDU_TYPE_AGENT 类型的线程 

消息转发 
coord 会初始化很多 command 

comman 的初始化方式: 放在 factory 类中作为 coordCB 的私有变量被调用


综合全文的讲述，coord处理client请求的流程

发送请求给coord节点

   coord先揪出这个请求是做什么

      交给对应的command处理

         查询（本地缓存或者远程获取的）catalog信息

         把消息转成节点间的内部消息

         转发给目标节点

         然后等待返回数据

     再把返回数据交给处理线程

线程把返回结果发送给client
