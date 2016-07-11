echo "Rodando programa"
mkdir resultados
mkdir resultados/media
cd resultados


for k in 1000000,1 2000000,2 3000000,3 4000000,4 6000000,6 8000000,8 16000000,16 32000000,32 48000000,48 64000000,64
do
	IFS=",";
	set $k;
	g++ -std=c++11 -fopenmp -DN=$1 -DNT=$2 ../main.cpp -o main
	for i in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20
	do		
		./main  >> $1-$2.txt
	done
done

echo "Calculando as médias"
for k in 1000000,1 2000000,2 3000000,3 4000000,4 6000000,6 8000000,8 16000000,16 32000000,32 48000000,48 64000000,64
do
	IFS=",";
	set $k;	
	./../calculateAverages $1-$2.txt >> media/medias.txt
done
