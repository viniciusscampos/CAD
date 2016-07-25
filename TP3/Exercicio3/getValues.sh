echo "Rodando programa"
mkdir tempos
mkdir tempos/media
cp anel tempos/anel
cd tempos

for i in 1 2 3 4 8
do
	for j in 1 2 3 4 5 6 7 8 9 10
	do		
		mpirun -np $i ./anel | grep "Tempo de execução:" | awk '{print $4}' >> tempos-$i.txt;
	done
done

echo "Calculando as médias"
for i in 1 2 3 4 8
do
	./../calculateAverages tempos-$i.txt >> media/medias.txt
done