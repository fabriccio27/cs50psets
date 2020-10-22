import csv
from sys import argv
from sys import exit

def main():

    if len(argv) != 3:
        print("Usage: python dna.py data.csv sequence.txt")
        exit(1)

    dbname = argv[1]
    seqname = argv[2]
    db = []
    STRCount = []

    fdb = open(dbname, newline="") # creo objeto archivo
    dbreader = csv.reader(fdb) # genero memoria donde guardar lo que leo
    for row in dbreader:
        db.append(row)
    # para cada fila en db a partir de la fila 1
    # convertir todas las columnas a partir de la 1 en enteros
    for row in db[1:]:
        row[1:] = [int(x) for x in row[1:]]

    # obtengo las STR de base de datos abierta, como list de strings
    listSTR = db[0][1:]
    cantidadSTR = len(listSTR)

    # abro secuencia
    fseq = open(seqname)
    seqstr = fseq.read()

    # obtengo cuenta de rachas de STR
    for i in range(cantidadSTR):
        STRCount.append(countSTR(seqstr, listSTR[i]))

    # comparar secuencia con base de datos
    for row in db:
        if row[1:] == STRCount:
            print(row[0])
            exit(0)
    print("No match")

    fdb.close()
    fseq.close()
    exit(0)


def countSTR(sequence, bases):

    seqlen = len(sequence)
    dif = len(bases)

    curr = 0
    count = 0
    for i in range(seqlen):
        while sequence[i:i+dif] == bases:
            curr += 1
            i += dif
        if curr > count:
            count = curr
        curr = 0

    return count

main()
