import junit.framework.TestCase;
import java.util.Arrays;
import java.util.Random;

/**
 * @author lemming
 * Testkod för flera sorteringsalgoritmer
 */
public class SortingTest extends TestCase {
	
	// Antalet sorterande algoritmer
	private static final int algs = 6;

	// Sorterande algoritmer
	private Sorting sorters[] = {
		new QuicksortJava(),
		new Quicksort(),
		new QuicksortRandomPivot(),
		new QuicksortInsertion(),
		new QuicksortInsertionRandom(),
		new InsertionSort()};

	// Namn på sorterande algoritmer, för debugging-ändamål (tm)
	private String sortingName[] =
		{"Java Quicksort implementation",
		"Quicksort",
		"Quicksort with random pivot",
		"Quicksort with insertion sort",
		"Quicksort with insertion sort and random pivot",
		"Insertion sort"};

	// Listor av tal, en för varje algoritm
	private int numbers[][] = new int[algs][];

	private static Random rand = new Random();

	/**
	 * Tests how the algorithms handle the empty list
	 */
	public void testEmptyList(){
		numbers[0] = new int[0];

		Test();
	}

	/**
	 * Tests how the algorithms handle a list with a single element.
	 */
	public void testSingleElementList(){

		numbers[0] = new int[1];
		numbers[0][0] = 42;

		Test();
	}

	/**
	 * Tests how the algorithms handle a list with two sorted elements.
	 */
	public void testTwoAscendingElements(){
		numbers[0] = new int[2];
		numbers[0][0] = 5;
		numbers[0][1] = 10;

		Test();
	}

	/**
	 * Tests how the algorithms handle a list with three elements.
	 */
	public void testThreeElements(){
		numbers[0] = new int[3];
		numbers[0][0] = 10;
		numbers[0][1] = 5;
		numbers[0][2] = 7;

		Test();
	}

	/**
	 * Tests how the algorithms handle a list with two unsorted elements.
	 */
	public void testTwoDescendingElements(){
		numbers[0] = new int[2];
		numbers[0][0] = 10;
		numbers[0][1] = 5;

		Test();
	}

	/**
	 * Tests how the algorithms handle a large list with pseudorandom elements.
	 */
	public void testRandomList(){
		numbers[0] = new int[4096];

		for(int i = 0; i < 4096; ++i)
			numbers[0][i] = rand.nextInt();

		Test();
	}

	/**
	 * Tests how the algorithms handle an already sorted list
	 */
	public void testAscendingList(){

		numbers[0] = new int[4096];

		for(int i = 0; i < 4096; ++i)
			numbers[0][i] = i;

		Test();
	}

	/**
	 * Tests how the algorithms handle a reversely sorted list
	 */
	public void testDescendingList(){
		numbers[0] = new int[4096];

		for(int i = 0; i < 4096; ++i)
			numbers[0][i] = 4096-i;

		Test();
	}

	/**
	 * Tests how the algorithms handle a list with equal elements
	 */
	public void testSameList(){
		numbers[0] = new int[4096];

		for(int i = 0; i < 4096; ++i)
			numbers[0][i] = 42;

		Test();
	}

	/**
	 * Runs test on the list.
	 */
	private void Test(){
		// Copy list for all the algorithms
		copyArrays();

		// Let all algorithms sort their own list
		sort();

		// Assert that all arrays are equal, it means that they're
		// sorted, as both Java's implementation and the insertion
		// sort is assumed to work, or atleast differ in case either
		// give the wrong answer
		equalArrays();
	}

	/**
	 * Runs sort for each algorithm on their list
	 */
	private void sort(){

		for(int i = 0; i < algs; i++)
			sorters[i].sort(numbers[i]);
	}

	/**
	 * Asserts equality between the arrays, to be run after sorting
	 */
	private void equalArrays(){
		for(int i = 1; i < algs; i++)
			assertTrue(sortingName[i], Arrays.equals(numbers[0], numbers[i]));
	}

	/**
	 * Copies the first list to the others, to give equal lists for test cases
	 */
	private void copyArrays(){
		for(int i = 1; i < algs; i++){
			numbers[i] = new int[numbers[0].length];
			System.arraycopy(numbers[0], 0, numbers[i], 0, numbers[0].length);
		}
	}
}
