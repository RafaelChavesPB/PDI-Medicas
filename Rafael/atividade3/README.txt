Executando scripts para Histograma:
. ../../venv/bin/activate 
python3 histogram.py ,/resources/ a.jpg

Executando scripts para imagens:
cd ./build
./gamma ../resources/ a.jpg <lowGamma> <highGamma>
./binarize ../resources/ a.jpg <threshold>
./layers ../resources/ a.jpg
./window_filter ../resources/ a.jpg <lowerbound> <upperbound>
