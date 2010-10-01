import junit.framework.TestCase;

/**
 * Test of MyStack
 * @author lemming
 */
public class MyStackTest extends TestCase {
	Stack<Integer> stack;
	/**
	 * Stack constructor
	 * @param name
	 */
	public MyStackTest(String name) {
		super(name);
	}

	/* (non-Javadoc)
	 * @see junit.framework.TestCase#setUp()
	 */
	protected void setUp() throws Exception {
		stack = new MyStack<Integer>();
		super.setUp();
	}

	/**
	 * Tests pushing of elements on to the stack.
	 */
	public void testPush() {
		stack.push(5);
		assertEquals(stack.top(), new Integer(5));
		stack.push(10);
		assertEquals(stack.top(), new Integer(10));
	}
	/**
	 * Pops element off top of the stack. Also tests for the exception.
	 */
	public void testPop() {
		stack.push(5);
		stack.push(10);
		assertEquals(stack.pop(), new Integer(10));
		assertEquals(stack.pop(), new Integer(5));
		try {
			stack.pop();
		}
		catch (java.util.EmptyStackException e) {
			return;
		}
		fail("Expected EmptyStackException");
	}

	/**
	 * Tests if its giving valid size
	 */
	public void testSize() {
		assertEquals(stack.size(), 0);
		stack.push(5);
		stack.push(10);
		assertEquals(stack.size(), 2);
		stack.pop();
		assertEquals(stack.size(), 1);
	}

	/**
	 * Tests if its isEmpty method is valid..
	 */
	public void testIsEmpty() {
		assertEquals(stack.isEmpty(), true);
		stack.push(5);
		stack.push(10);
		assertEquals(stack.isEmpty(), false);
		stack.pop();
		assertEquals(stack.isEmpty(), false);
		stack.pop();
		assertEquals(stack.isEmpty(), true);
	}

	/* (non-Javadoc)
	 * @see junit.framework.TestCase#tearDown()
	 */
	protected void tearDown() throws Exception {
		super.tearDown();
	}

}
