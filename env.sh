#
# Set up build environment.
#


export UVA_ROOT="$(dirname $(realpath ${BASH_SOURCE}))"
echo "export UVA_ROOT=${UVA_ROOT}"


# Run make
function m() {
  if [ ! -d "${UVA_ROOT}" ]; then
    echo "\${UVA_ROOT} is not a directory."
    return 1
  fi
  make -f ${UVA_ROOT}/Makefile $@
}


# Run unit tests
function t() {
  local UNITTESTS=("$@")
  local UNITTEST=

  if [ "${#UNITTESTS[@]}" -eq 0 ]; then
    for UNITTEST in *_unittest; do
      if [ -f ${UNITTEST} ]; then
        UNITTESTS+=(${UNITTEST})
      fi
    done
  fi

  local PASSED=()
  for UNITTEST in ${UNITTESTS[@]}; do
    ./${UNITTEST} && PASSED+=(${UNITTEST})
  done
  test ${#PASSED[@]} -eq ${#UNITTESTS[@]}
}
