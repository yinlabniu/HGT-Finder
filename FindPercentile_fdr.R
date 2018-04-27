#args[1] = file in

args=commandArgs(trailingOnly=TRUE)

if(length(args) != 2) {
	stop("Improper parameters:\n\n\tRscript FindPercentile.r [file name] [otuput file name]\n\n")
}

file=args[1];
outFile = args[2];
outFile = paste(outFile, ".stats", sep = "")

table = read.table(file, sep="\t", header = FALSE, comment.char = "");

mew = mean(table[,2]);
sigma = sd(table[,2]);

cat("", file = outFile)

xVals = c();
xName = c();

count = 1;
for(i in table[,1]) {
#cat(i, " ", table[count,2], "\n", file = stderr())
	x = pnorm(table[count,2], mean = mew, sd = sigma, lower.tail = FALSE);
	xVals=c(xVals, x);
	xName=c(xName, i)

	#cat(i, table[count,2], x, "\n", file = outFile, append = TRUE);
	count = count + 1;
}

xVals2 = xVals
xVals2 = sort(xVals2)

count = 1
max = 0
for(i in xVals2){
	if(i < count / length(xVals2) * 0.05) {
		max = i
	} else {
		break
	}
	count = count + 1
}

count = 1
for(i in xVals) {
	if(i <= max) {
		cat(xName[count], table[count,2], xVals[count], "PASS", "\n", file = outFile, append = TRUE, sep = "\t")
	} else {
		cat(xName[count], table[count,2], xVals[count], "FAIL", "\n", file = outFile, append = TRUE, sep = "\t")
	}
	count = count + 1
}

#outFile = paste(file, ".stats", sep = "")
#cat(q$qvalues, "\n", file = stderr());
#cat(paste("q=", q, "\n", sep = ""), file = outFile, append = TRUE)
