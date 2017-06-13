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
    date
    echo "Rozpoczynam procedure testowania..."
    
    # perform tests
    time_start=$(date +%s%N)
    ./test
    time_end=$(date +%s%N)

    # calculate duration
    duration=$(( time_end - time_start ))
    seconds=0
    mantissa=${duration: -9}
    (( ${#duration} > 9 )) && seconds=${duration::-9}

    date
    echo "Procedura testowania zakonczona."
    echo "Calkowity czas trwania testow: ~${seconds}.${mantissa}s"

    duration_out="${OUTDIR}/${MEASUREMENT_NAME}_${time_now}_duration_ns.txt"
    results_out="${OUTDIR}/${MEASUREMENT_NAME}_${time_now}_results.txt"

    echo "${duration}" > "${duration_out}"
    mv "${RESULTS}" "${results_out}"

    echo
    echo -n "Wygenerowane pliki z wynikami oraz czas trwania w nanosekundach "
    echo    "znajaduja sie w nastepujacych sciezkach:"
    echo "${results_out}"
    echo "${duration_out}"
    echo
cd - &> /dev/null
