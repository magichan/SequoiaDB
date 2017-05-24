内容: 
数据库分区设计
假设
Sequioadb 提供两种分区类型，水平分区和垂直分区。这两种分区类型和关系型数据库的水平分区和垂直有关系，但也不完全相同。
水平分区是将所有的元组划分成 n 个不想交的子集，不同的子集会在不同的复制组中，而垂直分区，在 SequoiaDB集群环境中，用户也可以将一个集合全局关系的属性分成若干子集，并在这些子集上作投影运算，将这些子集映射到另外的集合上，从而实现集合关系的垂直切分；该集合称之为主集合，每个切分的子集称为分区，分区映射的集合称为子集合；一个分区只能映射到一个子集合中，但一个子集合可以承载多个分区；分区在子集合之间可以通过垂直切分操作进行重映射。

在两种分区类型的基础上，针对分区键有两种分区方式，Hash 和 range ，对于水平分区 hash 和 range 都可以使用，垂直分区稚只能使用 range。 range 分区方式，是根据分区键的范围选择要插入的分区。Hash 分区方法是根据
Hash值(分区键所生成的）选择该条对象应该插入到那个分区表中。SequioaDB 
现在我们假设 id 在 1-100　是属于特殊用户，100~无穷大是属于普通用户，两种用户的基本信息一致，但在实际的应用中需要放到
不同的复制组中(或者说存储地), 因此使用水平分区的方式。
根据 SequioaDB 提供的分区方式和实际需要，选择使用水平　range 分区, 水平分区和垂直
实现

Sequioadb

db.createCS("forum")
db.foo.createCL("user")
db.foo.createCL(""
db.forum.user.insert({"id":1,"name":"admin","loginname":"admin_1","pass":"base64","eamil":"admin_1@forum.com","accessTOken":"base64"})

db.forum.user.split("datagroup","admingroup",{id:1},{id:100})
