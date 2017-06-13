#!/bin/bash
WORKDIR=${PWD}
MEASUREMENT_NAME=${1}
TESTDIR="${WORKDIR}/test"
APP="${TESTDIR}/test"
RESULTS="${TESTDIR}/results.txt"
OUTDIR="${WORKDIR}/results"

mkdir -p "${OUTDIR}"
cd "${TESTDIR}"
    time_now=$(date +%y%m%d%H%M%S)
    time_start=$(date +%s%N)
    ./test
    time_end=$(date +%s%N)
    duration=$(( time_end - time_start ))
    echo "${duration}" > "${OUTDIR}/${MEASUREMENT_NAME}_${time_now}_duration_ns.txt"
    mv "${RESULTS}" "${OUTDIR}/${MEASUREMENT_NAME}_${time_now}_results.txt"
cd -
