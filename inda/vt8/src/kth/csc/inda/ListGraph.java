package kth.csc.inda;

import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.Map;
import java.util.Set;

/**
 * An undirected graph with a fixed number of vertices implemented using
 * adjacency lists. Space complexity is O(V + E) where V is the number
 * of vertices and E the number of edges.
 * 
 * @author [Name]
 * @version [Date]
 */
public class ListGraph implements UndirectedGraph {
    /** Number of vertices in the graph. */
    private final int numVertices;

    /** Number of edges in the graph. */
    private int numEdges;

    /**
     * All vertices adjacent to v are stored in adjacentVertices[v].
     * No set is allocated if there are no adjacent vertices.
     */
    private final Set<Integer>[] adjacentVertices;

    /**
     * Edge costs are stored in a hash map. The key is an
     * Edge(v, w)-object and the cost is an Integer object.
     */
    private final Map<Edge, Integer> edgeCosts;

    /**
     * Constructs a ListGraph with v vertices and no edges.
     * Time complexity: O(v)
     * 
     * @throws IllegalArgumentException if v < 0
     */
    public ListGraph(int v) {
    	if(v < 0)
    		throw new IllegalArgumentException();
        numVertices = v;
        numEdges = 0;
        
        // The array will contain only Set<Integer> instances created
        // in addEdge(). This is sufficient to ensure type safety.
        @SuppressWarnings("unchecked")
        Set<Integer>[] a = new HashSet[numVertices];
        adjacentVertices = a;
        for(int i = 0; i < numVertices; ++i){
        	a[i] = new HashSet<Integer>();
        }

        edgeCosts = new HashMap<Edge, Integer>();
    }

    /** An undirected edge between v and w. */
    private static class Edge {
        // Invariant: v <= w
        final int v;
        final int w;

        Edge(int v, int w) {
            if (v <= w) {
                this.v = v;
                this.w = w;
            } else {
                this.v = w;
                this.w = v;
            }
        }

        @Override
        public boolean equals(Object o) {
            if (!(o instanceof Edge))
                return false;
            Edge e = (Edge) o;
            return v == e.v && w == e.w;
        }

        @Override
        public int hashCode() {
            return 31*v + w;
        }

        @Override
        public String toString() {
            return "(" + v + ", " + w + ")";
        }
    }

    /**
     * Returns the number of vertices in this graph.
     * Time complexity: O(1).
     * 
     * @return the number of vertices in this graph
     */
    public int numVertices() {
        return numVertices;
    }

    /**
     * Returns the number of edges in this graph.
     * Time complexity: O(1).
     * 
     * @return the number of edges in this graph
     */
    public int numEdges() {
        return numEdges;
    }

    /**
     * Returns the degree of vertex v.
     * Time complexity: O(1).
     * 
     * @param  v vertex
     * @return the degree of vertex v
     * @throws IllegalArgumentException if v is out of range
     */
    public int degree(int v) throws IllegalArgumentException {
    	if(v >= numVertices || v < 0)
    		throw new IllegalArgumentException();
        return adjacentVertices[v].size();
    }

    /**
     * Returns an iterator of vertices adjacent to v.
     * Time complexity for iterating over all vertices: O(n),
     * where n is the number of adjacent vertices.
     * 
     * @param  v vertex
     * @return an iterator of vertices adjacent to v
     * @throws IllegalArgumentException if v is out of range
     */
    public VertexIterator adjacentVertices(int v) {
    	if(v >= numVertices || v < 0)
    		throw new IllegalArgumentException();
        return new ListGraphVertexIterator(adjacentVertices[v].iterator());
    }

    /**
     * Returns true iff v is adjacent to w.
     * Time complexity: O(1).
     * 
     * @param  v vertex
     * @param  w vertex
     * @return true iff v is adjacent to w
     * @throws IllegalArgumentException if v or w are out of range
     */
    public boolean areAdjacent(int v, int w) {
    	if(Math.max(v, w) >= numVertices || Math.min(v, w) < 0)
    		throw new IllegalArgumentException();
        return adjacentVertices[v].contains(w);
    }

    /**
     * Returns the edge cost if v is adjacent to w and an edge cost
     * has been assigned, -1 otherwise. Time complexity: O(1).
     * 
     * @param  v vertex
     * @param  w vertex
     * @return true iff v is adjacent to w
     * @throws IllegalArgumentException if v or w are out of range
     */
    public int edgeCost(int v, int w) throws IllegalArgumentException {
    	if(Math.max(v, w) >= numVertices || Math.min(v, w) < 0)
    		throw new IllegalArgumentException();
    	return (edgeCosts.containsKey(new Edge(v, w))) ? edgeCosts.get(new Edge(v, w)) : -1;
    }

    /**
     * Inserts an undirected edge between vertex positions v and w.
     * (No edge cost is assigned.) Time complexity: O(1).
     * 
     * @param  v    vertex position
     * @param  w    vertex position
     * @throws IllegalArgumentException if v or w are out of range
     */
    public void addEdge(int v, int w) {
    	if(Math.max(v, w) >= numVertices || Math.min(v, w) < 0)
    		throw new IllegalArgumentException();
    	if(adjacentVertices[v].add(w)){ // om det lades till en ny (det inte redan fanns en)
    		numEdges++;
    		adjacentVertices[w].add(v);
    	}
    	
    }

    /**
     * Inserts an undirected edge with edge cost c between v and w.
     * Time complexity: O(1).
     * 
     * @param  c edge cost, c >= 0
     * @param  v vertex
     * @param  w vertex
     * @throws IllegalArgumentException if v or w are out of range
     * @throws IllegalArgumentException if c < 0
     */
    public void addEdge(int v, int w, int c) {
    	if (c < 0)
    		throw new IllegalArgumentException();

    	addEdge(v, w);
    	edgeCosts.put(new Edge(v, w), c);
    }

    /**
     * Removes the edge between v and w.
     * Time complexity: O(1).
     * 
     * @param  v vertex
     * @param  w vertex
     * @throws IllegalArgumentException if v or w are out of range
     */
    public void removeEdge(int v, int w) {
    	if(Math.max(v, w) > numVertices)
    		throw new IllegalArgumentException();
    	if(adjacentVertices[v].remove(w)){ // If it existed, remove from everywhere
    		numEdges--;
    		adjacentVertices[w].remove(v);
    		edgeCosts.remove(new Edge(v, w));
    	}
    }

    /**
     * Returns a string representation of this graph.
     * 
     * @return a String representation of this graph
     */
    @Override
    public String toString() {
        if(numEdges == 0)
        	return "Vertices: 0, Edges: {}";

        StringBuilder sb = new StringBuilder();
        sb.append("Vertices: " + numVertices + ", Edges: ");
        sb.append('{');
        for(int i = 0;i < numVertices; ++i){
        	for(Iterator<Integer> it = adjacentVertices[i].iterator(); it.hasNext();)
        	{
        		Integer v = it.next();
        		if(i > v)
        			continue;
        		sb.append('(');
        		sb.append(i);
        		sb.append(',');
        		sb.append(v);
        		int cost = edgeCost(i, v);
        		if(cost != -1)
        		{
        			sb.append(',');
        			sb.append(cost);
        		}
        		sb.append(')');
        		sb.append(", ");
        	}
        }

        sb.setLength(sb.length() - 2);
        sb.append('}');
        return sb.toString();
    }
}
