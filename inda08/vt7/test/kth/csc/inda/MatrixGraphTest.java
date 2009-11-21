package kth.csc.inda;

public class MatrixGraphTest extends AbstractGraphTest {
    /**
     * Returns an instance of an UndirectedGraph of the given size.
     */
    @Override
    public UndirectedGraph createGraph(int size) {
        return new MatrixGraph(size);
    }
}
