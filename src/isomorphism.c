/* isomorphism.c: compare two graphs with each other. */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "isomorphism.h"

char *progname;

int main(int argc, char **argv)
{
    int err;
    graph_stats stats;
    graph g1, g2;

    progname = *argv;
    if (argc != 3)
        fail("not enough arguments", 1);

    init_graph(&g1);
    init_graph(&g2);

    err = read_graph_file(argv[1], &g1);
    if (err == -1)
        fail(strerror(errno), 1);
    err = read_graph_file(argv[2], &g2);
    if (err == -1)
        fail(strerror(errno), 1);

    stats = is_isomorphic(&g1, &g2);
    switch (stats) {
    case VERTICES_UEQ:
        fprintf(stderr, "unequal number of vertices\n");
        break;
    case EDGES_UEQ:
        fprintf(stderr, "unequal number of edges\n");
        break;
    case DEGREES_UEQ:
        fprintf(stderr, "unequal distribution of degree values\n");
        break;
    case PIECES_UEQ:
        fprintf(stderr, "unequal number of graph pieces\n");
        break;
    case UNEQUAL:
        fprintf(stderr, "unequal after detailed analysis\n");
        break;
    case ISOMORPHIC:
        fprintf(stderr, "isomorphic\n");
        break;
    }

    free_graph(&g1);
    free_graph(&g2);

    return 0;
}

void init_graph(graph *graph)
{
    graph->vertices = 0;
    graph->num_edges = 0;
    graph->edges = NULL;
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

void free_graph(graph *graph)
{
    free(graph->edges);
}

void dump_graph(graph *graph)
{
    uint64_t i;

    for (i = 0; i < graph->num_edges; i++)
        fprintf(stderr, "edge %lu: %lu-%lu\n", i,
                graph->edges[i].vert1, graph->edges[i].vert2);
}

graph_stats is_isomorphic(graph *g1, graph *g2)
{
    if (g1->vertices != g2->vertices)
        return VERTICES_UEQ;
    if (g1->num_edges != g2->num_edges)
        return EDGES_UEQ;

    /* FIXME: cover degrees, graph pieces, and detailed analysis */

    return ISOMORPHIC;
}
