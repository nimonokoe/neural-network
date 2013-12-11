#define INPUT 784
#define HIDDEN 100
#define OUTPUT 10
INPUT <- 784
HIDDEN <- 100
OUTPUT <- 10

first.weight <-  matrix(rnorm(INPUT*HIDDEN, mean=0, sd=0.05), HIDDEN)
second.weight <- matrix(rnorm(HIDDEN*OUTPUT, mean=0, sd=0.05), OUTPUT)

write(first.weight, file="train-init1.csv")
write(second.weight, file="train-init2.csv")
