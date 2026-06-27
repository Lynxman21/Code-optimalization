#!/bin/bash

# Konfiguracja
PROGRAMS=("exe/normalize1.out" "exe/normalize2.out" "exe/normalize3.out" "exe/normalize4.out" "exe/normalize5.out" "exe/normalize6.out")
# Dodajemy listę plików o różnych rozmiarach
TEST_FILES=("tests/test-100kB.txt" "tests/test-250kB.txt" "tests/test-500kB.txt" "tests/test-duplicates.txt" "tests/test-punct.txt" "tests/test-spaces.txt")
OUTPUT_CSV="wyniki_pomiarow.csv"

echo "Program,Plik,Czas_Real[s],CPU_Percent,Max_RAM[KB]" > $OUTPUT_CSV

for file in "${TEST_FILES[@]}"; do
    echo "--- ROZMIAR DANYCH: $file ---"
    
    for prog in "${PROGRAMS[@]}"; do
        echo "Testowanie: $prog"
        
        /usr/bin/time -v ./$prog < $file 2> tmp_time.txt
            
        REAL_TIME=$(grep "Elapsed (wall clock) time" tmp_time.txt | awk '{print $NF}')
        CPU_PERC=$(grep "Percent of CPU this job got" tmp_time.txt | awk '{print $NF}' | sed 's/%//')
        MAX_RAM=$(grep "Maximum resident set size" tmp_time.txt | awk '{print $NF}')
            
            # Dodajemy nazwę pliku do CSV, żeby móc zrobić wykresy skalowalności
        echo "$prog,$file,$REAL_TIME,$CPU_PERC,$MAX_RAM" >> $OUTPUT_CSV
        echo "  $prog ($file) $REAL_TIME s"
    done
done

rm tmp_time.txt
echo "Gotowe!"