// Inspired by Ullman (1976) Subgraph-isomorphism algorithm
// Brandon Holt

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "defs.h"

/* @param g: (directed) graph to search in
   @param vertices: pattern of vertex colors to search for (in order)
   @return matches: list of vertices that mark the beginning of the pattern */
graphint pathIsomorphism(graph* g, color_t* pattern, graphint** matches) {
	const graphint NV = g->numVertices;
	const graphint * restrict edge = g->edgeStart; /* Edge domain */
	const graphint * restrict eV = g->endVertex; /* Edge domain */
	color_t * restrict marks = g->marks; /* Vertex domain */
		
	graphint *partials = (graphint*)xmalloc(NV*sizeof(graphint));
	graphint *matchstart = (graphint*)xmalloc(NV*sizeof(graphint));
	graphint np = 0;
	graphint p = 0;
	
	// copy vertices that match beginning of pattern
	for (graphint v=0; v<NV; v++) {
		if (marks[v] == pattern[p]) {
			partials[np] = v;
			matchstart[np] = v;
			np++;
		}
	}
	p++;
	while (pattern[p] != END) {
		color_t nextMark = pattern[p];
		graphint nextV, currV;
		graphint nextnp = 0;
		
		for (graphint i=0; i<np; i++) {
			currV = partials[i];
			for (graphint j=edge[currV]; j<edge[currV+1]; j++) {
				nextV = eV[j];
				if (marks[nextV] == nextMark) {
					partials[nextnp] = nextV;
					matchstart[nextnp] = matchstart[i]; // keep track of start of match
					nextnp++;
					break;
				}
			}
		}
		
		np = nextnp;
		p++;
	}
	
	*matches = (graphint*)xmalloc(np*sizeof(graphint));
	memcpy(*matches, matchstart, np*sizeof(graphint));
	free(partials);
	free(matchstart);
	
	return np;
}

void randomizeColors(graph *g, color_t minc, color_t maxc) {
	for (graphint i=0; i<g->numVertices; i++) {
		g->marks[i] = (rand() % (maxc-minc)) + minc;
	}
}

void print_match(graph *dirg, color_t *pattern, graphint startVertex) {
	graphint v = startVertex;
	color_t *c = pattern;
	printf("match %d: %d(%d)", 0, v, *c);
	c++;
	while (*c != END) {
		graphint nextV = -1;
		for (graphint i=dirg->edgeStart[v]; i<dirg->edgeStart[v+1]; i++) {
			if (dirg->marks[dirg->endVertex[i]] == *c) {
				nextV = dirg->endVertex[i];
				break;
			}
		}
		assert(nextV != -1);
		printf(" -> %d(%d)", nextV, *c);
		v = nextV;
		c++;
	}
	printf("\n");
}
