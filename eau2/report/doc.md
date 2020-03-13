**eau2 project**


**Introduction**
The eau2 system is a distributed key/value store meant for big data analysis. 
It is meant to parse and store very large data files, allowing users to execute queries very quickly.
The system should be incredibly fast and scalable. 

**Architecture**
The eau2 system can be split into 3 layers: 
    *Application layer
    *DataFrame layer
    *KV Store layer

The KV Store layer consists of a series of keys and values distributed across a network.
All keys know where thier data is stored and can be used for searching the network for the proper data.
The network contains a group of nodes, all connected to each other as well as a master server.

Above the KV Store layer is the DataFrame layer. In this layer, calls to the KVStore layer allow all of the data to be put together.
This layer provides an extra level of abstraction in the forms of DataFrames and distributed arrays.
Each DataFrame contains a set of distributed arrays.
Distributed arrays are arrays split into fixed-size chunks. These chunks are then distributed across the network.

The highest layer is the Application layer, this is where the user writes the code that implements this system.
The application can provide data to create DataFrames as well as query for modified versions of those DataFrames.
However, this layer isn't responsible for how the data is put together, distributed or retrieved.

**Implementation**
For our implementation we are currently focusing on the lower two layers of the system.

In the DataFrame layer, we are using a third party data adapter to parse a data file and read it into our DataFrame.
Our DataFrame stores the data in a series of Columns 

In the KV Store layer, 

**Use cases**

**Open questions**

**Status**
At the moment, we have built most of the DataFrame layer.
We are able to read data from a file, build a DataFrame from that file, and perform some small operations on that DataFrame.

In terms of upcoming work, we still need to build a functioning network. 
Then put a KV Store over that network, and integrate it with our DataFrame layer.
Finally we need to put some work into the connection to the Application layer so the user can easily work with the system.
We expect these 3 items to take a week each.