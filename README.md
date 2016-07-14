#phdeem

This is a wrapper for using the hdeem library from within a MPI context.

##Compilation and Installation

###Prerequisites

To compile this plugin, you need:

* MPI

* `libhdeem`

* FreeIPMI

For building the examples you'll additionally need:

* A C++ 11 compatible compiler

###Building

1. Create a build directory

        mkdir build
        cd build

2. Invoke CMake

    Simplest possible call:

        cmake ..

    If either `libhdeem` or FreeIPMI are not installed in standard paths, you need to pass hints to
    CMake. For `libhdeem` you have to use `HDEEM_ROOT_DIR` and for FreeIPMI `FreeIPMI_ROOT_DIR`, e.g.

        cmake .. -DHDEEM_ROOT_DIR=/opt/hdeem/ -DFreeIPMI_ROOT_DIR=/home/user/freeipmi

    These can be provided as environment variables to. You can also pass different locations for
    header files and libraries with `HDEEM_INCLUDE_DIR` and `HDEEM_LIBRARY` or `FreeIPMI_INCLUDE_DIR`
    and `FreeIPMI_LIBRARY` resp.

    If you want to build the examples, include `BUILD_EXAMPLES=on` in your CMake call, or for the
    tests `BUILD_TESTS=on` resp., e.g.:

        cmake .. -DBUILD_EXAMPLES=on -DBUILD_TESTS=on

3. Invoke make

        make

4. Copy the resulting `libphdeem.so` to a location listed in `LD_LIBRARY_PATH` or add the
    current path to `LD_LIBRARY_PATH` with

        export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:`pwd`

##Usage

###C-interface

The usage of *phdeem* is just the same as the usage of `libhdeem`. You just have to replace the
`hdeem_` in the function names with `phdeem_` and provide some additional parameters:

* `info`

    A `phdeem_info_t` storing the information about the caller for each process. You don't have to
    worry about the contents of this, it is just for internal context.

* `ret_val`

    A `phdeem_status_t` containing the return values of the internally called `libhdeem` and
    MPI functions. You can access the values via `mpi_ret_value` and `hdeem_ret_value` resp.

They are passed as references to all function calls as shown below:

```c
int ret;
phdeem_info_t info;
phdeem_status_t int_rets;
hdeem_bmc_data_t hdeem_data;

ret = phdeem_init( &hdeem_data, &info, MPI_COMM_WORLD, &int_rets );
```

The return values of the functions tell you if either

1. everything went fine (`PHDEEM_SUCCESS`) or

2. the calling process is not the root process on the node and therefore no call to hdeem has been
    made (`PHDEEM_NOT_ROOT`) or

3. an error occurred in MPI (`PHDEEM_MPI_ERROR`) or `libhdeem` (`PHDEEM_HDEEM_ERROR`) (with the
    corresponding return values in `ret_val`).

For more information take a look at the comments in the header file or the examples.

###C++-interface

The usage of *cpphdeem* is slightly different from the usage of the C-interface and hdeem itself as
you don't need to pass arguments to the function calls. These are handled internally and if you need
to access their values you have to use the appropriate getter methods.

Otherwise the usage of *cpphdeem* is similar to the usage of the 'blank' hdeem library. After
calling the constructor of the `connection` class, you can call it's methods just as you would call
hdeem functions (but without the parameters).

> *Note:*

> Clean-up tasks are **not** implicitly done in the destructor of the `connection` class to give
> you the maximum freedom in terms of usage. You have to explicitly call functions like `stop` and
> `clear` (corresponding to `hdeem_stop` and `hdeem_clear`) at the right points in your program.



> *Note:*

> `hdeem_version()` is not mapped at the moment.


> *Note:*

> *phdeem* is **not** thread-safe!

###Tests

*phdeem* uses a hash function to determine whether the calling process is the root process on a
specific node. This function may collide for some of your node names. If you encounter errors,
please check if these are caused by collisions of the hash function.

In order to do that you can use the `test_hash` program which expects a newline seperated list of
node names in a file called `nodes.txt` and prints the number of dublicates found. You can build the
`test_hash` program by passing `-DBUILD_TESTS=on` as an argument to you CMake call. If you encounter
a non-zero return value, please file an [issue on Github](https://github.com/tud-zih-energy/phdeem/issues).

###Environment variables

None. For environment variables influencing the build, see the *Building* section.

###If anything fails

1. Check if your hardware has hdeem capability.

2. Write an [issue on Github](https://github.com/tud-zih-energy/phdeem/issues).

3. Write a mail to the author.

##Author

* Philipp Trommler (philipp.trommler at tu-dresden dot de)
