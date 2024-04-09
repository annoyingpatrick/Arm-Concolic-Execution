# quick-arm-interpreter
Gonna be a powerful concolic execution tool against arm assembly.

## Environment setup
For mac user, add the libz3.dylib path to DYLD_LIBRARY_PATH
```
export DYLD_LIBRARY_PATH=/Library/Developer/CommandLineTools/Library/Frameworks/Python3.framework/Versions/3.9/lib:$DYLD_LIBRARY_PATH
```
## Quick start
Modify input.s to be the assembly under simulation
```
make all
./simulator input.s
```



## Installing Z3
From anywhere, install z3 by following these steps

```
git clone https://github.com/Z3Prover/z3.git
cd z3//
python scripts/mk_make.py
cd build
make
sudo make install
```
