All: OFile/BitScore.o OFile/Query.o OFile/Subject.o OFile/TaxTrace.o OFile/DBContainer.o OFile/ProcBlast.o OFile/Blast.o OFile/StatFile.o OFile/GFF.o HGTFinder DBFormatter

OFile/BitScore.o: Source/BitScore.cc
	g++ -c Source/BitScore.cc -o OFile/BitScore.o -std=c++0x

OFile/Query.o: Source/Query.cc
	g++ -c Source/Query.cc -o OFile/Query.o -std=c++0x

OFile/Subject.o: Source/Subject.cc
	g++ -c Source/Subject.cc -o OFile/Subject.o -std=c++0x

OFile/TaxTrace.o: Source/TaxTrace.cc
	g++ -c Source/TaxTrace.cc -o OFile/TaxTrace.o -std=c++0x

OFile/DBContainer.o: Source/DBContainer.cc
	g++ -c Source/DBContainer.cc -o OFile/DBContainer.o -std=c++0x

OFile/ProcBlast.o: Source/ProcBlast.cc 
	g++ -c Source/ProcBlast.cc -o OFile/ProcBlast.o -std=c++0x

OFile/Blast.o: Source/Blast.cc 
	g++ -c Source/Blast.cc -o OFile/Blast.o -std=c++0x

OFile/StatFile.o: Source/StatFile.cc
	g++ -c Source/StatFile.cc -o OFile/StatFile.o -std=c++0x

OFile/GFF.o: Source/GFF.cc
	g++ -c Source/GFF.cc -o OFile/GFF.o -std=c++0x

HGTFinder: Source/main.cc Source/get_parms.cc OFile/BitScore.o OFile/Query.o OFile/Subject.o OFile/TaxTrace.o OFile/DBContainer.o OFile/ProcBlast.o OFile/Blast.o OFile/StatFile.o OFile/GFF.o
	g++ Source/main.cc Source/get_parms.cc OFile/BitScore.o OFile/Query.o OFile/Subject.o OFile/TaxTrace.o OFile/DBContainer.o OFile/ProcBlast.o OFile/Blast.o OFile/StatFile.o OFile/GFF.o -o HGTFinder -std=c++0x

DBFormatter: DBFormatter_Source/formatDatabase.cc DBFormatter_Source/hashFunctions.cc
	g++ DBFormatter_Source/formatDatabase.cc DBFormatter_Source/hashFunctions.cc -o DBFormatter -std=c++0x