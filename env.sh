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
  local INPUT="$(default_input $1)"

  m unittests || return 1

  ./${INPUT}_unittest
}


# Check C++ style
function style() {
  local INPUT="$(default_input $1)"
  local STYLE='{BasedOnStyle: Chromium, Standard: Cpp11}'
  vimdiff -R "${INPUT}.cc" <(clang-format -style="${STYLE}" "${INPUT}.cc")
}


# Run presubmit tests
function presubmit() {
  local INPUT="$(default_input $1)"

  m presubmit || return 1

  local PROGRAM="./${INPUT}_presubmit"
  if [ ! -x "${PROGRAM}" ]; then
    echo "${PROGRAM} is not an executable"
    return 1
  fi

  local RET=0
  local INPUT_PATH=
  local OUTPUT_PATH=
  for INPUT_PATH in *in*; do
    if [ -x "${INPUT_PATH}" ]; then
      continue
    fi
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


function default_input() {
  if [ -n "$1" ]; then
    echo "${1%.cc}"
  else
    basename $(pwd)
  fi
}
