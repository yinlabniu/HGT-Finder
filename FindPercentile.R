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

count = 1;
for(i in table[,1]) {
#cat(i, " ", table[count,2], "\n", file = stderr())
	x = pnorm(table[count,2], mean = mew, sd = sigma, lower.tail = FALSE);
	xVals=c(xVals, x);

	#cat(i, table[count,2], x, "\n", file = outFile, append = TRUE);
	count = count + 1;
}

library(qvalue)

q = qvalue(xVals);

count = 1;
for(i in table[,1]) {
	#cat(i, table[count,2], xVals[count], q$qvalues[count], "\n", file = outFile, append = TRUE, sep = "\t");

	cat(i, table[count,2], xVals[count], file = outFile, append = TRUE, sep = "\t")

	if(length(q$qvalues) > 0) {
		cat("\t", q$qvalues[count], file = outFile, append = TRUE, sep = "")
	}

	cat("\n", file = outFile, append = TRUE)

	count = count + 1
}

#outFile = paste(file, ".stats", sep = "")
#cat(q$qvalues, "\n", file = stderr());
#cat(paste("q=", q, "\n", sep = ""), file = outFile, append = TRUE)
