echo "Rodando programa"
mkdir resultados
mkdir resultados/medias
cd resultados
for j in 1 2 4 8 16 32
do
	g++ -std=c++11 -fopenmp -DN=1000000 -DNT=$j ../main.cpp -o main
	for i in 1 2 3 4 5 6 7 8 9 10
	do		
		./main  >> $j.txt
	done
done

echo "Calculando as médias"
for f in *.txt
do
	cat $f >> medias/A$f
	./../calculateAverages medias/A$f >> medias/AV$f
	rm medias/A$f
done
