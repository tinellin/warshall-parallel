FLAGS = -O3

all: transitive_closure_seq transitive_closure_parallel

transitive_closure_seq: transitive_closure-seq.c
	$(CC) $(FLAGS) transitive_closure-seq.c -o transitive_closure_seq

transitive_closure_parallel: transitive_closure-parallel.c
	$(CC) $(FLAGS) transitive_closure-parallel.c -o transitive_closure_parallel

clean:
	rm -f transitive_closure_seq transitive_closure_parallel