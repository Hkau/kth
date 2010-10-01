import junit.framework.TestCase;

/**
 * JÄTTESERIER
 * @author lemming
 */
public class BinarySearchTreeTest extends TestCase {
	private BinarySearchTree<Integer> tree;

	public void testAdd(){
		tree = new BinarySearchTree<Integer>(1);
		assertTrue(tree.add(5));
		assertTrue(tree.add(3));
		assertTrue(tree.add(2));
		assertTrue(tree.add(4));
		assertTrue(!tree.add(2));
	}

	public void testSize() {
		tree = new BinarySearchTree<Integer>(6);
		tree.add(1);
		tree.add(2);
		tree.add(8);
		tree.add(1); //dublett
		tree.add(5);
		tree.add(7);

		assertEquals(tree.size(), 6);

		tree.add(9);
		
		assertEquals(tree.size(), 7);
	}

	public void testContains(){
		tree = new BinarySearchTree<Integer>(5);
		assertTrue(tree.contains(5));
		assertTrue(!tree.contains(2));

		tree.add(2);
		assertTrue(tree.contains(2));

		tree.add(4);
		tree.add(1);
		tree.add(42);
		tree.add(13);
		tree.add(5);
		assertTrue(tree.contains(5));
		assertTrue(tree.contains(13));
		assertTrue(!tree.contains(0));
	}

	public void testHeight(){
		tree = new BinarySearchTree<Integer>(5);
		assertEquals(tree.height(), 0);

		tree.add(4);
		tree.add(3);
		tree.add(7);
		tree.add(6);
		tree.add(5);
		tree.add(8);
		tree.add(10);
		tree.add(9);
		tree.add(11);

		assertEquals(tree.height(), 4);
	}
	
	public void testLeaves(){
		tree = new BinarySearchTree<Integer>(5);
		assertEquals(tree.leaves(), 1);

		tree.add(4);
		tree.add(3);
		tree.add(7);
		tree.add(6);
		tree.add(5);
		tree.add(8);
		tree.add(10);
		tree.add(9);
		tree.add(11);

		assertEquals(tree.leaves(), 4);
	}

	public void testToString(){
		tree = new BinarySearchTree<Integer>(5);
		assertTrue(tree.toString().equals("5"));

		tree.add(4);
		tree.add(3);
		tree.add(7);
		tree.add(6);
		tree.add(5);
		tree.add(8);
		tree.add(10);
		tree.add(9);
		tree.add(11);

		assertTrue(tree.toString().equals("3 4 5 6 7 8 9 10 11"));
	}
}
