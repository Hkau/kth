import junit.framework.TestCase;

/**
 * @author lemming
 *
 */
public class HashStringDictionaryTest extends TestCase {
	private HashStringDictionary dict;

	/* (non-Javadoc)
	 * @see junit.framework.TestCase#setUp()
	 */
	protected void setUp() throws Exception {
		dict = new HashStringDictionary(1024);
		super.setUp();
	}

	public void testAdd(){
		assertTrue(dict.add("Shoop"));
		assertTrue(dict.add("that"));
		assertTrue(dict.add("Woop"));
		assertTrue(!dict.add(new String("Shoop")));
	}
	
	public void testContains(){
		assertTrue(!dict.contains(""));
		assertTrue(!dict.contains("foo"));
		dict.add("string");
		assertTrue(dict.contains("string"));
		assertTrue(!dict.contains("bar"));
	}

	public void testRemove(){
		dict.add("string");
		dict.add("strang");
		dict.add("strang");
		dict.add("strung");
		assertTrue(dict.remove("string"));
		assertTrue(!dict.contains("string"));
		assertTrue(dict.remove("strang"));
		assertTrue(!dict.contains("strang"));
		assertTrue(dict.contains("strung"));
	}
	
}
