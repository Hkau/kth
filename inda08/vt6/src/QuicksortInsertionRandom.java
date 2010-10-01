import java.util.Random;

/**
 * 
 */

/**
 * @author lemming
 * Quicksort med insertionsort och slumpad pivot. Ingen pseudomedian.
 */
public class QuicksortInsertionRandom extends QuicksortInsertion{
	static Random rand = new Random();
	/**
	 * Generate a pseudorandom pivot from the list
	 */
	protected int getPivot(int[] v, int first, int last){
		return v[rand.nextInt(last-first+1)+first];
	}
}
