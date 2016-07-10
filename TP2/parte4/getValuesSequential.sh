echo "Rodando programa"
mkdir resultados
mkdir resultados/media
cd resultados


for k in 20 21
do	
	g++ -std=c++11 -fopenmp ../sequentialQuickSort.cpp -o sequentialQuickSort
	for i in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20
	do		
		./sequentialQuickSort $k  >> $k.txt
	done
done

echo "Calculando as médias"
for k in 20 21
do
	./../calculateAverages $k.txt >> media/medias.txt
done
