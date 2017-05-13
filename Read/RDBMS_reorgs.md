## database reorgs 
aim: to save data space and improve database efficiency and performance. 
The reason :  data in large RDMS tables eventually become fragmented. The size of tables and indexes inrcease as records 
become distributed ovre more data pages . More page reads and rows in non-join order during query execution slow query responses. 
To reclaim the wasted space,improve database uputime adn speed data accesss consider a stategy for reorganizinig  your database objects . 

Two type of tables: 
On-line databse : 
reorgs work incrementally by moving rows within the exisiting table to re-establish clustering, reclaim free space, and eliminate 
overflow rows.\

off-line database : 
reorgs are faster,but need  take the database off-line . With this method ,data is exported from the database into a 
dump file ( unload ) , THe databse objects set back up based on the extract typically re-ordered . Then they are returned to 
the same tablespace,where indexes are rebuild 
