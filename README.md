### ADS small searching engine

​	Due to the use of \<filesystem\>, the project **must be constructed under C++17**

​	We strongly recommend you to construct this project by CMake. After you ensure the environment of C++17 is enable. The following code is used to build this project.

```shell
mkdir build
cd build
cmake ..
```

​	After executing the code above, you may discover two executable file named "P1_dict" and "query".

​	The first one is used to build the inverted file index, it will created a directory "dict" automatically and load in the index, so It should be runned first.

​	Then you can run the "query" to search for key words inside the  Shakespeare's books.