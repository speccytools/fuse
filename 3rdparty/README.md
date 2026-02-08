# Third-Party Dependencies

This directory contains third-party dependencies required to build Fuse on Windows.

## Dependencies

### Required

- **libspectrum** - Git submodule at `libspectrum/`
  - Source: https://github.com/speccytools/libspectrum
  - Version: Latest from master branch
  - Status: Included as git submodule (relative path: `../libspectrum`)

### Optional (for enhanced features)

- **audiofile** - Patched version with Debian patches
  - Source: https://github.com/sbaldovi/audiofile (patched branch)
  - Archive: `audiofile-patched.zip`
  - Provides: Audio file format support
  - Note: Upstream version 0.3.6 is outdated; this patched version is recommended

- **libxml2** - XML parsing library
  - Source: http://xmlsoft.org/sources/libxml2-2.9.12.tar.gz
  - Archive: `libxml2-2.9.12.tar.gz`
  - Version: 2.9.12
  - Provides: XML configuration file support

## Building Dependencies

### libspectrum (Required)

Since libspectrum is a git submodule, initialize it first:

```bash
git submodule update --init --recursive
cd 3rdparty/libspectrum
./autogen.sh
./configure --with-fake-glib --without-libaudiofile --without-libgcrypt \
            --without-zlib --without-bzip2 --prefix=/usr/local
make
make install
```

### audiofile (Optional)

```bash
cd 3rdparty
unzip audiofile-patched.zip
cd audiofile-patched
./autogen.sh --disable-static --disable-flac --disable-docs --disable-examples
make
make install-strip
```

### libxml2 (Optional)

```bash
cd 3rdparty
tar -xzf libxml2-2.9.12.tar.gz
cd libxml2-2.9.12
./configure --disable-static --without-iconv --without-python --without-lzma
make
make install-strip
```

## Notes

- All dependencies should be built before building Fuse
- The build scripts in the root directory will handle building libspectrum automatically
- Optional dependencies can be omitted if you configure Fuse with `--without-*` flags
