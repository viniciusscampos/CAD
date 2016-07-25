echo "Rodando programa"
mkdir tempos
mkdir tempos/media
cp xmatrix tempos/xmatrix
cd tempos

for i in 100 200
do		
	mpirun -np $i ./xmatrix | grep "Tempo de execução:" | awk '{print $4}' >> tempos-$i.txt;
done
