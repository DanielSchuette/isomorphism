#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define fail(msg, code) {                                           \
    fprintf(stderr, "%s (%d): %s.\n", progname, __LINE__, msg);     \
    exit(code);                                                     \
}

char *progname;

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

int main(int argc, char **argv)
{
    int err;
    graph g1, g2;

    progname = *argv;
    if (argc != 3)
        fail("not enough arguments", 1);

    err = read_graph_file(argv[1], &g1);
    if (err == -1)
        fail(strerror(errno), 1);
    err = read_graph_file(argv[2], &g2);
    if (err == -1)
        fail(strerror(errno), 1);

    dump_graph(&g1);
    dump_graph(&g2);

    free(g1.edges);
    free(g2.edges);
    return 0;
}

int read_graph_file(char *name, graph *graph)
{
    int read;
    uint64_t idx, e1, e2;
    FILE *f;

    f = fopen(name, "r");
    if (!f)
        return -1;

    /*
     * In a graph file (.gf), every row holds two integers. The first pair is
     * the number of vertices and number of edges. Every following row holds
     * indices of two vertices that are connected by an edge. Thus, the second
     * integer on the first row must equal the number of rows in the file plus
     * one. Also, highest vertex index must not be larger than the first
     * integer on the first row minus one.
     */
    idx = read = 0;
    while (fscanf(f, "%lu %lu\n", &e1, &e2) == 2) {
        if (idx == 0) {
            graph->vertices = e1;
            graph->num_edges = e2;
            graph->edges = (edge *)malloc(sizeof(edge) * e2);
            if (!graph->edges) return -1;
        } else {
            /* ensure there aren't too many edges read */
            if (idx > graph->num_edges)
                fail("too many edges specified", 1);

            graph->edges[idx-1].vert1 = e1;
            graph->edges[idx-1].vert2 = e2;
        }
        idx++;
    };

    /* ensure there aren't too few edges read */
    if ((idx-1) != graph->num_edges)
        fail("too few edges specified", 1);

    return 0;
}

void dump_graph(graph *graph)
{
    uint64_t i;

    for (i = 0; i < graph->num_edges; i++)
        fprintf(stderr, "edge %lu: %lu-%lu\n", i,
                graph->edges[i].vert1, graph->edges[i].vert2);
}
