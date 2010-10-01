import java.util.Random;

/**
 * @author lemming
 * Quicksort med slumpad pivot
 */
public class QuicksortRandomPivot extends Quicksort{
	private static Random rand = new Random();

	/**
	 * Genererar slumpad pivot från listan.
	 */
	protected int getPivot(int[] v, int first, int last){
		return v[rand.nextInt(last-first+1)+first];
	}
}
