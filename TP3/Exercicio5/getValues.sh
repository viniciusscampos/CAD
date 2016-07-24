echo "Rodando programa"
mkdir tempos
mkdir tempos/media
cp xmatrix tempos/xmatrix
cd tempos

for i in 100
do		
	mpirun -np $i ./xmatrix | grep "Tempo de execução:" | awk '{print $4}' >> tempos.txt;
done

echo "Calculando as médias"
./../calculateAverages tempos.txt >> media/medias.txt

