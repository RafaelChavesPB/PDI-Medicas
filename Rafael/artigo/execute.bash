rm results.txt
for i in {1..10}
do
    python MlModel.py | grep -e "Test Loss" -e "Test Accuracy" >> results.txt
done
