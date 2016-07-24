echo "Rodando programa"
mkdir tempos
mkdir tempos/media
cp xmatrix tempos/xmatrix
cd tempos

for j in 1 2 3 4 5 6 7 8 9 10
do
	for i in 1 10 100 1000 5000
	do		
		mpirun -np $i ./xmatrix | grep "Tempo de execução:" | awk '{print $4}' >> tempos-$i.txt;
	done
done

echo "Calculando as médias"
for i in 1 10 100 1000 5000
do		
	./../calculateAverages tempos-$i.txt >> media/medias.txt
done


