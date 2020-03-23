#!/bin/bash
#arg1 ilosc watkow
#arg2 ilosc punkto
#mpicc -o carlo paralell_Carlo.c -lm

#Uruchom program ze zmieniającą się liczbą wątków, od 1 do 12.

# Każde uruchomienie powinno policzyć w sumie tyle samo punktów - rozmiar problemu pozostaje stały, praca jednego procesora maleje (eksperyment nieskalowany)
# for ((i=1; i<=$1; i++)); do
#     mpiexec --hostfile allnodes -np $i ./carlo $2
# done

# Za każdym razem każdy procesor powinien liczyć tyle samo punktów - rozmiar problemu rośnie, praca jednego procesora pozostaje stała (eksperyment skalowany)
# arg2 = 0
for ((i=1; i<=$1; i++)); do
    mpiexec --hostfile allnodes -np $i ./carlo $2 0
done



