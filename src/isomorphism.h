#ifndef _ISOMORPHISM_H_
#define _ISOMORPHISM_H_

#include <stdbool.h>
#include <stdint.h>

#define fail(msg, code)                                             \
    {                                                               \
        fprintf(stderr, "%s (%d): %s.\n", progname, __LINE__, msg); \
        exit(code);                                                 \
    }

typedef struct {
    uint64_t vert1;
    uint64_t vert2;
} edge;

typedef struct {
    uint64_t vertices;
    uint64_t num_edges;
    uint64_t *degrees;
    edge *edges;
} graph;

typedef enum {
    VERTICES_UEQ, /* unequal number of vertices */
    EDGES_UEQ,    /* unequal number of edges */
    DEGREES_UEQ,  /* unequal distribution of vertex degree values */
    PIECES_UEQ,   /* unequal number of pieces */
    UNEQUAL,      /* unequal after detailed analysis */
    ISOMORPHIC,   /* isomorphic graphs */
} graph_stats;

void init_graph(graph *);
int read_graph_file(char *, graph *);
void calc_degrees(graph *);
void free_graph(graph *);
void dump_graph(graph *);
graph_stats is_isomorphic(graph *, graph *);

void get_dist(uint64_t *d, graph *g);
bool comp_dist(uint64_t *, uint64_t *, uint64_t, uint64_t);

#endif /* _ISOMORPHISM_H_ */
