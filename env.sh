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


# Check C++ style
function style() {
  if [ -z "$1" ]; then
    echo "Usage: style file.cc"
    return 1
  fi
  vimdiff -R "$1" <(clang-format -style=Chromium "$1")
}


# Run presubmit tests
function presubmit() {
  if [ -z "$1" ]; then
    echo "Usage: presubmit file{.cc}"
    return 1
  fi

  local PROGRAM="./${1%.cc}"
  if [ ! -x "${PROGRAM}" ]; then
    echo "${PROGRAM} is not an executable"
    return 1
  fi

  local RET=0
  local INPUT_PATH=
  local OUTPUT_PATH=
  for INPUT_PATH in *in*; do
    OUTPUT_PATH="$(echo ${INPUT_PATH} | sed -e 's/in/out/')"
    if ! ${PROGRAM} < ${INPUT_PATH} | diff -q ${OUTPUT_PATH} - ; then
      ${PROGRAM} < ${INPUT_PATH} | diff -y ${OUTPUT_PATH} -
      RET=1
    fi
  done

  if [ "${RET}" -eq 0 ]; then
    echo "All tests passed"
  fi

  return $RET
}
