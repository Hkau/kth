import junit.framework.TestCase;

/**
 * Tests PostFixCalculator
 * @author lemming
 */
public class PostFixCalculatorTest extends TestCase {

	/**
	 * @param name
	 */
	public PostFixCalculatorTest(String name) {
		super(name);
	}

	/* (non-Javadoc)
	 * @see junit.framework.TestCase#setUp()
	 */
	protected void setUp() throws Exception {
		super.setUp();
	}

	/* (non-Javadoc)
	 * @see junit.framework.TestCase#tearDown()
	 */
	protected void tearDown() throws Exception {
		super.tearDown();
	}

	public void testValidPostFix(){
		assertEquals(PostFixCalculator.EvaluatePostFix("-2 2 +"), 0);
		assertEquals(PostFixCalculator.EvaluatePostFix("2 5 +"), 7);
		assertEquals(PostFixCalculator.EvaluatePostFix("1 2 + 3 *"), 9);
		assertEquals(PostFixCalculator.EvaluatePostFix("1 2 - 3 4 + *"), -7);
		assertEquals(PostFixCalculator.EvaluatePostFix("1 2 + 3 * 4 - 5 /"), 1);
		assertEquals(PostFixCalculator.EvaluatePostFix("2 3 4 5 + - *"), -12);
	}

	/**
	 * Tests error handling for missing operands.
	 */
	public void testEmptyStack(){
		try { // Stack underflows
			PostFixCalculator.EvaluatePostFix("2 4 5 + - *");
		}

		catch (java.util.EmptyStackException e) {
			return;
		}

		fail("Missing EmptyStackException");
	}

	/**
	 * Tests for handling of empty expression and excess operands.
	 */
	public void testIllegalArgument(){
		try { // Invalid arguments
			PostFixCalculator.EvaluatePostFix("2 2 2 +");
		}

		catch (IllegalArgumentException e) {
			return;
		}

		fail("Missing IllegalArgumentException");
	}

	/**
	 * Tests error handling of non-operand and non-operators.
	 */
	public void testNumberFormat(){
		try { // Invalid arguments
			PostFixCalculator.EvaluatePostFix("2 2 + z");
		}

		catch (NumberFormatException e) {
			return;
		}

		fail("Missing NumberFormatException");
	}
}
