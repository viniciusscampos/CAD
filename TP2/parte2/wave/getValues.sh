echo "Rodando programa"
mkdir tempos
mkdir tempos/media
cd tempos

g++ -O ../main.cc -o wave.exe
g++ ../plotmaker.cpp -std=c++11 -o plotmaker
for i in 1 2 3 4 5 6 7 8 9 10
do		
	./wave.exe | grep "Tempo de execução:" | awk '{print $4}' >> tempos.txt;./plotmaker wave 200 10
done

echo "Calculando as médias"
./../calculateAverages tempos.txt >> media/medias.txt

