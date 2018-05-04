All .txt code were compiled to ASM by using bash script

for i in {1..12}; do
../watcompiler $i.txt;
mv $i.s asm/;
gcc asm/$i.s -o out/$i;
done;

