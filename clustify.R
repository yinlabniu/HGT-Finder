# clustify.R
#	This script generates gene clusters given two files, a cutoff
#	value, and a +/- sign signifying whether we want to accept
#	values above or below the cutoff, respectively.
#
# Usage
#	Rscript clustify.R [gff file] [hit values file] [cutoff] [+|-] [hole size] [min distance]
#
#		[gff file]
#		A GFF3 formated file the matches the Names found in the hit 
#		values file.
#
#		[hit values file]
#		A tab delimited, 2 column file with each row containing:
#			Gene ID in the first column
#			Numerical value in the second column
#
#		[cutoff]
#		A numerical cutoff value to use for the second column
#
#		[1/-1]
#		A numerical value of 1 or -1.  1 Implies to get anything above the cutoff
#		while -1 implies to get anything below the cutoff
#
#		[hole size]
#		A numeric value to specify the number of invalid (not meeting
#		the cutoff) genes to allow between two valid (meeting the
#		cutoff) genes. 
#
#		[min distance]
#		Select the minimum distance between genes allowed within a 
#		cluster.  If set to a value below 0 (-1 for example), this
#		option will not be used.   

library(methods)
library(hash)

getGeneName = function(attrString) {
	attrs = c()
	name = ""
	ID = ""

	attrVect = unlist(strsplit(attrString, ';'))
	for(i in 1:length(attrVect)) {
		attr = unlist(strsplit(attrVect[i], '='))
		if(attr[1] == "Name") {
			name = attr[2]
		} else if(attr[1] == "ID") {
			ID = ""
		}
	}

	if(name != "") {
		return(name)
	} else {
		return(ID)
	}
}

getHits = function(hitF, getU, cut) {
	hitT = read.table(hitF, sep = "\t", comment.char = "")
	hitH = hash()

	for(i in 1:nrow(hitT)) {
		gene = toString(hitT[i,1])
		val = as.numeric(hitT[i,2])
		if(getU && val > as.numeric(cut)){
			hitH[[gene]] = 1
		} else if((!getU) && val < as.numeric(cut)) {
			hitH[[gene]] = 1
		}
	}

	return(hitH)
}

printClusters = function(gffF, hitH, holeSz, mDist) {
	gffT = read.table(gffF, sep = "\t")

	holeCount = 0;
	currClust = c()
	currHole = c()
	currChrom = ""
	prevEnd = -1
	prevChr = ""

	for(i in 1:nrow(gffT)) {
		if(toString(gffT[i,3]) != "gene") {
			next
		}

		chr = gffT[i, 1]
		start = gffT[i, 4]
		end = gffT[i, 5]

		if(prevChr == "") {
			prevChr = chr
		}

		if(prevEnd < 0) {
			prevEnd = start
		}

		chrom = toString(gffT[i, 1])

		if(chrom == "") {
			next
		}

		if(chrom != currChrom) {
			printCluster(currClust)
			currClust = c()
			currChrom = chrom
			currHole = c()
			holeCount = 0

			prevEnd = -1
		}

		gene = getGeneName(toString(gffT[i,9]))

		if(mDist > 0 && (prevEnd - start) > mDist) {
			printCluster(currClust)
			currClust = c()
			currHole = c()
			holeCount = 0

			prevEnd = -1
		} 


		if(toString(hitH[[gene]]) == 1) {
			if(length(currClust) == 0) {
				currHole = c()
			}

			valGene = paste("*", gene, "*", sep = "")
			currClust = c(currClust, currHole, valGene)
			currHole = c()
			holeCount = 0
		} else {
			if(length(currClust) == 0) {
				prevEnd = -1
			}

			holeCount = holeCount + 1
			currHole = c(currHole, gene)

			if(holeCount > holeSz) {
				printCluster(currClust)
				currClust = c()
				currHole = c()
				holeCount = 0

				prevEnd = -1
			}
		}
	}
}

printCluster = function(cluster) {
	if(length(cluster) < 2) {
		return()
	}

	for(i in cluster) {
		cat(i, "\t", sep = "")
	}
	cat("\n")
}

argv = commandArgs(trailingOnly = TRUE)

if(length(argv) != 6) {
	cat("Rscript clustify.R [gff file] [hit values file] [cutoff] [+|-] [hole size] [min distance]\n", file = stderr())

	stop()
}

gffF = argv[1]
hitF = argv[2]
cut = as.numeric(argv[3])
getU = FALSE
if(argv[4] == "+") {
	getU = TRUE
} else if(argv[4] == "-") {
	getU = FALSE
} else {
	cat("[+/-] not set appropriately, must either be '+' or '-'\n", file = stderr())
	stop()
}
holeSz = as.numeric(argv[5])
mDist = as.numeric(argv[6])

cat("Applying cutoff...", file = stderr())
hitH = getHits(hitF, getU, cut)
cat("Done\n", file = stderr())

cat("Printing Clusters...", file = stderr())
printClusters(gffF, hitH, holeSz, mDist)
cat("Done\n", file = stderr())

