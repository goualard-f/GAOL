mkdir bad
cat testnos testnos1 | ./dt | sed 's/[Ii][Nn][Ff][intyINTY]*/Infinity/g' >zap 2>&1
cmp dtst.out zap || mv zap bad/dtst.out
./dItest <testnos | sed 's/[Ii][Nn][Ff][intyINTY]*/Infinity/g' >zap 2>&1
cmp dI.out zap || mv zap bad/dI.out
./dItestsi <testnos | sed 's/[Ii][Nn][Ff][intyINTY]*/Infinity/g' >zap 2>&1
cmp dIsi.out zap || mv zap bad/dIsi.out
./ddtestsi <testnos | sed 's/[Ii][Nn][Ff][intyINTY]*/Infinity/g' >zap 2>&1
cmp ddsi.out zap || mv zap bad/ddsi.out
for i in dd d f x xL Q; do 
	 cat testnos rtestnos | ./"$i"test | sed 's/[Ii][Nn][Ff][intyINTY]*/Infinity/g' >zap 2>&1;
	 cmp $i.out zap || mv zap bad/$i.out; 
done
./strtodt testnos3 >bad/strtodt.out && rm bad/strtodt.out || \
cat bad/strtodt.out
./strtodtnrp testnos3 >bad/strtodtnrp.out && rm bad/strtodtnrp.out || \
cat bad/strtodtnrp.out
rmdir bad 
