package kth.csc.inda;

import java.util.HashSet;
import java.util.NoSuchElementException;
import java.util.Set;

import junit.framework.TestCase;

public abstract class AbstractGraphTest extends TestCase {
    /**
     * Returns an instance of an UndirectedGraph of the given size.
     */
    public abstract UndirectedGraph createGraph(int size);

    /** Test data. */
    private UndirectedGraph g0, g1, g4;
    
    /**
     * Sets up the test fixture.
     * Called before every test case method.
     */
    @Override
    protected void setUp() {
        // The empty graph.
        g0 = createGraph(0);

        // One node, one edge.
        g1 = createGraph(1);
        g1.addEdge(0, 0);

        // Two disconnected components of size 2.
        g4 = createGraph(4);
        g4.addEdge(0, 1);
        g4.addEdge(2, 3, 1);
    }

    /**
     * Tears down the test fixture.
     * Called after every test case method.
     */
    @Override
    protected void tearDown() {
        g0 = null;
        g1 = null;
        g4 = null;
    }

    public void testConstructor() {
        try {
            createGraph(-1);
            fail();
        } catch (IllegalArgumentException e) {
        }
    }

    public void testToString() {
        String g0s = "Vertices: 0, Edges: {}";
        String g1s = "Vertices: 1, Edges: {(0,0)}";
        String g4s1 = "Vertices: 4, Edges: {(0,1), (2,3,1)}";
        String g4s2 = "Vertices: 4, Edges: {(2,3,1), (0,1)}";
        assertTrue(g0.toString().equals(g0s));
        assertTrue(g1.toString().equals(g1s));
        assertTrue(g4.toString().equals(g4s1) ||
                g4.toString().equals(g4s2));
    }

    public void testNumVertices() {
        assertEquals(g0.numVertices(), 0);
        assertEquals(g1.numVertices(), 1);
        assertEquals(g4.numVertices(), 4);
    }

    public void testNumEdges() {
        assertEquals(g0.numEdges(), 0);
        assertEquals(g1.numEdges(), 1);
        assertEquals(g4.numEdges(), 2);
        g4.removeEdge(1, 2);
        assertEquals(g4.numEdges(), 2);
        g4.addEdge(0, 1);
        assertEquals(g4.numEdges(), 2);
        g4.addEdge(1, 2);
        assertEquals(g4.numEdges(), 3);
        g4.removeEdge(0, 1);
        assertEquals(g4.numEdges(), 2);
        g4.removeEdge(0, 1);
        assertEquals(g4.numEdges(), 2);
    }

    public void testEdges() {
        assertEquals(g1.edgeCost(0, 0), -1);
        assertEquals(g4.edgeCost(0, 1), -1);
        assertEquals(g4.edgeCost(2, 3), 1);
        assertEquals(g4.edgeCost(1, 2), -1);
        g1.removeEdge(0, 0);
        g4.removeEdge(0, 1);
        g4.removeEdge(2, 3);
        assertEquals(g1.edgeCost(0, 0), -1);
        assertEquals(g4.edgeCost(0, 1), -1);
        assertEquals(g4.edgeCost(2, 3), -1);
        g1.removeEdge(0, 0);
        g4.removeEdge(0, 1);
        g4.removeEdge(2, 3);
        assertEquals(g1.edgeCost(0, 0), -1);
        assertEquals(g4.edgeCost(0, 1), -1);
        assertEquals(g4.edgeCost(2, 3), -1);
    }

    public void testDegree() {
        assertEquals(g1.degree(0), 1);
        g1.removeEdge(0, 0);
        assertEquals(g1.degree(0), 0);
        assertEquals(g4.degree(0), 1);
        assertEquals(g4.degree(1), 1);
        g4.addEdge(1, 2);
        assertEquals(g4.degree(0), 1);
        assertEquals(g4.degree(1), 2);
        g4.removeEdge(0, 1);
        assertEquals(g4.degree(0), 0);
        assertEquals(g4.degree(1), 1);
        g4.removeEdge(0, 1);
        assertEquals(g4.degree(0), 0);
        assertEquals(g4.degree(1), 1);
        try {
            g0.degree(0);
            fail();
        } catch (IllegalArgumentException e) {
        }
        try {
            g1.degree(-1);
            fail();
        } catch (IllegalArgumentException e) {
        }
        try {
            g1.degree(1);
            fail();
        } catch (IllegalArgumentException e) {
        }
    }

    public void testAdjacentVertices() {
        VertexIterator pi = g1.adjacentVertices(0);
        assertTrue(pi.hasNext());
        assertEquals(pi.next(), 0);
        pi = g4.adjacentVertices(0);
        assertEquals(pi.next(), 1);
        assertFalse(pi.hasNext());
        g4.addEdge(0, 0);
        g4.addEdge(0, 1);
        g4.addEdge(0, 2);
        g4.addEdge(0, 3);
        pi = g4.adjacentVertices(0);
        Set<Integer> s = new HashSet<Integer>();
        for (int i = 0; i < 4; i++) {
            s.add(pi.next());
        }
        assertEquals(s.size(), 4);
        for (int i = 0; i < 4; i++) {
            assertTrue(s.contains(i));
        }
        assertFalse(pi.hasNext());
        try {
            pi.next();
            fail();
        } catch (NoSuchElementException e) {
        }
    }

    public void testAreAdjacent() {
        assertTrue(g1.areAdjacent(0, 0));
        assertFalse(g4.areAdjacent(0, 0));
        assertTrue(g4.areAdjacent(0, 1));
        assertFalse(g4.areAdjacent(1, 2));
        g4.addEdge(1, 2);
        assertTrue(g4.areAdjacent(1, 2));
    }
}
