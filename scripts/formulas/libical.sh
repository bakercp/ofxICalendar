#! /bin/bash
#
# libical
#
# uses CMake

# define the version
VER=3.0.4

FORMULA_TYPES=( "osx" "linux64")

LIB_NAME=$( basename "${BASH_SOURCE[0]}" .sh)

# download the source code and unpack it into LIB_NAME
function download() {
    local LIB_NAME=$( basename "${BASH_SOURCE[0]}" .sh)
    local LIB_NAME_VER=${LIB_NAME}-${VER}
    local FILE_SUFFIX=.tar.gz

    curl -LkO https://github.com/libical/libical/releases/download/v${VER}/${LIB_NAME_VER}${FILE_SUFFIX}
    tar -xjvf ${LIB_NAME_VER}${FILE_SUFFIX}
    mv ${LIB_NAME_VER} ${LIB_NAME}
    rm ${LIB_NAME_VER}${FILE_SUFFIX}
}

# prepare the build environment, executed inside the lib src dir
function prepare() {
  :
}

# executed inside the lib src dir
function build() {
    if [ "$TYPE" == "osx" ] || [ "$TYPE" == "linux64" ] ; then
        mkdir -p "build"
        pushd "build" || return
        cmake \
          -D STATIC_ONLY=True \
          -D CMAKE_DISABLE_FIND_PACKAGE_ICU=True \
          -D WITH_CXX_BINDINGS=True \
          -D ICAL_GLIB=False \
          -D ICAL_BUILD_DOCS=False \
          -D CMAKE_INSTALL_PREFIX="${LIBS_DIR}/${LIB_NAME}" \
          -D CMAKE_INSTALL_LIBDIR="${LIBS_DIR}/${LIB_NAME}/lib/${TYPE}" \
          ..
        make
        popd || return
    fi
}

# executed inside the lib src dir, first arg $1 is the dest libs dir root
function copy() {
  cd "${BUILD_DIR}/${LIB_NAME}/build" || exit 1
  make install

  # Clean up.
  pushd "${LIBS_DIR}/${LIB_NAME}/lib/${TYPE}/" || return
  rm -r cmake/
  rm -r pkgconfig/
  popd || return

}

# executed inside the lib src dir
function clean() {
  # TODO
  :
}
