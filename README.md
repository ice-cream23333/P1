### ADS small searching engine

​	Due to the use of \<filesystem\>, the project **must be constructed under C++17**

​	We strongly recommend you to construct this project by CMake after you download it and make sure the environment of C++17 is enable. You can use the following code to build this project.

```shell
mkdir build
cd build
cmake ..
```

​	After following code, you may discover two executable file named "P1_dict" and "query".

​	The first one is used to build the inverted file index, it will created a directory "dict" automatically and load in the index, so when running the project firstly, you should run this first.

​	Then you can run the "query" to search for key words inside the  Shakespeare's books.