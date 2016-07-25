echo "Rodando programa"
mkdir tempos
mkdir tempos/media
cp sequential tempos/sequential
cd tempos

for i in 1 2 3 4 5 6 7 8 9 10
do		
	./sequential | grep "Tempo de execução:" | awk '{print $4}' >> tempos.txt;
done

echo "Calculando as médias"
./../calculateAverages tempos.txt >> media/medias.txt

