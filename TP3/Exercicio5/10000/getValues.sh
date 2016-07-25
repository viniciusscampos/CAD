echo "Rodando programa"
mkdir tempos
cp xmatrix tempos/xmatrix
cd tempos

for i in 1 2 4 10 20 50
do		
	mpirun -np $i ./xmatrix | grep "Tempo de execução:" | awk '{print $4}' >> tempos-$i.txt;
done