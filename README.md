matlab_vtr
========

Matlab/Mex code to load VTK .vtr files into Matlab via a mex function.  Once built and on the Matlab path, data can be loaded as follows:

```
[density, velocity] = load_vtr( 'input.vtr', 'density', 'velocity' );
```