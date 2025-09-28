#!/bin/bash

> needle_timesV1.doc
> needle_timesV2.doc
> needle_timesV3.doc
> dboard_timesV1.doc
> dboard_timesV2.doc
> dboard_timesV3.doc

for j in {1..10}
do
    for i in 1e6 5e6 1e7 5e7 1e8 5e8 1e9
    do
        n=$(printf "%.0f\n" $i)
        ./needleV1 $n >> needle_timesV1.doc
        ./dboardV1 $n >> dboard_timesV1.doc
    done
done

for j in {1..10}
do
    for i in 1e6 5e6 1e7 5e7 1e8 5e8 1e9
    do
        for k in 2 4 8 12 16
        do
            n=$(printf "%.0f\n" $i)
            ./needleV2 $n $k >> needle_timesV2.doc
            ./needleV3 $n $k >> needle_timesV3.doc
            ./dboardV2 $n $k >> dboard_timesV2.doc
            ./dboardV3 $n $k >> dboard_timesV3.doc
        done
    done
done


