#phdeem

This is a wrapper for using the hdeem library from within a MPI context.

##Compilation and Installation

###Prerequisites

To compile this plugin, you need:

* MPI

* `libhdeem`

* FreeIPMI

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

3. Invoke make

        make

4. Copy the resulting `libphdeem.so` to a location listed in `LD_LIBRARY_PATH` or add the
    current path to `LD_LIBRARY_PATH` with

        export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:`pwd`

##Usage

The usage of *phdeem* is just the same as the usage of `libhdeem`. You just have to replace the
`hdeem_` in the function names with `phdeem_` and provide some additional parameters:

* `caller`

    A `phdeem_info_t` storing the information about the caller for each process. You don't have to
    worry about the contents of this, it is just for internal context.

* `ret_val`

    A `phdeem_int_ret_value_t` containing the return values of the internally called `libhdeem` and
    MPI functions. You can access the values via `mpi_ret_value` and `hdeem_ret_value` resp.

They are passed as references to all function calls as shown below:

    int ret;
    phdeem_info_t caller;
    phdeem_int_ret_value_t int_rets;
    hdeem_bmc_data_t hdeem_data;

    ret = phdeem_init( &hdeem_data, &caller, MPI_COMM_WORLD, &int_rets );

The return values of the functions tell you if either

1. everything went fine (`PHDEEM_SUCCESS`) or

2. the calling process is not the root process on the node and therefore no call to hdeem has been
    made (`PHDEEM_NOT_ROOT`) or

3. an error occurred in MPI (`PHDEEM_MPI_ERROR`) or `libhdeem` (`PHDEEM_HDEEM_ERROR`) (with the
    corresponding return values in `ret_val`).

For more information take a look at the comments in the header file or the examples.

> *Note:*

> `hdeem_version()` is not mapped at the moment.


> *Note:*

> *phdeem* is **not** thread-safe!

###Environment variables

None. For environment variables influencing the build, see the *Building* section.

###If anything fails

1. Check if your hardware has the hdeem capability.

2. Write an [issue on Github](https://github.com/tud-zih-energy/phdeem/issues).

3. Write a mail to the author.

##Author

* Philipp Trommler (philipp.trommler at tu-dresden dot de)
