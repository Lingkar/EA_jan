cp ../maxcut-instances/set0a/n0000012i00.txt maxcut_instance.txt
cp ../maxcut-instances/set0a/n0000012i00.bkv vtr.txt
java -classpath build/classes geneticalgorithm.Launcher 100 -1
