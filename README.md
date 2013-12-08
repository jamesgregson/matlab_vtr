matlab_vtr
========

Overview
------------

Matlab/Mex code to load VTK .vtr files into Matlab via a mex function.  Once built and on the Matlab path, data can be loaded as follows:

Information about a .vtr file can be found by simply calling the load_vtr function with the filename, which will print out the file dimensions (up to 4D), the bounding box and the point/cell data arrays that are included.  Data is assumed to be uniformly spaced.

```
>> load_vtr('solution_0009.vtr');
loading: solution_0009.vtr
  No arrays requested, listing file information...
    dimensions:   [200,300,1]
    bounding box: [-0.2,0.198,0,0.598,0,0]
    Point Data:
      density         [200,300,1,1]
      domain          [200,300,1,1]
      pressure        [200,300,1,1]
      signed_dis      [200,300,1,1]
      velocity        [200,300,1,3]
      velocity_fdt    [200,300,1,3]
      velocity_ref    [200,300,1,3]
done.
```

Specific arrays can be loaded by requesting them in the function call, e.g.

```
[den, vel] = load_vtr('solution_0009.vtr','density','velocity');
```

which will create den and vel arrays in the current workspace.  The function searches point-data first, then cell-data.  If it cannot find a specified array, the function exits and Matlab will print an error about output arguments not being set.

Building the MEX function
-----------------------------------

The extension can be built as follows. This has been tested on OS-X 10.7. From the root directory:

```
mkdir build
cd build
cmake  ..
make

cd ../mex
chmod +x build_mex.sh
./build_mex.sh
```

At this point you should be able to add the mex/ subdirectory to your Matlab path and use the function from anywhere.
