#ifndef _ISOMORPHISM_H_
#define _ISOMORPHISM_H_

#include <stdint.h>

#define fail(msg, code) {                                           \
    fprintf(stderr, "%s (%d): %s.\n", progname, __LINE__, msg);     \
    exit(code);                                                     \
}

typedef struct {
    uint64_t vert1;
    uint64_t vert2;
} edge;

typedef struct {
    uint64_t vertices;
    uint64_t num_edges;
    edge *edges;
} graph;

int read_graph_file(char *, graph *);
void dump_graph(graph *);

#endif /* _ISOMORPHISM_H_ */
