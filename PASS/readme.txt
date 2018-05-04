All .txt code were compiled to ASM by using bash script

for i in {1..12}; do
gcc $i.txt -o out/$i;
done;

