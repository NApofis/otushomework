#!/bin/bash
BINARY_HOME=./bin
INPUT_HOME=./input

if [ "$1" = "--unit" ]
then

  INPUT=${INPUT_HOME}/unit_test.csv
  OUTPUT=test_result

  cat ${INPUT} | ${BINARY_HOME}/mapper | sort -k1 | ${BINARY_HOME}/reducer > ${OUTPUT}
  cat ${INPUT} | ${BINARY_HOME}/mapper | sort -k1 | ${BINARY_HOME}/reducer --variance >> ${OUTPUT}

  RESULT=$(head -n 1 "${OUTPUT}")

  if [ "${RESULT}" != "129.8" ] # Test 1
  then
    echo "Unit test №1 failed"
    exit
  fi

  RESULT=$(head -n 2 "${OUTPUT}" | tail -n 1)

  if [ "${RESULT}" != "1784.16" ] # Test 2
  then
    echo "Unit test №2 failed"
    exit
  fi
  echo "Unit tests success"

else
  INPUT=${INPUT_HOME}/AB_NYC_2019.csv

  cat ${INPUT} | ${BINARY_HOME}/mapper | sort -k1 | ${BINARY_HOME}/reducer > output
  cat ${INPUT} | ${BINARY_HOME}/mapper | sort -k1 | ${BINARY_HOME}/reducer --variance >> output
  cat output
fi
