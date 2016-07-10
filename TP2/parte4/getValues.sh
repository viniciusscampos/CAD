echo "Rodando programa"
mkdir resultados
mkdir resultados/media
cd resultados


for k in 2,20 2,21 4,20 4,21 8,20 8,21
do	
	IFS=",";
	set $k;
	g++ -std=c++11 -fopenmp -DNT=$1 ../parallelQuickSort.cpp -o parallelQuickSort-$1
	for i in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20
	do		
		./parallelQuickSort-$1 $2  >> $1-$2.txt
	done
done

echo "Calculando as médias"
for k in 2,20 2,21 4,20 4,21 8,20 8,21
do
	IFS=",";
	set $k;
	./../calculateAverages $1-$2.txt >> media/medias.txt
done
