# Redis_project
I have implemented basic redis in c++.


What is Redis?
Redis is often referred as a *data structures* server. What this means is that Redis provides access to mutable data structures via a set of commands, which are sent using a *server-client* model with TCP sockets and a simple protocol. So different processes can query and modify the same data structures in a shared way.

Data structures are stored on the computer disk. So the Redis is Non-volatile.

Inside the src directory two c++ programm is present. One is server.cpp and another is client.cpp. Also a "database.txt" and a "setbase.txt" file is present which stores all the data and set respectively inserted by client or server.

Running Redis
--------------
To run redis just type:

    ```
     % cd src
     
     % ./server PORT(you have to pass the port number as arguments).
     
    ```


Playing with Redis
--------------------
You can use client to play with Redis. Start a server instance,
then in another terminal try the following:
```
% cd src
% ./client PORT(you have to pass the port number as arguments).

```

```
redis> set hello world
redis> 1
redis> get hello
redis> world

```

I have implemented six commands-> 1.SET 2.GET 3.EXPIRE 4.ZADD 5.ZRANK 6.ZRANGE.You can give the commands in any case.

1.SET COMMANND
---------------
Set key to hold the string value. Both the key and value are of string type. If any key is already present its value is overwritten.
Time complexity: O(1).
Return value: 1.
SET [KEY] [VALUE]
e.g 
```
redis> set foo bar // foo is the key and bar is the value
redis> 1.

```

Data Structures Used:- unordered_map
unordered_map is an associated container that stores elements formed by combination of key value and a mapped value.It is found in C++ STL(Standard Template Library).
Internally unordered_map is implemented using Hash Table, the key provided to map are hashed into indices of hash table that is why performance of data structure depends on hash function a lot but on an average the cost of search, insert and delete from hash table is O(1).

2.GET COMMAND
--------------
Get the value of key. If the key does not exist the special value nil is returned.
Time Complexity: O(1)
Return value: Value of the key // if key is present
              nil // if key is not present
GET [KEY]
e.g 

```
redis> set foo bar
redis> 1
redis> get foo
redis> bar
redis> get example
redis> nil

```

Data Structures Used:- unordered_map.

3.EXPIRE COMMAND
------------------
Set a timeout on key. After the timeout has expired, the key will automatically be deleted.
Time Complexity: O(1).
Return value:
1 if the timeout was set.
0 if key does not exist.

EXPIRE [KEY] [TIME]
KEY is the key name.
TIME is the time in seconds. i.e the key will live for next TIME seconds after the timeout set or after the expire command.
e.g

```
redis> set mykey myvalue
redis> expire mykey 10
redis> 1
redis> get mykey // immediately
redis> myvalue
redis> get mykey // after 10 seconds
redis> nil
redis> expire mykey 5
redis> 0

```

Data Structures Used:- unordered_map.
If a key is present then the the timeout value is updated and 1 is returned else 0 is returned. when get command is executed the time lapse between the execution time and present time is calculated. if the time lapse is greater than the given TIME than the key is deleted and nil is returned else the value is returned.

3.ZADD
--------
Adds all the specified members with the specified scores to the sorted set stored at key. If a specified member is already a member of the sorted set, the score is updated and the element reinserted at the right position to ensure the correct ordering.

If key does not exist, a new sorted set with the specified members as sole members is created.

The score values should be the string representation of a integer with range -INT_MAX to +INT_MAX

Time complexity: O(log(N)) for each item added, where N is the number of elements in the sorted set.
Return value: 1 

ZADD [SETNAME] [SCORE] [KEY]
e.g

```
redis> zadd myset 0 zero
redis> 1
redis> zadd myset 1 one
redis> 1

```

Data Structures Used:- unordered_map,ordered_set in C++ STL.

unordered_map is used to map between SETNAME and its sorted set.

ordered_set is used for sorted set.
Ordered set is a policy based data structure in g++ that keeps the unique elements in sorted order. It performs all the operations as performed by the set data structure in STL in log(n) complexity and performs two additional operations also in log(n) complexity .
order_of_key (k) : Number of items strictly smaller than k .
find_by_order(k) : K-th element in a set (counting from zero).

For more details refer to :
https://gcc.gnu.org/onlinedocs/libstdc++/manual/policy_data_structures.html

If in a set two or more keys are inserted with same value than the score key pairs is stored in lexicographical order of key
else from low to high of score value.

4.ZRANK
--------
Returns the rank of member in the sorted set stored at key, with the scores ordered from low to high. The rank (or index) is 0-based, which means that the member with the lowest score has rank 0.

Time complexity: O(log(N)) where N is the number of elements in the sorted set.

Return value:If member exists in the sorted set, Integer reply: the rank of member.
If member does not exist in the sorted set or key does not exist, Bulk string reply: nil.

ZRANK [SETNAME] [KEY]

e.g

```
redis> zadd myset 2 two
redis> 1
redis> zadd myset 0 zero
redis> 1
redis> zadd myset 3 three
redis> 1
redis> zrank myset two 
redis> 1
redis> zrank myset zero
redis> 0
redis> zrank myset four
redis> nil

```

Data Structures Used:- unordered_map,ordered_set in C++ STL.
Rank can be found by order_of_key (k) : Number of items strictly smaller than k . of ordered_set in C++ STL.
First the score is found using unordered_map then rank is calculated using order_of_key (k).

5.ZRANGE
---------
Returns the specified range of elements in the sorted set stored at key. The elements are considered to be ordered from the lowest to the highest score. Lexicographical order is used for elements with equal score.

Time complexity: O(log(N)+M) with N being the number of elements in the sorted set and M the number of elements returned.

Return Value:
list of elements in the specified range (optionally with their scores, in case the WITHSCORES option is given).

ZRANGE [SETNAME] [START] [STOP] [WITHSCORE]

Both start and stop are zero-based indexes, where 0 is the first element, 1 is the next element and so on. They can also be negative numbers indicating offsets from the end of the sorted set, with -1 being the last element of the sorted set, -2 the penultimate element and so on.

start and stop are inclusive ranges, so for example ZRANGE myzset 0 1 will return both the first and the second element of the sorted set

Out of range indexes will not produce an error. If start is larger than the largest index in the sorted set, or start > stop, an empty list is returned.

WITHSCORE is a string value "1" or "0" for displaying with scores or without scores respectively.

e.g

```
redis> zadd myset 2 two
redis> 1
redis> zadd myset 0 zero
redis> 1
redis> zadd myset 3 three
redis> 1
redis> zadd myset 4 four
redis> 1
redis> zrange myset 0 2 1
redis> zero
       0
       two 
       2 
       three 
       3
redis> zrange myset 0 -1
redis> zero
       0
       two 
       2 
       three 
       3
       four 
       4
   
   ```
   
Data Structures Used:- unordered_map,ordered_set in C++ STL.
First the key value pair is found by find_by_order(k) : K-th element in a set (counting from zero).
Then the iterator pointing to start element and stop element is found using find() in set in C++ STL in log(n) complexity.
Then iterate the range and hence the complexity is O(log(N)+M) with N being the number of elements in the sorted set and M the number of elements returned.

Maintaining the databse
-----------------------
Whenever the server wakes up it first process all data stores at "database.txt". i.e it inserts all the data into unordered_map and in ordered sets. 

when the server closes It clears the "database.txt" and again rewrites the changes or updated using the values stored in unordered_map.

Similarly for "setbase.txt".

Hence it is non-volatile.


REPORT
--------
1. I choose the C++ programming language because.

C++ is a powerful, efficient and fast language. It finds a wide range of applications – from GUI applications to 3D graphics for games to real-time mathematical simulations.

C++ has a rich function library.

C++ is an object-oriented programming language and includes classes, inheritance, polymorphism, data abstraction and encapsulation.

With the C++ client of redis, C++ programers can use its power easily.

2. I have implemented it using C++ STL (unordered_map) and policy based data structure (ordered_set) which can get values by O(1) complexity, and get ranks by O(log(N)).
It can be improved by skiplist which is used by redis. It has a better performance than STL's red-black tree.

Also I have use strings data types. It can be improved my Redis object:

```
typedef struct redisObject {
    unsigned type:4;
    unsigned encoding:4;
    unsigned lru:LRU_BITS; /* lru time (relative to server.lruclock) */
    int refcount;
    void *ptr;
} robj;

```

This structure can represent all the basic Redis data types like strings, lists, sets, sorted sets and so forth. The interesting thing is that it has a type field, so that it is possible to know what type a given object has, and a refcount, so that the same object can be referenced in multiple places without allocating it multiple times.

3. I have used C++ STL (unordered_map) and policy based data structure (ordered_set) which can get values by O(1) complexity, and get ranks by O(log(N)).

C++ STL is easy to code and use. Also it has better time complexity than other data structures since it uses red black tree
for implementation.

4.My implementation supports multi threaded operations. 

I have handled multiple clients by using select() linux command.
Select command allows to monitor multiple file descriptors, waiting until one of the file descriptors become active.

For example, if there is some data to be read on one of the sockets select will provide that information.

Select works like an interrupt handler, which gets activated as soon as any file descriptor sends any data.

Data structure used for select: fd_set
It contains the list of file descriptors to monitor for some activity.
There are four functions associated with fd_set:

```
fd_set readfds;

// Clear an fd_set
FD_ZERO(&readfds);  

// Add a descriptor to an fd_set
FD_SET(master_sock, &readfds);   

// Remove a descriptor from an fd_set
FD_CLR(master_sock, &readfds); 

//If something happened on the master socket , then its an incoming connection  
FD_ISSET(master_sock, &readfds); 

```



















              









