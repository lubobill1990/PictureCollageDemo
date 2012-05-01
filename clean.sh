#!/bin/bash

for i in ` find ./ -name 'CMakeFiles' -o -name 'CMakeCache.txt' `;

do rm -rf $i;

done

