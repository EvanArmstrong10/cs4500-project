# eau2
 
 
## Introduction
The eau2 system is a distributed key/value store built to support big data analysis. It handles all networking, data storage, and analysis to enable users to perform expensive operations on large data sets.
 
## Architecture
The eau2 system can be split into 3 layers:
   * Application layer
   * DataFrame layer
   * KV Store layer
 
The KV Store layer consists of a series of keys and values distributed across a network.
All keys know where their data is stored and can be used for searching the network for the proper data.
The network contains a group of client nodes connected to each other and a master node.
 
Above the KV Store layer is the DataFrame layer. In this layer, calls to the KVStore layer allow data to be structured as an easily accessible DataFrame. To the user, the DataFrame is  stored in columnar format. Under the hood, each DataFrame contains a set of distributed arrays with data spread across multiple nodes of the KV layer. These distributed arrays are arrays split into fixed-size chunks to enable easy serialization and distribution across the network.
 
The highest layer is the Application layer, where the user can query and manipulate data in the previous layer.

## Implementation
For our implementation we are currently focusing on the lower two layers of the system.
 
In the DataFrame layer, we are using a third-party data adapter to parse a data file and read it into our DataFrame. This decision was made because our DataFrame and data adapter were developed in different languages. By using a third-party adapter, the technical cost of rewriting these structures is avoided.
Our DataFrame stores data in a series of Columns. Each column will be a distributed array that can be spread across the KV Store.
 
In the KV Store layer, we will have a single Server and a series of Clients. The Clients will all register with the server to get the addresses of the other Clients in the system. The Clients will then communicate directly via their own sockets.
 
## Use cases
Since we are in the early stages of development, our use cases are fairly small.
Right now we can read a data file into a DataFrame and perform an action on that DataFrame. Data is not yet distributed or duplicated across nodes.
 
## Open questions
Which features of the Application will be built in to the system versus user built?
How will data concurrency be ensured across nodes?


## Status
At the moment, we have built most of the DataFrame layer.
We are able to read data from a file, build a DataFrame from that file, and perform some small operations on the resulting DataFrame.
 
In terms of upcoming work, we still need to build a functioning network.
The next step is building the distributed KV Store over that network and integrate it with our DataFrame structures.
Finally we need to put some work into the connection to the Application layer so the user can easily work with the system.
We expect these 3 items to take a week each.



