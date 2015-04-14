Compiling GCC as a cross compiler
=================================

Firstly, navigate to a directory where you will be temporarily storing the various sources.
In this example, it will be `~/src`.

```console
$ cd ~/src
$ curl https://ftp.gnu.org/gnu/binutils/binutils-2.25.tar.gz > binutils.tar.gz
$ curl ftp://ftp.mirrorservice.org/sites/sourceware.org/pub/gcc/releases/gcc-4.9.2/gcc-4.9.2.tar.gz > gcc.tar.gz
$ brew install isl cloog gmp mpfr libmpc
```

We need to set some env vars that describe where to put the gcc binaries once built.
In this example we will place the binaries in `~/src/eric/xc`, which is where the makefile expects them.
This will ensure nothing is installed globally and it'll all be contained inside the xc directory.

```console
$ mkdir ~/src/eric/xc
$ mkdir ~/src/eric/xc/bin
$ export PREFIX="~/src/eric/xc"
$ export TARGET=i686-elf
$ export PATH="$PREFIX/bin:$PATH"
```

Then we build binutils

```console
$ tar -xzf binutils.tar.gz
```

We need to move some of the brew installed libraries into the binutils directory.
The actual paths may differ - use `brew info <lib>` to find the path for the lib.

```console
$ cd ~/src/binutils
$ cp -r /usr/local/Cellar/isl/0.12.2 isl
$ cp -r /usr/local/Cellar/cloog/0.18.1 cloog
$ mkdir build
$ cd build
$ ../configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
$ make
$ make install
```

Next we build GCC

```console
$ cd ~/src
$ tar -xzf gcc.tar.gz
$ cd gcc
```

Again the actual paths may be different here - check with `brew info <lib>`.
You may also need to install libiconv from source (homebrew sometimes refuses to install it). Build it and put the result in a directory called `libiconv` in the gcc dir.

```console
$ cp -r /usr/local/Cellar/gmp/6.0.0a gmp
$ cp -r /usr/local/Cellar/mpfr/3.1.2-p8 mpfr
$ cp -r /usr/local/Cellar/libmpc/1.0.2 mpc
$ cp -r /usr/local/Cellar/isl/0.12.2 isl
$ cp -r /usr/local/Cellar/cloog/0.18.1 cloog
$ mkdir build
$ cd build
$ ../configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c --without-headers
$ make all-gcc
$ make all-target-libgcc
$ make install-gcc
$ make install-target-libgcc
```

Now you can safely delete `~/src/gcc` and `~/src/binutils`.
The various gcc binaries will be in `~/src/eric/xc`
